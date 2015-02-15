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

#include <stdio.h>
#include <stdlib.h>
#include "../JVM/definitions.h"
#include "../JVM/typedefinitions.h"
#include "../JVM/bajvm.h"
#include "../JVM/classfile.h"
#include "../JVM/stack.h"
#include "../JVM/bajvm.h"
#include "../JVM/heap.h"
#include "float.h"

char floatToCharArray()
{
    slot mySlot;
    f4 f = opStackGetValue(local).Float;          /* the float*/
    char buf[8];
    u1 i;
    for (i = 0; i < 8; ++i)
    {
        buf[i] = 0;
    }
#ifdef AVR8
    dtostrf( f, 7, 3, buf );
#else
    snprintf(buf, 7, "%f", f);
#endif
    u2 heapPos = getFreeHeapSpace(8 + 1);         /* char arr length + marker*/
    mySlot.stackObj.pos = heapPos;
    mySlot.stackObj.magic = OBJECTMAGIC;
/*mySlot.stackObj.type=STACKNEWARRAYOBJECT;*/
    mySlot.stackObj.arrayLength = (u1) 8;         /* char array length*/
    opStackPush(mySlot);
    HEAPOBJECTMARKER(heapPos).status = HEAPALLOCATEDARRAY;
    HEAPOBJECTMARKER(heapPos).magic = OBJECTMAGIC;
    HEAPOBJECTMARKER(heapPos++).mutex = MUTEXNOTBLOCKED;
    for (i = 0; i < 8; i++)
    {
        heapSetElement((slot) (u4)(*(buf + i)), heapPos++);
    }
    return 1;
}


char typeConvert()
{
    opStackPush(opStackGetValue(local));
    return 1;
}


/* char arr to float*/
char nativeParseFloat()
{
    slot mySlot = opStackGetValue(local);         /* the char array*/
    char buf[mySlot.stackObj.arrayLength];
    f4 f;
    u4 i;
    for (i = 0; i < mySlot.stackObj.arrayLength; i++)
        buf[i] = (u1) heapGetElement(mySlot.stackObj.pos + i + 1).UInt;
    buf[mySlot.stackObj.arrayLength] = 0;
#ifdef AVR8
    f=strtod(buf,NULL);
#else
    sscanf(buf, "%f", &f);
#endif
    opStackPush((slot) f);
    return 1;
}
