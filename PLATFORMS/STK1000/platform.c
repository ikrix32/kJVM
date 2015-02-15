/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
/* ************************************************************************ *
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
#include <string.h>
#include "atmel/usart.h"
#include "atmel/pio.h"
#include "atmel/spi.h"
#include "atmel/lcdc.h"
#include "lib2d.h"
#include "atmel/utils.h"
#include "atmel/sdram.h"
#include "atmel/mt481c2m32b2tg.h"
#include "../../JVM/typedefinitions.h"
#include "../../JVM/definitions.h"
#include "../../JVM/bajvm.h"
#include "../../JVM/heap.h"
#include "../../JVM/classfile.h"
#include "platform.h"

extern void ltv350qv_power_on(volatile avr32_spi_t * spi,
unsigned char chip_select);
extern int display_virtual_bm(lcdc_conf_t *lcdc_conf, void * bm_file);
extern void usdelay(unsigned long usec);
extern void usart_printHex(volatile avr32_usart_t * usart,
const unsigned long n);
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "atmel/pm.h"

void initHW()
{
#define SWITCHES_MASK 0x000000FF
#define SWITCH0 0x00000001
#define SWITCH1 0x00000002
#define SWITCH2 0x00000004
#define SWITCH3 0x00000008

/*! \brief Start address of the bitmap file */
#define BITMAP_FILE_ADDRESS 0x00400000

/*! \brief LCD controller configuration */
    lcdc_conf_t ltv350qv_conf =
    {
        dmabaddr1: 0x10000000, dmabaddr2: 0,
        burst_length: 4, xres: 320, yres: 240, set2dmode: MODE_2D_ON,
        virtual_xres: 640, virtual_yres: 480, frame_rate: 75, lcdcclock
        : 40000000, guard_time: 2, memor: BIG_ENDIAN, ifwidth: 0,
        scanmod: SINGLE_SCAN, distype: TFT, invvd: NORMAL, invframe
        : INVERTED, invline: INVERTED, invclk: INVERTED, invdval
        : INVERTED, clkmod: ALWAYS_ACTIVE, pixelsize: BPP_32,
        ctrstval: 0x0f, ctrst_ena: ENABLED, ctrst_pol: NORMAL, ctrst_ps
        : PRE_HALF, mval: 0, mmode: EACH_FRAME, hpw: 16, hbp: 15,
        hfp: 33, vpw: 1, vbp: 10, vfp: 10, vhdly: 0,
    };
    volatile avr32_spi_t * spi = &AVR32_SPI0;
    unsigned int input;
    volatile avr32_pio_t *piob = &AVR32_PIOB;
    volatile avr32_pm_t *ppm = &AVR32_PM;

    static sdram_info *info;
    unsigned long sdram_size, tmp, i;
    int noErrors = 0;
    volatile unsigned long *sdram = (void *) STK1000_SDRAM_BASE;

    info->physical_address = STK1000_SDRAM_BASE;
    info->cols = mt481c2m32b2tg_cols;
    info->rows = mt481c2m32b2tg_rows;
    info->banks = mt481c2m32b2tg_banks;
    info->cas = mt481c2m32b2tg_cas;
    info->twr = mt481c2m32b2tg_twr;
    info->trc = mt481c2m32b2tg_trc;
    info->trp = mt481c2m32b2tg_trp;
    info->trcd = mt481c2m32b2tg_trcd;
    info->tras = mt481c2m32b2tg_tras;
    info->txsr = mt481c2m32b2tg_txsr;

/* Calculate sdram size */
    sdram_size = 1 << (info->rows + info->cols + info->banks + 2);

/* initialize the avr32 sdram controller */
    sdram_init(info);

/* initialize the external sdram chip */
//  mt481c2m32b2tg_init(info);

    board_init();
    piob->per = SWITCHES_MASK;
    piob->codr = SWITCHES_MASK;
    lcd_pio_config();

    memset(
        (void *) ltv350qv_conf.dmabaddr1,
        0,
        ltv350qv_conf.virtual_xres * ltv350qv_conf.virtual_yres
        * ltv350qv_conf.pixelsize / 8);

//print(usart, "Setting up SPI for LTV350QV panel\n");
    init_spiMaster(spi);

    ltv350qv_power_on(spi, 1);

/* Power manager setup
 * Enable CLOCK for LCDC in HSBMASK
 */
    ppm->hsbmask |= 0x00000080;
