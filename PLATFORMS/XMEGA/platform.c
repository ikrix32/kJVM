// its the xplain board
/**
 mu� mit serial avrdude und stromversorgung über jtag programmiert werden /dev/ttyUSB0 oder USB mit usb-dev
 aus autoconf.h
das arbeitet mit 9600 bauds
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <board.h>
#include <avr/io.h>
#include <clk/sys.h>
#include <led.h>
#include <uart.h>
#include <uart/ctrl.h>
#include <uart/uart_xmega.h>
#include "../../JVM/definitions.h"
#include "../../JVM/typedefinitions.h"
#include "../../JVM/bajvm.h"
#include "platform.h"
#include "native.h"
#include "../../JVM/heap.h"
#include "../../JVM/classfile.h"

extern void getCharsFromFlash(u1*, u1, char*);
static int uart_putchar(char c, FILE *stream)
{
    if (c == '\n')
        uart_putchar('\r', stream);

// Wait for the transmit buffer to be empty
    while (!(USARTC0.STATUS & USART_DREIF_bm))
        ;

// Put our character into the transmit buffer
    USARTC0.DATA = c;

    return 0;
}


FILE uartAVR8 = FDEV_SETUP_STREAM(conOut, conIn, _FDEV_SETUP_RW);
//static FILE mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

#define UART_ID CONFIG_UART_ID

#ifndef CONFIG_BOARD_LED_READY
# define CONFIG_BOARD_LED_READY BOARD_LED0_ID
#endif
#ifndef CONFIG_BOARD_LED_XFER
# define CONFIG_BOARD_LED_XFER  BOARD_LED1_ID
#endif
void uart_init(void);
void uart_init(void)
{
// Set the TxD pin high - set PORTC DIR register bit 3 to 1
    PORTC.OUTSET = PIN3_bm;

// Set the TxD pin as an output - set PORTC OUT register bit 3 to 1
    PORTC.DIRSET = PIN3_bm;

// Set baud rate & frame format
    USARTC0.BAUDCTRLB = 0;                        // BSCALE = 0 as well
    USARTC0.BAUDCTRLA = 12;

// Set mode of operation
    USARTC0.CTRLA = 0;                            // no interrupts please
    USARTC0.CTRLC = 0x03;                         // async, no parity, 8 bit data, 1 stop bit

// Enable transmitter only
    USARTC0.CTRLB = USART_TXEN_bm;
}


void initHW()
{
//  uart_init();
    sysclk_init();
    board_init();
// CLK_PER = CLK_CPU an PD7 ausgeben
    PORTD.DIR = (1 << PIN7);
    PORTCFG.CLKEVOUT = PORTCFG_CLKOUT_PD7_gc;

    uart_enable_clock(UART_ID);
    uart_ctrl_init_defaults(UART_ID);
    uart_enable(UART_ID, UART_FLAG_RX | UART_FLAG_TX);

// Light up a LED to show that we are ready
    led_activate(CONFIG_BOARD_LED_READY);
    bool led_on = true;
    char data;

    for (;;)
    {

        while (!uart_get_byte(UART_ID, &data))
            ;

        if (data == 'r')
            break;

// toggle a LED each time we get data
        if (led_on == true)
        {
            led_activate(CONFIG_BOARD_LED_XFER);
            led_on = false;
        }
        else
        {
            led_deactivate(CONFIG_BOARD_LED_XFER);
            led_on = true;
        }

        while (!uart_put_byte(UART_ID, data + 2))
            ;
    }

    stdout = stdin = stderr = &uartAVR8;
//stdout = &mystdout;

}


// damned atmels low level driver
char conIn(FILE* stream)
{
    char c;

//   while(!uart_rx_buffer_is_full(UART_ID)) ;
//uart_get_byte(UART_ID, &c);
    while (!uart_get_byte(UART_ID, &c))
        ;

    return c;
}


void conOut(char c)
{
//while (! uart_tx_buffer_is_empty(UART_ID));
//  uart_put_byte(UART_ID, c);
    while (!uart_put_byte(UART_ID, c))
        ;

    if (c != '\n')
        return;

//while ( !uart_tx_buffer_is_empty(UART_ID));
//uart_put_byte(UART_ID,'\r');
    while (!uart_put_byte(UART_ID, '\r'))
        ;
}


char conStat()
{
    return uart_rx_buffer_is_full_priv(UART_ID);
}


/* all class files stored for linux in DS (malloc)*/
/* for avr8 all class files in flash */
void initVM()                                     /* read, analyze classfiles and fill structures*/
{
    u1* addr;
    char buf[5];
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
        cs[cN].classFileStartAddress = addr + 4;  // after length of class;
        analyzeClass(&cs[cN]);
        addr += cs[cN].classFileLength + 4;
        printf_P(PSTR("bootclass: %x length:%x loaded\n"), cN,
            cs[cN].classFileLength);
    }

    printf_P(PSTR("load java application classes: \n"));

#ifdef WITHMON
    addr = (char*)AVR8_FLASH_APP_BASE;
#endif
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
        printf_P(PSTR("appclass: %x length:%x loaded\n"), cN,
            cs[cN].classFileLength);

    }

    DEBUGPRINTHEAP;
}                                                 // initVM


void exit(int n)
{
    goto * 0xf002;                                /*asm	 (INLINEASM(jmp,0xf002));*/
}


/*
 * \file
 *
 * \brief UART example application that loops back data it receives.
 *
 * Copyright (C) 2009 Atmel Corporation. All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 * Atmel AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
