/* you must exchange crt0.o /usr/avr32/lib/ucr1 with AT32UC3A-1.2.2ES software framework crt0.o*/
/* i doesnt know why*/
/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
/* fuer lehrzwecke,...*/
#include <stdio.h>
#include <avr32/io.h>
#include <string.h>
#include "atmel/evk1100.h"
#include "atmel/compiler.h"
#include "atmel/dip204.h"
#include "atmel/intc.h"
#include "atmel/gpio.h"
#include "atmel/flashc.h"
#include "atmel/pm.h"
#include "atmel/cycle_counter.h"
#include "atmel/spi.h"
#include "atmel/rtc.h"
#include "atmel/sdramc.h"

#include "../../JVM/typedefinitions.h"
#include "../../JVM/bajvm.h"
#include "../../JVM/heap.h"
#include "platform.h"
#include "native.h"

#define INT0          0                           /*!< Lowest interrupt priority level.*/
#define EXAMPLE_GCLK_ID             0
#define EXAMPLE_GCLK_PIN            AVR32_PM_GCLK_0_1_PIN
#define EXAMPLE_GCLK_FUNCTION       AVR32_PM_GCLK_0_1_FUNCTION

/* usart0 for terminal*/
void initHW()
{
#ifndef WITHMON
/* pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);*/
/* start PLL0 and switch main clock to PLL0 output */
/* Also set-up a generic clock from PLL0 and output it to a gpio pin. */
/*On EVK1100, check GCLK0 pin with an oscilloscope, the frequency should be 48MHz. On EVK1100, GCLK_0 is pin number //51 (PB19) Since LED5 is also on PB19, you should also see LED5 turn red.*/
    local_start_pll0(&AVR32_PM);
/* Now toggle LED0 using a GPIO */
/*	gpio_tgl_gpio_pin(LED0_GPIO);*/
    software_delay();
    usart1Init();                                 /* 1200 baud*/
    usart0Init();                                 /*bh not with monitor*/

    static const gpio_map_t DIP204_SPI_GPIO_MAP =
    {
        {
            DIP204_SPI_SCK_PIN,
            DIP204_SPI_SCK_FUNCTION               /* SPI Clock.*/
        },
        {                                         /* MISO.*/
            DIP204_SPI_MISO_PIN, DIP204_SPI_MISO_FUNCTION
        },
        {                                         /* MOSI.*/
            DIP204_SPI_MOSI_PIN, DIP204_SPI_MOSI_FUNCTION
        },
        {                                         /* Chip Select NPCS.*/
            DIP204_SPI_NPCS_PIN, DIP204_SPI_NPCS_FUNCTION
        }
    };

    Disable_global_interrupt();                   /* Disable all interrupts.*/
    INTC_init_interrupts();                       /* init the interrupts*/
/* Register the compare interrupt handler to the interrupt controller.*/
/* compare_irq_handler is the interrupt handler to register.*/
/* AVR32_CORE_COMPARE_IRQ is the IRQ of the interrupt handler to register.*/
/* AVR32_INTC_INT0 is the interrupt priority level to assign to the group of this IRQ.*/
/* void INTC_register_interrupt(__int_handler handler, unsigned int irq, unsigned int int_lev);*/
    INTC_register_interrupt(&compare_irq_handler, AVR32_CORE_COMPARE_IRQ,
        AVR32_INTC_INT0);
    Enable_global_interrupt();                    /* Enable all interrupts.*/

/* add the spi options driver structure for the LCD DIP204*/
    spi_options_t spiOptions =
    {
        .reg = DIP204_SPI_NPCS, .baudrate = 1000000,
        .bits = 8, .spck_delay = 0, .trans_delay = 0, .stay_act = 1,
        .spi_mode = 0, .fdiv = 0, .modfdis = 1
    };

/* Assign I/Os to SPI*/
    gpio_enable_module(DIP204_SPI_GPIO_MAP,
        sizeof(DIP204_SPI_GPIO_MAP) / sizeof(DIP204_SPI_GPIO_MAP[0]));
    spi_initMaster(DIP204_SPI, &spiOptions);      /* Initialize as master*/

/* Set selection mode: variable_ps, pcs_decode, delay*/
    spi_selectionMode(DIP204_SPI, 0, 0, 0);
    spi_enable(DIP204_SPI);                       /* Enable SPI*/
                                                  /*bh setup chip registers*/
    spi_setupChipReg(DIP204_SPI, &spiOptions, 4 * FOSC0);

/* configure local push buttons*/
/*dip204_example_configure_push_buttons_IT();*/

/* configure local joystick*/
/*dip204_example_configure_joystick_IT();*/

/*dip204_hide_cursor();*/
    initTimer();
    sdramc_init(48000000);                        /*FOSC0) bh not with monitor*/
    dip204_init(backlight_PWM, TRUE);
    dip204_set_cursor_position(1, 1);
    dip204_write_string("That writes for you ");
    dip204_set_cursor_position(2, 2);
    dip204_write_string("not java its C");
    dip204_set_cursor_position(1, 3);
    dip204_write_string("FWR-BA EVK1100");
    dip204_set_cursor_position(1, 4);
    dip204_write_string("now BAJOS please:");
#endif
    stdIOInit();
}


