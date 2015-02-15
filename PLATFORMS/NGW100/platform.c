/* a bad and ugly hack!!*/
/* ************************************************************************ *\ 
Copyright (c) 2006, Atmel Corporation All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the
following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. The name of ATMEL may not be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE EXPRESSLY AND SPECIFICALLY
DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE

POSSIBILITY OF SUCH DAMAGE.

\* ************************************************************************ */
#include <avr32/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "atmel/pio.h"
#include "atmel/gpio.h"
#include "atmel/usart.h"
#include "atmel/sdram.h"
#include "atmel/hsdramc1.h"
#include "atmel/hmatrix2.h"
#include "atmel/intc.h"
#include "atmel/atngw100.h"
#include "atmel/pm_at32ap7000.h"
#include "atmel/cycle_counter.h"
#include "../../JVM/definitions.h"
#include "../../JVM/typedefinitions.h"
#include "../../JVM/bajvm.h"
#include "../../JVM/heap.h"
#include "../../JVM/classfile.h"
#include "platform.h"
#include "native.h"

#define __raw_writel(v,a)   (*(volatile unsigned int   *)(a) = (v))
#define writel(v,a)     __raw_writel(v,a)
#define HMATRIX_BASE        0xFFF00800
#define __raw_readl(a)      (*(volatile unsigned int   *)(a))
#define readl(a)        __raw_readl(a)
#define P2SEG           0xa0000000
#define P2SEGADDR(a)        ((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) | P2SEG))
#define uncached(addr)      ((void *)P2SEGADDR(addr))

#define NB_CLOCK_CYCLE_DELAY_SHORTER    10000     /* 1 ms if fCPU==20MHz adjust it*/
#define NB_CLOCK_CYCLE_DELAY_SHORT    1000000     /* 100 ms if fCPU==20MHz*/
#define NB_CLOCK_CYCLE_DELAY_LONG    20000000     /* 1 s if fCPU==20MHz*/

/* COUNT/COMPARE match interrupt handler*/
/* GCC-specific syntax to declare an interrupt handler. The interrupt handler*/
/* registration is done in the main function using the INTC software driver module.*/
__attribute__((__interrupt__)) static void compare_irq_handler(void)
{
/* Count the number of times this IRQ handler is called.*/
/* Clear the pending interrupt(writing a value to the COMPARE register clears*/
/* any pending compare interrupt requests). Schedule the COUNT&COMPARE match*/
/* interrupt to happen every NB_CLOCK_CYCLE_DELAY cycles.*/
/* AP7000 don't reset COUNT on compare match. We need to offset next COMPARE.*/
    U32 next_compare = Get_sys_count() + NB_CLOCK_CYCLE_DELAY_SHORTER;
/* Avoid disabling compare			*/
    Set_sys_compare((next_compare == 0) ? 1 : next_compare);
    timerMilliSec++;
}


typedef char avr32_piomap_t[][2];

void initHW()
{
/* Reset PM. Makes sure we get the expected clocking after a soft reset (e.g.: JTAG reset)*/
    pm_reset();
    usartInit();
    stdIOInit();
    static const struct sdram_info sdram =
    {
        .phys_addr = NGW_SDRAM_BASE,
        .row_bits = 13, .col_bits = 9, .bank_bits = 2, .cas = 3, .twr = 2,
        .trc = 7, .trp = 2, .trcd = 2, .tras = 5, .txsr = 5,
    };

/*MT481C2M16B2TG SDRAM */
    hmatrix2_writel(SFR4, 1 << 1);
    sdram_init(&sdram);
    Disable_global_interrupt();
    INTC_init_interrupts();
/* Register the compare interrupt handler to the interrupt controller.*/
/* compare_irq_handler is the interrupt handler to register.*/
/* AVR32_CORE_COMPARE_IRQ is the IRQ of the interrupt handler to register.*/
/* AVR32_INTC_INT0 is the interrupt priority level to assign to the group of this IRQ.*/
/* void INTC_register_interrupt(__int_handler handler, unsigned int irq, unsigned int int_lev);*/
    INTC_register_interrupt(&compare_irq_handler, AVR32_CORE_COMPARE_IRQ,
        AVR32_INTC_INT0);
/* Enable all interrupts.*/
    Enable_global_interrupt();
/* Schedule the COUNT&COMPARE match interrupt in NB_CLOCK_CYCLE_DELAY_SHORT */
/* clock cycles from now.*/
    U32 next_compare = Get_sys_count() + NB_CLOCK_CYCLE_DELAY_SHORTER;
/* Avoid disabling compare			*/
                                                  /* GO*/
    Set_sys_compare(((next_compare == 0) ? 1 : next_compare));
}


