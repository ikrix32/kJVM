/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
/* ************************************************************************ */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../JVM/definitions.h"
#include "../../JVM/typedefinitions.h"

#define     BYTES(word) ((word)*2)
#define     STRING(a,b) #a" "#b
#define     INLINEASM(a,b)  STRING(a,b)
#define     LARGEBOOTSTART  0xf000
#define     BOARDRAMEND 0x2200
#define     MONSTART    LARGEBOOTSTART
#define     MONRAM      (BOARDRAMEND-0x100)
#define     SYSTIMEMILLISEC (MONRAM+0x40)
#include "../../JVM/classfile.h"
#include "../../JVM/heap.h"

#ifndef WITHMON
void startTimer1();
void startUart0();
#endif
void VT102Attribute(u1 fgcolor, u1 bgcolor);
int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);
void exit(int);
