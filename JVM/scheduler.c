/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
#ifndef TINYBAJOS_MULTITASKING

#include<stdio.h>
#include "definitions.h"
#include "typedefinitions.h"
#include "kjvm.h"
#include "classfile.h"
#include "stack.h"
#include "scheduler.h"
#include "heap.h"
/*
 //EXAMPLE FOR ITERATING OVER ALL THREADS:
 void 	iterateOverThreads2(){
 u1 k,i,max;
 ThreadControlBlock* t;
 printf("start\n");
 for(i=0;i<(MAXPRIORITY);i++){
 max=(threadPriorities[i].count);
 t=threadPriorities[i].cb;
 for(k=0;k<max;k++){
 //do something with t
 printf("NR: %d\n", t->tid);
 //do something with t
 t=t->succ;
 }
 }
 }
 */

void interruptThread(ThreadControlBlock* thread)
{
    if ((thread))
    {
        if (currentThreadCB != (thread))
        {
            currentThreadCB->numTicks = 0;
        }

        (thread)->isMutexBlockedOrWaitingForObject = NULLOBJECT;
        (thread)->lockCount[0] = 1;
        (thread)->hasMutexLockForObject[0] = (thread)->obj;
        (thread)->state = THREADNOTBLOCKED;
    }
}


/*
 * finds corresponding ThreadCB to java object by ceh
 */
ThreadControlBlock* findThreadCB(const slot obj)
{
    for (int i = 0; i < (MAXPRIORITY); i++)
    {
        for (int k = 0; k < (threadPriorities[i].count); k++)
        {
            if ((threadPriorities[i].cb->obj).UInt == obj.UInt)
            {
                return threadPriorities[i].cb;
            }
            threadPriorities[i].cb = threadPriorities[i].cb->succ;
        }
    }

    errorExit(78, "thread not found");

    return NULL;
}


/*
 * notifys a waiting blocked thread for given object by ceh
 */
void notifyThread(const slot obj)
{
    ThreadControlBlock* cb;

    for (int i = 0; i < (MAXPRIORITY); i++)
    {
        const u1 max = (threadPriorities[i].count);
        cb = threadPriorities[i].cb;
        for (int k = 0; k < max; k++)
        {
            if ((cb->state == THREADWAITBLOCKED)
            && ((cb->isMutexBlockedOrWaitingForObject).UInt == obj.UInt))
            {
                cb->state = THREADWAITAWAKENED;
                return;
            }
            cb = cb->succ;
        }
    }
}


/*
 * notifys a waiting blocked threadfor given object by ceh
 */
void awakeThreadFromMutex(const slot obj)
{
    ThreadControlBlock* cb;

    for (int i = 0; i < (MAXPRIORITY); i++)
    {
        const u1 max = (threadPriorities[i].count);
        cb = threadPriorities[i].cb;
        for (int k = 0; k < max; k++)
        {
            if ((cb->state == THREADMUTEXBLOCKED)
            && ((cb->isMutexBlockedOrWaitingForObject).UInt == obj.UInt))
            {
                cb->state = THREADNOTBLOCKED;
                setMutexOnObject(cb, obj);
                return;
            }
            cb = cb->succ;
        }
    }
}


/*
 * release Mutex on given object and given thread by ceh
 * t is in general the current running thread; obj is the object which should be locked and awakeThread could be defined, if you know which
 * thread must be awaked (is needed for interrupt); if it is neces sary to search for awakeThread, set it to NULL
 */
