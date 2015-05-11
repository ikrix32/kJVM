#ifndef __HEAP__
#define __HEAP__

void heapInit(void);
void heapSetElement(const slot e,const u2 pos);              // mb jf switched order of arguments
slot heapGetElement(const u2 pos);
slot*heapGetElementRef(const u2 pos);
u2   heapGetNextObjectPos(const u2 pos);

u2   heapAllocElement(const u2 length,const u1 type,stackObjectInfo* stackObjectRef,const u1 rootCheck);
void heapMergeFreeBlocks(void);
// garbage collection
void checkObjects(void);
u1   canItBeAnObject(const u2);

u4   heapGetFreeMemory(void);
u4   heapGetTotalMemory(void);
void heapCollectGarbage(void);

#endif
