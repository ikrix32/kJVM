#include "config.h"

#ifndef TINYBAJOS_MULTITASKING
#include <stdlib.h>
#include "kjvm.h"
#include "classfile.h"
#include "stack.h"
#include "scheduler.h"
#include "heap.h"

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


// finds corresponding ThreadCB to java object by ceh
ThreadControlBlock* findThreadCB(const slot obj)
{
    for (int k = 0; k < threadList.count; k++)
    {
        if ((threadList.cb->obj).UInt == obj.UInt)
        {
            return threadList.cb;
        }
        threadList.cb = threadList.cb->succ;
    }

    ERROREXIT(78, "thread not found");

    return NULL;
}

// notifys a waiting blocked thread for given object by ceh
void notifyThread(const slot obj)
{
    ThreadControlBlock* cb = threadList.cb;
    for (int k = 0; k < threadList.count; k++)
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


// notifys a waiting blocked threadfor given object by ceh
void awakeThreadFromMutex(const slot obj)
{
    ThreadControlBlock* cb = threadList.cb;
    for (int k = 0; k < threadList.count; k++)
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

        ERROREXIT(-1, "Unlock Exception\n");

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
        // mutex is free, I (the thread) have not the mutex and I can get the mutex for the object
        t->isMutexBlockedOrWaitingForObject = NULLOBJECT;
        // get the lock
        HEAPOBJECTMARKER(obj.stackObj.pos).mutex = MUTEXBLOCKED;
        int i = 0;
        // I had not the mutex for this object (but perhaps for others), now I have the look
        for (i = 0; i < MAXLOCKEDTHREADOBJECTS; i++)
            if (t->hasMutexLockForObject[i].UInt != NULLOBJECT.UInt)
                continue;
            else
                break;
        if (i == MAXLOCKEDTHREADOBJECTS)
        {
            ERROREXIT(-1, "too many locks\n");
        }
        // entry for this object in the array of mutexed objects for the thread
        t->lockCount[i] = 1;
        t->hasMutexLockForObject[i] = obj;
    } // mutex is blocked
    else
    {
        t->state = THREADMUTEXBLOCKED;
        t->isMutexBlockedOrWaitingForObject = obj;
        //force scheduling
        t->numTicks = 0;
    }
}


// generates a new thread by ceh
void createThread (void)
{
    if(threadList.count == MAXTHREADS)
    {
        ERROREXIT(-2, "to many threads\n");
    }
    ThreadControlBlock* t = (ThreadControlBlock*) malloc(sizeof(ThreadControlBlock));

    opStackInit(&(t->opStackBase));
    methodStackInit(&(t->methodStackBase));
    t->methodSpPos = (threadList.count == 0) ? 0 : 5;
    t->tid = tid++;
    t->state = THREADNOTBLOCKED;

    //set thread in matching priority list
    if (currentThreadCB == NULL)//==the first thread (main thread) is created
    {
        currentThreadCB = t;
        t->obj = NULLOBJECT;
        u4* mainThreadPriority = (u4*)malloc(2 * sizeof(u4));
        mainThreadPriority[0] = (u4) NORMPRIORITY;// priority (and alive) of main thread -> immutable
        mainThreadPriority[1] = (u4) 1;           // alive -> doesnt need for main thread??
        t->pPriority = mainThreadPriority;
    }
    else
    {
        cN = opStackGetValue(local).stackObj.classNumber;
        //if (!findFieldByRamName("priority", 8, "I", 1))
        if (!findFieldByName(cN, cN,"priority", 8, "I", 1,0))
        {
            ERROREXIT(77, "field priority not found");
        }
        t->pPriority = (u4*) (heapGetBase() + opStackGetValue(local).stackObj.pos + fN + 1);
        // position of int field priority of the thread creating object, next field is aLive
        // restore class number of object
        cN = opStackGetValue(local).stackObj.classNumber;
        t->obj = opStackGetValue(local);
    }
    *((t->pPriority) + 1) = (u4) 1;               // isALive == true
    t->numTicks = *(t->pPriority);
    insertThreadIntoPriorityList(t);

    if (t->tid != 0)//not main thread
    {
        do
        {
            mN = findMethodByName(cN,"run", 3, "()V", 3);
            if (mN != INVALID_METHOD_ID)
                break;

            cN = findSuperClass(cN);
        }while((cN != INVALID_CLASS_ID));
#if DEBUG//only n debug throw exception
        if(mN == INVALID_CLASS_ID)
             ERROREXIT(123, "run method not found");
#endif
        *(t->methodStackBase + 0) = (u2) 0;
        *(t->methodStackBase + 1) = cN;
        *(t->methodStackBase + 2) = mN;
        *(t->methodStackBase + 3) = getStartPC(cN,mN);
        *(t->methodStackBase + 4) = findMaxLocals(cN,mN);
        // reference to caller object (from start())
        *(t->opStackBase) = opStackGetValue(local);
        //verbosePrintf("cN x%x mN x%x startPC x%x\n", cN, mN, *(t->methodStackBase + 3));
    }

    for (int i = 0; i < MAXLOCKEDTHREADOBJECTS; i++)
    {
        t->hasMutexLockForObject[i] = NULLOBJECT;
        t->lockCount[i] = 0;
    }
    t->isMutexBlockedOrWaitingForObject = NULLOBJECT;
}


