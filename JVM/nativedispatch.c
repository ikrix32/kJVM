/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
#include <stdio.h>
#include <stdlib.h>
#include "typedefinitions.h"
/* insert and update here functionForNativeMethodType-arrays for classes with native methods*/
/* array length ->  at least up to last native method < methods_count*/
/* look at methods in the *.java or *.class file in increasing order */
/* if method is non native -> insert NULL, otherwise pointer to nativce C-function*/

#include "../JAVALANGNATIVE/object.h"
#include "../JAVALANGNATIVE/string.h"
#include "../JAVALANGNATIVE/thread.h"
#include "../JAVALANGNATIVE/interruptthread.h"
#include "../JAVALANGNATIVE/lock.h"
#include "../JAVALANGNATIVE/float.h"
/* fill this array with classes containing native methods*/

const char* nativeClassNames[] =
{
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
#include "../PLATFORMS/LINUX/native.h"

functionForNativeMethodType functionForNativePlatFormMethod[] =
{
    nativeCharIn,
    nativeCharOut,
    nativeExit,
    currentTimeMillis
};
#endif

#ifdef CH
#include "../PLATFORMS/CHARON/native.h"

functionForNativeMethodType functionForNativePlatFormMethod[] =
{
    nativeCharIn,
    nativeCharOut,
    nativeExit,
    currentTimeMillis,
    charLCDOut,
    controlLCD,
    getTemperature,
    getAnalogueValue,
    nativeConStat,
    nativeSetBarGraph,
    nativeGetDIL,
    nativeSetPort,
    nativeGetPort,
    nativeSer1CharIn,
    nativeSer1CharOut,
    nativeSer1Stat
};
#endif

#ifdef AM
#include "../PLATFORMS/ARDUINOMEGA/native.h"

functionForNativeMethodType functionForNativePlatFormMethod[] =
{
    nativeCharIn,
    nativeCharOut,
    nativeExit,
    currentTimeMillis,
    nativeConStat,
    nativeSetData,
    nativeGetData,
};
#endif

#ifdef XPLAIN
#include "../PLATFORMS/XMEGA/native.h"

functionForNativeMethodType functionForNativePlatFormMethod[] =
{
    nativeCharIn,
    nativeCharOut,
    nativeExit,
    currentTimeMillis,
    nativeConStat,
    nativeSetData,
    nativeGetData
};
#endif

#ifdef EVK1100
#include "../PLATFORMS/EVK1100/native.h"

functionForNativeMethodType functionForNativePlatFormMethod[] =
{
    nativeCharIn,                                 /* for our board only TXD,RXD works correct on UART0*/
    nativeCharOut,
    conStat,
    getButtons,
    setOnBoardLEDs,
    charLCDOut,
    controlLCD,
    nativeExit,
    currentTimeMillis,
    ser1In,
    ser1Out,
    ser1Stat,
    ser1RTS,
    ser1DTR,
    pwmStart,
    pwmStop,
    adcGetValue
/* usb, ether, sdcard*/
};
#endif

#ifdef EVK1104
#include "../PLATFORMS/EVK1104/native.h"

functionForNativeMethodType functionForNativePlatFormMethod[] =
{
    nativeCharIn,                                 /* for our board only TXD,RXD works correct on UART0*/
    nativeCharOut,
    conStat,
    nativeExit,
    currentTimeMillis,
    initDisplay,
    clearDisplay,
    drawPixel,
    drawHorizLine,
    drawVertLine,
    drawLine,
    drawFillRect,
    drawFilledCircle,
    drawChar,
    getButtons,
    isButtonPressed,
    setLEDon,
    setLEDoff,
    adcInit,
    adcEnable,
    adcDisable,
    adcGetValue
};
#endif

#ifdef NGW100
#include "../PLATFORMS/NGW100/native.h"
functionForNativeMethodType functionForNativePlatFormMethod[] =
{
    nativeCharIn,
    nativeCharOut,
    nativeExit,
    currentTimeMillis
};
#endif

#ifdef STK1000
#include "../PLATFORMS/STK1000/native.h"
functionForNativeMethodType functionForNativePlatFormMethod[] =
{
    nativeCharIn,
    nativeCharOut,
    nativeExit,
    currentTimeMillis,
/*Grafik-Methoden*/
    drawPointRGB,
    drawFillRectRGB,
    drawRectangleRGB,
    clearScreenRGB,
    drawRoundRectRGB,
    drawEllipseRGB,
    drawCircleRGB,
    drawLineRGB,
    drawPointHSB,
    drawLineHSB,
    drawTriangleFill,
    clearZBuffer,
    drawPointHSBZBuffer,
    drawLineHSBZBuffer,
/*Schrift-Methoden*/
    setFont,
    setFontWindow,
    setFontAutoLineFeed,
    setFontFixedFont,
    setFontCursor,
    setFontColor,
    drawChar,
    drawCharAt,
    getCharWidth
};
#endif

functionForNativeMethodType functionForNativeStringMethod[] =
{
    nativeStringLength, nativeCharAt
};

#ifndef TINYBAJOS_MULTITASKING
functionForNativeMethodType functionForNativeThreadMethod[] =
{
    start, yield,//krix
    ksleep, currentThread, interrupt, interrupted, isInterrupted,
    nativeSetPriority, join
};
#endif

functionForNativeMethodType functionForNativeFloatMethod[] =
{
    floatToCharArray, nativeParseFloat, typeConvert, typeConvert
};

functionForNativeMethodType functionForNativeObjectMethod[] =
{
#ifndef TINYBAJOS_MULTITASKING
    notify, notifyAll, nativeWait, waitTime,
#endif
    getDataAddress
};

#ifndef TINYBAJOS_MULTITASKING
functionForNativeMethodType functionForNativeLockMethod[] =
{
    lock, unlock,
    tryLock, lock,                                //the cloning is correct!
    unlock                                        //the cloning is correct!
};

functionForNativeMethodType functionForNativeInterruptThreadMethod[] =
{
    static_sei, static_cli, initInterrupt, removeInterrupt,
    forceInterrupt
};
#endif

/* insert array of function pointer*/
const functionForNativeMethodType* funcArray[] =
{
    functionForNativePlatFormMethod, functionForNativeObjectMethod,
    functionForNativeStringMethod,
#ifndef TINYBAJOS_MULTITASKING
    functionForNativeThreadMethod, functionForNativeLockMethod,
    functionForNativeInterruptThreadMethod,
#endif
    functionForNativeFloatMethod
};