void releaseMutexOnObject(ThreadControlBlock* t,const slot obj,
                          ThreadControlBlock* awakeThread)
{
    if (HEAPOBJECTMARKER(obj.stackObj.pos).mutex != MUTEXBLOCKED)
    {
        return;                                   //object is not locked!
    }
    int i;
    for (i = 0; i < MAXLOCKEDTHREADOBJECTS; i++)  /* must be in*/
        if (t->hasMutexLockForObject[i].UInt == obj.UInt)
            break;
    if (i == MAXLOCKEDTHREADOBJECTS)
    {

        errorExit(-1, "Unlock Exception\n");

        return;

    }
    t->lockCount[i]--;
    if (!t->lockCount[i])
    {
        t->hasMutexLockForObject[i] = NULLOBJECT; /* free lock*/
        HEAPOBJECTMARKER(obj.stackObj.pos).mutex = MUTEXNOTBLOCKED;

        //awake a blocked thread
        if (awakeThread)
        {
            awakeThread->state = THREADNOTBLOCKED;
            setMutexOnObject(awakeThread, obj);
        }
        else
        {
            awakeThreadFromMutex(obj);
        }
    }
}


/*
 * set Mutex on given object and given thread by ceh
 * in difference to it implementation in interpreter.c is it not possible to lock a locked object again
 * this behavior is necessa ry for interrupthandling
 */
void setMutexOnObject(ThreadControlBlock* t,const slot obj)
{
    if (HEAPOBJECTMARKER(obj.stackObj.pos).mutex == MUTEXNOTBLOCKED)
    {
        /* mutex is free, I (the thread) have not the mutex and I can get the mutex for the object*/
        t->isMutexBlockedOrWaitingForObject = NULLOBJECT;
        /* get the lock*/
        HEAPOBJECTMARKER(obj.stackObj.pos).mutex = MUTEXBLOCKED;
        int i = 0;
        /* I had not the mutex for this object (but perhaps for others), now I have the look*/
        for (i = 0; i < MAXLOCKEDTHREADOBJECTS; i++)
            if (t->hasMutexLockForObject[i].UInt != NULLOBJECT.UInt)
                continue;
            else
                break;
        if (i == MAXLOCKEDTHREADOBJECTS)
        {

            errorExit(-1, "too many locks\n");

        }
        /* entry for this object in the array of mutexed objects for the thread*/
        t->lockCount[i] = 1;                      /* count (before 0)*/
        t->hasMutexLockForObject[i] = obj;
    }                                             // mutex is blocked
    else
    {
        t->state = THREADMUTEXBLOCKED;            /*mutex blocked*/
        t->isMutexBlockedOrWaitingForObject = obj;
        //force scheduling
        t->numTicks = 0;
    }
}


/*
 * generates a new thread by ceh
 */
void createThread (void)
{
    if(numThreads == MAXTHREADS)
    {
        errorExit       (-2, "to many threads\n");
    }
    ThreadControlBlock* t = (ThreadControlBlock*) malloc(sizeof(ThreadControlBlock));

    opStackInit(&(t->opStackBase));
    methodStackInit(&(t->methodStackBase));
    t->methodSpPos = (numThreads == 0) ? 0 : 5;
    t->tid = tid++;
    t->state = THREADNOTBLOCKED;

    //set thread in matching priority list
    if (currentThreadCB == NULL)                  //==the first thread (main thread) is created
    {
        //init priority array because first thread is created
        int i;
        for (i = 0; i < (MAXPRIORITY); i++)
        {
            threadPriorities[i].cb = NULL;
            threadPriorities[i].count = 0;
        }
        currentThreadCB = t;
        t->obj = NULLOBJECT;
        mainThreadPriority[0] = (u4) NORMPRIORITY;// priority (and alive) of main thread -> immutable
        mainThreadPriority[1] = (u4) 1;           // alive -> doesnt need for main thread??
        t->pPriority = mainThreadPriority;
    }
    else
    {
        cN = opStackGetValue(local).stackObj.classNumber;
        if (!findFieldByRamName("priority", 8, "I", 1))
        {
            errorExit(77, "field priority not found");
        }
        t->pPriority = (u4*) (heapBase + opStackGetValue(local).stackObj.pos + fN + 1);
        // position of int field priority of the thread creating object, next field is aLive
        // restore class number of object
        cN = opStackGetValue(local).stackObj.classNumber;
        t->obj = opStackGetValue(local);
    }
    *((t->pPriority) + 1) = (u4) 1;               // isALive == true
    t->numTicks = *(t->pPriority);
    insertThreadIntoPriorityList(t);
    if (numThreads != 0)
    {
        for (;;)
        {
            if (findMethodByName(cN,"run", 3, "()V", 3))
                break;
            if (!findSuperClass(cN))
                errorExit(123, "run method not found");
        }
        *(t->methodStackBase + 0) = (u2) 0;
        *(t->methodStackBase + 1) = cN;
        *(t->methodStackBase + 2) = mN;
        *(t->methodStackBase + 3) = getStartPC(cN,mN);
        *(t->methodStackBase + 4) = findMaxLocals(cN,mN);
        /* reference to caller object (from start())*/
        *(t->opStackBase) = opStackGetValue(local);
        //verbosePrintf("cN x%x mN x%x startPC x%x\n", cN, mN, *(t->methodStackBase + 3));
    }

    for (int i = 0; i < MAXLOCKEDTHREADOBJECTS; i++)
    {
        t->hasMutexLockForObject[i] = NULLOBJECT;
        t->lockCount[i] = 0;
    }
    t->isMutexBlockedOrWaitingForObject = NULLOBJECT;
    numThreads++;
}


