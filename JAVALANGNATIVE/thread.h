/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */

// Thread
#ifndef TINYBAJOS_MULTITASKING
char isAlive(void);
char start(void);
char yield(void);
char currentThread(void);
char nativeSetPriority(void);
char interrupt(void);
char interrupted(void);
char isInterrupted(void);
char isDaemon(void);
char setDaemon(void);
char join(void);
char ksleep(void);//krix
char jointimeout(void);
char nativeSleep(void);
#endif
