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
#include "../JVM/stack.h"
#include "string.h"

char nativeStringLength()
{
    slot mySlot = opStackGetValue(local);
    if (mySlot.stackObj.magic == CPSTRINGMAGIC)
    {
        methodStackPush(cN);
        cN = (u1) (mySlot.stackObj.classNumber);
        opStackPush( toSlot( (u4) getU2( CP( cN, getU2( CP( cN, mySlot.stackObj.pos) + 1)) + 1)));
        cN = methodStackPop();
    } else
        opStackPush(toSlot((u4) 0xffff));
    return 1;
}


char nativeCharAt()
{
    slot mySlot = opStackGetValue(local);
    if (mySlot.stackObj.magic == CPSTRINGMAGIC)
    {
        methodStackPush(cN);
        cN = (u1) (mySlot.stackObj.classNumber);
        opStackPush(toSlot((u4) getU1(CP(cN,getU2(CP(cN,mySlot.stackObj.pos) + 1))
                                      + 3 + (u2) opStackGetValue(local + 1).UInt)));
        cN = methodStackPop();
    } else
        opStackPush(toSlot((u4) 0));
    return 1;
}