/* all class files stored for linux in DS (malloc)*/
/* for avr8 all class files in flash */
/* for uc3a and standalone ap7000:	bootclasses in flash*/
/* 		application classes  DS(Ram) -> hard coded !!!*/
void initVM()                                     /* read, analyze classfiles and fill structures*/
{
    u4 length;
    char* addr;
    u4 temp;
    char buf[5];
    classFileBase = (char*) UC3A_FLASH_JAVA_BASE; /* boot classes in flash*/
/* use malloc!! */
                                                  /* app classes in sdram*/
    appClassFileBase = (char*) UC3A_SDRAM_JAVA_BASE;
    heapInit();                                   /* linux avr8 malloc , others hard coded!*/
    length = 0;
/* analyze bootclasses, which are programmed in flash*/
    strncpy(buf, classFileBase, 4);
    buf[4] = 0;
    sscanf(buf, "%4d", &temp);
    numClasses = (u1) temp;
    addr = classFileBase + 4;                     /* after numclasses*/
    for (cN = 0; cN < numClasses; cN++)
    {
        strncpy(buf, addr, 4);
        sscanf(buf, "%4d", &temp);
        cs[cN].classFileStartAddress = addr + 4;  /* after length of class*/
        cs[cN].classFileLength = temp;            /*(u1)(*addr)+256*(u1)(*(addr+1));*/
        analyzeClass(&cs[cN]);
        addr += cs[cN].classFileLength + 4;
        printf("%x %x %x %x\n", numClasses, cN, cs[cN].classFileStartAddress,
            cs[cN].classFileLength);
    }
    printf("%x bootclasses are loaded\n", cN);
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
    } while (1);
/*!!*/
    numClasses = cN;
    DEBUGPRINTHEAP;
}


#ifndef WITHMON

/* This source file is part of the ATMEL AVR32-SoftwareFramework-AT32UC3A-1.2.2ES Release */

/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief LCD DIP204 example driver for EVK1100 board.
 *
 * This file provides a useful example for the LCD DIP204 on SPI interface.
 * Press PB0 to see the full set of available chars on the LCD
 * Press PB1 to decrease the backlight power of the LCD
 * Press PB2 to increase the backlight power of the LCD
 * Use Joystick to see arrows displayed on the LCD
 * Press Joystick to return to the idle screen
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with : SPI and PWM
 * - AppNote:
 *
 * \author               Atmel Corporation: http: www.atmel.com
 *                       Support and FAQ: http: support.atmel.no
 *
 *****************************************************************************/