/* all class files stored for linux in DS (malloc)*/
/* for avr8 all class files in flash */
/* for uc3a and standalone ap7000:	bootclasses in flash*/
/* 		application classes  DS(Ram) -> hard coded !!!*/
#ifdef LINUX
void initVM(int argc, char* argv[])
{
#else
    void initVM(void)
    {
#endif
/* read, analyze classfiles and fill structures*/
        u4 length;
        char* addr;
        u4 temp;
        char buf[5];

                                                  /* boot classes in flash*/
        classFileBase = (char*) NGW_FLASH_JAVA_BASE;
                                                  /* app classes in sdram*/
        appClassFileBase = (char*) NGW_SDRAM_JAVA_BASE;

        heapInit();                               /* linux avr8 malloc , others hard coded!*/
        length = 0;

/* analyze bootclasses, which are programmed in flash*/
        strncpy(buf, classFileBase, 4);
        buf[4] = 0;
        sscanf(buf, "%4d", &temp);
        numClasses = (u1) temp;
        addr = classFileBase + 4;                 /* after numclasses*/
        for (cN = 0; cN < numClasses; cN++)
        {
            strncpy(buf, addr, 4);
            sscanf(buf, "%4d", &temp);
                                                  /* after length of class*/
            cs[cN].classFileStartAddress = addr + 4;
            cs[cN].classFileLength = temp;        /*(u1)(*addr)+256*(u1)(*(addr+1));*/
            analyzeClass(&cs[cN]);
            addr += cs[cN].classFileLength + 4;
        }
        printf("%d bootclasses are loaded\n", cN);
/* thats to boot classes*/
/* now the application classes*/
        addr = appClassFileBase;
        length = 0;
        do
        {
            printf("load application classes-> type \"w\" \n");
            cs[cN].classFileStartAddress = addr + length;
            cs[cN].classFileLength = readClassFile(NULL,
                cs[cN].classFileStartAddress);
            printf("\n");
            length += cs[cN].classFileLength;
            analyzeClass(&cs[cN]);
            cN++;
            printf("still another appl. class ? (y) \n");
            if (conIn() != 'y')
                break;
        } while (cs[cN].classFileLength != 0);
/*!!*/
        numClasses = cN;
        printf("numClasses %d\n", numClasses);
        DEBUGPRINTHEAP;
    }

    char conIn()
    {
        return (char) usart_getchar(&AVR32_USART1);
    }

    void conOut(char c)
    {
        usart_bw_write_char(&AVR32_USART1, (int) c);
    }

    int __attribute__((weak)) _read(int file, char * ptr, int len)
    {
        int i;
/*if ( !do_not_use_oscall_coproc ) return _read_sim(file, ptr, len);*/
/*if (file != 0)return unimplemented_syscall("_read with filedes != 0");*/
        for (i = 0; i < len; i++)
            ptr[i] = (char) conIn();
        return len;
    }

/**
 * Low-level write command.
 * When newlib buffer is full or fflush is called, this will output
 * data to correct location.
 * 1 and 2 is stdout and stderr which goes to usart
 * 3 is framebuffer
 */
    int __attribute__((weak)) _write(int file, char * ptr, int len)
    {
        int i;
/*if ( !do_not_use_oscall_coproc ) return _write_sim(file, ptr, len);*/
/*if ( (file != 1)&& (file != 2) && (file!=3)) return unimplemented_syscall("_write with file != 1 or 2");*/
/* if(file==3){
 for(i = 0 ; i < len; i++){
 avr32fb_putchar(ptr[i]);
}
} else */
        {
            for (i = 0; i < len; i++)
            {
                conOut(ptr[i]);
                if (ptr[i] == '\n')
                    conOut('\r');
            }
        }
        return len;
    }

    void stdIOInit()
    {
/*To configure standard I/O streams as unbuffered, you can simply:*/
        setbuf(stdin, NULL);
        setbuf(stdout, NULL);
    }

    void usartInit()
    {
        int cpu_hz = 20000000;
        struct usart_options_t opt;
        volatile struct avr32_usart_t *usart = &AVR32_USART1;
        gpio_map_t usart_piomap =
        {
            {
                AVR32_USART1_RXD_0_PIN,
                AVR32_USART1_RXD_0_FUNCTION
            }
            ,
            {
                AVR32_USART1_TXD_0_PIN,
                AVR32_USART1_TXD_0_FUNCTION
            }
        };

/* Set options for the USART*/
        opt.baudrate = 115200;
        opt.charlength = 8;
        opt.paritytype = USART_NO_PARITY;
        opt.stopbits = USART_1_STOPBIT;
        opt.channelmode = USART_NORMAL_CHMODE;
/* Initialize it in RS232 mode*/
        usart_init_rs232(usart, &opt, cpu_hz);
        pio_enable_module(usart_piomap,
            sizeof(usart_piomap) / sizeof(usart_piomap[0]));
    }

    void exit(int status)
    {
        nativeExit(0);
    }

    u2 readClassFile(char* fileName, char* addr)
    {

        int i;
        char c = conIn();                         /* dummy w*/
        if (c == 'w')
        {
            conOut(4);                            /*turn off echo*/
            c = conIn();                          /*s*/
            while ((c = conIn()) == 'p')
            {
/*if (c=='p) 	// apage comes*/
                c = conIn();
                c = conIn();                      /* address*/
                conOut('w');
                for (i = 0; i < 256; i++)
                    *(addr++) = conIn();
                conOut('w');
            }
            conOut(5);                            /* turn on echo uploadend*/
            while ((c = conIn()) == ' ')
                ;
            i = 0;
            do
            {
                i += 16 * i + ((c <= '9') ? (c - '0') : (c - 'a' + 10));
                conOut(c);
            } while ((c = conIn()) != 0xa);
            return (u2) i;
        }
        else
        {
/*uploadends2*/
            conOut(5);                            /* turn on echo*/
            return (u2) 0;
        }
    }
