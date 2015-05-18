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
#include "object.h"
#include "nstring.h"
#include "thread.h"
#include "interruptthread.h"
#include "lock.h"
#include "float.h"

#include "stack.h"
#include "heap.h"

extern u1 local;

u1 methodInvokedStatic = 0;
u1 methodRetun = 0;

u1 sizeOfNativeTypes[] = {0,sizeof(jboolean),sizeof(jbyte),sizeof(jchar),
                            sizeof(jshort),sizeof(jint),sizeof(jfloat),
                            sizeof(jbooleanArray),sizeof(jbyteArray),sizeof(jcharArray),
                            sizeof(jshortArray),sizeof(jintArray),sizeof(jfloatArray)};

NativeType charToType(const char type[2])
{
    const int tIndex = type[0] == '[' ? 1 : 0;

    if(type[tIndex] == 'V') return VOID;
    if(type[tIndex] == 'B') return tIndex == 0 ? BYTE : BYTEARRAY;
    if(type[tIndex] == 'Z') return tIndex == 0 ? BOOLEAN : BOOLEANARRAY;
    if(type[tIndex] == 'C') return tIndex == 0 ? CHAR : CHARARRAY;

    if(type[tIndex] == 'S') return tIndex == 0 ? SHORT: SHORTARRAY;
    if(type[tIndex] == 'I') return tIndex == 0 ? INT  : INTARRAY;
    if(type[tIndex] == 'F') return tIndex == 0 ? FLOAT: FLOATARRAY;

    DNOTSUPPORTED;

    return 0;
}

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

        default:
        {
            DNOTSUPPORTED;
        }break;
    }
}

void paramReadArrayVM(void *out,const NativeType type,const slot sl)
{
    if(sl.stackObj.magic == MAGIC_OBJECT)
    {
        const slot heapObject = HEAPOBJECT(sl.stackObj.pos);

        switch (type) {
            case BOOLEANARRAY:
            {
                jbooleanArray* array = (jbooleanArray*)out;

                array->length = heapObject.heapObjMarker.length - 1;
                array->values = (jboolean*)malloc(array->length * sizeof(jboolean));

                for(int i = 0; i < array->length;i++)
                {
                    const slot val = HEAPOBJECT(sl.stackObj.pos + 1 + i);
                    paramReadVM(array->values + i,BOOLEAN,val);
                }
            }break;
            case BYTEARRAY:
            {
                jbyteArray* array = (jbyteArray*)out;

                array->length = heapObject.heapObjMarker.length - 1;
                array->values = (jbyte*)malloc(array->length * sizeof(jbyte));

                for(int i = 0; i < array->length;i++)
                {
                    const slot val = HEAPOBJECT(sl.stackObj.pos + 1 + i);
                    paramReadVM(array->values + i,BYTE,val);
                }
            }break;
            case CHARARRAY:
            {
                jcharArray* array = (jcharArray*)out;

                array->length = heapObject.heapObjMarker.length - 1;
                array->values = (jchar*)malloc(array->length * sizeof(jchar));

                for(int i = 0; i < array->length;i++)
                {
                    const slot val = HEAPOBJECT(sl.stackObj.pos + 1 + i);
                    paramReadVM(array->values + i,CHAR,val);
                }
            }break;
            case SHORTARRAY:
            {
                jshortArray* array = (jshortArray*)out;

                array->length = heapObject.heapObjMarker.length - 1;
                array->values = (jshort*)malloc(array->length * sizeof(jshort));

                for(int i = 0; i < array->length;i++)
                {
                    const slot val = HEAPOBJECT(sl.stackObj.pos + 1 + i);
                    paramReadVM(array->values + i,SHORT,val);
                }
            }break;
            case INTARRAY:
            {
                jintArray* array = (jintArray*)out;

                array->length = heapObject.heapObjMarker.length - 1;
                array->values = (jint*)malloc(array->length * sizeof(jint));

                for(int i = 0; i < array->length;i++)
                {
                    const slot val = HEAPOBJECT(sl.stackObj.pos + 1 + i);
                    paramReadVM(array->values + i,INT,val);
                }
            }break;
            case FLOATARRAY:
            {
                jfloatArray* array = (jfloatArray*)out;

                array->length = heapObject.heapObjMarker.length - 1;
                array->values = (jfloat*)malloc(array->length * sizeof(jfloat));

                for(int i = 0; i < array->length;i++)
                {
                    const slot val = HEAPOBJECT(sl.stackObj.pos + 1 + i);
                    paramReadVM(array->values + i,FLOAT,val);
                }
            }break;
            default:{
                DNOTSUPPORTED;
            }break;
        }
    }
}

