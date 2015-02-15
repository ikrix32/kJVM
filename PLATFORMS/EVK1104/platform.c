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
#include "atmel/evk1104.h"
#include "atmel/compiler.h"

#ifndef WITHMON
#include "atmel/compiler.h"
#include "atmel/evk1104.h"
#include "atmel/power_clocks_lib.h"
#include "atmel/intc.h"
#include "atmel/gpio.h"
#include "atmel/spi.h"
#include "atmel/pm.h"
#include "atmel/led.h"
#include "atmel/et024006dhu.h"
#include "atmel/qt60168.h"
#include "atmel/conf_qt60168.h"
#include "atmel/delay.h"
#include "atmel/flashc.h"
#include "atmel/cycle_counter.h"
#include "atmel/rtc.h"
#include "atmel/sdramc.h"
#include "atmel/iobinding.h"
#define INT0          0                           /*!< Lowest interrupt priority level.*/
#define EXAMPLE_GCLK_ID             0
#define EXAMPLE_GCLK_PIN            AVR32_PM_GCLK_0_1_PIN
#define EXAMPLE_GCLK_FUNCTION       AVR32_PM_GCLK_0_1_FUNCTION
#endif

#include "../../JVM/typedefinitions.h"
#include "../../JVM/bajvm.h"
#include "../../JVM/heap.h"
#include "platform.h"
#include "native.h"

/* usart0 for terminal*/
void initHW()
{
#ifndef WITHMON
    pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);
/* Now toggle LED0 using a GPIO */
/*	gpio_tgl_gpio_pin(LED0_GPIO);*/
    usart1Init();                                 // 57600 ACM  -> minikermit
    initTimer();
    sdramc_init(48000000);

/* START init Touch */
    static const gpio_map_t QT60168_SPI_GPIO_MAP =
    {
        {
            QT60168_SPI_SCK_PIN,
            QT60168_SPI_SCK_FUNCTION              // SPI Clock.
        },
        {                                         // MISO.
            QT60168_SPI_MISO_PIN, QT60168_SPI_MISO_FUNCTION
        },
        {                                         // MOSI.
            QT60168_SPI_MOSI_PIN, QT60168_SPI_MOSI_FUNCTION
        },
        {                                         // Chip Select NPCS.
            QT60168_SPI_NPCS0_PIN, QT60168_SPI_NPCS0_FUNCTION
        }
    };

// SPI options.
    spi_options_t spiOptions =
    {
        .reg = QT60168_SPI_NCPS,
        .baudrate = QT60168_SPI_MASTER_SPEED,     // Defined in conf_qt60168.h.
        .bits = QT60168_SPI_BITS,                 // Defined in conf_qt60168.h.
        .spck_delay = 0, .trans_delay = 0, .stay_act = 0, .spi_mode = 3,
        .modfdis = 1
    };

// Assign I/Os to SPI.
    gpio_enable_module(QT60168_SPI_GPIO_MAP,
        sizeof(QT60168_SPI_GPIO_MAP) / sizeof(QT60168_SPI_GPIO_MAP[0]));

// Initialize as master.
    spi_initMaster(QT60168_SPI, &spiOptions);

// Set selection mode: variable_ps, pcs_decode, delay.
    spi_selectionMode(QT60168_SPI, 0, 0, 0);

// Enable SPI.
    spi_enable(QT60168_SPI);

// Initialize QT60168 with SPI clock Osc0.
    spi_setupChipReg(QT60168_SPI, &spiOptions, FOSC0);

    qt60168_init(FOSC0);

/* END init Touch */
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
    u1 i = 0;
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
        printf("%x %x\n", cs[cN].classFileStartAddress, cs[cN].classFileLength);

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

/* \brief Software Delay
 *
 */
static void software_delay(void)
{
    volatile int i;
    for (i = 0; i < 1000000; i++)
        ;
}


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

#else
void exit(int status)
{
    Monitor(status);
}
#endif

u2 readClassFile(char* fileName, char* addr)
{
    int i;
    char c = conIn();                             /* dummy w*/
    if (c == 'w')
    {
        conOut(4);                                /*turn off echo*/
        c = conIn();                              /*s*/
        while ((c = conIn()) == 'p')
        {
/*if (c=='p) 	// apage comes*/
            c = conIn();
            c = conIn();                          /* address*/
            conOut('w');
            for (i = 0; i < 256; i++)
                *(addr++) = conIn();
            conOut('w');
        }
        conOut(5);                                /* turn on echo uploadend*/
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
        conOut(5);                                /* turn on echo*/
        return (u2) 0;
    }
}
