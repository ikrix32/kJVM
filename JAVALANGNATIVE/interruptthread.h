/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */

#ifndef TINYBAJOS_MULTITASKING
char static_sei(void);
char static_cli(void);
char initInterrupt();
char removeInterrupt();
char forceInterrupt();
#endif
