/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
/* ************************************************************************ */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../JVM/definitions.h"
#include "../../JVM/typedefinitions.h"
#include "lcd.h"

#define     BYTES(word) ((word)*2)
#define     STRING(a,b) #a" "#b
#define     INLINEASM(a,b)  STRING(a,b)
#define     LARGEBOOTSTART  0xf000
#define     BOARDRAMEND 0x8000
#define     MONSTART    LARGEBOOTSTART
#define     MONRAM      (BOARDRAMEND-0x100)
#define     SYSTIMEMILLISEC (MONRAM+0x40)

#include "../../JVM/classfile.h"
#include "../../JVM/heap.h"

/* CPU frequency */
#define F_CPU 14745000UL
/* UART baud rate */
#define BAUD0  115200
#define BAUD1  115200
//#define BAUD1 9600

void initHW();
void initVM();
void VT102Attribute(u1 fgcolor, u1 bgcolor);
void timer_Init();
void exit(int n);
int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);
void uart1Init();
