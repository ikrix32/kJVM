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
extern u1 numClasses;
extern classStructure cs[MAXCLASSES];

///#define DEBUG_GARBAGE_COLLECTOR
#ifdef DEBUG_GARBAGE_COLLECTOR
char* heapElemTypes[4] = { "FREE","STATIC_OBJ","OBJ","ARRAY"};
void heapPrintBlocks(void);
void heapPrintStackReferences(ThreadControlBlock* thread);
#endif

void heapMergeFreeBlocks(void);
void checkObjects(void);
u1   canItBeAnObject(const u2);
u1   heapCompactMemory();

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
    u2 freeSpace = MAXHEAP - heapTop;
    u2 crtPosition = 0;
    do
    {
        const heapObjectMarker marker = (heapBase + crtPosition)->heapObjMarker;
        if(marker.status == HEAPFREESPACE){
            freeSpace += marker.length;
        }
        crtPosition = heapGetNextObjectPos(crtPosition);
    } while (crtPosition < heapTop);
    return freeSpace;
}

u4   heapGetTotalMemory(void){
    return MAXHEAP;
}

void heapCollectGarbage()
{
    //PRINTF("heapCollectGarbage\n");
    checkObjects();
    heapMergeFreeBlocks();
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

u2 heapFitElement(const u2 length)
{//todo - maybe best fit algorithm will be better
    u2 nextElementPos = 0;
    do//Search to fit in a slot that was freed
    {
        heapObjectMarker* marker = &(heapBase + nextElementPos)->heapObjMarker;
        if (marker->status == HEAPFREESPACE){
            if(marker->length >= length)
            {
#ifdef DEBUG_GARBAGE_COLLECTOR
                PRINTF("Searching for %d free space,found %d at pos %d.\n",length,marker->length,nextElementPos);
#endif
                marker->length -= length;
                nextElementPos += marker->length;
                return nextElementPos;
            }
#ifdef DEBUG_GARBAGE_COLLECTOR
            else
                PRINTF("Searching for %d free space,not found %d at pos %d.\n",length,marker->length,nextElementPos);
#endif

        }
        nextElementPos = heapGetNextObjectPos(nextElementPos);
    } while (nextElementPos < heapTop);

    return 0;
}

/* Returns address to newly allocated element (not heap block marker!)*/
u2 heapAllocElement(const u2 elemLength,const u1 type,stackObjectInfo* stackObjectRef,const u1 rootCheck)
{
    const u2 length = elemLength + 1;
    int tryIndex = 0;

    do{
        if(tryIndex == 0 || tryIndex == 2 || tryIndex == 5){
            // circulars are recognizable reference on heap without regard to opstack !!!
            if (heapTop + length < MAXHEAP)
            {
                heapSetObjectMarker(heapTop,length,type,stackObjectRef,rootCheck);
                heapTop += length;
                return stackObjectRef->pos + 1;
            }// free space on heap
        }

        if(tryIndex == 3 || tryIndex == 6){
            u2 elemPos = heapFitElement(length);
            if(elemPos > 0){
                heapSetObjectMarker(elemPos,length,type,stackObjectRef,rootCheck);
                return elemPos + 1;
            }
        }

        if(tryIndex == 1){
            //todo - check if this is working when having more threads running
            heapCollectGarbage();
        }

        if(tryIndex == 4){
            heapCompactMemory();
        }
        tryIndex++;
    }while (tryIndex <= 6);

    ERROREXIT(-1,"Error: OutOfMemory, no free heap space for object of length: %d.\n",length);
    return 0;
}

void heapUpdateStackReferences(const u2 src,const u2 size,const u2 offset)
{
#ifndef TINYBAJOS_MULTITASKING

    ThreadControlBlock* thread = threadList.cb;
    for (int k = 0; k < threadList.count; k++)
    {
        u2 opStackPos = k == 0 ? opStackGetSpPos() : *(thread->methodStackBase + thread->methodSpPos - 1);

        while (opStackPos > 0)
        {
            opStackPos--;
            slot* stackSlot = (thread->opStackBase + opStackPos);

            if( stackSlot->stackObj.magic == OBJECTMAGIC
            &&  src <= stackSlot->stackObj.pos
            && stackSlot->stackObj.pos <= src + size)
            {
                stackSlot->stackObj.pos = stackSlot->stackObj.pos - offset;
            }
        }
        thread = thread->succ;
    }
#else
    u2 opStackPos = opStackGetSpPos();
    while (opStackPos > 0)
    {
        slot* stackSlot = (thread->opStackBase + opStackPos);

        if(src <= stackSlot->stackObj.pos && stackSlot->stackObj.pos <= src + size)
        {
            stackSlot->stackObj.pos = stackSlot->stackObj.pos - offset;
        }
    }
#endif
    for(int i = 0; i < numClasses;i++)
    {
        if(cs[i].classInfo.stackObj.magic == OBJECTMAGIC
        && src <= cs[i].classInfo.stackObj.pos
        && cs[i].classInfo.stackObj.pos <= src + size)
        {
            cs[i].classInfo.stackObj.pos -= offset;
        }
    }
}

void heapUpdateObjectsReferences(const u2 src,const u2 size,const u2 offset)
{
    u2 crtBlock = 0;
    do{
        heapObjectMarker* heapBlockMarker = &(heapBase + crtBlock)->heapObjMarker;
        if (heapBlockMarker->status != HEAPFREESPACE)
        {// seraching objects references
            for (int i = 1; i < heapBlockMarker->length; i++)
            {
                slot* heapObject = (heapBase + crtBlock + i);
                //can be a object reference
                if(heapObject->stackObj.magic == OBJECTMAGIC && heapObject->UInt != NULLOBJECT.UInt)
                {
                    if(heapObject->stackObj.pos >= src && heapObject->stackObj.pos <= src + size)
                    {//this can be a reference to an object from the shifted memory block
                        u2 heapObjectPos = 0;
                        do
                        {//find the object
                            if(heapObjectPos == heapObject->stackObj.pos)
                                heapObject->stackObj.pos -= offset;
                           
                            heapObjectPos = heapGetNextObjectPos(heapObjectPos);
                        } while (heapObjectPos < src + size);

                    }
                }
            }
        }
    } while ((crtBlock = heapGetNextObjectPos(crtBlock)) < heapTop);
}

void heapMemShift(const u2 dest,const u2 src,const u2 size){
    for(int i = 0;i < size;i++){
        *(heapBase + dest + i) = *(heapBase + src + i);
    }
}

u1 heapCompactMemory()
{
    PRINTF("Warning: Heap compacting is running,this is proccessor intensive,to avoid this increase heap size.\n");
    u2 crtPosition = 0;
    do
    {//find first free space
        heapObjectMarker* marker = &(heapBase + crtPosition)->heapObjMarker;
        if(marker->status == HEAPFREESPACE)
        {//found free space search next record
            u2 nextPosition = crtPosition;
            while ((nextPosition = heapGetNextObjectPos(nextPosition)) < heapTop)
            {
                const heapObjectMarker nextMarker = (heapBase + nextPosition)->heapObjMarker;
                if(nextMarker.status == HEAPFREESPACE)
                {
                    const u2 offset = marker->length;
                    const u2 length= nextPosition - (crtPosition + offset);
#ifdef DEBUG_GARBAGE_COLLECTOR
                    PRINTF("Compacting memory [%d,%d] - %d\n",crtPosition + offset,crtPosition + offset + length,offset);
                    heapPrintBlocks();
                    heapPrintStackReferences(threadList.cb);
#endif
                    heapUpdateObjectsReferences(crtPosition + offset,length,offset);
                    heapMemShift(crtPosition,crtPosition + offset,length);
                   
                    marker = &(heapBase + nextPosition - offset)->heapObjMarker;
                    marker->status = HEAPFREESPACE;
                    marker->length = nextMarker.length + offset;

                    heapUpdateStackReferences(crtPosition + offset,length,offset);
                    nextPosition -= offset;
                    crtPosition = nextPosition;
#ifdef DEBUG_GARBAGE_COLLECTOR
                    PRINTF("Done Compacting.\n");
                    heapPrintBlocks();
                    heapPrintStackReferences(threadList.cb);
#endif
                }
            }
            return 1;
        }
        crtPosition = heapGetNextObjectPos(crtPosition);
    } while (crtPosition < heapTop);

    return 0;
}

void heapMergeFreeBlocks()
{
#ifdef DEBUG_GARBAGE_COLLECTOR
    PRINTF("Heap merge\n");
    heapPrintBlocks();
    u2 noFreeBlocks = 0;
#endif
    u2 nextElementPos = 0;

    heapObjectMarker* elemMarker = &(heapBase + nextElementPos)->heapObjMarker;
    while ((nextElementPos = heapGetNextObjectPos(nextElementPos)) < heapTop)
    {
        heapObjectMarker* nextMarker = &(heapBase + nextElementPos)->heapObjMarker;

        if (elemMarker->status == HEAPFREESPACE &&  nextMarker->status == HEAPFREESPACE)
        {
#ifdef DEBUG_GARBAGE_COLLECTOR
            noFreeBlocks++;
#endif
            elemMarker->length += nextMarker->length;
        } else
            elemMarker = nextMarker;
    }
#ifdef DEBUG_GARBAGE_COLLECTOR
    if(noFreeBlocks > 0){
        heapPrintBlocks();
        PRINTF("Heap merged %d free blocks.\n",noFreeBlocks);
    }
#endif

    if(elemMarker->status == HEAPFREESPACE){
        heapTop = heapTop - elemMarker->length;
    }}

void checkObjects()
{
    //static objects or objects in opstack (root objects) or objects, referenced by other objects (circular??)
    // Will this be enough?
    // Do I bump heap objects (rootCheck = 1), the other I give abschu free !!
    u2 nextElementPos = 0;

    heapObjectMarker* heapBlockMarker = NULL;
#ifdef DEBUG_GARBAGE_COLLECTOR
    heapPrintBlocks();

    ThreadControlBlock* thread = threadList.cb;
    for (int k = 0; k < threadList.count; k++)
        heapPrintStackReferences(thread);
#endif
    do
    {
        heapBlockMarker = &(heapBase + nextElementPos)->heapObjMarker;
        heapBlockMarker->rootCheck = 0;
        if(heapBlockMarker->status == HEAPFREESPACE
        || heapBlockMarker->status == HEAPALLOCATEDSTATICCLASSOBJECT)
        {// empty or static
            heapBlockMarker->rootCheck = 1;
            continue;
        }
#ifndef TINYBAJOS_MULTITASKING
        ThreadControlBlock* thread = threadList.cb;
        for (int k = 0; k < threadList.count; k++)
        {
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

                if(stackSlot.stackObj.pos   == nextElementPos
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
#else
        u2 opStackPos = opStackGetSpPos();
        while (opStackPos > 0)
        {
            opStackPos--;
            const slot stackSlot = *(thread->opStackBase + opStackPos);

            if(stackSlot.stackObj.pos   == nextElementPos
            && stackSlot.stackObj.magic == OBJECTMAGIC)
            {//Found a reference to this heap memory block can go to next block
                heapBlockMarker.rootCheck = 1;
                break;
            }
        }
#endif
    } while ((nextElementPos = heapGetNextObjectPos(nextElementPos)) < heapTop);

#ifdef DEBUG_GARBAGE_COLLECTOR
    heapPrintBlocks();
#endif
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
#ifdef DEBUG_GARBAGE_COLLECTOR
    u2 noFreedOjects = 0;
#endif
    do
    {
        heapBlockMarker = &(heapBase + nextElementPos)->heapObjMarker;
        if (heapBlockMarker->rootCheck == 0){
            heapBlockMarker->status = HEAPFREESPACE;
#ifdef DEBUG_GARBAGE_COLLECTOR
            noFreedOjects++;
#endif
        }
    }while ((nextElementPos = heapGetNextObjectPos(nextElementPos)) < heapTop);
#ifdef DEBUG_GARBAGE_COLLECTOR
    heapPrintBlocks();
    if(noFreedOjects > 0){
        PRINTF("Freed %d objects.\n",noFreedOjects);
    }
#endif
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

#ifdef DEBUG_GARBAGE_COLLECTOR
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
        PRINTF("[#%d|%d|%x]",opStackPos,stackSlot.stackObj.pos,stackSlot.stackObj.magic);
        if(stackSlot.stackObj.magic == OBJECTMAGIC)
        {
            const heapObjectMarker heapMarker =  HEAPOBJECTMARKER(stackSlot.stackObj.pos);
            PRINTF("\t[#%d|%s|%d|%s]\n",stackSlot.stackObj.pos,heapElemTypes[heapMarker.status],heapMarker.length,(heapMarker.rootCheck ? "true" : "false"));
        }else{
            PRINTF("\tNotAnObject\n");
        }
    }
    PRINTF("End Stack Objects\n");
}
#endif
