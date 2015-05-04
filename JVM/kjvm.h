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

#ifdef __DEF_GLOBALS__
#define GLOBAL
#define INIT__(_a) =(_a)
#else
#define GLOBAL extern
#define INIT__(_a)
#endif
#undef __DEF_GLOBALS__


void vm_init(void);
s1 vm_run(const u1 classId);

GLOBAL u2 pc;                                     // active thread
GLOBAL u1 cN;                                     // class structure Number
GLOBAL u1 mN;                                     // method Number in class structure
GLOBAL u1 fN;                       		  // field number in class or object
GLOBAL u1 local INIT__(0);


#ifndef AVR8
GLOBAL char* classFileBase INIT__(NULL);
GLOBAL u4    crtByteCodeSize;
#endif

GLOBAL u1 numClasses INIT__(0);

#ifndef TINYBAJOS_MULTITASKING
GLOBAL ThreadControlBlock* currentThreadCB INIT__(NULL);
GLOBAL u1 tid INIT__(0);

//priority		lists
GLOBAL ThreadPriorityList threadList;
GLOBAL ThreadControlBlock* interruptVectors[NUMBEROFINTERRUPTS];
#endif

GLOBAL classStructure cs[MAXCLASSES];	// static allocated !!!



 
#endif
