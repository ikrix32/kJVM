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
#include "config.h"
#ifndef TINYBAJOS_MULTITASKING
#include <stdio.h>
//#include <stdlib.h>
#include "definitions.h"
#include "typedefinitions.h"
#include "kjvm.h"
#include "classfile.h"
#include "stack.h"
#include "scheduler.h"
#include "heap.h"
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
    if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex == MUTEXBLOCKED) {
        opStackPush(toSlot((u4) 0));
    } else {
        lock();
        opStackPush(toSlot((u4) 1));
    }
    return 1;
}
#endif
