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

//#include <stdio.h>
//#include <stdlib.h>
#include "kjvm.h"
#include "stack.h"
#include "object.h"
#include "heap.h"

#ifndef TINYBAJOS_MULTITASKING
/* "java/lang/Object","notify","notifyAll","wait","waitTime","getDataAddress"*/
char notify()
{
    if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex != MUTEXBLOCKED)
    {
        ERROREXIT(253,"Notify on not locked mutex");
    }

    notifyThread(opStackGetValue(local));
    return 0;
}

void updateThreadState(const u1 newThreadState,const slot object)
{
    const u1 oldThreadState = newThreadState == THREADNOTBLOCKED ? THREADMUTEXBLOCKED : THREADWAITBLOCKED;

    /*can not be ->IllegalMonitorStateException*/
    ThreadControlBlock* cb = threadList.cb;
    for (int k = 0; k < threadList.count; k++)
    {
        if (cb->state == oldThreadState
        && (cb->isMutexBlockedOrWaitingForObject).UInt == object.UInt)
        {
            cb->state = newThreadState;
            if(newThreadState == THREADNOTBLOCKED)
                cb->isMutexBlockedOrWaitingForObject = NULLOBJECT;
        }
        cb = cb->succ;
    }
}
char notifyAll()
{
    const slot obj = opStackGetValue(local);
    if (HEAPOBJECTMARKER(obj.stackObj.pos).mutex != MUTEXBLOCKED)
    {
        exit(249);
    }
    updateThreadState(THREADWAITAWAKENED, obj);
    
    return 0;
}

char nativeWait()
{
    const slot object = opStackGetValue(local);
    if (HEAPOBJECTMARKER(object.stackObj.pos).mutex != MUTEXBLOCKED)
    {
        ERROREXIT(254,"Wait without blocked mutex");
    }
    /*can not be ->IllegalMonitorStateException*/
    HEAPOBJECTMARKER(object.stackObj.pos).mutex = MUTEXNOTBLOCKED;/* free lock for another thread and lock this */

    updateThreadState(THREADNOTBLOCKED,object);

    //its better to change own state after notify, to avoid cycles
    currentThreadCB->isMutexBlockedOrWaitingForObject = object;
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
