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
#include "at32stk1000.h"

void usart_print(volatile struct avr32_usart_t * usart, char *str)
{
	while (*str != '\0')
		usart_putchar(usart, *str++);
}


void usart_printHex(volatile avr32_usart_t * usart, const unsigned long n)
{
	char tmp[9];
	int i;

	for (i = 0; i < 8; i++) {
		unsigned long nibble;

		nibble = (n >> (28 - 4 * i)) & 0x0F;

		if (nibble < 10) {
			tmp[i] = nibble + '0';
		} else {
			tmp[i] = nibble - 10 + 'a';
		}
	}

	tmp[8] = 0;
	usart_print(usart, tmp);
}

static int mt481c2m32b2tg_init(const sdram_info *info)
{
	volatile unsigned long *sdram = (unsigned long *) info->physical_address;
	volatile avr32_sdramc_t *sdramc = &AVR32_SDRAMC;
	unsigned long dummy_read;
	unsigned long busHz;
	unsigned int i;

	if ( sdram_init(info) )
		return INVALID_ARGUMENT;

	/* Precharge All command is issued to the SDRAM */
	sdramc->mr = MODE_PRECHARGE;
	dummy_read = sdramc->mr;
	sdram[0] = 0;

	/* Provide eight auto-refresh (CBR) cycles */
	sdramc->mr = MODE_AUTOREFRESH;
	dummy_read = sdramc->mr;
	for (i = 0; i < 8; i++)
	  {
		sdram[0] = 0;
	  }

	/* A mode register set (MRS) cycle is issued to program
	* SDRAM parameters, in particular CAS latency and burst
	* length.
	*/
	
	/* CAS from info struct, burst length 1, serial burst type */
	sdramc->mr = MODE_LOAD_MR;
	dummy_read = sdramc->mr;
	sdram[0x020] = 0;

	/* A Normal Mode command is provided, 3 clocks after tMRD is met. */
	dummy_read = sdramc->mr;
	sdramc->mr = MODE_NORMAL;
	dummy_read = sdramc->mr;
	sdram[0] = 0;
	
	/* Write refresh rate into SDRAMC refresh timer count register */
	busHz = pm_read_module_frequency(PM_HSB_SDRAMC);
	sdramc->tr = ( ( 156 * (busHz / 1000) ) / 10000 );

	return SUCCESS;
}

static int init_uart_a(void)
{ 
	struct usart_options_t opt;
	volatile struct avr32_usart_t *usart = &AVR32_USART1;

	avr32_piomap_t usart_piomap = {				   \
		{AVR32_USART1_RXD_0_PIN, AVR32_USART1_RXD_0_FUNCTION}, \
		{AVR32_USART1_TXD_0_PIN, AVR32_USART1_TXD_0_FUNCTION}   \
	};

	opt.baudrate = 115200;
	opt.charlength = 8;
	opt.paritytype = USART_NO_PARITY;
	opt.stopbits = USART_1_STOPBIT;
	opt.channelmode = USART_NORMAL_CHMODE;

	/* Initialize it in RS232 mode */
	if(usart_init_rs232(usart, &opt, pm_read_module_frequency(PM_PBA_USART1) ) )
		return INVALID_ARGUMENT;

	/* Setup pio for USART */
	pio_enable_module(usart_piomap, 2);

	return SUCCESS;
}

int board_init( void )
{
	volatile struct avr32_usart_t *usart = &AVR32_USART1;
	volatile struct pll_opt_t pll_opt;

	/* Initialize USART */
	if(init_uart_a())
		return INVALID_ARGUMENT;
	usart_print(usart, "STK1000 initialization:\n");
	usart_print(usart, " -> USART ... OK\n");


	/* Setup PLL at 40 MHz */
	usart_print(usart, " -> PLL0 ... ");
	pll_opt.oscillator=PM_OSC0;
	pll_opt.pll=PM_PLL0;
	pll_opt.multiplier=2;
	pll_opt.divider=1;
	pm_set_pll(&pll_opt);

	pm_set_mclk_source(PM_PLL0);

	if(init_uart_a()) /* reinitialize due to clock change */
		return INVALID_ARGUMENT;
	usart_print(usart, "OK\n");
	
	/* Initialize SDRAMC */
	usart_print(usart, " -> SDRAM ... ");
	if (mt481c2m32b2tg_init(&sdram) )
		return ERROR;
	usart_print(usart, "OK\n");


	/* Done. Print frequency and return */
	usart_print(usart, "STK1000 is successfully initialized at 0x");
	usart_printHex(usart, pm_read_mclk()/1000000 );
	usart_print(usart, " MHz\n");

	return 0;
}

