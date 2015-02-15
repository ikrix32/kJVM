/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief LCD controller example for the STK1000.
 *  
 *  This example shows the use of the 2D addressing mode on the LCD controller.
 *  The virtual frame buffer is two times the size of the LCD viewing area.
 *  Before you run this application program the picture (AVR32.bmp) into the flash at 
 *  address 0x00400000. Use the avr32program application for this purpose.
 *  avr32program -F bin -O 0x00400000
 *  If you intend to program the picture to another location change the define 
 *  BITMAP_FILE_ADDRESS accordingly.
 * 
 *  The input (switches) header marked J25, used for moving around the virtual frame buffer,
 *  must be connected to the header labeled J1 (PORTB[0..7]).
 * 
 *  To move the viewing area use following switches:
 *  Switch0: Move viewing area 10 pixels to the right
 *  Switch1: Move viewing area 10 pixels to the left
 *  Switch2: Move viewing area 10 lines up
 *  Switch3: Move viewing area 10 lines down
 *
 * - Compiler:           GNU GCC for AVR32
 * - Supported devices:  All AVR32AP devices with a SIDSA LCD controller
 * - AppNote:            AVR32114 Using the AVR32 LCD controller
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 * $Name$
 * $Revision: 668M $
 * $RCSfile$
 * $Date: 2008-05-07 15:58:43 +0200 (Wed, 07 May 2008) $
 *****************************************************************************/

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

#include <string.h>
#include "usart.h"
#include "pio.h"
#include "spi.h"
#include "lcdc.h"
#include "utils.h"
#include "at32stk1000.h"
void myfill(lcdc_conf_t *lcdc_conf);
void setPixelRGB(unsigned short x, unsigned short y, unsigned char red, unsigned char green, unsigned char blue)	;
extern void ltv350qv_power_on(volatile avr32_spi_t * spi, unsigned char chip_select);
extern int display_virtual_bm(lcdc_conf_t *lcdc_conf, void * bm_file);
extern void usdelay(unsigned long usec);
extern void usart_printHex(volatile avr32_usart_t * usart, const unsigned long n);

int increment_frame_base(lcdc_conf_t *lcdc_conf, int pixel, int line);
void lcd_pio_config(void);
void init_spiMaster(volatile avr32_spi_t * spi);
void print(volatile struct avr32_usart_t * usart, char *str);

#define SWITCHES_MASK 0x000000FF
#define SWITCH0 0x00000001
#define SWITCH1 0x00000002
#define SWITCH2 0x00000004
#define SWITCH3 0x00000008

/*! \brief Start address of the bitmap file */ 
#define BITMAP_FILE_ADDRESS 0x00400000

/*! \brief LCD controller configuration */
lcdc_conf_t ltv350qv_conf ={
		
	dmabaddr1: 0x10000000,
	dmabaddr2: 0,
	burst_length: 4,
	xres: 320,
	yres: 240,
	set2dmode: MODE_2D_ON,
	virtual_xres: 640,
	virtual_yres: 480,
	frame_rate: 75,
	lcdcclock: 40000000,
	guard_time: 2,
	memor: BIG_ENDIAN,
	ifwidth: 0,
	scanmod: SINGLE_SCAN,
	distype: TFT,
	invvd: NORMAL,
	invframe: INVERTED,
	invline: INVERTED,
	invclk: INVERTED,
	invdval: INVERTED,
	clkmod: ALWAYS_ACTIVE,
	pixelsize: BPP_32,
	ctrstval: 0x0f,
	ctrst_ena: ENABLED,
	ctrst_pol: NORMAL,
	ctrst_ps: PRE_HALF,
	mval: 0,
	mmode: EACH_FRAME,
	hpw: 16,
	hbp: 15,
	hfp: 33,
	vpw: 1,	
	vbp: 10,
	vfp: 10,
	vhdly: 0,
};
/*! \brief USART used for console output */
volatile avr32_usart_t *usart = &AVR32_USART1;

 /*! \brief Initialise SPI in master mode for the LCD
 *
 *  \param spi Pointer to the correct avr32_spi_t struct
 */
