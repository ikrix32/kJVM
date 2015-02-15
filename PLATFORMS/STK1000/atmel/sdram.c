/* ************************************************************************

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

************************************************************************ */

#include "sdram.h"

#define MODE_NORMAL		0
#define MODE_NOP		1
#define MODE_PRECHARGE		2
#define MODE_LOAD_MR		3
#define MODE_AUTOREFRESH	4
#define MODE_EXT_LOAD_MR	5
#define MODE_POWER_DOWN		6

static void sdram_delay(int us)
{
	volatile int i, loop_limit;

	loop_limit = us * 100;
	for (i=0; i<loop_limit;i++)
		asm volatile("nop");
}

int sdram_init(const sdram_info *info)
{
	volatile avr32_sdramc_t *sdram = &AVR32_SDRAMC;
	volatile avr32_hmatrix_t *hmatrix = &AVR32_HMATRIX;
	volatile avr32_pio_t *pio = &AVR32_PIOE;

	/* Enable SDRAM mode for CS1 in the BAMBI mux */
	hmatrix->sfr[AVR32_SDRAMC_HMATRIX_NR] |= 0x0002;
	hmatrix->sfr[AVR32_SDRAMC_HMATRIX_NR] |= 0x0100;

	/* Setup SDRAM info */
	sdram->cr = ( (info->cols-8) << AVR32_SDRAMC_CR_NC ) |
		( (info->rows-11) << AVR32_SDRAMC_CR_NR ) |
		( (info->banks-1) << AVR32_SDRAMC_CR_NB ) |
		( info->cas << AVR32_SDRAMC_CR_CAS ) |
		( info->twr << AVR32_SDRAMC_CR_TWR ) |
		( info->trc << AVR32_SDRAMC_CR_TRC ) |
		( info->trp << AVR32_SDRAMC_CR_TRP ) |
		( info->trcd << AVR32_SDRAMC_CR_TRCD ) |
		( info->tras << AVR32_SDRAMC_CR_TRAS ) |
		( info->txsr << AVR32_SDRAMC_CR_TXSR );

	if(info->bus_width == 16 ){
		sdram->cr = 1<<AVR32_SDRAMC_CR_DBW_OFFSET;
	} else if ( info->bus_width == 32 ) {
		// Initialize data bus bits 31-16 (muxed with the LCDC).
		pio->asr |= 0x0000FFFF;
		pio->pdr |= 0x0000FFFF;
	}

	sdram_delay(400);

	return SUCCESS;
}
