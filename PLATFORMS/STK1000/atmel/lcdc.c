/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief LCD controller driver for AVR32AP devices.
 *
 * This file defines a useful set of functions for the LCD controller
 * on AVR32AP devices.
 *
 * - Compiler:           GNU GCC for AVR32
 * - Supported devices:  All AVR32AP devices with a SIDSA LCD controller
 * - AppNote:            AVR32114 Using the AVR32 LCD controller
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 * $Name$
 * $Revision: 595 $
 * $RCSfile$
 * $Date: 2007-08-24 10:13:07 +0200 (Fri, 24 Aug 2007) $
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

#include "lcdc.h"

/*! \brief Configures the LCD module
 * 
 * \param lcdc_conf Pointer to LCD controller configuration structure
 * 
 * \return Status
 *  \retval 0 = success
 *  \retval -1 = invalid argument
 *
 */
int lcdc_init(lcdc_conf_t* lcdc_conf)
{
	volatile avr32_lcdc_t *plcdc = &AVR32_LCDC;
	unsigned char valid_data_lines = 0;
	unsigned char pixel_size = 0;
	unsigned char clkval = 0;
	unsigned int pixel_clock_theo = 0;
	unsigned short lineval, hozval;
	

	/* Turn off LCD Controller (core first then DMA) */
	plcdc->pwrcon &= ~(1 << AVR32_LCDC_PWRCON_PWR_OFFSET);
	plcdc->dmacon &= ~(1 << AVR32_LCDC_DMACON_DMAEN_OFFSET);
	
	/* LCDFRCFG */
	if(lcdc_conf->distype != TFT){
		switch(lcdc_conf->scanmod){
		case SINGLE_SCAN:
			switch(lcdc_conf->ifwidth){
			case IF_WIDTH4:
				valid_data_lines = 4;
				break;
			case IF_WIDTH8:
				valid_data_lines = 8;
				break;
			default:
				return -1;
		}
		
		case DUAL_SCAN:
			switch(lcdc_conf->ifwidth){
			case IF_WIDTH8:
				valid_data_lines = 4;
				break;
			case IF_WIDTH16:
				valid_data_lines = 8;
				break;
			default:
				return -1;
			}
		default:
			return -1;
		}
	}
		
	lineval = lcdc_conf->yres - 1;
	switch(lcdc_conf->distype){
	case STN_MONO:
		hozval = (lcdc_conf->xres / valid_data_lines) - 1;
		break;
	case STN_COLOR: 
		hozval = (lcdc_conf->xres * 3 / valid_data_lines) - 1;
		break;
	case TFT:
		hozval = lcdc_conf->xres - 1;
		break;
	default:
		return -1;
	}
	plcdc->lcdfrmcfg = (lineval & AVR32_LCDC_LINEVAL_MASK) |
			((hozval << AVR32_LCDC_HOZVAL) & AVR32_LCDC_HOZVAL_MASK);
	

	/* Calculation of theoretical pixel clock */
	switch(lcdc_conf->distype){
		case STN_MONO:
			pixel_clock_theo = lcdc_conf->frame_rate * lcdc_conf->xres * lcdc_conf->yres / valid_data_lines;
			break;
		case STN_COLOR:
			pixel_clock_theo = lcdc_conf->frame_rate * lcdc_conf->xres * lcdc_conf->yres * 3 / valid_data_lines;
			break;
		case TFT:
			pixel_clock_theo = lcdc_conf->frame_rate * lcdc_conf->xres * lcdc_conf->yres;
			break;
		default:
			return -1;
	}
	clkval = (lcdc_conf->lcdcclock / (2 * pixel_clock_theo));
	if (clkval == 0) {
		plcdc->lcdcon1 = 1; /* bypass pixel clock */
	} else {
		plcdc->lcdcon1 = ((clkval - 1) << AVR32_LCDC_LCDCON1_CLKVAL) & AVR32_LCDC_LCDCON1_CLKVAL_MASK;
	}
	
	/* LCDCON2 */
	switch(lcdc_conf->pixelsize){
		case 1: pixel_size = 0;break;
		case 2: pixel_size = 1;break;
		case 4: pixel_size = 2;break;
		case 8: pixel_size = 3;break;
		case 16: pixel_size = 4;break;
		case 24: pixel_size = 5;break;
		case 32: pixel_size = 6;break;
		default:
			return -1;
	}
	plcdc->lcdcon2 = (lcdc_conf->distype & AVR32_LCDC_LCDCON2_DISTYPE_MASK) |
			((lcdc_conf->scanmod << AVR32_LCDC_LCDCON2_SCANMOD) & AVR32_LCDC_LCDCON2_SCANMOD_MASK) |
			((lcdc_conf->ifwidth << AVR32_LCDC_LCDCON2_IFWIDTH) & AVR32_LCDC_LCDCON2_IFWIDTH_MASK) |
			((lcdc_conf->invvd << AVR32_LCDC_LCDCON2_INVVD) & AVR32_LCDC_LCDCON2_INVVD_MASK) |
			((lcdc_conf->invframe << AVR32_LCDC_LCDCON2_INVFRAME) & AVR32_LCDC_LCDCON2_INVFRAME_MASK) |
			((pixel_size << AVR32_LCDC_LCDCON2_PIXELSIZE) & AVR32_LCDC_LCDCON2_PIXELSIZE_MASK) |
			((lcdc_conf->invline << AVR32_LCDC_LCDCON2_INVLINE) & AVR32_LCDC_LCDCON2_INVLINE_MASK) |
			((lcdc_conf->invclk << AVR32_LCDC_LCDCON2_INVCLK) & AVR32_LCDC_LCDCON2_INVCLK_MASK) |
			((lcdc_conf->invdval << AVR32_LCDC_LCDCON2_INVDVAL) & AVR32_LCDC_LCDCON2_INVDVAL_MASK) |
			((lcdc_conf->clkmod << AVR32_LCDC_LCDCON2_CLKMOD) & AVR32_LCDC_LCDCON2_CLKMOD_MASK) |
			((lcdc_conf->memor << AVR32_LCDC_LCDCON2_MEMOR) & AVR32_LCDC_LCDCON2_MEMOR_MASK);

	/* Timings */
	plcdc->lcdtim1 = (lcdc_conf->vfp & AVR32_LCDC_LCDTIM1_VFP_MASK) |
			((lcdc_conf->vbp << AVR32_LCDC_LCDTIM1_VBP) & AVR32_LCDC_LCDTIM1_VBP_MASK) |
			(((lcdc_conf->vpw - 1) << AVR32_LCDC_LCDTIM1_VPW) & AVR32_LCDC_LCDTIM1_VPW_MASK) |
			((lcdc_conf->vhdly << AVR32_LCDC_LCDTIM1_VHDLY) & AVR32_LCDC_LCDTIM1_VHDLY_MASK);

	plcdc->lcdtim2 = (lcdc_conf->hbp & AVR32_LCDC_HBP_MASK) |
			(((lcdc_conf->hpw - 1) << AVR32_LCDC_LCDTIM2_HPW) & AVR32_LCDC_LCDTIM2_HPW_MASK) |
			((lcdc_conf->hfp << AVR32_LCDC_LCDTIM2_HFP) & AVR32_LCDC_LCDTIM2_HFP_MASK);

	/* Interrupts */
	plcdc->idr = 0xFFFFFFFF;

	/* Toggle rate */
	plcdc->lcdmval = (lcdc_conf->mval & AVR32_LCDC_LCDMVAL_MVAL_MASK) |
			((lcdc_conf->mmode << AVR32_LCDC_LCDMVAL_MMODE_OFFSET) & AVR32_LCDC_LCDMVAL_MMODE_MASK);
	
	/* Contrast */
	plcdc->contrast_val = lcdc_conf->ctrstval;
	plcdc->contrast_ctr = (lcdc_conf->ctrst_ps & AVR32_LCDC_CONTRAST_CTR_PS_MASK) |
			((lcdc_conf->ctrst_pol << AVR32_LCDC_CONTRAST_CTR_POL_OFFSET) & AVR32_LCDC_CONTRAST_CTR_POL_MASK) |
			((lcdc_conf->ctrst_ena << AVR32_LCDC_CONTRAST_CTR_ENA_OFFSET) & AVR32_LCDC_CONTRAST_CTR_ENA_MASK);

	/* Setup FIFO */
	int lcd_fifo_size = lcdc_conf->scanmod ? 256 : 512;
	plcdc->lcdfifo = lcd_fifo_size - (2 * lcdc_conf->burst_length + 3);
	
	/* DMA base address */
	plcdc->dmabaddr1 = lcdc_conf->dmabaddr1;
	if(lcdc_conf->scanmod == DUAL_SCAN){
		plcdc->dmabaddr2 = lcdc_conf->dmabaddr2;
	}
	/* DMA frame configuration
	* The frame size is measured in words
	*/
	plcdc->dmafrmcfg = ((((lcdc_conf->xres * lcdc_conf->yres * lcdc_conf->pixelsize) + 31 )/ 32) & AVR32_LCDC_DMAFRMCFG_FRMSIZE_MASK) |
			(((lcdc_conf->burst_length - 1) << AVR32_LCDC_DMAFRMCFG_BRSTLEN) & AVR32_LCDC_DMAFRMCFG_BRSTLEN_MASK);
	
	/* 2D configuration */
	if(lcdc_conf->set2dmode){
		/* Assumed is that the frame starts at a word boundary -> no pixel offset needed */
		plcdc->dma2dcfg = ((lcdc_conf->virtual_xres - lcdc_conf->xres) * (lcdc_conf->pixelsize / 8)) & AVR32_LCDC_DMA2DCFG_ADDRINC_MASK;
	}

	/* wait for DMA engine to become idle */
	while (plcdc->dmacon & AVR32_LCDC_DMACON_DMABUSY);

	/* and enable DMA with updated configuration */
	if(lcdc_conf->set2dmode){
		plcdc->dmacon = (1 << AVR32_LCDC_DMACON_DMAEN_OFFSET) | 
			(1 << AVR32_LCDC_DMACON_DMAUPDT_OFFSET) | 
			(1 << AVR32_LCDC_DMACON_DMA2DEN_OFFSET);
	}
	else{
		plcdc->dmacon = (1 << AVR32_LCDC_DMACON_DMAEN_OFFSET) | 
			(1 << AVR32_LCDC_DMACON_DMAUPDT_OFFSET);
	}
	/* enable LCD */
	plcdc->pwrcon |= (lcdc_conf->guard_time << AVR32_LCDC_PWRCON_GUARD_TIME_OFFSET) & AVR32_LCDC_PWRCON_GUARD_TIME_MASK;
	
	/* Wait for the LCDC core to become idle and enable it */
	while (plcdc->PWRCON.busy == 1);
	plcdc->PWRCON.pwr = 1;

	return 0;
}
