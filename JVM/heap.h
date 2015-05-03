#ifndef __HEAP__
#define __HEAP__

void heapInit(void);
void heapSetElement(const slot e,const u2 pos);              // mb jf switched order of arguments
slot heapGetElement(const u2 pos);
slot*heapGetBase(void);
u2   getNextHeapObjectPos(const u2 pos);

// garbage collection
u2   getFreeHeapSpace(const u2 length);
u2   getHeapFreeSpace(void);
void checkObjects(void);
u1   canItBeAnObject(const u2);
#endif
