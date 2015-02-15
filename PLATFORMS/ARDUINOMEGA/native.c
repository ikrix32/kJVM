/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */

/* remember:*/
/* invokespecial Operand Stack*/
/* ..., objectref, [arg0, [arg1 ...]] -> ...*/
/* invokestatic: Operand Stack*/
/* ..., [arg0, [arg1 ...]] -> ...*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef AVR8
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#endif
#include "../../JVM/definitions.h"
#include "../../JVM/typedefinitions.h"
#include "../../JVM/bajvm.h"
#include "../../JVM/stack.h"
#include "../../JVM/classfile.h"
#include "../../JVM/scheduler.h"
#include "../../JVM/heap.h"

#include "native.h"
#include "platform.h"

#ifdef WITHMON
#define     STRING(a,b)     #a" "#b
#define     INLINEASM(a,b)  STRING(a,b)
extern char (*conIn)();
#endif

char nativeCharIn()
{
#ifdef WITHMON
    opStackPush((slot)(u4)conIn());
#else
    opStackPush((slot) (u4) uart_getchar(&uartAVR8));
#endif
    return 1;
}


char nativeCharOut()
{
    char val = opStackGetValue(local + 1).UInt;

    printf("%c", val);

    return 0;
}


char nativeExit()
{
    exit(-1);
    return 0;
}


char currentTimeMillis()
{
#ifdef WITHMON
    opStackPush((slot)(u4)((*(u4*)SYSTIMEMILLISEC)&0x7ffff));
#else
    opStackPush((slot) (u4) timerMilliSec);
#endif
    return 1;
}


/* added 2009 by: Friedrich Gro�e & Rainer Kirchhoff, FHW-BA Berlin*/
/* Deutsche Post IT-Services GmbH & MSA Auer GmbH*/
char nativeConStat()
{
    opStackPush((slot)(u4)((UCSR0A >> RXC0) & 0x01));
    return 1;
}


char nativeSetData()                              //sdram
{
/*Get possible 16-Bit adr.*/
    u2 portAddr = opStackGetValue(local + 1).bytes[0];
    portAddr |= (opStackGetValue(local + 1).bytes[1] << 8);
/*Get value*/
    u2 portValue = opStackGetValue(local + 2).bytes[0];
/*Assign Value to Port (Adr)*/
    *((u2*) portAddr) = portValue;
    return 0;
}


char nativeGetData()                              //sdarm
{
/*Get possible 16-Bit adr.*/
    u2 portAddr = opStackGetValue(local + 1).bytes[0];
    portAddr |= opStackGetValue(local + 1).bytes[1] << 8;
    opStackPush((slot) (u4) (char) *((u2*) portAddr));
    return 1;
}