/*! \page License
 * Copyright (c) 2007, Atmel Corporation All rights reserved.
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
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*! \mainpage
 * \section intro Introduction
 * This documents data structures, functions, variables, defines, enums, and
 * typedefs in the software. <BR>It also gives an example of the usage of the
 * DIP204 LCD on EVK1100. \n \n
 * <b>Example's operating mode:</b>
 * <ul>
 * <li>A default message is displayed on the 4 lines of the LCD
 * <li>Press PB0 to see the full set of available chars on the LCD
 * <li>Press PB1 to decrease the backlight power of the LCD
 * <li>Press PB2 to increase the backlight power of the LCD
 * <li>Use the joystick to see arrows displayed on the LCD
 * <li>Press the joystick to see a circle displayed on the LCD and to return to the
 *     idle screen (displaying the default message)
 *
 * </ul>
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC for AVR32 and IAR Systems compiler
 * for AVR32. Other compilers may or may not work.
 *
 * \section deviceinfo Device Info
 * All AVR32UC devices with an SPI module can be used. This example has been tested
 * with the following setup:
 *- EVK1100 evaluation kit
 *
 * \section setupinfo Setup Information
 * CPU speed: <i> 12 MHz </i>
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http: www.atmel.com/products/AVR32/">Atmel AVR32</A>.\n
 * Support and FAQ: http: support.atmel.no
 */

/*! define the push button to see available char map on LCD */
#define GPIO_CHARSET            GPIO_PUSH_BUTTON_0
/*! define the push button to decrease back light power */
#define GPIO_BACKLIGHT_MINUS    GPIO_PUSH_BUTTON_1
/*! define the push button to increase back light power */
#define GPIO_BACKLIGHT_PLUS     GPIO_PUSH_BUTTON_2
/*! flag set when joystick display starts to signal main function to clear this display */
unsigned short display;

/*! flag set when time out starts and cleared when timeout occurs */
volatile unsigned char TimeOut = 0;

/*! current char displayed on the 4th line */
unsigned short current_char = 0;

/*!
 * \brief interrupt handler for compare IT.
 */

__attribute__((__interrupt__)) static void compare_irq_handler(void)
{
    TimeOut = 1;
/* Disable the compare and exception generation feature: set COMPARE to 0.*/
    Set_sys_compare(0);
}


/*!
 * \brief The Push Buttons interrupt handler.
 */
__attribute__((__interrupt__)) static void dip204_example_PB_int_handler(void)
{
    unsigned short i;
/* display all available chars */
    if (gpio_get_pin_interrupt_flag(GPIO_CHARSET))
    {
/* go to first column of 4th line */
        dip204_set_cursor_position(1, 4);
/* display 20 chars of charmap */
        for (i = current_char; i < current_char + 0x10; i++)
        {
            dip204_write_data(i);
        }
        dip204_write_string("    ");
/* mark position in charmap */
        current_char = i;
/* reset marker */
        if (current_char >= 0xFF)
        {
            current_char = 0x10;
        }
/* allow new interrupt : clear the IFR flag */
        gpio_clear_pin_interrupt_flag(GPIO_CHARSET);
    }
/* increase backlight power */
    if (gpio_get_pin_interrupt_flag(GPIO_BACKLIGHT_PLUS))
    {
        dip204_set_backlight(backlight_power_increase);
/* allow new interrupt : clear the IFR flag */
        gpio_clear_pin_interrupt_flag(GPIO_BACKLIGHT_PLUS);
    }
/* decrease backlight power */
    if (gpio_get_pin_interrupt_flag(GPIO_BACKLIGHT_MINUS))
    {
        dip204_set_backlight(backlight_power_decrease);
/* allow new interrupt : clear the IFR flag */
        gpio_clear_pin_interrupt_flag(GPIO_BACKLIGHT_MINUS);
    }
}


