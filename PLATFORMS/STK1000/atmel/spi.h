/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief SPI bus example driver.
 *
 * This file defines a usefull set of functions for the SPI interface on
 * AVR32 devices. The driver handles normal polled usage and direct memory
 * access (PDC) usage.
 *
 * - Compiler:           IAR EWAAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with a SPI module can be used.
 * - AppNote:            AVR32105 - Master and Slave SPI Driver
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support email: avr32@atmel.com
 *
 * $Revision: 506 $
 * $Date: 2007-05-21 20:00:05 +0200 (Mon, 21 May 2007) $
 *****************************************************************************/

/* Copyright (c) 2006, Atmel Corporation All rights reserved.
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

#ifndef _AVR32_SPI_H_
#define _AVR32_SPI_H_

#ifdef __GNUC__
#include <avr32/io.h>
#elif __ICCAVR32__
#include <avr32/ioap7000.h>
#else
#error No known compiler used
#endif

/*! \brief Timeout for spi read and write blocking functions */
#define SPI_TIMEOUT		10000
/*! \brief Enable PDC functions for SPI */
#define SPI_ENABLE_PDC		1

/*! \brief Error codes used by SPI driver */
enum {
	SPI_ERROR = -1,
	SPI_OK = 0,
	SPI_ERROR_TIMEOUT = 1,
	SPI_ERROR_ARGUMENT,
	SPI_ERROR_OVERRUN,
	SPI_ERROR_MODE_FAULT,
	SPI_ERROR_OVERRUN_AND_MODE_FAULT
};

/*! \brief Parameters for setting options for slave chips */
struct spi_options_t {
	/*! \brief Which register to setup */
	unsigned char reg;

	/*! \brief Prefered baudrate for the SPI */
	unsigned int baudrate;

	/*! \brief Number of bits in each character (8 - 16) */
	unsigned char bits;

	/*! \brief Delay before first clock pulse after selecting slave (in MCK) */
	unsigned char spck_delay;

	/*! \brief Delay between each transfer/character (in MCK) */
	unsigned char trans_delay;

	/*! \brief Set this chip to stay active after last transfer to it */
	unsigned char stay_act;

	/*! \brief Which of the four SPI-modes to use when transmitting */
	unsigned char spi_mode;

	/*! \brief fdiv Downsample MCK by a factor of N before passing it
	 *         to the baudrate generator (will give downsample all
	 *         communcation to 1/N of the speed, and increase delays
	 *         by a factor of N (N=32).
	 */
	unsigned char fdiv;

	/*! \brief modfdis This bit disables the mode fault detection \
	 *         (without this bit SPI in master mode will disable
	 *         itself if another master tries to address it)
	 */
	unsigned char modfdis;
};

#ifdef SPI_ENABLE_PDC
struct spi_pdc {
	unsigned long none[0x40];	/* 0x000 */
	unsigned long rpr;		/* 0x100 */
	unsigned long rcr;		/* 0x104 */
	unsigned long tpr;		/* 0x108 */
	unsigned long tcr;		/* 0x10c */
	unsigned long rnpr;		/* 0x110 */
	unsigned long rncr;		/* 0x114 */
	unsigned long tnpr;		/* 0x118 */
	unsigned long tncr;		/* 0x11c */
	unsigned long ptcr;		/* 0x120 */
	unsigned long ptsr;		/* 0x124 */
};
typedef struct spi_pdc spi_pdc_t;
#endif

void spi_reset(volatile avr32_spi_t *spi);

int spi_initSlave(volatile avr32_spi_t *spi,
		unsigned char bits,
		unsigned char spi_mode);

int spi_initTest(volatile avr32_spi_t *spi);

int spi_initMaster(volatile avr32_spi_t *spi, struct spi_options_t *options);

int spi_selectionMode(volatile avr32_spi_t *spi,
		unsigned char variable_ps,
		unsigned char pcs_decode,
		unsigned char delay);

int spi_selectChip(volatile avr32_spi_t *spi, unsigned char chip);

int spi_unselectChip(volatile avr32_spi_t *spi, unsigned char chip);

int spi_setupChipReg(volatile avr32_spi_t *spi,
		struct spi_options_t *options,
		unsigned int cpuHz);

void spi_enable(volatile avr32_spi_t *spi);

void spi_disable(volatile avr32_spi_t *spi);

int spi_write(volatile avr32_spi_t *spi, unsigned short data);

int spi_variableSlaveWrite(volatile avr32_spi_t *spi,
		unsigned short data,
		unsigned char pcs,
		unsigned char lastxfer);

unsigned char spi_readRegisterFullCheck(volatile avr32_spi_t *spi);

int spi_read(volatile avr32_spi_t *spi, unsigned short *data);

#ifdef SPI_ENABLE_PDC
void spi_disablePDC(volatile avr32_spi_t *spi);

void spi_enablePDC(volatile avr32_spi_t *spi);

int spi_set_RxBuf(volatile avr32_spi_t *spi,
		void *rp,
		unsigned short rc,
		void *rnp,
		unsigned short rnc);

int spi_set_TxBuf(volatile avr32_spi_t *spi,
		void *tp,
		unsigned short tc,
		void *tnp,
		unsigned short tnc);

unsigned short spi_getRcr(volatile avr32_spi_t *spi);

unsigned short spi_getTcr(volatile avr32_spi_t *spi);
#endif /* SPI_ENABLE_PDC */

unsigned char spi_getStatus(volatile avr32_spi_t *spi);

#endif /* #ifndef _AVR32_SPI_H_ */

