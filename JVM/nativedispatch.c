/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
#include <stdio.h>
//#include <stdlib.h>
#include "typedefinitions.h"
/* insert and update here functionForNativeMethodType-arrays for classes with native methods*/
/* array length ->  at least up to last native method < methods_count*/
/* look at methods in the *.java or *.class file in increasing order */
/* if method is non native -> insert NULL, otherwise pointer to nativce C-function*/

#include "object.h"
#include "nstring.h"
#include "thread.h"
#include "interruptthread.h"
#include "lock.h"
#include "float.h"

//krix
#include "nativedispach.h"

u1 nativeDispath(const char* methodName,const char* methodDescription){
    printf("Calling native method:%s with description:%s\n",methodName,methodDescription);
    return 1;
}

//end krix

/* fill this array with classes containing native methods*/

const char* nativeClassNames[] = {
    "platform/PlatForm", "java/lang/Object",
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
    floatToCharArray, nativeParseFloat, typeConvert, typeConvert
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
    functionForNativePlatFormMethod, functionForNativeObjectMethod,//332bytes
    functionForNativeStringMethod,//244bytes
#ifndef TINYBAJOS_MULTITASKING
    functionForNativeThreadMethod, functionForNativeLockMethod,//620bytes
    functionForNativeInterruptThreadMethod,//248bytes
#endif
    functionForNativeFloatMethod//844bytes
};
