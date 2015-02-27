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
//#include <stdlib.h>
#include "definitions.h"
#include "typedefinitions.h"
#include "kjvm.h"
#include "classfile.h"
#include "stack.h"
#include "scheduler.h"
#include "heap.h"
#include "object.h"
#ifndef TINYBAJOS_MULTITASKING
/* "java/lang/Object","notify","notifyAll","wait","waitTime","getDataAddress"*/
char notify()
{
    if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex
        != MUTEXBLOCKED)
    {
        exit(253);
    }

    notifyThread(opStackGetValue(local));
    return 0;
}


char notifyAll()
{
    ThreadControlBlock* cb;
    if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex != MUTEXBLOCKED)
    {
        exit(249);
    }
    /*can not be ->IllegalMonitorStateException*/
    for (int i = 0; i < (MAXPRIORITY); i++)
    {
        const u1 max = (threadPriorities[i].count);
        cb = threadPriorities[i].cb;
        for (int k = 0; k < max; k++)
        {
            if ((cb->state == THREADWAITBLOCKED)
            && ((cb->isMutexBlockedOrWaitingForObject).UInt == opStackGetValue(local).UInt))
                cb->state = THREADWAITAWAKENED;
            cb = cb->succ;
        }
    }
    return 0;
}


char nativeWait()
{
    if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex != MUTEXBLOCKED)
    {
        exit(254);
    }
    /*can not be ->IllegalMonitorStateException*/
    HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex= MUTEXNOTBLOCKED;/* free lock for another thread and lock this */

    ThreadControlBlock* myTCB;
    for (int i = 0; i < (MAXPRIORITY); i++)
    {
        const u1 max = (threadPriorities[i].count);
        myTCB = threadPriorities[i].cb;
        for (int k = 0; k < max; k++)
        {
            //alle blocked for object wecken!
            if ((myTCB->isMutexBlockedOrWaitingForObject.UInt == opStackGetValue(local).UInt)
            && (myTCB->state == THREADMUTEXBLOCKED))
            {
                myTCB->state = THREADNOTBLOCKED;  //!!
                myTCB->isMutexBlockedOrWaitingForObject = NULLOBJECT;
            }

            myTCB = myTCB->succ;
        }
    }
    //its better to change own state after notify, to avoid cycles
    currentThreadCB->isMutexBlockedOrWaitingForObject = opStackGetValue(local);
    currentThreadCB->state = THREADWAITBLOCKED;
    return 0;
}


char waitTime()
{
    return 0;
}
#endif

char getDataAddress()
{
    return 1;/* ret val is  on Stack !!*/
}
