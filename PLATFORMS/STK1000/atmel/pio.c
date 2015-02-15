/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief pio driver functions
 *
 * This file contains basic pio driver functions.
 *
 *
 * - Compiler:           IAR EWAAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with a PIO module can be used.
 *                       The example is written for AP7000 and STK1000.
 * - AppNote:            AVR32111 - Using the AVR32 PIO Controller
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support email: avr32@atmel.com
 *
 * $Name:  $
 * $Revision: 1.4 $
 * $RCSfile: pio.c,v $
 * $Date: 2006/05/10 12:01:41 $
 *****************************************************************************/

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

#include "pio.h"

volatile avr32_pio_t *pioGetHandle(int port)
{
	switch (port) {
		case 0:
			return &AVR32_PIOA;
		case 1:
			return &AVR32_PIOB;
		case 2:
			return &AVR32_PIOC;
		case 3:
			return &AVR32_PIOD;
		case 4:
			return &AVR32_PIOE;
		default :
			break;
	}
        return (avr32_pio_t *) -1;
}

int pio_setup_pin(int pin, int function)
{
	volatile avr32_pio_t *pio = pioGetHandle(pin/32);


	/* Disable pio control */
	pio->pdr |= (1<<(pin%32));
	pio->pudr |= (1<<(pin%32));

	/* Enable the correct function */
	switch(function){
		case 0:
			pio->asr |= (1<<(pin%32));
			break;
		case 1:
			pio->bsr |= (1<<(pin%32));
			break;
		default:
			return PIO_INVALID_ARGUMENT;
	}
	return PIO_SUCCESS;
}

void pio_enable_module(avr32_piomap_t piomap, int size)
{
	int i;

	for(i=0; i<size; i++){
		pio_setup_pin(**piomap, *(*piomap+1) );
		piomap++;
	}
}