/*
 * Inserts thread into the correct priority list and if the priority of
 * the current running thread is lower than these, current thread gets
 * his last tick(last bytecode executed)
 * by Christopher-Eyk Hrabia
 */
void insertThreadIntoPriorityList(ThreadControlBlock* t)
{
    if (threadList.cb == NULL)
        threadList.cb = t;

    //find apropiate priority before insert
    t->pred = threadList.cb;
    t->succ = threadList.cb->succ;
    threadList.cb->succ = t;
    t->succ->pred = t;

    threadList.count++;
    const u1 priority = (*t->pPriority - 1);
    const u1 crtPriority = (*currentThreadCB->pPriority - 1);
    if (crtPriority < priority)
    {// force scheduling
        currentThreadCB->numTicks = 0;
    }
}

/* Function does only remove the given Thread from his current list
 * but does not delete the thread
 * list will be recognised by *(thread->pPriority)!! Do not edit before!
 * by Christopher-Eyk Hrabia
 */
void removeThreadFromPriorityList(ThreadControlBlock* t)
{
    if (threadList.count == 1)        //last thread of current priority
    {
        threadList.cb = NULL;
    }
    else
    {
        t->pred->succ = t->succ;
        t->succ->pred = t->pred;
        if (t == threadList.cb)
        {
            threadList.cb = t->pred;
        }
    }
    threadList.count--;
}


// Delete one thread, which is not actualThread by Christopher-Eyk Hrabia
void deleteNotCurrentThread(ThreadControlBlock** t)
{
    *(((*t)->pPriority) + 1) = (u4) 0; // isALive == false
    removeThreadFromPriorityList(*t);
    free((*t)->methodStackBase);
    free((*t)->opStackBase);
    free((*t));
    *t = NULL;
}

void setupCurrentThread()
{
    methodStackSetBase(currentThreadCB->methodStackBase);
    methodStackSetSpPos(currentThreadCB->methodSpPos);
    opStackSetBase(currentThreadCB->opStackBase);
    opStackSetSpPos(methodStackPop());
    pc = methodStackPop();
    mN = methodStackPop();
    cN = methodStackPop();
    local = methodStackPop();
}

// Delete actualThread by Christopher-Eyk Hrabia
void deleteThread(void)
{
    deleteNotCurrentThread(&currentThreadCB);
    //*((currentThreadCB->pPriority) + 1) = (u4) 0; // isALive == false
    //removeThreadFromPriorityList(currentThreadCB);

    //free(currentThreadCB->methodStackBase);
    //free(currentThreadCB->opStackBase);
    //free(currentThreadCB);
    currentThreadCB = threadList.cb;
    setupCurrentThread();
}


/* the scheduling process function, this function is called after every bytecode
 * one thread can run until is numTicks are 0. The numTicks a thread gets after he
 * was scheduled is his priority. The scheduler schedules(get active) if the numTicks of
 * current thread are 0. //CEH
 */
void scheduler(void)
{
    if (threadList.count == 1)
        return;

    //A Thread runs until his numTicks is 0
    if (currentThreadCB->numTicks-- > 0
    && (currentThreadCB->state == THREADNOTBLOCKED))
        return;

    // select a runnable thread
    ThreadControlBlock* found = NULL;
    u1 threadFound = 0;

    found = threadList.cb;
    //printf("Current %d, Found thread:%d, next:%d\n",currentThreadCB->tid,found->tid,found->succ->tid);
    for (int n = 0; n < threadList.count; n++)
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
#if DEBUG
    if (!threadFound)
    {
        ERROREXIT(111, "SCHEDULING ERROR!\n");
    }
#endif
    // assume: not all threads are blocked
    if (found == currentThreadCB /*&& ((found->state)==THREADNOTBLOCKED)*/) {
        currentThreadCB->numTicks = *(currentThreadCB->pPriority);
        return;
    }

    // scheduling -> next thread
    methodStackPush(local);
    methodStackPush(cN);
    methodStackPush(mN);
    methodStackPush(pc);
    methodStackPush((u2)(opStackGetSpPos()));
    currentThreadCB->methodSpPos = methodStackGetSpPos();

    //printf("Switch threads: crt:%d next:%d nextPriority:%d\n",currentThreadCB->tid,found->tid,*found->pPriority);
    currentThreadCB = found;
    threadList.cb = currentThreadCB;
    //reset numTicks
    currentThreadCB->numTicks = *currentThreadCB->pPriority;
    setupCurrentThread();
}
#endif
