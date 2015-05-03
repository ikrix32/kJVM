
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

//GLOBAL slot* opStackBase INIT__(NULL);
//GLOBAL u2*   methodStackBase INIT__(NULL);

void opStackInit(slot** m)// per thread, fixed size
{
    if ((*m = (slot*) malloc((size_t)OPSTACKSIZE * sizeof(slot))) == NULL)
        MALLOCERR(OPSTACKSIZE * sizeof(slot), "op stack");
}

void opStackPush(const slot val)
{
    *(opSp++) = val;
#ifdef DEBUGOPSTACK
    if ((opSp - opStackBase)>maxOpStack) maxOpStack = opSp - opStackBase;
#endif// DEBUGOPSTACK
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
#ifdef DEBUGOPSTACK
    if ((opSp - opStackBase) > maxOpStack) maxOpStack = opSp - opStackBase;
#endif                                        // DEBUGOPSTACK
}

/** Methods stack **/
void methodStackInit(u2** m)
{
    if ((*m=(u2*)calloc((size_t)METHODSTACKSIZE,sizeof(u2)))==NULL)
        MALLOCERR(METHODSTACKSIZE * sizeof(u2), "method stack");
}

void methodStackPush(const u2 val)
{
    *(methodSp++) = val;
#ifdef DEBUGMETHODSTACK
    if ((methodSp-methodStackBase)>maxMethodStack) maxMethodStack=methodSp-methodStackBase;
#endif                                        // DEB0UGME TODSTACK
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
#ifdef DEBUGMETHODSTACK
    if ((methodSp - methodStackBase) > maxMethodStack) maxMethodStack = methodSp - methodStackBase;
#endif// DEBUGMETODSTACK
}


u1 methodStackEmpty()
{
    return (methodSp == methodStackBase) ? 1 : 0;
}