/*!
 * \brief The joystick interrupt handler.
 */
__attribute__((__interrupt__)) static void dip204_example_Joy_int_handler(void)
{
    if (gpio_get_pin_interrupt_flag(GPIO_JOYSTICK_UP))
    {
        dip204_set_cursor_position(19, 1);
        dip204_write_data(0xDE);
        display = 1;
/* allow new interrupt : clear the IFR flag */
        gpio_clear_pin_interrupt_flag(GPIO_JOYSTICK_UP);
    }
    if (gpio_get_pin_interrupt_flag(GPIO_JOYSTICK_DOWN))
    {
        dip204_set_cursor_position(19, 3);
        dip204_write_data(0xE0);
        display = 1;
/* allow new interrupt : clear the IFR flag */
        gpio_clear_pin_interrupt_flag(GPIO_JOYSTICK_DOWN);
    }
    if (gpio_get_pin_interrupt_flag(GPIO_JOYSTICK_LEFT))
    {
        dip204_set_cursor_position(18, 2);
        dip204_write_data(0xE1);
        display = 1;
/* allow new interrupt : clear the IFR flag */
        gpio_clear_pin_interrupt_flag(GPIO_JOYSTICK_LEFT);
    }
    if (gpio_get_pin_interrupt_flag(GPIO_JOYSTICK_RIGHT))
    {
        dip204_set_cursor_position(20, 2);
        dip204_write_data(0xDF);
        display = 1;
/* allow new interrupt : clear the IFR flag */
        gpio_clear_pin_interrupt_flag(GPIO_JOYSTICK_RIGHT);
    }
    if (gpio_get_pin_interrupt_flag(GPIO_JOYSTICK_PUSH))
    {
        dip204_set_cursor_position(19, 2);
        dip204_write_data(0xBB);
        dip204_set_cursor_position(1, 4);
        dip204_write_string("  AT32UC3A Series   ");
        display = 1;
/* allow new interrupt : clear the IFR flag */
        gpio_clear_pin_interrupt_flag(GPIO_JOYSTICK_PUSH);
    }
}


/*!
 * \brief function to configure push button to generate IT upon rising edge
 */
void dip204_example_configure_push_buttons_IT(void)
{
    gpio_enable_pin_interrupt(GPIO_CHARSET, GPIO_RISING_EDGE);
    gpio_enable_pin_interrupt(GPIO_BACKLIGHT_PLUS, GPIO_RISING_EDGE);
    gpio_enable_pin_interrupt(GPIO_BACKLIGHT_MINUS, GPIO_RISING_EDGE);
/* Disable all interrupts */
    Disable_global_interrupt();
/* register PB0 handler on level 1 */
    INTC_register_interrupt(
        &dip204_example_PB_int_handler,
        AVR32_GPIO_IRQ_0 + (GPIO_BACKLIGHT_PLUS / 8), AVR32_INTC_INT1);
    INTC_register_interrupt(
        &dip204_example_PB_int_handler,
        AVR32_GPIO_IRQ_0 + (GPIO_BACKLIGHT_MINUS / 8), AVR32_INTC_INT1);
    INTC_register_interrupt(
        &dip204_example_PB_int_handler,
        AVR32_GPIO_IRQ_0 + (GPIO_CHARSET / 8), AVR32_INTC_INT1);
/* Enable all interrupts */
    Enable_global_interrupt();
}


/*!
 * \brief function to configure joystick to generate IT upon falling edge
 */