/* Enable generic clock PLL0 for LCD controller pixel clock*/
    ppm->gcctrl[7] |= 0x00000006;

    lcdc_init(&ltv350qv_conf);

/* print the image into the virtual framebuffer */
//myfill(&ltv350qv_conf);

    display_bm(((void *) 0x00700000));
    lcdInitZBuffer();
    initFont();
    lcdSelectFontFromArray(0);

    stdIOInit();
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
        u1 i = 0;
        char* addr;
        u4 temp;
        char buf[5];

#ifdef STK1000
                                                  /* boot classes in flash*/
        classFileBase=(char*)STK1000_FLASH_JAVA_BASE;
        appClassFileBase=STK1000_SDRAM_JAVA_BASE; /* app classes in sdram*/
#endif

#if (AVR32LINUX||LINUX)
        classFileBase=(char*)malloc((size_t) MAXBYTECODE);
        if (classFileBase==NULL)
            errorExit(-1,"malloc error while trying to allocate %d bytes for class files.\n", MAXBYTECODE);
#endif

        heapInit();                               /* linux avr8 malloc , others hard coded!*/
        length = 0;
#if LINUX|| AVR32LINUX
        if (argc > MAXCLASSES)
            errorExit(-1,"ERROR: trying to load %d classes, MAXCLASSES is %d\n", argc, MAXCLASSES);
        for (cN=0; cN < argc; cN++)
        {
            cs[cN].classFileStartAddress=classFileBase+length;
            cs[cN].classFileLength=readClassFile(argv[cN+1], cs[cN].classFileStartAddress);
            analyzeClass(&cs[cN]);
            length+=cs[cN].classFileLength;
            if (length > MAXBYTECODE)
            {
                printf("MAXBYTECODE reached!\n"); exit(-1);
            }
        }
#endif
#ifdef STK1000
/* analyze bootclasses, which are programmed in flash*/
        strncpy(buf,classFileBase,4);
        buf[4]=0;
        sscanf(buf,"%4d",&temp);
        numClasses=(u1)temp;
        addr=classFileBase+4;                     /* after numclasses*/
        for (cN=0; cN<numClasses;cN++)
        {
            strncpy(buf,addr,4);
            sscanf(buf,"%4d",&temp);
            cs[cN].classFileStartAddress=addr+4;  /* after length of class*/
            cs[cN].classFileLength=temp;          /*(u1)(*addr)+256*(u1)(*(addr+1));*/
            analyzeClass(&cs[cN]);
            addr+=cs[cN].classFileLength+4;
            printf("bootclasse %d %dloaded\n",cN,numClasses);
        }
        printf("%d bootclasses are loaded\n",cN);
/* thats to boot classes*/
/* now the application classes*/
        addr=appClassFileBase;
        length=0;
        do
        {
            printf("load application classes-> type \"w\" \n");
            cs[cN].classFileStartAddress=addr+length;
            cs[cN].classFileLength=readClassFile(NULL,cs[cN].classFileStartAddress);
            printf("\n");
            length+=cs[cN].classFileLength;
            analyzeClass(&cs[cN]);
            cN++;
            printf("still another appl. class ? (y) \n");
            if (conIn()!='y') break;
        }
        while(cs[cN].classFileLength !=0);
/*!!*/
#endif
        numClasses = cN;
        DEBUGPRINTHEAP;
    }

    char conIn()
    {
        return (char) usart_getchar(&AVR32_USART1);
    }

