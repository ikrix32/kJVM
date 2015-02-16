/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
/* C-functions for native methods*/
/* native void method -> C-function ret value 0*/
/* native non void method -> c-function ret value 1 (ret value on java -opStack)*/
/* remember:*/
/* invokespecial Operand Stack*/
/* ..., objectref, [arg0, [arg1 ...]] -> ...*/
/* invokestatic: Operand Stack*/
/* ..., [arg0, [arg1 ...]] -> ...*/
#ifndef TINYBAJOS_MULTITASKING
#include <stdio.h>
#include <stdlib.h>
#include "../JVM/definitions.h"
#include "../JVM/typedefinitions.h"
#include "../JVM/bajvm.h"
#include "../JVM/classfile.h"
#include "../JVM/stack.h"
#include "../JVM/bajvm.h"
#include "../JVM/scheduler.h"
#include "../JVM/heap.h"
#include "lock.h"
#include "interruptthread.h"
#ifdef AVR8
#include <avr/interrupt.h>
#endif

char static_sei()
{
#ifdef AVR8
    sei();
#endif
    return 0;
}


char static_cli()
{
#ifdef AVR8
    cli();
#endif
    return 0;
}


char initInterrupt()
{

    u1 vectorNr = opStackGetValue(local + 1).UInt;

    //interruptNr not valid
    if (vectorNr >= NUMBEROFINTERRUPTS)
        return 0;

    interruptVectors[vectorNr] = findThreadCB(opStackGetValue(local));

    return 0;
}


char removeInterrupt()
{
    u1 vectorNr = opStackGetValue(local).UInt;
    //interruptNr not valid
    if (vectorNr >= NUMBEROFINTERRUPTS)
        return 0;
    deleteNotCurrentThread(&(interruptVectors[vectorNr]));
    return 0;
}


char forceInterrupt()
{
    u1 vectorNr = opStackGetValue(local).UInt;
    //interruptNr not valid
    if (vectorNr >= NUMBEROFINTERRUPTS)
        return 0;
    interruptThread(interruptVectors[vectorNr]);
    return 0;
}
#endif