void dip204_example_configure_joystick_IT(void)
{
    gpio_enable_pin_interrupt(GPIO_JOYSTICK_UP, GPIO_FALLING_EDGE);
    gpio_enable_pin_interrupt(GPIO_JOYSTICK_DOWN, GPIO_FALLING_EDGE);
    gpio_enable_pin_interrupt(GPIO_JOYSTICK_RIGHT, GPIO_FALLING_EDGE);
    gpio_enable_pin_interrupt(GPIO_JOYSTICK_PUSH, GPIO_FALLING_EDGE);
    gpio_enable_pin_interrupt(GPIO_JOYSTICK_LEFT, GPIO_FALLING_EDGE);

/* Disable all interrupts */
    Disable_global_interrupt();
/* register PB0 handler on level 1 */
    INTC_register_interrupt(&dip204_example_Joy_int_handler,
        AVR32_GPIO_IRQ_0 + (GPIO_JOYSTICK_UP / 8), AVR32_INTC_INT1);
    INTC_register_interrupt(&dip204_example_Joy_int_handler,
        AVR32_GPIO_IRQ_0 + (GPIO_JOYSTICK_DOWN / 8), AVR32_INTC_INT1);
    INTC_register_interrupt(&dip204_example_Joy_int_handler,
        AVR32_GPIO_IRQ_0 + (GPIO_JOYSTICK_RIGHT / 8), AVR32_INTC_INT1);
    INTC_register_interrupt(&dip204_example_Joy_int_handler,
        AVR32_GPIO_IRQ_0 + (GPIO_JOYSTICK_LEFT / 8), AVR32_INTC_INT1);
    INTC_register_interrupt(&dip204_example_Joy_int_handler,
        AVR32_GPIO_IRQ_0 + (GPIO_JOYSTICK_PUSH / 8), AVR32_INTC_INT1);
/* Enable all interrupts */
    Enable_global_interrupt();
}


/*!
 * \brief Software delay
 */
void delay_ms(unsigned short time_ms)
{
    unsigned long u32CountVal, u32CompareVal;
    TimeOut = 0;
    u32CountVal = Get_sys_count();
                                                  /* WARNING: MUST FIT IN 32bits.*/
    u32CompareVal = u32CountVal + ((unsigned long) time_ms * (FOSC0 / 1000));
    Set_sys_compare(u32CompareVal);               /* GO*/
/*  The previous COMPARE write succeeded.*/
/* Loop until the COUNT&COMPARE match triggers.*/
    while (!TimeOut)
        ;
}


/* \brief Software Delay
 *
 */
static void software_delay(void)
{
    volatile int i;
    for (i = 0; i < 1000000; i++)
        ;
}


/*
 ## Author: H.-Christian Hecht, CoMedServ GmbH, IT 2006
 ## Method: void rtc_irq(void);
## Parameters: none
## Return Value: none
##
## Interrupt handler for Timer
*/

__attribute__((__interrupt__)) void rtc_irq()
{
    timerMilliSec++;
/* clear the interrupt flag*/
    rtc_clear_interrupt(&AVR32_RTC);
}


