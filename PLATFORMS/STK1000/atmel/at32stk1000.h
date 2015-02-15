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
#ifndef _at32stk1000_h_
#define	_at32stk1000_h_

#include <avr32/io.h>
#include "sdram.h"
#include "usart.h"
#include "pm.h"
#include "pio.h"
#include "errno.h"
#include "utils.h"

#define CPU_FAMILY	"AVR32_AP"
#define CPU_TYPE	"AT32AP7000"
#define	MAX_FREQ	150000000

#define DEFAULT_FREQ	OSC0_FREQ
#define DEFAULT_UART 	UART_A

/* Define the on-board oscillators in Hz */
#define OSC0_FREQ	20000000
#define OSC1_FREQ	12000000

/* Define the on-board memory sizes */
#define SDRAM_SIZE	8

/* Define the peripheral names */
#define UART_A	AVR32_USART1
#define UART_C	AVR32_USART3
#define IRDA	AVR32_USART2


static const struct sdram_info sdram = {
        .physical_address      = AVR32_EBI1_ADDRESS,	// 0x10000000
		.bus_width	= 32,
        .rows           = 11,
        .cols           = 8,
        .banks          = 2,
        .cas            = 3,
        .twr            = 2,
        .trc            = 7,
        .trp            = 2,
        .trcd           = 2,
        .tras           = 5,
        .txsr           = 5,
};


int board_init( void );

#endif