void init_spiMaster(volatile avr32_spi_t * spi)
{

	struct spi_options_t spiOptions;
	avr32_piomap_t spi_piomap = {					\
		{AVR32_SPI0_SCK_0_PIN, AVR32_SPI0_SCK_0_FUNCTION},	\
		{AVR32_SPI0_MISO_0_PIN, AVR32_SPI0_MISO_0_FUNCTION},	\
		{AVR32_SPI0_MOSI_0_PIN, AVR32_SPI0_MOSI_0_FUNCTION},	\
		{AVR32_SPI0_NPCS_0_PIN, AVR32_SPI0_NPCS_0_FUNCTION},	\
		{AVR32_SPI0_NPCS_1_PIN, AVR32_SPI0_NPCS_1_FUNCTION},	\
		{AVR32_SPI0_NPCS_2_PIN, AVR32_SPI0_NPCS_2_FUNCTION},	\
		{AVR32_SPI0_NPCS_3_PIN, AVR32_SPI0_NPCS_3_FUNCTION},	\
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
void lcd_pio_config(void){
	
		avr32_piomap_t piomap = {
			{ AVR32_LCDC_CC_0_0_PIN, AVR32_LCDC_CC_0_0_FUNCTION },
			{ AVR32_LCDC_DVAL_0_0_PIN, AVR32_LCDC_DVAL_0_0_FUNCTION },
			{ AVR32_LCDC_HSYNC_0_PIN, AVR32_LCDC_HSYNC_0_FUNCTION },
			{ AVR32_LCDC_MODE_0_0_PIN, AVR32_LCDC_MODE_0_0_FUNCTION },
			{ AVR32_LCDC_PCLK_0_PIN, AVR32_LCDC_PCLK_0_FUNCTION },
			{ AVR32_LCDC_PWR_0_PIN, AVR32_LCDC_PWR_0_FUNCTION },
			{ AVR32_LCDC_VSYNC_0_PIN, AVR32_LCDC_VSYNC_0_FUNCTION },
			{ AVR32_LCDC_DATA_0_0_PIN, AVR32_LCDC_DATA_0_0_FUNCTION },
			{ AVR32_LCDC_DATA_1_0_PIN, AVR32_LCDC_DATA_1_0_FUNCTION },
			{ AVR32_LCDC_DATA_2_0_PIN, AVR32_LCDC_DATA_1_0_FUNCTION },
			{ AVR32_LCDC_DATA_3_0_PIN, AVR32_LCDC_DATA_1_0_FUNCTION },
			{ AVR32_LCDC_DATA_4_0_PIN, AVR32_LCDC_DATA_1_0_FUNCTION },
			{ AVR32_LCDC_DATA_5_PIN, AVR32_LCDC_DATA_5_FUNCTION },
			{ AVR32_LCDC_DATA_6_PIN, AVR32_LCDC_DATA_6_FUNCTION },
			{ AVR32_LCDC_DATA_7_PIN, AVR32_LCDC_DATA_7_FUNCTION },
			{ AVR32_LCDC_DATA_8_0_PIN, AVR32_LCDC_DATA_8_0_FUNCTION },
			{ AVR32_LCDC_DATA_9_0_PIN, AVR32_LCDC_DATA_9_0_FUNCTION },
			{ AVR32_LCDC_DATA_10_0_PIN, AVR32_LCDC_DATA_10_0_FUNCTION },
			{ AVR32_LCDC_DATA_11_0_PIN, AVR32_LCDC_DATA_11_0_FUNCTION },
			{ AVR32_LCDC_DATA_12_0_PIN, AVR32_LCDC_DATA_12_0_FUNCTION },
			{ AVR32_LCDC_DATA_13_PIN, AVR32_LCDC_DATA_13_FUNCTION },
			{ AVR32_LCDC_DATA_14_PIN, AVR32_LCDC_DATA_14_FUNCTION },
			{ AVR32_LCDC_DATA_15_PIN, AVR32_LCDC_DATA_15_FUNCTION },
			{ AVR32_LCDC_DATA_16_0_PIN, AVR32_LCDC_DATA_16_0_FUNCTION },
			{ AVR32_LCDC_DATA_17_0_PIN, AVR32_LCDC_DATA_17_0_FUNCTION },
			{ AVR32_LCDC_DATA_18_0_PIN, AVR32_LCDC_DATA_18_0_FUNCTION },
			{ AVR32_LCDC_DATA_19_0_PIN, AVR32_LCDC_DATA_19_0_FUNCTION },
			{ AVR32_LCDC_DATA_20_0_PIN, AVR32_LCDC_DATA_20_0_FUNCTION },
			{ AVR32_LCDC_DATA_21_0_PIN, AVR32_LCDC_DATA_21_0_FUNCTION },
			{ AVR32_LCDC_DATA_22_PIN, AVR32_LCDC_DATA_22_FUNCTION },
			{ AVR32_LCDC_DATA_23_PIN, AVR32_LCDC_DATA_23_FUNCTION }
			
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
int increment_frame_base(lcdc_conf_t *lcdc_conf, int pixel, int line){
	
	volatile avr32_lcdc_t *plcdc = &AVR32_LCDC;
	unsigned long base1;
	
	/* increment frame pointer by lines */
	base1 = plcdc->dmabaddr1 + lcdc_conf->virtual_xres * lcdc_conf->pixelsize / 8 * line;
	/* increment frame pointer by pixel */
	base1 += 4 * pixel;

	/* do not allow to move the viewing area out of the virtual frame buffer */
	if((base1 >= lcdc_conf->dmabaddr1 ) && 
			base1 <= (lcdc_conf->dmabaddr1 + lcdc_conf->virtual_xres * lcdc_conf->pixelsize / 8 * (lcdc_conf->virtual_yres - lcdc_conf->yres) ))
		plcdc->dmabaddr1 = base1;
	else 
		return -1;
	
	/* update DMA configuration DMAUPDT */
	plcdc->dmacon |=  (1 << AVR32_LCDC_DMACON_DMAUPDT_OFFSET);
	return 0;
}
/*! \brief 2D addressing mode example for the STK1000
 *  Connect switches pins to J1.
 *  Switch0: Move viewing area 10 pixels to the right
 *  Switch1: Move viewing area 10 pixels to the left
 *  Switch2: Move viewing area 10 lines up
 *  Switch3: Move viewing area 10 lines down
 */ 
int main (void)
{
	volatile avr32_spi_t * spi = &AVR32_SPI0;
	unsigned int input;
	volatile avr32_pio_t *piob = &AVR32_PIOB;
	volatile avr32_pm_t *ppm = &AVR32_PM;
	
	board_init();
	piob->per = SWITCHES_MASK;
	piob->codr = SWITCHES_MASK;
	lcd_pio_config();
	
	memset((void *)ltv350qv_conf.dmabaddr1, 0, ltv350qv_conf.virtual_xres * ltv350qv_conf.virtual_yres * ltv350qv_conf.pixelsize / 8);
	
	print(usart, "Setting up SPI for LTV350QV panel\n");
	init_spiMaster(spi);
	
	print(usart,"Initializing LTV350QV panel\n");
	ltv350qv_power_on(spi, 1);
	
	print(usart, "Enabling LCD controller\n");
	/* Power manager setup 
	* Enable CLOCK for LCDC in HSBMASK
	*/
	ppm->hsb_mask |= 0x00000080;	
	/* Enable generic clock PLL0 for LCD controller pixel clock*/
	ppm->gcctrl[7] |= 0x00000006;
	
	print(usart, "Initializing LCD controller\n");
	lcdc_init(&ltv350qv_conf);
	
	print(usart, "Filling frame buffer with data\n");
	/* print the image into the virtual framebuffer */
	myfill(&ltv350qv_conf);
unsigned short x,y;
//unsigned char red,green,blue;
for (x=20; x<300;x++)	 {
setPixelRGB(x, 30, 255, 0, 0);
setPixelRGB(x, 200, 0, 255, 0);
}
for (y=30; y<200;y++)	 {
setPixelRGB(20, y, 0, 0, 255);
setPixelRGB(300, y,255, 255, 255);
}

	while(1){
		
		usdelay(100000);
		/* get input from the switches */
		input = ~( piob->pdsr & SWITCHES_MASK);
		
		if(input & SWITCH0){
			increment_frame_base(&ltv350qv_conf, 10, 0);
		}
		if(input & SWITCH1){
			increment_frame_base(&ltv350qv_conf, -10, 0);
		}
		if(input & SWITCH2){
			increment_frame_base(&ltv350qv_conf, 0, -10);
		}
		if(input & SWITCH3){
			increment_frame_base(&ltv350qv_conf, 0, 10);
		}
	}
	return 0;
}
