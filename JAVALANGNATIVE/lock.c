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

char lock()
{
    setMutexOnObject(currentThreadCB, opStackGetValue(local));
    return 0;
}


char unlock()
{
    releaseMutexOnObject(currentThreadCB, opStackGetValue(local), NULL);
    return 0;
}


char tryLock()
{
    if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex
        == MUTEXBLOCKED)
    {
        opStackPush(toSlot((u4) 0));
    }
    else
    {
        lock();
        opStackPush(toSlot((u4) 1));
    }
    return 1;
}
#endif
