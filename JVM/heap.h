/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07
#ifndef __HEAP__
#define __HEAP__

void heapInit(void);
void heapSetElement(slot e, u2 pos);              // mb jf switched order of arguments
slot heapGetElement(u2 pos);
u2 getNextHeapObjectPos(u2 pos);

// garbage collection
u2 getFreeHeapSpace(u2 length);
void checkObjects(void);
u1 canItBeAnObject( u2);
#endif