/*
 ## Author: H.-Christian Hecht, CoMedServ GmbH, IT 2006
 ## Method: void initTimer(void);
## Parameters: none
## Return Value: none
##
## start the timer, set the interrupt handling method
*/
void initTimer()
{
/* Disable all interrupts. */
    Disable_global_interrupt();
/* The INTC driver has to be used only for GNU GCC for AVR32.*/
#if __GNUC__
/* Initialize interrupt vectors.*/
/*bh  INTC_init_interrupts();*/
/* Register the RTC interrupt handler to the interrupt controller.*/
    INTC_register_interrupt(&rtc_irq, AVR32_RTC_IRQ, INT0);
#endif
/* Initialize the RTC*/
/*psel = log(Fosc/Frtc)/log(2)-1; Frtc wanted f, Fosc 32KHz*/
    if (!rtc_init(&AVR32_RTC, RTC_OSC_32KHZ, 4))  /* 1khz*/
    {
/*usart_write_line(&AVR32_USART0, "Error initializing the RTC\r\n");*/
        {
            printf("Error initializing the RTC\r\n");
            exit(-1);
        }
    }
/* Set top value to 0 to generate an interruption every Milli-seconds */
    rtc_set_top_value(&AVR32_RTC, 0);
/* Enable the interruptions*/
    rtc_enable_interrupt(&AVR32_RTC);
/* Enable the RTC*/
    rtc_enable(&AVR32_RTC);
/* Enable global interrupts*/
    Enable_global_interrupt();
}
#endif
int __attribute__((weak)) _read(int file, char * ptr, int len)
{
    int i;
/*if ( !do_not_use_oscall_coproc ) return _read_sim(file, ptr, len);*/
/*if (file != 0)return unimplemented_syscall("_read with filedes != 0");*/
    for (i = 0; i < len; i++)
    {
        ptr[i] = (char) conIn();
    }
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


#ifndef WITHMON
void usart0Init()
{
    static const gpio_map_t USART_GPIO_MAP =
    {
        {
            AVR32_USART0_RXD_0_PIN,
            AVR32_USART0_RXD_0_FUNCTION
        }
        ,
        {
            AVR32_USART0_TXD_0_PIN,
            AVR32_USART0_TXD_0_FUNCTION
        }
    };
/* USART options.*/
    static const usart_options_t USART_OPTIONS =
    {
        .baudrate = 57600,
//.baudrate     = 115200,
        .charlength = 8, .paritytype = USART_NO_PARITY, .stopbits =
        USART_1_STOPBIT, .channelmode = USART_NORMAL_CHMODE
    };

/* Switch main clock to external oscillator 0 (crystal).*/
/*pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);*/
/* Assign GPIO to USART.*/
    gpio_enable_module(USART_GPIO_MAP,
        sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));

/* Initialize USART in RS232 mode.*/
                                                  /*FOSC0/2); //why???*/
    usart_init_rs232(&AVR32_USART0, &USART_OPTIONS, 24000000);
}


void usart1Init()
{
    static const gpio_map_t USART_GPIO_MAP =
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
/* USART options.*/
    static const usart_options_t USART_OPTIONS =
    {
        .baudrate = 1200,
/*    .baudrate     = 115200,*/
        .charlength = 7, .paritytype = USART_NO_PARITY, .stopbits =
        USART_2_STOPBITS, .channelmode = USART_NORMAL_CHMODE
    };

/* Switch main clock to external oscillator 0 (crystal).*/
/*  pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);*/
/* Assign GPIO to USART.*/
    gpio_enable_module(USART_GPIO_MAP,
        sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));

/* Initialize USART in RS232 mode.*/
                                                  /*;FOSC0);*/
    usart_init_rs232(&AVR32_USART1, &USART_OPTIONS, 24000000);
}


/* Copyright (c) 2007, Atmel Corporation All rights reserved.
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
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* Start PLL0, enable a generic clock with PLL0 output then switch main clock to PLL0 output.
 All calculations in this function suppose that the Osc0 frequency is 12MHz. */
