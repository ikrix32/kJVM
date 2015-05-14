/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */

//#include <stdio.h>
//#include <stdlib.h>
#include "definitions.h"
/* insert and update here functionForNativeMethodType-arrays for classes with native methods*/
/* array length ->  at least up to last native method < methods_count*/
/* look at methods in the *.java or *.class file in increasing order */
/* if method is non native -> insert NULL, otherwise pointer to nativce C-function*/

#include <stdlib.h> //malloc
#include "object.h"
#include "nstring.h"
#include "thread.h"
#include "interruptthread.h"
#include "lock.h"
#include "float.h"

#include "stack.h"

//krix
#include "nativedispach.h"

extern u1 local;

u1 sizeOfType(char type,u1 isPointer)
{
    switch (type) {
        case 'B':
        {
            return isPointer ? sizeof(byteArray*) : sizeof(s1);
        }break;
        case 'C':
        {
            return isPointer ? sizeof(charArray*) : sizeof(char);
        }break;
        case 'I':
        {
            return isPointer ? sizeof(intArray*) : sizeof(int);
        }break;
        case 'F':
        {
            return isPointer ? sizeof(floatArray*) : sizeof(float);
        }break;
        case 'Z':
        {//boolean
            return isPointer ? sizeof(byteArray*) : sizeof(s1);
        }break;
        case 'S':
        {
            return isPointer ? sizeof(shortArray*) : sizeof(s2);
        }break;
            
        default:
        {
            DNOTSUPPORTED;
        }break;
    }
    return 0;
}

void* paramReadArray(void* out,char type){
    switch (type) {
        case 'Z':
        case 'B':
        {
            s1* p = (s1*)out;
            p[0] = opStackGetValue(local + 1).Int & 0x000000ff;
            return ++p;
        }break;
        case 'C':
        {
            char* p = (char*)out;
            p[0] = opStackGetValue(local + 1).UInt & 0x000000ff;
            return ++p;
        }break;
        case 'I':
        {
            int* p = (int*)out;
            p[0] = opStackGetValue(local + 1).Int;
            return ++p;
        }break;
        case 'F':
        {
            float* p = (float*)out;
            p[0] = opStackGetValue(local + 1).Float;
            return ++p;
        }break;
        case 'S':
        {
            s2* p = (s2*)out;
            p[0] = opStackGetValue(local + 1).Int & 0x0000ffff;
            return ++p;
        }break;
            
        default:
        {
            DNOTSUPPORTED;
        }break;
    }

    return 0;
}
void* paramRead(void* input,void *output,char type,int isArray){
    switch (type)
    {
        case 'Z':
        case 'B':
        {
            s1* i = (s1*)input;
            s1* o = (s1*)output;
            o[0] = i[0] & 0xff;
            return ++i;
        }break;
        case 'C':
        {
            char* i = (char*)input;
            char* o = (char*)output;
            o[0] = i[0];
            return ++i;
        }break;
        case 'I':
        {
            int* i = (int*)input;
            int* o = (int*)output;
            o[0] = i[0];
            return ++i;
        }break;
        case 'F':
        {
            float* i = (float*)input;
            float* o = (float*)output;
            o[0] = i[0];
            return ++i;
        }break;
        case 'S':
        {
            s2* i = (s2*)input;
            s2* o = (s2*)output;
            o[0] = i[0];
            return ++i;
        }break;

        default:
        {
            DNOTSUPPORTED;
        }break;
    }
    return NULL;
}
void* paramReadVM(void *out,char type,int paramIndex){
    const slot val = opStackGetValue(local + paramIndex);

    switch (type)
    {
        case 'Z':
        case 'B':
        {
            s1* p = (s1*)out;
            p[0] = val.Int & 0x000000ff;
            return ++p;
        }break;
        case 'C':
        {
            char* p = (char*)out;
            p[0] = val.UInt & 0x000000ff;
            return ++p;
        }break;
        case 'I':
        {
            int* p = (int*)out;
            p[0] = val.Int;
            return ++p;
        }break;
        case 'F':
        {
            float* p = (float*)out;
            p[0] = val.Float;
            return ++p;
        }break;
        case 'S':
        {
            s2* p = (s2*)out;
            p[0] = val.Int & 0x0000ffff;
            return ++p;
        }break;
            
        default:
        {
            DNOTSUPPORTED;
        }break;
    }

}
void* nativeDispatch(const u2 classId,const u2 methodId,void* param);
/*  
 *  The Java Virtual Machine uses local variables to pass parameters on method invocation.
 *  On class method invocation, any parameters are passed in consecutive local variables starting from local variable 0.
 *  On instance method invocation, local variable 0 is always used to pass a reference to the object on which
 *  the instance method is being invoked (this in the Java programming language).
 *  Any parameters are subsequently passed in consecutive local variables starting from local variable 1
 */
 u1 nativeDispath(const u1 isInvokeStatic,const u2 classId,const u2 methodId,const char* methodName,const char* methodDescription){
     // PRINTF("Calling native method:%s with description:%s\n",methodName,methodDescription);
    
    void* param = NULL;
    
    //coumpute parameters size
    int i = 0;
    int pSize = 0;
    //parse parameters
    while (methodDescription[i] != ')')
    {
        if(methodDescription[i] == '[') {
            i++;
            pSize += sizeOfType(methodDescription[i],1);
        }else if(methodDescription[i] != '(')
            pSize += sizeOfType(methodDescription[i],0);
        i++;
    }
    param = malloc((size_t) pSize);

    void* parameters = param;

    i = 0;
    int paramIndex = isInvokeStatic ? 0 : 1;
    //parse parameters
    while (methodDescription[i] != ')')
    {
        if (methodDescription[i] == '['){
            i++;
            param = paramReadArray( param, methodDescription[i]);
            paramIndex++;
        }else if(methodDescription[i] != '('){
            param = paramReadVM(param, methodDescription[i],paramIndex);
            paramIndex++;
        }
        i++;
    }
    nativeDispatch(classId,methodId,parameters);
    return 1;
}

