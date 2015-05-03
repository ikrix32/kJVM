//
//  kJVM
//
//  Created by Florin Cristian on 15/02/15.
//  Copyright (c) 2015 Florin Cristian. All rights reserved.
//

#ifndef __KJVM__
#define __KJVM__
#include "definitions.h"
#include "scheduler.h"

void vm_init(void);
s1 vm_run(const u1 classId);

u2 pc;                                // active thread
u1 cN;                                // class structure Number
u1 mN;                                // method Number in class structure
u1 fN;                       		  // field number in class or object
u1 local;

#ifndef AVR8
char* classFileBase;
u4    crtByteCodeSize;
#endif

u1 numClasses;
#ifndef TINYBAJOS_MULTITASKING
ThreadControlBlock* currentThreadCB;
u1 numThreads;
u1 tid;
//array of priority		lists
ThreadPriorityList threadPriorities[MAXPRIORITY] ;
ThreadControlBlock* interruptVectors[NUMBEROFINTERRUPTS];
#endif

classStructure cs[MAXCLASSES];	// static allocated !!!
 
#endif