void local_start_pll0(volatile avr32_pm_t* pm)
{
    pm_switch_to_osc0(pm, FOSC0, OSC0_STARTUP);   /* Switch main clock to Osc0.*/

/* Setup PLL0 on Osc0, mul=3 ,no divisor, lockcount=16, ie. 12Mhzx8 = 96MHz output */
/*void pm_pll_setup(volatile avr32_pm_t* pm,
 unsigned int pll,
 unsigned int mul,
unsigned int div,
unsigned int osc,
unsigned int lockcount) {
*/
    pm_pll_setup(pm, 0,                           /* use PLL0*/
        7,                                        /* MUL=7 in the formula*/
        1,                                        /* DIV=1 in the formula*/
        0,                                        /* Sel Osc0/PLL0 or Osc1/PLL1*/
        16);                                      /* lockcount in main clock for the PLL wait lock*/

/*
 This function will set a PLL option.
 *pm Base address of the Power Manager (i.e. &AVR32_PM)
pll PLL number 0
pll_freq Set to 1 for VCO frequency range 80-180MHz, set to 0 for VCO frequency range 160-240Mhz.
pll_div2 Divide the PLL output frequency by 2 (this settings does not change the FVCO value)
pll_wbwdisable 1 Disable the Wide-Bandith Mode (Wide-Bandwith mode allow a faster startup time and out-of-lock time). 0 to enable the Wide-Bandith Mode.
*/
/* PLL output VCO frequency is 96MHz. We divide it by 2 with the pll_div2=1. This enable to get later main clock to 48MHz */
    pm_pll_set_option(pm, 0, 1, 1, 0);

/* Enable PLL0 */
/*
 void pm_pll_enable(volatile avr32_pm_t* pm,
 unsigned int pll) {
*/
    pm_pll_enable(pm, 0);

/* Wait for PLL0 locked */
    pm_wait_for_pll0_locked(pm);

/* Setup generic clock on PLL0, with Osc0/PLL0, no divisor */
/*
 void pm_gc_setup(volatile avr32_pm_t* pm,
 unsigned int gc,
unsigned int osc_or_pll, //Use Osc (=0) or PLL (=1)
unsigned int pll_osc,  // Sel Osc0/PLL0 or Osc1/PLL1
unsigned int diven,
unsigned int div) {
*/
    pm_gc_setup(pm, EXAMPLE_GCLK_ID, 1,           /* Use Osc (=0) or PLL (=1), here PLL*/
        0,                                        /* Sel Osc0/PLL0 or Osc1/PLL1*/
        0,                                        /* disable divisor*/
        0);                                       /* no divisor*/

/* Enable Generic clock */
    pm_gc_enable(pm, EXAMPLE_GCLK_ID);

/* Set the GCLOCK function to the GPIO pin */
    gpio_enable_module_pin(EXAMPLE_GCLK_PIN, EXAMPLE_GCLK_FUNCTION);

/* Divide PBA clock by 2 from main clock (PBA clock = 48MHz/2 = 24MHz).
 Pheripheral Bus A clock divisor enable = 1
 Pheripheral Bus A select = 0
Pheripheral Bus B clock divisor enable = 0
Pheripheral Bus B select = 0
High Speed Bus clock divisor enable = 0
High Speed Bus select = 0
*/
    pm_cksel(pm, 1, 0, 0, 0, 0, 0);

/* Set one wait-state (WS) for flash controller. 0 WS access is up to 30MHz for HSB/CPU clock.*/
/* As we want to have 48MHz on HSB/CPU clock, we need to set 1 WS on flash controller.*/
    flashc_set_wait_state(1);

    pm_switch_to_clock(pm, AVR32_PM_MCSEL_PLL0);  /* Switch main clock to 48MHz */
}


/*
 void exit(int status)	{
 char* startAddr=(char*)0x80020000;
goto *startAddr;
}
*/
#else
void exit(int status)
{
    Monitor(status);
}
#endif

u2 readClassFile(char* fileName, char* addr)
{
#if (AVR32UC3A||AVR32AP7000)
    int i;
    char c=conIn();                               /* dummy w*/
    if (c =='w')
    {
        conOut(4);                                /*turn off echo*/
        c=conIn();                                /*s*/
        while ((c=conIn())=='p')
        {
/*if (c=='p) 	// apage comes*/
            c=conIn();
            c=conIn();                            /* address*/
            conOut('w');
            for (i=0; i < 256;i++) *(addr++)=conIn();
            conOut('w');
        }
        conOut(5);                                /* turn on echo uploadend*/
        while((c=conIn())==' ');
        i=0;
        do
        {
            i+=16*i+ ((c<='9')? (c-'0'): (c-'a'+10));
            conOut(c);
        }
        while ((c=conIn())!= 0xa);
        return (u2)i;
    }
    else
    {
/*uploadends2*/
        conOut(5);                                /* turn on echo*/
        return (u2) 0;
    }
#endif
}
