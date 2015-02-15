/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Power on and power down sequence for the LTV350QV display on the
 * STK1000.
 *
 * - Compiler:           GNU GCC for AVR32
 * - Supported devices:  All AVR32AP devices with a SIDSA LCD controller
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


#include "spi.h"
#include "pm.h"
#include <stdio.h>
#include <sys/sysregs.h>

#define sysreg_read(addr)            __builtin_mfsr(addr)
#define sysreg_write(addr, value)    __builtin_mtsr(addr, value)


#define write_reg(spi, reg, val)				\
	do {						\
		ret = ltv350qv_write_reg(spi, reg, val);	\
		if (ret){				\
			goto out;			\
		}\
	} while (0)

// bh usart_print(usart, "Error spi timeout\n");\

extern volatile struct avr32_usart_t *usart;
extern void usart_print(volatile struct avr32_usart_t * usart, char *str);

static inline unsigned int get_count(void)
{
	return sysreg_read(SYSREG_COUNT);
}

/*! \brief Delay execution for a specificated time
 * 
 *  \param usec Delay in microseconds
 */
void usdelay(unsigned long usec)
{
	unsigned long start, stop;
	unsigned long ticks;

	start = get_count();
	ticks = usec * (pm_read_mclk() / 1000000);
	stop = start + ticks;

	if (start > stop)
		while (get_count() > start) ;
	
	while (get_count() < stop) ;
}
/*! \brief Write to a display register
 * 
 *  \param spi Pointer to the SPI interface
 *  \param reg Register
 *  \param val Register value
 */
static int ltv350qv_write_reg(volatile avr32_spi_t * spi, unsigned char reg, unsigned short val)
{
	int i;
	unsigned char buffer[3];

	buffer[0] = 0x74;
	buffer[1] = 0x00;
	buffer[2] = reg & 0x7f;
	
	for( i = 0; i <= 2; i++){
		if( spi_variableSlaveWrite(spi, buffer[i], 1, (i == 2 ? 1 : 0)) != SPI_OK)
			return -1;
	}

	buffer[0] = 0x76;
	buffer[1] = val >> 8;
	buffer[2] = val;
	
	for( i = 0; i <= 2; i++){
		if( spi_variableSlaveWrite(spi, buffer[i], 1, (i == 2 ? 1 : 0)) != SPI_OK)
			return -1;
	}
	return 0;
}

/*! \brief Power on sequence for the display
 * 
 *  \param spi Pointer to the SPI interface
 *  \param chip_select Chip select number
 */
void ltv350qv_power_on(volatile avr32_spi_t * spi, unsigned char chip_select)
{
	int ret;

	//usart_print(usart,"ltv350qv: do power on sequence\n");
	spi_selectChip(spi, chip_select);
	/* write startup procedure */
	write_reg(spi, 9, 0x0000);
	usdelay(15000);
	write_reg(spi, 9, 0x4000);
	write_reg(spi, 10, 0x2000);
	write_reg(spi, 9, 0x4055);
	usdelay(55000);
	write_reg(spi, 1, 0x409d);
	write_reg(spi, 2, 0x0204);
	write_reg(spi, 3, 0x0100);
	write_reg(spi, 4, 0x3000);
	write_reg(spi, 5, 0x4003);
	write_reg(spi, 6, 0x000a);
	write_reg(spi, 7, 0x0021);
	write_reg(spi, 8, 0x0c00);
	write_reg(spi, 10, 0x0103);
	write_reg(spi, 11, 0x0301);
	write_reg(spi, 12, 0x1f0f);
	write_reg(spi, 13, 0x1f0f);
	write_reg(spi, 14, 0x0707);
	write_reg(spi, 15, 0x0307);
	write_reg(spi, 16, 0x0707);
	write_reg(spi, 17, 0x0000);
	write_reg(spi, 18, 0x0004);
	write_reg(spi, 19, 0x0000);

	usdelay(20000);
	write_reg(spi, 9, 0x4a55);
	write_reg(spi, 5, 0x5003);

	//usart_print(usart,"ltv350qv: power on sequence done\n");
out:
	return;
}
/*! \brief Power off sequence for the dispaly
 * 
 *  \param spi Pointer to the SPI interface
 *  \param chip_select Chip select number
 */
void ltv350qv_power_off(volatile avr32_spi_t * spi, unsigned char chip_select)
{
	int ret;
	spi_selectChip(spi, chip_select);
	//usart_print(usart,"ltv350qv: do power off sequence\n");
	/* GON -> 0, POC -> 0 */
	write_reg(spi, 9, 0x4055);
	/* DSC -> 0 */
	write_reg(spi, 5, 0x4003);
	/* VCOMG -> 0 */
	write_reg(spi, 10, 0x2103);

	usdelay(1000000);

	/* AP[2:0] -> 000 */
	write_reg(spi, 9, 0x4050);

	//usart_print(usart,"ltv350qv: power off sequence done\n");
out:
	return;
}