#define FAILURE -1
#define SUCCESS 0
    void conOut(char c)
    {
        usart_bw_write_char(&AVR32_USART1, (int) c);
    }

    void hexConOut(char c)
    {
        conOut(0x30 + ((c < 10) ? c : (c + 'A' - '0' - 10)));
    }

    void byteConOut(char c)
    {
        hexConOut((c & 0xF0) >> 4);
        hexConOut(c & 0x0F);
    }

    void wordConOut(int c)
    {
        byteConOut((char) ((c & 0xFF000000) >> 24));
        byteConOut((char) ((c & 0x00FF0000) >> 16));
        byteConOut((char) ((c & 0x0000FF00) >> 8));
        byteConOut((char) (c & 0x000000FF));
    }
    char ascii2Hex(char c)
    {
        return ((c <= '9') ? (c - '0') : ((c < 'G') ? (c - 'A' + 10) : (c - 'a'
            + 10)));
    }

    int __attribute__((weak)) _read(int file, char * ptr, int len)
    {
        int i;
//if ( !do_not_use_oscall_coproc ) return _read_sim(file, ptr, len);
//if (file != 0)return unimplemented_syscall("_read with filedes != 0");
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
//if ( !do_not_use_oscall_coproc ) return _write_sim(file, ptr, len);
//if ( (file != 1)&& (file != 2) && (file!=3)) return unimplemented_syscall("_write with file != 1 or 2");
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
//To configure standard I/O streams as unbuffered, you can simply:
        setbuf(stdin, NULL);
        setbuf(stdout, NULL);
    }

    void init_spiMaster(volatile avr32_spi_t * spi);
    int increment_frame_base(lcdc_conf_t *lcdc_conf, int pixel, int line);
    void lcd_pio_config(void);
    void init_spiMaster(volatile avr32_spi_t * spi);
    void print(volatile struct avr32_usart_t * usart, char *str);

    void VT102Attribute(u1 fgcolor, u1 bgcolor)
    {
        printf("%c", 0x1b);
        printf("%c", '[');
//            printf("%c",'4');
        printf("%c", fgcolor);
//printf("%c",';');
//printf("%c",40 + bgcolor);
        printf("%c", 'm');
    }

/*! \brief USART used for console output */
    volatile avr32_usart_t *usart = &AVR32_USART1;

/*! \brief Initialise SPI in master mode for the LCD
 *
 *  \param spi Pointer to the correct avr32_spi_t struct
 */
    void init_spiMaster(volatile avr32_spi_t * spi)
    {

        struct spi_options_t spiOptions;
        avr32_piomap_t spi_piomap =
        {
            {
                AVR32_SPI0_SCK_0_PIN,
                AVR32_SPI0_SCK_0_FUNCTION
            }
            ,
            {
                AVR32_SPI0_MISO_0_PIN,
                AVR32_SPI0_MISO_0_FUNCTION
            }
            ,
            {
                AVR32_SPI0_MOSI_0_PIN,
                AVR32_SPI0_MOSI_0_FUNCTION
            }
            ,
            {
                AVR32_SPI0_NPCS_0_PIN,
                AVR32_SPI0_NPCS_0_FUNCTION
            }
            ,
            {
                AVR32_SPI0_NPCS_1_PIN,
                AVR32_SPI0_NPCS_1_FUNCTION
            }
            ,
            {
                AVR32_SPI0_NPCS_2_PIN,
                AVR32_SPI0_NPCS_2_FUNCTION
            }
            ,
            {
                AVR32_SPI0_NPCS_3_PIN,
                AVR32_SPI0_NPCS_3_FUNCTION
            }
            ,
        };
        pio_enable_module(spi_piomap, 7);

        spiOptions.reg = 1;
        spiOptions.baudrate = 1500000;
        spiOptions.bits = 8;
        spiOptions.spck_delay = 0;
        spiOptions.trans_delay = 0;
        spiOptions.stay_act = 1;
        spiOptions.spi_mode = 3;

        spiOptions.fdiv = 0;
        spiOptions.modfdis = 0;

/* Initialize as master */
        spi_initMaster(spi, &spiOptions);

/* Set master mode; variable_ps, pcs_decode, delay */
        spi_selectionMode(spi, 0, 0, 0);

/* Select slave chip 1 (SPI_NPCS1) */
        spi_selectChip(spi, 1);

        spi_setupChipReg(spi, &spiOptions, pm_read_module_frequency(PM_PBA_SPI0));

        spi_enable(spi);
    }

/*! \brief Print a string of characters to an usart
 *
 * \param *usart The usart to write to
 * \param *str The string of characters
 *
 * \return;
 */
    void print(volatile struct avr32_usart_t * usart, char *str)
    {
        while (*str != '\0')
            usart_putchar(usart, *str++);
    }

