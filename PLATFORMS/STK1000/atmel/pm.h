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

#ifndef _AVR32_PM_H_
#define _AVR32_PM_H_

#include <avr32/io.h>
#include "errno.h"
#include "at32stk1000.h"

#define PM_TIMEOUT 500000

#define PM_OSC0_HZ OSC0_FREQ
#define PM_OSC1_HZ OSC1_FREQ

#define PM_OSC0 0
#define PM_OSC1 1

#define PM_PLL0 2
#define PM_PLL1 3

#define PM_PBB_CLOCK		10
#define PM_PBA_CLOCK		11
#define PM_HSB_CLOCK		12
#define PM_CPU_CLOCK		13

#define PM_INT_SOURCES		7

/* CPUMASK */
#define PM_CPU_DOMAIN 0
#define PM_CPU_DOMAIN_SIZE 1
#define PM_CPU_PICO 0

/*HSB*/
#define PM_HSB_DOMAIN 1
#define PM_HSB_DOMAIN_SIZE 11
#define PM_HSB_HEBI 32
#define PM_HSB_PBA 33
#define PM_HSB_PBB 34
#define PM_HSB_SDRAMC 35
#define PM_HSB_HSBBRIDGE 36
#define PM_HSB_HISI 37
#define PM_HSB_USB 38
#define PM_HSB_LCDC 39
#define PM_HSB_MACB0 40
#define PM_HSB_MACB1 41
#define PM_HSB_DMAC 42

/*PBA*/
#define PM_PBA_DOMAIN 2
#define PM_PBA_DOMAIN_SIZE 17
#define PM_PBA_SPI0 64
#define PM_PBA_SPI1 65
#define PM_PBA_TWI 66
#define PM_PBA_USART0 67
#define PM_PBA_USART1 68
#define PM_PBA_USART2 69
#define PM_PBA_USART3 70
#define PM_PBA_SSC0 71
#define PM_PBA_SSC1 72
#define PM_PBA_SSC2 73
#define PM_PBA_PIOA 74
#define PM_PBA_PIOB 75
#define PM_PBA_PIOC 76
#define PM_PBA_PIOD 77
#define PM_PBA_PIOE 78
#define PM_PBA_PIOF 80
#define PM_PBA_HPDC 81

/* PBB */
#define PM_PBB_DOMAIN 3
#define PM_PBB_DOMAIN_SIZE 15
#define PM_PBB_SM 96
#define PM_PBB_INTC 97
#define PM_PBB_HMATRIX 98
#define PM_PBB_TIMER0 99
#define PM_PBB_TIMER1 100
#define PM_PBB_PWM 101
#define PM_PBB_MACB0 102
#define PM_PBB_MACB1 103
#define PM_PBB_DAC 104
#define PM_PBB_MMCI 105
#define PM_PBB_AUDIOC 106
#define PM_PBB_HISI 107
#define PM_PBB_USB 108
#define PM_PBB_HSMC 109
#define PM_PBB_HSDRAMC 110
#define PM_PBB_HECC 111

struct pll_opt_t{
	unsigned int pll;
	unsigned int multiplier;
	unsigned int divider;
	unsigned int oscillator;
};

struct clk_sel_opt_t{
	unsigned int clock;
	unsigned int divider;
	unsigned int div_enable;
};

struct gen_clk_opt_t{
	unsigned int div;
	unsigned int diven;
	unsigned int cen;
	unsigned int pllsel;
	unsigned int oscsel;
};

/* reset all clocks and set mclk to OSC0 */
int pm_reset( void );

/* wait for lock bit to settle */
int pm_wait_for_lock(unsigned int lockbit);



/* Modify a specific pll */
int pm_set_pll( volatile struct pll_opt_t *opt );

/* Read a specific pll */
int pm_read_pll_frequency( unsigned int pll );

/*Read a specific oscillator value*/
int pm_read_osc( unsigned int osc );



/* Set the source for the main clock */
int pm_set_mclk_source(unsigned int source);

/* Read the main clock src */
int pm_read_mclk_source( void );

/* Read the main clock frequency*/
int pm_read_mclk( void );



int pm_read_clock_domain_scaler(unsigned int clock_domain);

/* Select clocks for different domains of the system */
int pm_set_clock_domain_scaler(volatile struct clk_sel_opt_t *opt);

/* Return the value of the scaling register */
int pm_read_scaling_register(void);

/* Read the actual clock frequency a specific module is running at*/
int pm_read_module_frequency(int module);

/* Unmask a single module clock part of a clock domain */
int pm_unmask_module_clock( unsigned int clock);

/* Mask a single module clock part of a clock domain */
int pm_mask_module_clock( unsigned int clock);

/* Unmask all module clocks */
void pm_unmask_all_module_clocks( void );

/* Mask all module clocks */
void pm_mask_all_module_clocks( void );



/* Enable interrupt */
int pm_interrupt_enable( unsigned int source );

/* Disable interrupt*/
int pm_interrupt_disable( unsigned int source );

/* Status of interrupt*/
int pm_interrupt_status( unsigned int source );

/* Clear interrupt */
int pm_interrupt_clear( unsigned int source );

/* Clear interrupt */
int pm_interrupt_mask( unsigned int source );



/* Modify the generic clock */
int pm_generic_clock_control( struct gen_clk_opt_t *opt, unsigned char genclk);


#endif /* _AVR32_SYSTEM_MANAGER_H_ */
