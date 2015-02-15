/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */

/* arduinoMega */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../JVM/definitions.h"
#include "../../JVM/typedefinitions.h"
#include "../../JVM/bajvm.h"
#include "platform.h"
#include "native.h"
#include <ctype.h>
#include <avr/pgmspace.h>
#include "arduinomega.h"

void getCharsFromFlash(u1*, u1, char*);

FILE uartAVR8 = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

#ifdef WITHMON
/* Monitor  functions (bamo128 -> cs.ba-berlin.de)*/
#define mainLoop    BYTES(LARGEBOOTSTART+2)       /* Ruecksprung in Monitor aus Programm mit goto*/
#define saveCPU     BYTES(LARGEBOOTSTART+62)      /*Time2Comp	// BOOTSTART+62		*/
#define mySysClock      BYTES(LARGEBOOTSTART+56)  /* timer1 oc1A int */

char (*conIn)() =(void*)(LARGEBOOTSTART+4);
void (*conOut)(char) =(void*)(LARGEBOOTSTART+6);
void (*startTimer1)() =(void*)(LARGEBOOTSTART+58);
void (*stopTimer1)() =(void*)(LARGEBOOTSTART+60);

int uart_putchar(char c, FILE *stream)
{
    conOut(c);
    if (c=='\n') conOut('\r');
    return 0;
}


int uart_getchar(FILE *stream)
{
    return conIn();
}


// monitor interrupt für step ...
ISR(TIMER2_COMPA_vect) __attribute__ ((naked));
ISR(TIMER2_COMPA_vect) {asm volatile (INLINEASM(jmp,saveCPU));}

/* monitor interrupt for sysclock millisec */
ISR(TIMER1_COMPA_vect) __attribute__ ((naked));
ISR(TIMER1_COMPA_vect) {asm volatile (INLINEASM(jmp,mySysClock));}

#else
void conOut(char c)
{
    uart_putchar(c, &uartAVR8);
}


char conIn()
{
    return uart_getchar(&uartAVR8);
}


int uart_putchar(char c, FILE *stream)
{
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    if (c == '\n')
        uart_putchar('\r', stream);
    return 0;
}


int uart_getchar(FILE *stream)
{
    loop_until_bit_is_set(UCSR0A, RXC0);
    return (int) UDR0;
}


ISR(TIMER1_COMPA_vect)
{
    timerMilliSec++;
}


void startTimer1()
{
    TCCR1A = 0;
    OCR1A = (unsigned int) OCR1ATIME;
    TCCR1B = 1 + (1 << WGM12);                    // clock divider 1 (sysclock), ctc-mode
    TIMSK1 = (1 << OCIE1A);
    sei();
// global interrupt zulassen
}
#endif

void initHW()
{
    asm volatile ( "ldi r24,1");
    asm volatile ( "clr r1");
    asm volatile ("out 0x3b, r24");
// RAMPZ  ever 1
#ifndef WITHMON
    startUart0();
#endif
    startTimer1();
    stdout = stdin = stderr = &uartAVR8;
}


/* all class files stored for linux in DS (malloc)*/
/* for avr8 all class files in flash */
void initVM()                                     /* read, analyze classfiles and fill structures*/
{
    u1* addr;
    char buf[4];
    heapInit();                                   /* linux avr8 malloc , others hard coded!*/
// all classes in flash for arduinoMega and CharonII
    addr = (u1*) AVR8_FLASH_JAVA_BASE;
    buf[4] = 0;
    getCharsFromFlash(addr, 4, buf);
    sscanf(buf, "%4d", (char*) &numClasses);
    addr += 4;                                    // after numclasses*

    for (cN = 0; cN < numClasses; cN++)
    {
        getCharsFromFlash(addr, 4, buf);
        sscanf(buf, "%4d", (char*) &cs[cN].classFileLength);
                                                  // after length of class;
        cs[cN].classFileStartAddress = (char*) addr + 4;
        analyzeClass(&cs[cN]);
        addr += cs[cN].classFileLength + 4;

        verbosePrintf("bootclass: %x length:%x loaded\n", cN,
            cs[cN].classFileLength);
    }

    verbosePrintf("load java application classes: \n");

#ifdef WITHMON
    addr=(char*)AVR8_FLASH_APP_BASE;
#endif
    getCharsFromFlash(addr, 4, buf);
    sscanf(buf, "%4d", (char*) &numClasses);
    numClasses += cN;
    addr += 4;                                    // after numclasses
    for (; cN < numClasses; cN++)
    {
        getCharsFromFlash(addr, 4, buf);
        sscanf(buf, "%4d", (char*) &cs[cN].classFileLength);
                                                  // after length of class
        cs[cN].classFileStartAddress = (char*) addr + 4;
        analyzeClass(&cs[cN]);
        addr += cs[cN].classFileLength + 4;

        verbosePrintf("appclass: %x length:%x loaded\n", numClasses,
            cs[cN].classFileLength);

    }

    DEBUGPRINTHEAP;
}


#ifndef TINYBAJOS_OTHER
void VT102Attribute(u1 fgcolor, u1 bgcolor)
{

#ifdef AVR8                                   // change all avr8 string to flash strings gives more data ram space for java!!
    printf_P(PSTR("%c"),0x1b);
#else
    printf("%c", 0x1b);
#endif

#ifdef AVR8                                   // change all avr8 string to flash strings gives more data ram space for java!!
    printf_P(PSTR("%c"),'[');
#else
    printf("%c", '[');
#endif

/*
 #ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
 printf_P(PSTR("%c"),'4');
#else
printf("%c",'4');
#endif
*/

#ifdef AVR8                                   // change all avr8 string to flash strings gives more data ram space for java!!
    printf_P(PSTR("%c"),fgcolor);
#else
    printf("%c", fgcolor);
#endif

/*
 #ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
 printf_P(PSTR("%c"),';');
#else
printf("%c",';');
#endif
*/
/*
 #ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
 printf_P(PSTR("%c"),40 + bgcolor);
#else
printf("%c",40 + bgcolor);
#endif
*/

#ifdef AVR8                                   // change all avr8 string to flash strings gives more data ram space for java!!
    printf_P(PSTR("%c"),'m');
#else
    printf("%c", 'm');
#endif

}
#endif

void exit(int n)
{
#ifdef DEBUGOPSTACK
    printf(" maxOperandenStack: x%x",maxOpStack);
#endif                                        //DEBUOPSTACK
#ifdef DEBUGMETHODSTACK
    printf(" maxMethodStack: x%x",maxMethodStack);
#endif                                        //DEBUGMETHODSTACK
#ifndef WITHMON
    asm volatile (" ma: rjmp	ma	");
#else
    goto *0xf002;                                 /*asm	 (INLINEASM(jmp,0xf002));*/
#endif                                        //WITHMON
}
