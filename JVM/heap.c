/*
 * @ikrix
 */
#include <stdlib.h>
#include "definitions.h"
#include "kjvm.h"
#include "stack.h"

#include "heap.h"

static slot* heapBase;
static u2    heapTop = MAXHEAP;

extern u1 cN;
extern ThreadControlBlock* currentThreadCB;
extern ThreadPriorityList threadList;
extern classStructure cs[MAXCLASSES];

char* heapElemTypes[4] = { "FREE","STATIC_OBJ","OBJ","ARRAY"};
void heapPrintBlocks()
{
    u2 nextElementPos = 0;
    do//Search to fit in a slot that was freed
    {
        const heapObjectMarker marker = HEAPOBJECTMARKER(nextElementPos);
        PRINTF("\t[#%d|%s|%d|%s]\n",nextElementPos,heapElemTypes[marker.status],marker.length,(marker.rootCheck ? "true" : "false"));
        nextElementPos = heapGetNextObjectPos(nextElementPos);

    } while (nextElementPos < heapTop);
    PRINTF("\n");
}

void heapPrintStackReferences(ThreadControlBlock* thread){
    u2 opStackPos = 0;
    if(thread->tid == currentThreadCB->tid){//current thread
        opStackPos = opStackGetSpPos();
    }else{
        opStackPos = *(thread->methodStackBase + thread->methodSpPos - 1);
    }
    PRINTF("Print Stack Objects for thread id:%d :\n",thread->tid);
    while (opStackPos > 0)
    {
        opStackPos--;
        
        const slot stackSlot = *(thread->opStackBase + opStackPos);
        if(stackSlot.stackObj.magic == OBJECTMAGIC)
        {
            const heapObjectMarker heapMarker =  HEAPOBJECTMARKER(stackSlot.stackObj.pos);
            PRINTF("\t[#%d|%s|%d|%s]\n",stackSlot.stackObj.pos,heapElemTypes[heapMarker.status],heapMarker.length,(heapMarker.rootCheck ? "true" : "false"));
        }else{
            PRINTF("NotAnObject\n");
        }
    }
     PRINTF("End Stack Objects\n");
}

/* heap */
void heapInit()
{
    if ((heapBase = ( slot*) malloc(sizeof(slot) * (size_t)MAXHEAP)) == NULL){
        ERROREXIT(-1, "malloc error\n");// heap fixed size!!
    }

    while (heapTop > 0)
        *(heapBase + (--heapTop)) = NULLOBJECT;

    DEBUGPRINTHEAP;
}

slot heapGetElement(const u2 pos)
{
    return *(heapBase + pos);
}

slot* heapGetElementRef(const u2 pos)
{
    return (heapBase + pos);
}

void heapSetElement(const slot e,const u2 pos)
{
    *(heapBase + pos) = e;
}

u2 heapGetNextObjectPos(const u2 pos)
{
    return pos + (heapBase + pos)->heapObjMarker.length;
}

u4 heapGetFreeMemory(){
    return MAXHEAP - heapTop;
}

u4   heapGetTotalMemory(void){
    return MAXHEAP;
}

void heapCollectGarbage()
{
    heapPrintBlocks();
    checkObjects();
    heapPrintBlocks();
    heapMergeFreeBlocks();
    heapPrintBlocks();
}

void heapSetObjectMarker(const u2 position,const u2 length,const u1 type,stackObjectInfo* stackObjectRef,const u1 rootCheck)
{
    heapObjectMarker* marker = &(heapBase + position)->heapObjMarker;

    marker->status = type;
    marker->length = length;// get exact length slots
    marker->magic = OBJECTMAGIC;
    marker->mutex = MUTEXNOTBLOCKED;
    marker->rootCheck = rootCheck;

    stackObjectRef->pos = position;
    stackObjectRef->magic = OBJECTMAGIC;
}

/* Returns address to newly allocated element (not heap block marker!)*/
u2 heapAllocElement(const u2 elemLength,const u1 type,stackObjectInfo* stackObjectRef,const u1 rootCheck)
{
    const u2 length = elemLength + 1;
    // circulars are recognizable reference on heap without regard to opstack !!!
    if (heapTop + length < MAXHEAP)
    {
        heapSetObjectMarker(heapTop,length,type,stackObjectRef,rootCheck);
        heapTop += length;
        heapPrintBlocks();
        return stackObjectRef->pos + 1;
    }// free space on heap

    //heapMergeFreeBlocks();

    //checkObjects();
    heapCollectGarbage();

    u2 nextElementPos = 0;
    do//Search to fit in a slot that was freed
    {
        const heapObjectMarker marker = (heapBase + nextElementPos)->heapObjMarker;//HEAPOBJECTMARKER(nextElementPos);
        if (marker.status == HEAPFREESPACE){
            if(marker.length >= length){
                PRINTF("Searching for %d free space,found %d at pos %d.\n",length,marker.length,nextElementPos);
                heapSetObjectMarker(nextElementPos,length,type,stackObjectRef,rootCheck);
                return nextElementPos + 1;// may be get more space than length
            }else{
                PRINTF("Searching for %d free space,not found %d at pos %d.\n",length,marker.length,nextElementPos);
            }
        }
        nextElementPos = heapGetNextObjectPos(nextElementPos);
    } while (nextElementPos < heapTop);

    ERROREXIT(-1," No free heap space for object/array of length: 0x%x\n",length);
    return 0;
}

