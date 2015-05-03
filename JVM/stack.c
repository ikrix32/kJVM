
#include <stdlib.h>

#include "kjvm.h"

#include "stack.h"

#ifdef DEBUGSTACK
#define PRINTMAXSTACK     { if ((opSp-opStackBase) >= OPSTACKSIZE) { printf("stack error tid: %d max: %d\n",currentThreadCB->tid,OPSTACKSIZE);}}
#else
#define PRINTMAXSTACK
#endif

/* op stack holds  locals and operanden*/
/* method stack holds global variable (cN, mN, local,..)*/
static slot* opSp;
static u2*   methodSp;

static slot* opStackBase;
static u2*   methodStackBase;

void opStackInit(slot** m)// per thread, fixed size
{
    if ((*m = (slot*) malloc((size_t)OPSTACKSIZE * sizeof(slot))) == NULL)
        MALLOCERR(OPSTACKSIZE * sizeof(slot), "op stack");
}

void opStackPush(const slot val)
{
    *(opSp++) = val;
}

/*  sp grothws with increasing addresses*/
/* and shows to TOS -> first free place*/
slot opStackPop()
{
    return *(--opSp);
}

/* operand stack stores 4 bytes*/
slot opStackPeek()
{
    return *(opSp - 1);
}

void    opStackPoke(const slot val)
{
    *(opSp - 1) = val;
}


void opStackSetValue(const u2 pos,const slot val)
{
    *(opStackBase + pos) = val;
}


slot opStackGetValue(const u2 pos)
{
    return*( opStackBase + pos);
}


u2 opStackGetSpPos()
{
    return (opSp - opStackBase);
}


/* relative to actual base*/
void opStackSetSpPos(const u2 pos)
{
    opSp = pos + opStackBase;
}

void opStackSetBase(slot* base){
    opStackBase = base;
}

slot* opStackGetBase(){
    return opStackBase;
}

/** Methods stack **/
void methodStackInit(u2** m)
{
    if ((*m = (u2*)calloc((size_t)METHODSTACKSIZE,sizeof(u2)))==NULL)
        MALLOCERR(METHODSTACKSIZE * sizeof(u2), "method stack");
}

void methodStackPush(const u2 val)
{
    *(methodSp++) = val;
}

u2 methodStackPop()
{
    return *(--methodSp);
}

u2 methodStackPeek()
{
    return *(methodSp - 1);
}

u2 methodStackGetSpPos()
{
    return (methodSp - methodStackBase);
}

/* relative to actual base*/
void methodStackSetSpPos(const u2 pos)
{
    methodSp = pos + methodStackBase;
}


u1 methodStackEmpty()
{
    return (methodSp == methodStackBase) ? 1 : 0;
}

void    methodStackSetBase(u2* base){
    methodStackBase = base;
}
u2*     methodStackGetBase(){
    return methodStackBase;
}



