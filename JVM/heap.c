/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "definitions.h"
#include "bajvm.h"
#include "stack.h"
#ifdef AVR8
#include <avr/pgmspace.h>
#endif
#include "heap.h"
/* heap */
void heapInit()
{
#if (AVR8||LINUX||AVR32LINUX)
    if ((heapBase=( slot*)malloc(sizeof( slot)*(size_t)MAXHEAP))==NULL)
    {
#ifdef AVR8                               // change all avr8 string to flash strings gives more data ram space for java!!
        printf_P(PSTR("malloc error\n"));
#else
        printf("malloc error\n");
#endif
        exit(-1);                                 /* heap fixed size!!*/
    }
#else
/* make it better*/
    heapBase = (slot*) ((u4)(appClassFileBase + MAXBYTECODE));
#endif
    while (heapTop > 0)
        *(heapBase + (--heapTop)) = NULLOBJECT;
    DEBUGPRINTHEAP;
}


slot heapGetElement(u2 pos)
{
    return *(heapBase + pos);
}


void heapSetElement(slot e, u2 pos)
{
    *(heapBase + pos) = e;
}


u2 getNextHeapObjectPos(u2 pos)
{
    return ((pos + HEAPOBJECTMARKER(pos).length) < heapTop) ? pos
        + HEAPOBJECTMARKER(pos).length : (heapTop + 1);
}


u2 getFreeHeapSpace(u2 length)
{
/* erkennnen circularer referencen on heap ohne bezug zu opstack !!*/
    if ((heapTop + length - 1) < MAXHEAP)
    {
        HEAPOBJECTMARKER(heapTop).length = length; // get exact length slots
        heapTop += length;
        return heapTop - length;
    }                                             /* free space on heap*/
    u2 nextElementPos = 0;
    do                                            /* first fit*/
    {
        if ((HEAPOBJECTMARKER(nextElementPos).status == HEAPFREESPACE)
            && ((HEAPOBJECTMARKER(nextElementPos).length) >= length))	{
            return nextElementPos;	// may be get more space than length
	    }                /* first fit	 */
    } while ((nextElementPos = getNextHeapObjectPos(nextElementPos)) < heapTop);
    checkObjects();
/* noch mal probieren*/
    nextElementPos = 0;
    do
    {
        if ((HEAPOBJECTMARKER(nextElementPos).status == HEAPFREESPACE)
            && ((HEAPOBJECTMARKER(nextElementPos).length) >= length))
            return nextElementPos;                /* first fit	 */
    } while ((nextElementPos = getNextHeapObjectPos(nextElementPos)) < heapTop);

/* verschmelzen=merge free heap space to bigger blocks*/
    int schmelz;

    verbosePrintf("Heap merge\n");
    for (schmelz = 0; schmelz < 20; schmelz++)
    {
        nextElementPos = 0;
        u2 oldElementPos = 0;
        while ((nextElementPos = getNextHeapObjectPos(nextElementPos))
            < heapTop)
        {
/*printf("next %x %x stat: %x magic %x %x\n",nextElementPos,oldElementPos, //HEAPOBJECTMARKER(nextElementPos).status,*/
/* HEAPOBJECTMARKER(nextElementPos).magic, HEAPOBJECTMARKER(nextElementPos).rootCheck);		*/
            if ((HEAPOBJECTMARKER(nextElementPos).status == HEAPFREESPACE)
                && (HEAPOBJECTMARKER(oldElementPos).status == HEAPFREESPACE))
            {
                HEAPOBJECTMARKER(oldElementPos).length += HEAPOBJECTMARKER(
                    nextElementPos).length;
                break;
            } else
            oldElementPos = nextElementPos;
        }
/* jetzt noch mal Probieren*/
        nextElementPos = 0;
        do
        {
/*printf("next %x  stat: %x magic %x %x\n",nextElementPos, HEAPOBJECTMARKER(nextElementPos).status,*/
/*HEAPOBJECTMARKER(nextElementPos).magic, HEAPOBJECTMARKER(nextElementPos).rootCheck);*/
            if ((HEAPOBJECTMARKER(nextElementPos).status == HEAPFREESPACE)
                && ((HEAPOBJECTMARKER(nextElementPos).length) >= length))
                return nextElementPos;            /* first fit	 */
        } while ((nextElementPos = getNextHeapObjectPos(nextElementPos))
            < heapTop);
    }
/* alles umsonst*/

    verbosePrintf(" no free heap space for object/array of length: 0x%x",
        length);

    exit(-1);
}


