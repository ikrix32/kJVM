#ifndef __SCHEDULER__
#define __SCHEDULER__

#define DEFAULTTICKS 5
// number of byte code instructions, counter
#define NORMPRIORITY 5
#define MAXPRIORITY 10
#define MINPRIORITY 1

#include "definitions.h"

#ifndef TINYBAJOS_MULTITASKING
void createThread(void);
void deleteThread(void);

void interruptThread(ThreadControlBlock* thread);
ThreadControlBlock* findThreadCB(const slot obj);
void deleteNotCurrentThread(ThreadControlBlock** t);
void notifyThread(const slot obj);
void awakeThreadFromMutex(const slot obj);
void setMutexOnObject(ThreadControlBlock* t,const slot obj);
void releaseMutexOnObject(ThreadControlBlock* t, slot obj,
ThreadControlBlock* awakeThread);

void scheduler(void);
void insertThreadIntoPriorityList(ThreadControlBlock* t);
void removeThreadFromPriorityList(ThreadControlBlock* t);
#endif
#endif