void paramWriteVM(void *input,const NativeType type,slot* val)
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

        default:
        {
            DNOTSUPPORTED;
        }break;
    }
}

void paramWriteArrayVM(void *input,const NativeType type,slot* sl,const u1 alloc)
{
    switch (type) {
        case BOOLEANARRAY:
        {
            jbooleanArray* array = (jbooleanArray*)input;
            if(alloc){
                heapAllocElement(array->length,HEAP_ARRAY,&sl->stackObj);
                sl->stackObj.arrayLength = array->length;
            }
            for(int i = 0; i < array->length;i++)
            {
                slot* val = heapGetElementRef(sl->stackObj.pos + 1 + i);
                paramWriteVM(array->values + i,BOOLEAN,val);

            }
            free(array->values);
        }break;
        case BYTEARRAY:
        {
            jbyteArray* array = (jbyteArray*)input;
            if(alloc){
                heapAllocElement(array->length,HEAP_ARRAY,&sl->stackObj);
                sl->stackObj.arrayLength = array->length;
            }
            for(int i = 0; i < array->length;i++)
            {
                slot* val = heapGetElementRef(sl->stackObj.pos + 1 + i);
                paramWriteVM(array->values + i,BYTE,val);
            }
            free(array->values);
        }break;
        case CHARARRAY:
        {
            jcharArray* array = (jcharArray*)input;
            if(alloc){
                heapAllocElement(array->length,HEAP_ARRAY,&sl->stackObj);
                sl->stackObj.arrayLength = array->length;
            }
            for(int i = 0; i < array->length;i++)
            {
                slot* val = heapGetElementRef(sl->stackObj.pos + 1 + i);
                paramWriteVM(array->values + i,CHAR,val);
            }
            free(array->values);
        }break;
        case SHORTARRAY:
        {
            jshortArray* array = (jshortArray*)input;
            if(alloc){
                heapAllocElement(array->length,HEAP_ARRAY,&sl->stackObj);
                sl->stackObj.arrayLength = array->length;
            }
            for(int i = 0; i < array->length;i++)
            {
                slot* val = heapGetElementRef(sl->stackObj.pos + 1 + i);
                paramWriteVM(array->values + i,SHORT,val);
            }
            free(array->values);
        }break;
        case INTARRAY:
        {
            jintArray* array = (jintArray*)input;
            if(alloc){
                heapAllocElement(array->length,HEAP_ARRAY,&sl->stackObj);
                sl->stackObj.arrayLength = array->length;
            }
            for(int i = 0; i < array->length;i++)
            {
                slot* val = heapGetElementRef(sl->stackObj.pos + 1 + i);
                paramWriteVM(array->values + i,INT,val);
            }
            free(array->values);
        }break;
        case FLOATARRAY:
        {
            jfloatArray* array = (jfloatArray*)input;
            if(alloc){
                heapAllocElement(array->length,HEAP_ARRAY,&sl->stackObj);
                sl->stackObj.arrayLength = array->length;
            }
            for(int i = 0; i < array->length;i++)
            {
                slot* val = heapGetElementRef(sl->stackObj.pos + 1 + i);
                paramWriteVM(array->values + i,FLOAT,val);
            }
            free(array->values);
        }
        default:{
            DNOTSUPPORTED;
        }break;
    }
}

void kvmWriteReturn(kvm_mem_pointer in,const NativeType type)
{
    slot stackSlot;
    if(type < BOOLEANARRAY){
        paramWriteVM(in,type,&stackSlot);
    }else{
        paramWriteArrayVM(in,type,&stackSlot,1);
    }
    opStackPush(stackSlot);
    methodRetun = 1;
}

void kvmParamRead(kvm_mem_pointer out,const NativeType type,const int paramIndex)
{
    const u1 offset = methodInvokedStatic ? 0 : 1;
    const slot slot = opStackGetValue(local + offset + paramIndex);
    if (type < BOOLEANARRAY){
        paramReadVM( out, type,slot);
    }else {
        paramReadArrayVM( out, type,slot);
    }
}

void kvmParamWrite(kvm_mem_pointer in,const NativeType type,const int paramIndex){
    const u1 offset = methodInvokedStatic ? 0 : 1;
    slot slot = opStackGetValue(local + offset + paramIndex);
    paramWriteArrayVM(in,type,&slot,0);
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

//end krix

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