void checkObjects()
{
/* static objects or objects in opstack (root objects) or objects, referenced by other objects (circular??)*/
/* ob das reicht?*/
/* ich markiere heapobjekte (rootCheck=1), die anderen geben ich zum abschu� frei!!*/
    u2 opSPPos;
    u2 nextElementPos = 0;
    u1 i;
    u1 stillAConcatedObject;
#ifndef TINYBAJOS_MULTITASKING
    ThreadControlBlock* tCB;
    u1 k, max;
#endif
    do
    {
        HEAPOBJECTMARKER(nextElementPos).rootCheck = 0;
        if ((HEAPOBJECTMARKER(nextElementPos).status == HEAPFREESPACE)
            || (HEAPOBJECTMARKER(nextElementPos).status
            == HEAPALLOCATEDSTATICCLASSOBJECT))   /* empty or static*/
        {
            HEAPOBJECTMARKER(nextElementPos).rootCheck = 1;
            continue;
        }
#ifndef TINYBAJOS_MULTITASKING
        for (i = 0; i < (MAXPRIORITY); i++)       //searching for root objects on stack
        {
            tCB = threadPriorities[i].cb;
            max = (threadPriorities[i].count);
            for (k = 0; k < max; k++)
            {
                opSPPos = *(tCB->methodStackBase + tCB->methodSpPos - 1);
                while (opSPPos > 0)
                {
                    if ((nextElementPos
                        == ((*(tCB->opStackBase + (--opSPPos))).stackObj.pos))
                        && (((*(tCB->opStackBase + (opSPPos))).stackObj.magic)
                        == OBJECTMAGIC))          //UInt)
                    {
                        HEAPOBJECTMARKER(nextElementPos).rootCheck = 1;
                        break;
                    }
                }
                if (HEAPOBJECTMARKER(nextElementPos).rootCheck == 1)
                {
                    break;
                }
                tCB = tCB->succ;
            }
            if (HEAPOBJECTMARKER(nextElementPos).rootCheck == 1)
            {
                break;
            };
        }
#else
        opSPPos=opStackGetSpPos();
        while (opSPPos>0)
        {
            if ( (nextElementPos==((*(opStackBase+(--opSPPos))).stackObj.pos)) &&
                                                  //UInt)
                ( ((*(opStackBase+(opSPPos))).stackObj.magic)==OBJECTMAGIC ))
            {
                HEAPOBJECTMARKER(nextElementPos).rootCheck=1;
                break;
            }
        }
#endif
    } while ((nextElementPos = getNextHeapObjectPos(nextElementPos)) < heapTop);
/* alle Objekte von den stacks erreichbar (root elements)	markiert	*/
/* jetzt suche ich nur noch im heap*/
/* markierte objekte können referencen zu anderen objekten halten*/
/* diese markiere ich auch, solange bis ich keins mehr finde*/
    do
    {
        stillAConcatedObject = 0;
        nextElementPos = 0;
        do
        {
            if (HEAPOBJECTMARKER(nextElementPos).rootCheck == 1)
            {
/* seraching for "objects in root-objects"*/
                for (i = 1; i < HEAPOBJECTMARKER(nextElementPos).length; i++)
                    if ((HEAPOBJECT(nextElementPos + i).stackObj.magic
                    == OBJECTMAGIC)
                    && HEAPOBJECT(nextElementPos + i).UInt
                    != NULLOBJECT.UInt && canItBeAnObject(
                    HEAPOBJECT(nextElementPos + i).stackObj.pos)
                    && (HEAPOBJECTMARKER(
                    HEAPOBJECT(nextElementPos + i).stackObj.pos).magic
                    == OBJECTMAGIC)
                    && (HEAPOBJECTMARKER(
                    HEAPOBJECT(nextElementPos + i).stackObj.pos).rootCheck
                    == 0))
                {
                    HEAPOBJECTMARKER(
                        HEAPOBJECT(nextElementPos + i).stackObj.pos).rootCheck
                        = 1;
                    stillAConcatedObject = 1;
                }
            }
/*if(stillAConcatedObject) break;*/
        } while ((nextElementPos = getNextHeapObjectPos(nextElementPos))
            < heapTop);
    } while (stillAConcatedObject);

    nextElementPos = 0;
    while ((nextElementPos = getNextHeapObjectPos(nextElementPos)) < heapTop)
        if (!HEAPOBJECTMARKER(nextElementPos).rootCheck)
            HEAPOBJECTMARKER(nextElementPos).status = HEAPFREESPACE;
}


u1 canItBeAnObject(u2 pos)
{
    u1 retV = 0;                                  // can not be an object
    u2 nextElementPos = 0;
    do
    {
        if (nextElementPos == pos)
        {
            retV = 1;
            break;
        }
    } while ((nextElementPos = getNextHeapObjectPos(nextElementPos)) < heapTop);
    return retV;
}
