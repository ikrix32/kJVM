/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
/* fuer lehrzwecke,...*/
/* version 0.1 vom 1.10.07*/

#include <stdio.h>
#include <stdlib.h>
#ifdef AVR8
#include <avr/pgmspace.h>
#endif

#ifdef DEBUGSTACK
#define PRINTMAXSTACK     { if ((opSp-opStackBase) >= OPSTACKSIZE) { printf("stack error tid: %d max: %d\n",currentThreadCB->tid,OPSTACKSIZE);}}
#else
#define PRINTMAXSTACK
#endif

#include "kjvm.h"
#include "definitions.h"
#include "stack.h"

/* op stack holds  locals and operanden*/
/* method stack holds global variable (cN, mN, local,..)*/
#ifndef AVR8
static slot* opSp;
static u2*   methodSp;
#else
slot*   opSp;
u2*     methodSp;
#endif                                            // AVR8
void opStackInit(slot** m)                        /* per thread, fixed size */
{

#if (LINUX || AVR8 || AVR32LINUX)
    if ((*m = (slot*) calloc((size_t)OPSTACKSIZE,sizeof(slot))) == NULL)
        MALLOCERR(OPSTACKSIZE * sizeof(slot), "op stack");
#else
    /* classfiles - heap - (opstack methodstack)/ per thread*/
    /*make it better*/
    *m = (slot*) ((u4) appClassFileBase + MAXBYTECODE + 4 * MAXHEAP
                  + numThreads * (4 * OPSTACKSIZE + 2 * METHODSTACKSIZE));
#endif
}


//all these functions are rewritten in assembler to increase speed => routin es_stack.asm
#ifndef AVR8
inline void opStackPush(const slot val)
{
    *(opSp++) = val;
#ifdef DEBUGOPSTACK
    if ((opSp - opStackBase)>maxOpStack) maxOpStack = opSp - opStackBase;
#endif// DEBUGOPSTACK
}


/*  sp grothws with increasing addresses*/
/* and shows to TOS -> first free place*/

inline slot opStackPop()
{
    return *(--opSp);
}


/* operand stack stores 4 bytes*/
inline slot opStackPeek()
{
    return *(opSp - 1);
}


inline void    opStackPoke(const slot val)
{
    *(opSp - 1) = val;
}


inline void opStackSetValue(const u2 pos,const slot val)
{
    *(opStackBase + pos) = val;
}


inline slot opStackGetValue(const u2 pos)
{
    return*( opStackBase + pos);
}


inline u2 opStackGetSpPos()
{
    return (opSp - opStackBase);
}


/* relat ive to actual base*/
inline void opStackSetSpPos(const u2 pos)
{
    opSp = pos + opStackBase;
#ifdef DEBUGOPSTACK
    if ((opSp - opStackBase) > maxOpStack) maxOpStack = opSp - opStackBase;
#endif                                        // DEBUGOPSTACK
}
#endif

void methodStackInit(u2** m)
{
#if (LINUX||AVR8||AVR32LINUX)
    if ((*m=(u2*)calloc((size_t)METHODSTACKSIZE,sizeof(u2)))==NULL)
        MALLOCERR(METHODSTACKSIZE * sizeof(u2), "method stack");
#else
    *m = (u2*) ((u4)(
                     appClassFileBase + MAXBYTECODE + 4 * MAXHEAP + 4 * OPSTACKSIZE
                     + numThreads * (4 * OPSTACKSIZE + 2 * METHODSTACKSIZE)));
#endif
}


#ifndef AVR8                                      //all these functions are rewritten in assembler toincrease speed => routines_stack.asm
inline void methodStackPush(const u2 val)
{
    *(methodSp++) = val;
#ifdef DEBUGMETHODSTACK
    if ((methodSp-methodStackBase)>maxMethodStack) maxMethodStack=methodSp-methodStackBase;
#endif                                        // DEB0UGME TODSTACK
}


inline u2 methodStackPop()
{
    return *(--methodSp);
}


inline u2 methodStackPeek()
{
    return *(methodSp - 1);
}


inline u2 methodStackGetSpPos()
{
    return (methodSp - methodStackBase);
}


/* relative to actual base*/
inline void methodStackSetSpPos(const u2 pos)
{
    methodSp = pos + methodStackBase;
#ifdef DEBUGMETHODSTACK
    if ((methodSp - methodStackBase) > maxMethodStack) maxMethodStack = methodSp - methodStackBase;
#endif// DEBUGMETODSTACK
}


inline u1 methodStackEmpty()
{
    return (methodSp == methodStackBase) ? 1 : 0;
}
#endif
