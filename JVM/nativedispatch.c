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
typedef struct byteArray
{
    s1* values;
    u1  length;
} byteArray;

typedef struct charArray
{
    char* values;
    u1    length;
} charArray;

typedef struct shortArray
{
    s2*  values;
    u1   length;
} shortArray;

typedef struct intArray
{
    int* values;
    u1   length;
} intArray;

typedef struct floatArray
{
    float*  values;
    u1      length;
} floatArray;

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

void* paramRead(void* out,char type,int paramIndex){
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

u1 nativeDispath(const char* methodName,const char* methodDescription){
    PRINTF("Calling native method:%s with description:%s\n",methodName,methodDescription);
    
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
        }else
            pSize += sizeOfType(methodDescription[i],0);
        i++;
    }
    param = malloc((size_t) pSize);
    i = 0;
    int paramIndex = 0;
    //parse parameters
    while (methodDescription[i] != ')')
    {
        if (methodDescription[i] == '['){
            i++;
            param = paramReadArray( param, methodDescription[i]);
            paramIndex++;
        }else{
            param = paramRead(param, methodDescription[i],paramIndex);
            paramIndex++;
        }
        i++;
    }
    
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
    getDataAddress
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
