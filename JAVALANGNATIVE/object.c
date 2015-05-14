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
#include "scheduler.h"

extern u1 local;
extern ThreadControlBlock* currentThreadCB;

#ifndef TINYBAJOS_MULTITASKING
/* "java/lang/Object","notify","notifyAll","wait","waitTime","getDataAddress"*/
char notify()
{
#ifdef DEBUG
    if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex != MUTEXBLOCKED)
        ERROREXIT(253,"Notify on not locked mutex");
#endif
    notifyThread(opStackGetValue(local));
    return 0;
}

char notifyAll()
{
    const slot object = opStackGetValue(local);
#ifdef DEBUG
    if (HEAPOBJECTMARKER(object.stackObj.pos).mutex != MUTEXBLOCKED)
        ERROREXIT(249,"Wait without blocked mutex");
#endif
    updateThreadState(object, THREADWAITBLOCKED, THREADWAITAWAKENED,0,0,0);

    return 0;
}

char nativeWait()
{
    const slot object = opStackGetValue(local);
#ifdef DEBUG
    if (HEAPOBJECTMARKER(object.stackObj.pos).mutex != MUTEXBLOCKED)
    {
        ERROREXIT(254,"Wait without blocked mutex");
    }
#endif
    /*can not be ->IllegalMonitorStateException*/
    /* free lock for another thread and lock this */
    HEAPOBJECTMARKER(object.stackObj.pos).mutex = MUTEXNOTBLOCKED;

    updateThreadState(object,THREADMUTEXBLOCKED,THREADNOTBLOCKED,1,0,0);

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

/*char getDataAddress()
{
 return 1;// ret val is  on Stack !!
}*/
