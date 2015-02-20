/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
#ifndef _KJVM_OBJECT_
#define _KJVM_OBJECT_
// Object
#ifndef TINYBAJOS_MULTITASKING
char nativeWait(void);
char notify(void);
char notifyAll(void);
char nativeWait(void);
char waitTime(void);
#endif
char getDataAddress(void);
#endif