/*! \brief Sets up the pins for the LCD on the STK1000
 *
 */
    void lcd_pio_config(void)
    {

        avr32_piomap_t piomap =
        {
            {
                AVR32_LCDC_CC_0_0_PIN,
                AVR32_LCDC_CC_0_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DVAL_0_0_PIN,
                AVR32_LCDC_DVAL_0_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_HSYNC_0_PIN,
                AVR32_LCDC_HSYNC_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_MODE_0_0_PIN,
                AVR32_LCDC_MODE_0_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_PCLK_0_PIN,
                AVR32_LCDC_PCLK_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_PWR_0_PIN,
                AVR32_LCDC_PWR_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_VSYNC_0_PIN,
                AVR32_LCDC_VSYNC_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_0_0_PIN,
                AVR32_LCDC_DATA_0_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_1_0_PIN,
                AVR32_LCDC_DATA_1_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_2_0_PIN,
                AVR32_LCDC_DATA_1_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_3_0_PIN,
                AVR32_LCDC_DATA_1_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_4_0_PIN,
                AVR32_LCDC_DATA_1_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_5_PIN,
                AVR32_LCDC_DATA_5_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_6_PIN,
                AVR32_LCDC_DATA_6_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_7_PIN,
                AVR32_LCDC_DATA_7_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_8_0_PIN,
                AVR32_LCDC_DATA_8_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_9_0_PIN,
                AVR32_LCDC_DATA_9_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_10_0_PIN,
                AVR32_LCDC_DATA_10_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_11_0_PIN,
                AVR32_LCDC_DATA_11_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_12_0_PIN,
                AVR32_LCDC_DATA_12_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_13_PIN,
                AVR32_LCDC_DATA_13_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_14_PIN,
                AVR32_LCDC_DATA_14_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_15_PIN,
                AVR32_LCDC_DATA_15_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_16_0_PIN,
                AVR32_LCDC_DATA_16_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_17_0_PIN,
                AVR32_LCDC_DATA_17_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_18_0_PIN,
                AVR32_LCDC_DATA_18_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_19_0_PIN,
                AVR32_LCDC_DATA_19_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_20_0_PIN,
                AVR32_LCDC_DATA_20_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_21_0_PIN,
                AVR32_LCDC_DATA_21_0_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_22_PIN,
                AVR32_LCDC_DATA_22_FUNCTION
            }
            ,
            {
                AVR32_LCDC_DATA_23_PIN,
                AVR32_LCDC_DATA_23_FUNCTION
            }

        };
        pio_enable_module(piomap, 31);

    }
/*! \brief Move the viewing area in the virtual frame buffer
 *
 *  \param lcdc_conf Pointer to the LCD controller configuration
 *  \param pixel Number of pixels to move the viewing area
 *  \param line Number of lines to move the viewing area
 *
 *  \return Returns the status of the movement
 *    \retval 0 Movement succeeded
 *    \retval -1 Movement was not successfull (viewing are is out of the
 *               virtual frame buffer
 */
    int increment_frame_base(lcdc_conf_t *lcdc_conf, int pixel, int line)
    {

        volatile avr32_lcdc_t *plcdc = &AVR32_LCDC;
        unsigned long base1;

/* increment frame pointer by lines */
        base1 = plcdc->dmabaddr1 + lcdc_conf->virtual_xres * lcdc_conf->pixelsize
            / 8 * line;
/* increment frame pointer by pixel */
        base1 += 4 * pixel;

/* do not allow to move the viewing area out of the virtual frame buffer */
        if ((base1 >= lcdc_conf->dmabaddr1) && base1 <= (lcdc_conf->dmabaddr1
            + lcdc_conf->virtual_xres * lcdc_conf->pixelsize / 8
            * (lcdc_conf->virtual_yres - lcdc_conf->yres)))
            plcdc->dmabaddr1 = base1;
        else
            return -1;

/* update DMA configuration DMAUPDT */
        plcdc->dmacon |= (1 << AVR32_LCDC_DMACON_DMAUPDT_OFFSET);
        return 0;
    }

    u2 readClassFile(char* fileName, char* addr)
    {

#if LINUX||AVR32LINUX
        int fd;
        u2 classFileLength=-(u2)((long)addr%(1<<16))-1;
        if ((fd=open(fileName,O_RDONLY))==-1)
            perror(fileName);
        while (read(fd,addr++,1));
        return classFileLength+=(long)addr;
#endif
#if (AVR32UC3A||AVR32AP7000)
        int i;
        char c=conIn();                           /* dummy w*/
        if (c =='w')
        {
            conOut(4);                            /*turn off echo*/
            c=conIn();                            /*s*/
            while ((c=conIn())=='p')
            {
/*if (c=='p) 	// apage comes*/
                c=conIn();
                c=conIn();                        /* address*/
                conOut('w');
                for (i=0; i < 256;i++) *(addr++)=conIn();
                conOut('w');
            }
            conOut(5);                            /* turn on echo uploadend*/
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
            conOut(5);                            /* turn on echo*/
            return (u2) 0;
        }
#endif
    }
