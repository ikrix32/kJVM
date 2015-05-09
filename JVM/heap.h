#ifndef __HEAP__
#define __HEAP__

void heapInit(void);
void heapSetElement(const slot e,const u2 pos);              // mb jf switched order of arguments
slot heapGetElement(const u2 pos);
slot*heapGetElementRef(const u2 pos);
slot*heapGetBase(void);
u2   heapGetNextObjectPos(const u2 pos);

u2   heapGetFreeSpace(const u2 length);
// garbage collection
void checkObjects(void);
u1   canItBeAnObject(const u2);

u4   heapGetFreeMemory(void);
u4   heapGetTotalMemory(void);
void heapCollectGarbage(void);

#endif