/*
 * Inserts thread into the correct priority list and if the priority of
 * the current running thread is lower than these, current thread gets
 * his last tick(last bytecode executed)
 * by Christopher-Eyk Hrabia
 */
void insertThreadIntoPriorityList(ThreadControlBlock* t)
{
    ThreadControlBlock* pos;
    const u1 prio = *t->pPriority - 1;
    if (prio > 10)
        exit(99);

    pos = threadPriorities[prio].cb;
    if (pos == NULL)
    {
        threadPriorities[prio].cb = t;
        t->pred = t;
        t->succ = t;
    }
    else
    {
        t->pred = pos;
        t->succ = pos->succ;
        pos->succ = t;
        t->succ->pred = t;
    }
    threadPriorities[prio].count++;
    if ((*currentThreadCB->pPriority - 1) < prio)
    {
        currentThreadCB->numTicks = 0;            // force scheduling
    }
}


/*
 * Function does only remove the given Thread from his current list
 * but does not deleteth>e thread
 * list will be recognised by *(thread->pPriority)!! Do not edit before!
 * by Christopher-Eyk Hrabia
 */
void removeThreadFromPriorityList(ThreadControlBlock* t)
{
    //	ThreadControlBlock* temp=t->succ;
    const u1 prio = *t->pPriority - 1;
    if (prio > 10)
        exit(100);

    if (threadPriorities[prio].count == 1)        //last thread of current priority
    {
        threadPriorities[prio].cb = NULL;
    }
    else
    {
        t->pred->succ = t->succ;
        t->succ->pred = t->pred;
        if (t == threadPriorities[prio].cb)
        {
            threadPriorities[prio]. cb = t->pred;
        }
    }
    threadPriorities[prio].count--;
}


/*
 * Delete one thread, which is not actualThread by Christopher-Eyk Hrabia
 */
void deleteNotCurrentThread(ThreadControlBlock** t)
{
    removeThreadFromPriorityList(*t);
    free((*t)->methodStackBase);
    free((*t)->opStackBase);
    free((*t));
    *t = NULL;
    numThreads--;
}


/*
 * Delete actualThread by Christopher-Eyk Hrabia
 */
