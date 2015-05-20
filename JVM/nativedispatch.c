/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */

//#include <stdio.h>
//#include <stdlib.h>
/* insert and update here functionForNativeMethodType-arrays for classes with native methods*/
/* array length ->  at least up to last native method < methods_count*/
/* look at methods in the *.java or *.class file in increasing order */
/* if method is non native -> insert NULL, otherwise pointer to nativce C-function*/

//krix
#include "nativedispach.h"

#include <stdlib.h> //malloc
#include "definitions.h"
#include "object.h"
#include "nstring.h"
#include "thread.h"
#include "interruptthread.h"
#include "lock.h"
#include "float.h"

#include "stack.h"
#include "heap.h"
#include "scheduler.h"
#include "classfile.h"

extern u1 local;
extern u1 cN;
extern classStructure cs[MAXCLASSES];
extern ThreadControlBlock* currentThreadCB;

u1 methodInvokedStatic = 0;
u1 methodRetun = 0;

u1 sizeOfNativeTypes[] = {  sizeof(jboolean),sizeof(jbyte),sizeof(jchar),
                            sizeof(jshort),sizeof(jint),sizeof(jfloat),
                            sizeof(jbooleanArray),sizeof(jbyteArray),sizeof(jcharArray),
                            sizeof(jshortArray),sizeof(jintArray),sizeof(jfloatArray)};

void paramReadVM(void *out,const NativeType type,const slot val)
{
    switch (type)
    {
        case BOOLEAN:
        {
            jboolean* p = (jboolean*)out;
            p[0] = val.Int & 0x00000001;
        }break;
        case BYTE:
        {
            jbyte* p = (jbyte*)out;
            p[0] = val.Int & 0x000000ff;
        }break;
        case CHAR:
        {
            jchar* p = (jchar*)out;
            p[0] = val.UInt & 0x000000ff;
        }break;

        case SHORT:
        {
            jshort* p = (jshort*)out;
            p[0] = val.Int & 0x0000ffff;
        }break;

        case INT:
        {
            jint* p = (jint*)out;
            p[0] = val.Int;
        }break;
        case FLOAT:
        {
            jfloat* p = (jfloat*)out;
            p[0] = val.Float;
        }break;
        case BOOLEANARRAY:
        case BYTEARRAY:
        case CHARARRAY:
        case SHORTARRAY:
        case INTARRAY:
        case FLOATARRAY:
        {
            if(val.stackObj.magic == MAGIC_OBJECT)
            {
                const slot heapObject = HEAPOBJECT(val.stackObj.pos);

                const NativeType elemType = type - BOOLEANARRAY;
                jbyteArray* array = (jbyteArray*)out;

                array->length = heapObject.heapObjMarker.length - 1;
                array->values = (jbyte*)malloc(array->length * sizeOfNativeTypes[elemType]);

                for(int i = 0; i < array->length;i++)
                {
                    const slot elem = HEAPOBJECT(val.stackObj.pos + 1 + i);
                    paramReadVM(array->values + i * sizeOfNativeTypes[elemType],elemType,elem);
                }
            }
        }break;
        default:
        {
            DNOTSUPPORTED;
        }break;
    }
}

void paramWriteVM(void *input,const NativeType type,slot* val,const u1 alloc)
{
    switch (type)
    {
        case BOOLEAN:
        {
            jboolean* p = (jboolean*)input;
            val->Int = p[0] & 0x00000001;
        }break;
        case BYTE:
        {
            jbyte* p = (jbyte*)input;
            val->Int = p[0] & 0x000000ff;
        }break;
        case CHAR:
        {
            jchar* p = (jchar*)input;
            val->UInt = p[0] & 0x000000ff;//todo - create mask from sizeof(jchar)
        }break;

        case SHORT:
        {
            jshort* p = (jshort*)input;
            val->Int = p[0] & 0x0000ffff;
        }break;

        case INT:
        {
            jint* p = (jint*)input;
            val->Int = p[0];
        }break;
        case FLOAT:
        {
            jfloat* p = (jfloat*)input;
            val->Float = p[0];
        }break;
        case BOOLEANARRAY:
        case BYTEARRAY:
        case CHARARRAY:
        case SHORTARRAY:
        case INTARRAY:
        case FLOATARRAY:
        {
            //void paramWriteArrayVM(void *input,const NativeType type,slot* sl,const u1 alloc)
            jbyteArray* array = (jbyteArray*)input;
            if(alloc){
                heapAllocElement(array->length,HEAP_ARRAY,&val->stackObj);
                val->stackObj.arrayLength = array->length;
            }
            const NativeType elemType = type - BOOLEANARRAY;
            for(int i = 0; i < array->length;i++)
            {
                slot* elem = heapGetElementRef(val->stackObj.pos + 1 + i);
                paramWriteVM(array->values +(i * sizeOfNativeTypes[elemType]),elemType,elem,0);
            }
            free(array->values);
        }break;

        default:
        {
            DNOTSUPPORTED;
        }break;
    }
}

