/* ***********************************************************************

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
#include "mt481c2m32b2tg.h"

void mt481c2m32b2tg_init(const sdram_info *info)
{
	volatile unsigned long *sdram = (unsigned long *) info->physical_address;
	unsigned long dummy_read;
	unsigned int i;

	volatile avr32_sdramc_t *hsdramc = &AVR32_SDRAMC;

	// Precharge All command is issued to the SDRAM
	hsdramc->mr = MODE_PRECHARGE;
	dummy_read = hsdramc->mr;
	sdram[0] = 0;

	// Provide eight auto-refresh (CBR) cycles
	hsdramc->mr = MODE_AUTOREFRESH;
	dummy_read = hsdramc->mr;
	for (i = 0; i < 8; i++)
	  {
		sdram[0] = 0;
	  }

	// A mode register set (MRS) cycle is issued to program
	// SDRAM parameters, in particular CAS latency and burst
	// length.
	
	// CAS from info struct, burst length 1, serial burst type
	hsdramc->mr = MODE_LOAD_MR;
	dummy_read = hsdramc->mr;
	sdram[0x020] = 0;

	// A Normal Mode command is provided, 3 clocks after tMRD is met.
	dummy_read = hsdramc->mr;
	hsdramc->mr = MODE_NORMAL;
	dummy_read = hsdramc->mr;
	sdram[0] = 0;

	// Write refresh rate into SDRAMC refresh timer count register
	hsdramc->tr =  mt481c2m32b2tg_tr;
}