void deleteThread(void)
{
    *((currentThreadCB->pPriority) + 1) = (u4) 0; // isALive == false
    removeThreadFromPriorityList(currentThreadCB);

    int i = MAXPRIORITY - 1;
    while (threadPriorities[i].count == 0)//it should not be possible that i becomes lower than 0 therefore NO CHECK
    {
        i--;
    }

    free(currentThreadCB->methodStackBase);
    free(currentThreadCB->opStackBase);
    free(currentThreadCB);
    currentThreadCB = threadPriorities[i].cb;
    methodStackBase = currentThreadCB->methodStackBase;
    methodStackSetSpPos(currentThreadCB->methodSpPos);
    opStackBase = currentThreadCB->opStackBase;
    opStackSetSpPos(methodStackPop());
    pc = methodStackPop();
    mN = methodStackPop();
    cN = methodStackPop();
    local = methodStackPop();
    numThreads--;
}


/*
 * the scheduling process function, this function is called after every bytecode
 * one thread can run until is numTicks are 0. The numTicks a thread gets after he
 * was scheduled is his priority. The scheduler schedules(get active) if the numTicks of
 * current thread are 0. //CEH
 */
void scheduler(void)
{
    if (numThreads == 1)
        return;

    //A Thread runs until his numTicks is 0
    if (((currentThreadCB->numTicks--) && ((currentThreadCB->state) == THREADNOTBLOCKED)))
        return;

    // select a runnable thread
    u1 threadFound;
    ThreadControlBlock* found = NULL;

    threadFound = 0;

    //printf("threads %d\n",numThreads);

    for (int p = (MAXPRIORITY - 1); p != 255; p--)
    {
        //printf("Prio %d\n",p);
        if ((found = threadPriorities[p].cb) == NULL)
            continue;

        for (int n = 0; n < threadPriorities[p].count; n++)
        {
            found = found->succ;
            //printf("in sched prio: %d, n: %d, t->state: %d\n",p,found->tid,found->state);
            if ((found->state) == THREADNOTBLOCKED)
            {
                //printf("New Thread= %d\n",found->tid);
                threadFound = 1;                  //signal nested loop break
                break;
            }                                     // I take it

            if ((found->state) == THREADMUTEXBLOCKED)
                continue;                         // next n

            if ((found->state) == THREADWAITBLOCKED)
                continue;                         // next n

            if (((found->state) == THREADWAITAWAKENED)
            && ((HEAPOBJECTMARKER((found->isMutexBlockedOrWaitingForObject).stackObj.pos).mutex) == MUTEXBLOCKED))
                continue;

            /* awakened and mutexnotblocked*/
            //if ((found->state)==THREADWAITAWAKENED)	{ //not nesessary because no other state ist possible
            HEAPOBJECTMARKER((found->isMutexBlockedOrWaitingForObject).stackObj.pos).mutex = MUTEXBLOCKED;
            found->state = THREADNOTBLOCKED;
            found->isMutexBlockedOrWaitingForObject = NULLOBJECT;
            threadFound = 1;                      //signal nested loop break
            break;
            //}
        }                                         // end for n
        if (threadFound)
            break;
    }                                             // end for p
    if (!threadFound)
    {
        errorExit(111, "SCHEDULING ERROR!\n");
    }
    // assume: not all threads are blocked
    if (found == currentThreadCB /*&& ((found->state)==THREADNOTBLOCKED)*/) {
        currentThreadCB->numTicks = *(currentThreadCB->pPriority);
        return;
    }


    /* scheduling -> next thread*/
    methodStackPush(local);
    methodStackPush(cN);
    methodStackPush(mN);
    methodStackPush(pc);
    methodStackPush((u2)(opStackGetSpPos()));
    currentThreadCB->methodSpPos = methodStackGetSpPos();
    currentThreadCB = found;
    threadPriorities[*currentThreadCB->pPriority - 1].cb = currentThreadCB;
    //reset numTicks
    currentThreadCB->numTicks = *currentThreadCB->pPriority;
    methodStackBase = currentThreadCB->methodStackBase;
    methodStackSetSpPos(currentThreadCB->methodSpPos);
    opStackBase = currentThreadCB->opStackBase;
    opStackSetSpPos(methodStackPop());
    pc = methodStackPop();
    mN = methodStackPop();
    cN = methodStackPop();
    local = methodStackPop();
}
#endif