void kvmWriteReturn(kvm_mem_pointer in,const NativeType type)
{
    slot stackSlot;
    paramWriteVM(in,type,&stackSlot,1);
    opStackPush(stackSlot);
    methodRetun = 1;
}

void kvmParamRead(kvm_mem_pointer out,const NativeType type,const int paramIndex)
{
    const u1 offset = methodInvokedStatic ? 0 : 1;
    const slot slot = opStackGetValue(local + offset + paramIndex);
    paramReadVM( out, type,slot);
}

void kvmParamWrite(kvm_mem_pointer in,const NativeType type,const int paramIndex){
    const u1 offset = methodInvokedStatic ? 0 : 1;
    slot slot = opStackGetValue(local + offset + paramIndex);
    paramWriteVM(in,type,&slot,0);
}


extern void microkernelNativeDispatch(const u2 classId,const u2 methodId);
/*  
 *  The Java Virtual Machine uses local variables to pass parameters on method invocation.
 *  On class method invocation, any parameters are passed in consecutive local variables starting from local variable 0.
 *  On instance method invocation, local variable 0 is always used to pass a reference to the object on which
 *  the instance method is being invoked (this in the Java programming language).
 *  Any parameters are subsequently passed in consecutive local variables starting from local variable 1
 */
u1 nativeDispath(const u1 isInvokeStatic,const u2 classId,const u2 methodId)
{
    methodInvokedStatic = isInvokeStatic;
    methodRetun = 0;

    microkernelNativeDispatch(classId,methodId);

    return methodRetun;
}


kvm_internal jvoid java_lang_Object_notify0(){
#ifdef DEBUG
    if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex != MUTEXBLOCKED)
        ERROREXIT(253,"Notify on not locked mutex");
#endif
    notifyThread(opStackGetValue(local));
}

kvm_internal jvoid java_lang_Object_notifyAll1(){
    const slot object = opStackGetValue(local);
#ifdef DEBUG
    if (HEAPOBJECTMARKER(object.stackObj.pos).mutex != MUTEXBLOCKED)
        ERROREXIT(249,"Wait without blocked mutex");
#endif
    updateThreadState(object, THREADWAITBLOCKED, THREADWAITAWAKENED,0,0,0);
}

kvm_internal jvoid java_lang_Object_wait2(){
    const slot object = opStackGetValue(local);
#ifdef DEBUG
    if (HEAPOBJECTMARKER(object.stackObj.pos).mutex != MUTEXBLOCKED)
    {
        ERROREXIT(254,"Wait without blocked mutex");
    }
#endif
    //can not be ->IllegalMonitorStateException
    // free lock for another thread and lock this
    HEAPOBJECTMARKER(object.stackObj.pos).mutex = MUTEXNOTBLOCKED;

    updateThreadState(object,THREADMUTEXBLOCKED,THREADNOTBLOCKED,1,0,0);

    //its better to change own state after notify, to avoid cycles
    currentThreadCB->isMutexBlockedOrWaitingForObject = object;
    currentThreadCB->state = THREADWAITBLOCKED;
}

kvm_internal jvoid java_lang_Object_waitTime3(jint param0){
}

kvm_internal jint java_lang_Float_floatToIntBits0(jfloat param0){
    return (jint)param0;
}

kvm_internal jfloat java_lang_Float_intBitsToFloat1(jint param0){
    return (jfloat)param0;
}

jint nativeStringOp(const u1 len,const u1 index){
    const slot mySlot = opStackGetValue(local);
    if (mySlot.stackObj.magic == MAGIC_CPSTRING)
    {
        //methodStackPush(cN);
        const u2 classId = (u2) (mySlot.stackObj.classNumber);

        const u2 strPos = getU2(classId,CP(classId, mySlot.stackObj.pos) + 1);
        //cN = methodStackPop();
        if(len != 0){
            const u2 strLen = UTF8_GET_LENGTH(classId,strPos);
            return strLen;
        }else{
            const u2 strFirstChar = CP(classId,strPos) + 3;
            const jchar character = getU1(classId, strFirstChar + index);
            //cN = methodStackPop();
            return character;
        }
    }
    return len ? 0xffff : 0;
}
kvm_internal jint java_lang_String_nativeStringLength0(){
    return nativeStringOp(1,0);
}

kvm_internal jchar java_lang_String_nativeCharAt1(jint charIndex){
    return nativeStringOp(0,charIndex);
}

