/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
// development board CharonII
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../JVM/definitions.h"
#include "../../JVM/typedefinitions.h"
#include "../../JVM/bajvm.h"
#include "lcd.h"
#include "platform.h"
#include "native.h"

extern u1 getU1Flash( u2);
extern void getCharsFromFlash(char*, u1, char*);

// Monitor  functions (bamo128 -> cs.ba-berlin.de)
#define mainLoop    BYTES(LARGEBOOTSTART+2)       // Ruecksprung in Monitor aus Programm mit goto
#define saveCPU     BYTES(LARGEBOOTSTART+62)      //Time2Comp	// BOOTSTART+62
#define mySysClock      BYTES(LARGEBOOTSTART+56)  /* timer1 overflow interrupt */

FILE uartAVR8 = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

SIGNAL(SIG_OUTPUT_COMPARE2)
__attribute__ ((naked));
SIGNAL(SIG_OUTPUT_COMPARE2)
{
    asm volatile (INLINEASM(jmp,saveCPU));
}                                                 // monitor for step-mode


SIGNAL(SIG_OUTPUT_COMPARE1A)
__attribute__ ((naked));
SIGNAL(SIG_OUTPUT_COMPARE1A)
{
    asm volatile (INLINEASM(jmp,mySysClock));
}


/* monitor for sysclock millisec */

char (*conIn)() = (void*) (LARGEBOOTSTART + 4);
void (*conOut)(char) = (void*) (LARGEBOOTSTART + 6);
void (*startTimer1)() = (void*) (LARGEBOOTSTART + 58);
void (*stopTimer1)() = (void*) (LARGEBOOTSTART + 60);

void initHW()
{
    startTimer1();
    uart1Init();
    stdout = stdin = stderr = &uartAVR8;
    LCD_Init();
}


/* all class files stored for linux in DS (malloc)*/
/* for avr8 all class files in flash */
void initVM()                                     /* read, analyze classfiles and fill structures*/
{
    u1* addr;
    u1 buf[5];

    heapInit();                                   //linux avr8 malloc , others hard coded
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
        cs[cN].classFileStartAddress = addr + 4;  // after length of class;
        analyzeClass(&cs[cN]);
        addr += cs[cN].classFileLength + 4;
        printf("bootclass: %x length:%x loaded\n", cN, cs[cN].classFileLength);
    }

    printf("load java application classes: \n");
    addr = (char*) AVR8_FLASH_APP_BASE;
    getCharsFromFlash(addr, 4, buf);
    sscanf(buf, "%4d", (char*) &numClasses);
    numClasses += cN;
    addr += 4;                                    // after numclasses
    for (; cN < numClasses; cN++)
    {
        getCharsFromFlash(addr, 4, buf);
        sscanf(buf, "%4d", (char*) &cs[cN].classFileLength);
        cs[cN].classFileStartAddress = addr + 4;  // after length of class
        analyzeClass(&cs[cN]);
        addr += cs[cN].classFileLength + 4;
        printf("appclass: %x length:%x loaded\n", cN, cs[cN].classFileLength);
    }
    DEBUGPRINTHEAP;

/*
 // take a global var (dummy -> 8Byte)

getCharsFromFlash((char*)AVR8_FLASH_JAVA_BASE,4,(void*)&dummy);
sscanf((char*)&dummy,"%4d",(u1*)&dummy1);
numClasses=(u1)dummy1;
addr=(char*)AVR8_FLASH_JAVA_BASE+4;		// after numclasses
for (cN=0; cN<numClasses;cN++)			{
getCharsFromFlash(addr,4,(void*)&dummy); // num bytes in class
sscanf((char*)&dummy,"%4d",(u1*)&dummy1);
cs[cN].classFileLength=(u2)dummy1;
cs[cN].classFileStartAddress=addr+4;	// after length of class
analyzeClass(&cs[cN]);
addr+=cs[cN].classFileLength+4;
printf("bootclass: %x loaded\n",cN);	}
printf("load java application classes: \n");
addr=(char*)AVR8_FLASH_APP_BASE;
getCharsFromFlash(addr,4,(void*)&dummy);	// num classes
addr+=4;
sscanf((char*)&dummy,"%4d",dummy1);
numClasses=(u1)dummy1;
numClasses+=cN;
for (;cN<numClasses;cN++)				{
getCharsFromFlash(addr,4,(void*)&dummy);	// bytes
sscanf((char*)&dummy,"%4d",(u1*)&dummy1);
cs[cN].classFileLength=(u2)dummy1;
cs[cN].classFileStartAddress=addr+4;	// after length of class
printf("ttt %x %x %x %x %x \n",numClasses,cs,cN,cs[cN].classFileStartAddress,cs[cN].classFileLength);
analyzeClass(&cs[cN]);
addr+=cs[cN].classFileLength+4;
printf("application class: %x loaded\n",cN);	}
DEBUGPRINTHEAP;
printf("%x\n",numClasses);
*/
}


void VT10q2Attribute(u1 fgcolor, u1 bgcolor)
{
    printf("%c", 0x1b);
    printf("%c", '[');
//  printf("%c",'4');
    printf("%c", fgcolor);
//printf("%c",';');
//printf("%c",40 + bgcolor);
    printf("%c", 'm');
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


void exit(int n)
{
    goto *0xf002;                                 //asm	 (INLINEASM(jmp,0xf002));
}


void uart1Init()
{
    /* Set baud rate */UBRR1H = (unsigned char) ((F_CPU / 16 / BAUD1) >> 8);
    UBRR1L = (unsigned char) (F_CPU / 16 / BAUD1);
    /* Enable receiver and transmitter */UCSR1B = (1 << RXEN1) | (1 << TXEN1);
    /* Set frame format: 8data, 2stop bit */UCSR1C = (1 << USBS1) | (3
        << UCSZ10);
}
