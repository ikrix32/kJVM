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
#include "stack.h"
#include "thread.h"

char start()
{
    cN = opStackGetValue(local).stackObj.classNumber;
    createThread();
    return 0;
}


char yield()                                      //by ceh
{
    //force scheduling!
    currentThreadCB->numTicks = 0;
    return 0;
}


char currentThread()
{
    opStackPush(currentThreadCB->obj);
    return 1;
}


char nativeSetPriority()                          //by ceh
{
    // thread is alive -> thread control block exists
    // cN -> of method (Thread)
    const u1 newPrio = opStackGetValue(local + 1).UInt;
    const slot soi = opStackGetValue(local);
    cN = soi.stackObj.classNumber;                // of object, which calls the method
    //if (!findFieldByRamName("priority", 8, "I", 1))
    if (!findFieldByName(cN, cN,"priority", 8, "I", 1,0))
    {
        ERROREXIT(78,"field priority not found\n");
    }

    // position of int field priority of the thread creating object
    const u4* pCurrentPrio = (u4*) (heapGetBase() + soi.stackObj.pos + fN + 1);
    if (newPrio == (*pCurrentPrio))
        return 0;                                 // nothing to do
                                                  // search thread control block af calling object
    ThreadControlBlock* found = findThreadCB(soi);

    removeThreadFromPriorityList(found);
    *(found->pPriority) = newPrio;
    insertThreadIntoPriorityList(found);
    return 0;
}


//to do
char interrupt()
{
    return 0;
}


char interrupted()
{
    return 1;
}


char isInterrupted()
{
    return 1;
}


char ksleep()//krix
{
    return 0;
}


char join()
{
    return 0;
}


char jointimeout()
{
    return 0;
}


char nativeSleep()
{
    return 0;
}
#endif
