/*
 * BAJOS BA-JAVA OPERATING SYSTEM
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
// a jvm for 8 Bit MC !!, e.g. atmega128 CharonII and linux and avr32
// version 0.1 vom 1.10.07

#ifndef __KJVM__
#define __KJVM__
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "typedefinitions.h"
#include "limits.h"
#include "definitions.h"
#include "scheduler.h"

#ifndef TINYBAJOS_ERROREXIT
void errorExitFunction(int nr, const char *format, ...);
#endif
void initHW(void);
#ifdef LINUX
void initVM(int argc, char* argv[]);
#else
void initVM(void);
#endif
#ifdef __DEF_GLOBALS__
#define GLOBAL
#define INIT__(_a) =(_a)
#else
#define GLOBAL extern
#define INIT__(_a)
#endif
#undef __DEF_GLOBALS__
GLOBAL u2 pc;                                     // active thread
GLOBAL u1 cN;                                     // class structure Number
GLOBAL u1 mN;                                     // method Number in class structure
GLOBAL u1 fN;                       		  // field number in class or object 
GLOBAL u1 local INIT__(0);
GLOBAL slot* opStackBase INIT__(NULL);
GLOBAL u2* methodStackBase INIT__(NULL);
GLOBAL slot* heapBase INIT__(NULL);
GLOBAL u2 heapTop INIT__(MAXHEAP);
#ifndef AVR8
GLOBAL char* classFileBase INIT__(NULL);
#endif
#if (AVR32UC3A || AVR32AP7000)
GLOBAL char* appClassFileBase INIT__(NULL);
#endif
GLOBAL u1 numClasses INIT__(0);
#ifndef TINYBAJOS_MULTITASKING
GLOBAL ThreadControlBlock* currentThreadCB INIT__(NULL);
GLOBAL u1 numThreads INIT__(0);
GLOBAL u1 tid INIT__(0);
GLOBAL u4 mainThreadPriority[2];                  // priority (and alive) of main thread -> immutable
                                                  //array of priority		lists
GLOBAL ThreadPriorityList threadPriorities[MAXPRIORITY] ;
GLOBAL ThreadControlBlock* interruptVectors[NUMBEROFINTERRUPTS];
#endif
GLOBAL classStructure cs[MAXCLASSES];	// static allocated !!!
#ifndef AVR8
GLOBAL u4 timerMilliSec INIT__(0);
#else
#ifndef WITHMONGLOBAL
u4 timerMilliSec INIT__(0);
#endif
#endif

GLOBAL u1 timeOut INIT__(0);
//GLOBAL u2 endBajos INIT__(0);
#ifdef AVR8
GLOBAL FILE uartAVR8;
#endif                                            //AVR8
#ifdef DEBUGOPSTACK
u2 maxOpStack INIT__(0);
#endif                                            // DEBUGOPSTACK
#ifdef DEBUGMETHODSTACK
u2 maxMethodStack INIT__(0);
#endif                                            // DEBUGMETHODSTACK
#endif                                            // __BAJVM