kvm_internal jvoid java_lang_Thread_start0(){
    cN = opStackGetValue(local).stackObj.classNumber;
    createThread();
}

kvm_internal jvoid java_lang_Thread_yield1(){
    //force scheduling!
    currentThreadCB->numTicks = 0;
}

kvm_internal jvoid java_lang_Thread_sleep2(jint param0){
}

kvm_internal jvoid java_lang_Thread_currentThread3(){
    opStackPush(currentThreadCB->obj);
}

kvm_internal jvoid java_lang_Thread_interrupt4(){}

kvm_internal jboolean java_lang_Thread_interrupted5(){
    return 1;
}

kvm_internal jboolean java_lang_Thread_isInterrupted6(){
    return 0;
}

kvm_internal jvoid java_lang_Thread_join7(){
}

kvm_internal jint java_lang_Runtime_freeMemory3(){
    return heapGetFreeMemory();
}

kvm_internal jint java_lang_Runtime_totalMemory4(){
    return heapGetTotalMemory();
}

kvm_internal jvoid java_lang_Runtime_gc5(){
    heapCollectGarbage();
}

#ifndef KNATIVE_DISPATCH
/* fill this array with classes containing native methods*/
const char* nativeClassNames[] = {
    "platform/PlatForm","java/lang/Runtime","java/lang/Object",
    "java/lang/String",
#ifndef TINYBAJOS_MULTITASKING
    "java/lang/Thread", "java/util/concurrent/Lock",
    "java/util/concurrent/InterruptThread",
#endif
    "java/lang/Float"
};

/*
 !!!IMPORTANT!!!!
 For every class in array nativeClassNames it is necessary to define a array like below.
You need to set a function pointer for every native class in the order of the java class.
For every not native method you must set a !!NULL!! pointer into the array. Also you need
to set NULL for a not explicit defined default
constructor.

EXAMPLE
functionForNativeMethodType functionForNativePlatFormMethod[] =	{
NULL, // << this is for the default constructor, which is not explicit in the java file
nativeCharIn,
NULL, here is a some arbitary normal java function in the java class
nativeCharOut,
nativeExit,
currentTimeMillis
};

After you have written your array, you must set the array into the array at the bottom "funcArray"
at the corresponding position as in array nativeClassNames

In general it is better to set all native methods at the beginning of the class and explicity define the
constructor to save ram.
*/

u2 numNativeClassNames = sizeof(nativeClassNames) / sizeof(char*);

#if (LINUX||AVR32LINUX)
#include "native.h"

functionForNativeMethodType functionForNativePlatFormMethod[] = {
    nativeCharIn,
    nativeCharOut,
    nativeExit,
    currentTimeMillis
};
functionForNativeMethodType functionForNativeRuntimeMethod[] = {
    NULL,//<clinit>
    NULL,//<init>
    NULL,//getRuntime
    nativeFreeMemory,
    nativeTotalMemory,
    nativeCollectGarbage,
};
#endif

functionForNativeMethodType functionForNativeStringMethod[] = {
    nativeStringLength, nativeCharAt
};

#ifndef TINYBAJOS_MULTITASKING
functionForNativeMethodType functionForNativeThreadMethod[] = {
    start, yield,//krix
    ksleep, currentThread, interrupt, interrupted, isInterrupted,
    nativeSetPriority, join
};
#endif

functionForNativeMethodType functionForNativeFloatMethod[] = {
    /*floatToCharArray, nativeParseFloat,*/ typeConvert, typeConvert
};

functionForNativeMethodType functionForNativeObjectMethod[] = {
#ifndef TINYBAJOS_MULTITASKING
    notify, notifyAll, nativeWait, waitTime,
#endif
    //getDataAddress
};

#ifndef TINYBAJOS_MULTITASKING
functionForNativeMethodType functionForNativeLockMethod[] = {
    lock, unlock,
    tryLock, lock,                                //the cloning is correct!
    unlock                                        //the cloning is correct!
};

functionForNativeMethodType functionForNativeInterruptThreadMethod[] = {
    static_sei, static_cli, initInterrupt, removeInterrupt,
    forceInterrupt
};
#endif

/* insert array of function pointer*/
const functionForNativeMethodType* funcArray[] = {//2288bytes
    functionForNativePlatFormMethod,functionForNativeRuntimeMethod,functionForNativeObjectMethod,//332bytes
    functionForNativeStringMethod,//244bytes
#ifndef TINYBAJOS_MULTITASKING
    functionForNativeThreadMethod, functionForNativeLockMethod,//620bytes
    functionForNativeInterruptThreadMethod,//248bytes
#endif
    functionForNativeFloatMethod//844bytes
};
#endif