void heapMergeFreeBlocks()
{
    //PRINTF("Heap merge\n");
    //heapPrintBlocks();
    u2 nextElementPos = 0;
    u2 noFreeBlocks = 0;

    heapObjectMarker* elemMarker = &(heapBase + nextElementPos)->heapObjMarker;
    while ((nextElementPos = heapGetNextObjectPos(nextElementPos)) < heapTop)
    {
        heapObjectMarker* nextMarker = &(heapBase + nextElementPos)->heapObjMarker;

        if (elemMarker->status == HEAPFREESPACE &&  nextMarker->status == HEAPFREESPACE)
        {
            noFreeBlocks++;
            elemMarker->length += nextMarker->length;
        } else
            elemMarker = nextMarker;
    }

    if(noFreeBlocks > 0){
        // heapPrintBlocks();
        PRINTF("Heap merged %d free blocks.\n",noFreeBlocks);
    }

    if(elemMarker->status == HEAPFREESPACE){
        heapTop = heapTop - elemMarker->length;
        PRINTF("Heap top:%d\n",heapTop);
    }}

void checkObjects()
{
    //static objects or objects in opstack (root objects) or objects, referenced by other objects (circular??)
    // Will this be enough?
    // Do I bump heap objects (rootCheck = 1), the other I give abschu free !!
    u2 nextElementPos = 0;

    heapObjectMarker* heapBlockMarker = NULL;
    do
    {
        heapBlockMarker = &(heapBase + nextElementPos)->heapObjMarker;
        heapBlockMarker->rootCheck = 0;//HEAPOBJECTMARKER(nextElementPos).rootCheck = 0;
        if(heapBlockMarker->status == HEAPFREESPACE
        || heapBlockMarker->status == HEAPALLOCATEDSTATICCLASSOBJECT)// empty or static
        {
            heapBlockMarker->rootCheck = 1;
            continue;
        }
#ifndef TINYBAJOS_MULTITASKING
        ThreadControlBlock* thread = threadList.cb;
        for (int k = 0; k < threadList.count; k++)
        {
            heapPrintStackReferences(thread);
            
            u2 opStackPos = 0;
            if(k == 0){//tCB->tid == currentThreadCB->tid)
                //current thread
                opStackPos = opStackGetSpPos();
            }else{
                opStackPos = *(thread->methodStackBase + thread->methodSpPos - 1);
            }

            while (opStackPos > 0)
            {
                opStackPos--;
                const slot stackSlot = *(thread->opStackBase + opStackPos);
                //const heapObjectMarker heapMarker =  HEAPOBJECTMARKER(stackSlot.stackObj.pos);
                
                if(stackSlot.stackObj.pos == nextElementPos
                && stackSlot.stackObj.magic == OBJECTMAGIC)
                {//Found a reference to this heap memory block mark as root for root check
                    heapBlockMarker->rootCheck = 1;
                    break;
                }
            }
            //Found a reference to this heap memory block can go to next block
            if (heapBlockMarker->rootCheck == 1)
            {
                break;
            }
            thread = thread->succ;
        }
        /*if (heapBlockMarker->rootCheck == 1)
        {
            break;
        }*/
#else
        u2 opSPPos=opStackGetSpPos();
        while (opSPPos > 0)
        {
            if( (nextElementPos == ((*(opStackGetBase() + (--opSPPos))).stackObj.pos))
               &&(((*(opStackGetBase() + (opSPPos))).stackObj.magic)==OBJECTMAGIC ))
            {
                HEAPOBJECTMARKER(nextElementPos).rootCheck = 1;
                break;
            }
        }
#endif
    } while ((nextElementPos = heapGetNextObjectPos(nextElementPos)) < heapTop);

    // All static objects and stack referenced objects are selected(marked as roots)
    // Now mark childs of roots
    u1 stillAConcatedObject = 0;
    do
    {
        stillAConcatedObject = 0;
        nextElementPos = 0;
        do
        {
            heapBlockMarker = &(heapBase + nextElementPos)->heapObjMarker;
            if (heapBlockMarker->rootCheck == 1 && heapBlockMarker->status != HEAPFREESPACE)
            {   // seraching for "objects in root-objects"
                for (int i = 1; i < heapBlockMarker->length; i++)
                {
                    const slot heapObject = HEAPOBJECT(nextElementPos + i);
                    if(heapObject.stackObj.magic == OBJECTMAGIC
                    && heapObject.UInt != NULLOBJECT.UInt
                    && canItBeAnObject(heapObject.stackObj.pos))
                    {
                        heapObjectMarker* heapObjectChild = &(heapBase + heapObject.stackObj.pos)->heapObjMarker;
                        if(heapObjectChild->magic == OBJECTMAGIC && heapObjectChild->rootCheck == 0)
                        {
                            heapObjectChild->rootCheck = 1;
                            stillAConcatedObject = 1;
                        }
                    }
                }
            }
        } while ((nextElementPos = heapGetNextObjectPos(nextElementPos)) < heapTop);
    } while (stillAConcatedObject);

    nextElementPos = 0;
    u2 noFreedOjects = 0;

    do
    {
        heapBlockMarker = &(heapBase + nextElementPos)->heapObjMarker;
        if (heapBlockMarker->rootCheck == 0){
            noFreedOjects++;
            heapBlockMarker->status = HEAPFREESPACE;
        }
    }while ((nextElementPos = heapGetNextObjectPos(nextElementPos)) < heapTop);

    if(noFreedOjects > 0){
        PRINTF("Freed %d objects.\n",noFreedOjects);
    }
}

u1 canItBeAnObject(const u2 pos)
{
    u2 nextElementPos = 0;
    do
    {
        if (nextElementPos == pos)
            return 1;

        nextElementPos = heapGetNextObjectPos(nextElementPos);
    } while (nextElementPos < heapTop);
    // cannot be an object
    return 0;
}