void nativeCall(const u2 classId,const u2 methodId,void* paramters){
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

/* Native method dispatcher - AUTOGENERATED by jvm exporter, DO NOT CHANGE!!!*/
extern void* paramRead(void* in,void *out,char type,int paramIndex);

void tests_NativeMethodsTest_nativeVoidMetod1(){
    PRINTF("Void method\n");
}

byte tests_NativeMethodsTest_nativeMethod2(byte param0){
    PRINTF("Void method:%d\n",param0);
    return param0;
}

char tests_NativeMethodsTest_nativeMethod3(char param0){
    PRINTF("Void method:%c\n",param0);
    return param0;
}

int tests_NativeMethodsTest_nativeMethod4(int param0){
    PRINTF("Void method:%d\n",param0);
    return param0;
}

float tests_NativeMethodsTest_nativeMethod5(float param0){
    PRINTF("Void method:%f\n",param0);
    return param0;
}

void tests_NativeMethodsTest_nativeMethod6(byte param0,char param1,int param2,float param3){
    PRINTF("Void method:byte:%d,char:%d,int:%d,float:%f\n",param0,param1,param2,param3);
}

void tests_NativeMethodsTest_nativeMethod7(byteArray param0){
    PRINTF("Void method\n");
}

void tests_NativeMethodsTest_nativeMethod8(charArray param0){
    PRINTF("Void method\n");
}

void tests_NativeMethodsTest_nativeMethod9(intArray param0){
    PRINTF("Void method\n");
}

void tests_NativeMethodsTest_nativeMethod10(floatArray param0){
    PRINTF("Void method\n");
}

byteArray tests_NativeMethodsTest_nativeMethod11(){
    PRINTF("Void method\n");
    byteArray x;
    return x;
}

void* nativeDispatch(const u2 classId,const u2 methodId,void* param){
    void* ret = NULL;

    if(classId == 46 && methodId == 1){
        tests_NativeMethodsTest_nativeVoidMetod1();
    }
    if(classId == 46 && methodId == 2){
        byte param0; param = paramRead(param,&param0,'B',0);
        byte r = tests_NativeMethodsTest_nativeMethod2( param0);
        ret = &r;
    }
    if(classId == 46 && methodId == 3){
        char param0; param = paramRead(param,&param0,'C',0);
        char r = tests_NativeMethodsTest_nativeMethod3( param0);
        ret = &r;
    }
    if(classId == 46 && methodId == 4){
        int param0; param = paramRead(param,&param0,'I',0);
        int r = tests_NativeMethodsTest_nativeMethod4( param0);
        ret = &r;
    }
    if(classId == 46 && methodId == 5){
        float param0; param = paramRead(param,&param0,'F',0);
        float r = tests_NativeMethodsTest_nativeMethod5( param0);
        ret = &r;
    }
    if(classId == 46 && methodId == 6){
        byte param0; param = paramRead(param,&param0,'B',0);
        char param1; param = paramRead(param,&param1,'C',0);
        int param2; param = paramRead(param,&param2,'I',0);
        float param3; param = paramRead(param,&param3,'F',0);
        tests_NativeMethodsTest_nativeMethod6( param0, param1, param2, param3);
    }
    if(classId == 46 && methodId == 7){
        byteArray param0; param = paramRead(param,&param0,'B',1);
        tests_NativeMethodsTest_nativeMethod7( param0);
    }
    if(classId == 46 && methodId == 8){
        charArray param0; param = paramRead(param,&param0,'C',1);
        tests_NativeMethodsTest_nativeMethod8( param0);
    }
    if(classId == 46 && methodId == 9){
        intArray param0; param = paramRead(param,&param0,'I',1);
        tests_NativeMethodsTest_nativeMethod9( param0);
    }
    if(classId == 46 && methodId == 10){
        floatArray param0; param = paramRead(param,&param0,'F',1);
        tests_NativeMethodsTest_nativeMethod10( param0);
    }
    if(classId == 46 && methodId == 11){
        byteArray r = tests_NativeMethodsTest_nativeMethod11();
        ret = &r;
    }	return ret;
}

