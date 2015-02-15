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

#include "pm.h"

/* reset all clocks and set mclk to osc0 */
int pm_reset( void )
{
	avr32_pm_t *pm = (void *) AVR32_PM_ADDRESS;
	unsigned int status = SUCCESS;
	int disable_clockmask = 0xFFFFffff;

	pm_set_mclk_source(PM_OSC0);

	pm->cksel = 0x00000000;
	status |= pm_wait_for_lock(AVR32_PM_ISR_CKRDY);

	pm->cpumask = disable_clockmask;
	status |= pm_wait_for_lock(AVR32_PM_ISR_MSKRDY);

	pm->hsbmask = disable_clockmask;
	status |= pm_wait_for_lock(AVR32_PM_ISR_MSKRDY);

	pm->pbamask = disable_clockmask;
	status |= pm_wait_for_lock(AVR32_PM_ISR_MSKRDY);

	pm->pbbmask = disable_clockmask;
	status |= pm_wait_for_lock(AVR32_PM_ISR_MSKRDY);

	pm->ier = 0x7F; /* enable all interrupts*/
	pm->icr |= 0x7F; /* clear all interrupts */

	return status;
}

int pm_wait_for_lock(unsigned int lockbit)
{
	unsigned int timeout = PM_TIMEOUT;
	unsigned int lockStatus=0;

  	while (!lockStatus){
  		lockStatus =  pm_interrupt_status(lockbit);
		--timeout;
		if (timeout==0)
			return PM_LOCK_ERROR;
  	}

	pm_interrupt_clear(lockbit);

	return SUCCESS;
}

/* Modify a specific pll */
int pm_set_pll(volatile struct pll_opt_t *opt)
{
	volatile avr32_pm_t *pm = (void *) AVR32_PM_ADDRESS;
	unsigned int pllreg, lockbit;

	/* Ensure that mclk is driven by the current pll */
	if(pm_read_mclk_source()==PM_PLL0)
		pm_set_mclk_source(PM_OSC0);

	if ( (opt->multiplier == 0) | (opt->divider == 0) )
		return INVALID_ARGUMENT;

	pllreg = (1<<AVR32_PM_PLL0_PLLCOUNT_OFFSET)|(1<<AVR32_PM_PLL0_PLLEN_OFFSET);

  	// add multiplier
  	if (opt->multiplier != 0)
    	pllreg = pllreg | (((opt->multiplier) -1 ) << AVR32_PM_PLLMUL_OFFSET);
  	// add divider
  	if(opt->divider != 0)
    	pllreg = pllreg | (((opt->divider) - 1) << AVR32_PM_PLLDIV_OFFSET);
	// select oscillator
  	if(opt->oscillator <= 1)
    	pllreg = pllreg | ((opt->oscillator) << AVR32_PM_PLLOSC_OFFSET);


	/* CHECK FOR VALID ARGUMENTS*/
	if( opt->pll == PM_PLL0 )
		pm->pll0 = pllreg;
	else if ( opt->pll == PM_PLL1 )
		pm->pll1 = pllreg;
	else
		return INVALID_ARGUMENT;

	// Get lock bit
	if(opt->pll == PM_PLL0){
		lockbit =  AVR32_PM_ISR_LOCK0;
	}
	else{
	  	lockbit =  AVR32_PM_ISR_LOCK1;
	}

	return pm_wait_for_lock(lockbit);
}

int pm_read_pll_frequency( unsigned int pll )
{
	volatile avr32_pm_t *pm = (void *) AVR32_PM_ADDRESS;
	unsigned int mul,div,osc;

	if ( pll==PM_PLL0 ){
		mul = ( (pm->pll0&AVR32_PM_PLL0_PLLMUL_MASK)>>AVR32_PM_PLL0_PLLMUL_OFFSET)+1;
		div = ( (pm->pll0&AVR32_PM_PLL0_PLLDIV_MASK)>>AVR32_PM_PLL0_PLLDIV_OFFSET)+1;
		if((pm->pll0&AVR32_PM_PLL0_PLLOSC_MASK)>>AVR32_PM_PLL0_PLLOSC_OFFSET) /* Oscillator #1 */
			osc = PM_OSC1_HZ;
		else
			osc = PM_OSC0_HZ;
	}
	else if ( pll==PM_PLL1 ){
		mul = ( (pm->pll1&AVR32_PM_PLL1_PLLMUL_MASK)>>AVR32_PM_PLL1_PLLMUL_OFFSET)+1;
		div = ( (pm->pll1&AVR32_PM_PLL1_PLLDIV_MASK)>>AVR32_PM_PLL1_PLLDIV_OFFSET)+1;
		if((pm->pll1&AVR32_PM_PLL1_PLLOSC_MASK)>>AVR32_PM_PLL1_PLLOSC_OFFSET) /* Oscillator #1 */
			osc = PM_OSC1_HZ;
		else
			osc = PM_OSC0_HZ;
	}
	else
		return INVALID_ARGUMENT;

	return (mul*osc)/div;
}


/*Read a specific oscillator value*/
int pm_read_osc( unsigned int osc )
{
	if(osc == 0)
		return PM_OSC0_HZ;
	else if(osc == 1)
		return PM_OSC1_HZ;
	else
		return INVALID_ARGUMENT;
}

/* Set the source for the main clock */
int pm_set_mclk_source(unsigned int source)
{
	volatile avr32_pm_t *pm = (void *) AVR32_PM_ADDRESS;

	if( (source == PM_OSC0) ){
		pm->mcctrl = (0 << AVR32_PM_PLLSEL_OFFSET);
	}
	else if( source ==PM_PLL0 ){
		pm->mcctrl = (1 << AVR32_PM_PLLSEL_OFFSET);
	}
	else
		return INVALID_ARGUMENT;

	return pm_wait_for_lock(AVR32_PM_ISR_CKRDY);

}



/* Read the main clock src */
int pm_read_mclk_source( void )
{
	int source;
	avr32_pm_t *pm = (void *) AVR32_PM_ADDRESS;

	source = (pm->mcctrl)&AVR32_PM_PLLSEL_MASK;

	if(source==0)
		return PM_OSC0;
	else if(source==AVR32_PM_PLLSEL_MASK)
		return PM_PLL0;
	else
		return INVALID_ARGUMENT;
}



/* Read the main clock frequency*/
int pm_read_mclk( void )
{
	unsigned int f_hz, source;

	/* Get mclk clock source */
	source = pm_read_mclk_source();

	/* check wheter oscillator 0 is used */
	if( source == PM_OSC0 )
		f_hz = PM_OSC0_HZ;
	/* check wheter pll 0 is used */
	else if ( source == PM_PLL0 )
		f_hz = pm_read_pll_frequency(PM_PLL0);
	else
		return INVALID_ARGUMENT;

	return f_hz;
} /* End pm_read_mclk() */



/* Get the clock frequency for the given clock domain */
int pm_read_clock_domain_scaler(unsigned int clock_domain)
{
	static avr32_pm_t *pm = (void *) AVR32_PM_ADDRESS;
	int divider, offset;

	/* Get offset for divider */
	if(clock_domain == PM_PBB_DOMAIN)
		offset = AVR32_PM_CKSEL_PBBSEL_OFFSET;
	else if(clock_domain == PM_PBA_DOMAIN)
		offset = AVR32_PM_CKSEL_PBASEL_OFFSET;
	else if(clock_domain == PM_HSB_DOMAIN)
		offset = AVR32_PM_CKSEL_HSBSEL_OFFSET;
	else if(clock_domain == PM_CPU_DOMAIN)
		offset = AVR32_PM_CKSEL_CPUSEL_OFFSET;
	else
		return INVALID_ARGUMENT;


	divider = pm->cksel & (AVR32_PM_CKSEL_CPUSEL_MASK<<offset);
	divider = divider >> offset;

	return (divider+1);

} /* End pm_read_clock_domain_scaler() */



/* Select clocks for different domains of the system */
int pm_set_clock_domain_scaler(volatile struct clk_sel_opt_t *opt)
{
	volatile avr32_pm_t *pm = (void *) AVR32_PM_ADDRESS;
	int offset,new_settings;

	// Check for invalid division factor
	if( (opt->div_enable == 1) & ((opt->divider == 0)|(opt->divider >= (1<<AVR32_PM_CKSEL_CPUSEL_SIZE))) )
		return INVALID_ARGUMENT;

	// Get offset for bitfields
	switch (opt->clock){
		case PM_PBB_DOMAIN:
			offset = AVR32_PM_CKSEL_PBBSEL_OFFSET;
			break;
		case PM_PBA_DOMAIN:
			offset = AVR32_PM_CKSEL_PBASEL_OFFSET;
			break;
		case PM_HSB_DOMAIN:
			offset = AVR32_PM_CKSEL_HSBSEL_OFFSET;
			break;
		case PM_CPU_DOMAIN:
			offset = AVR32_PM_CKSEL_CPUSEL_OFFSET;
			break;
		default:
			return INVALID_ARGUMENT;
			break;
	} /* end switch */

	// Check whether division should be disabled
	if(opt->div_enable == 0){
		new_settings = 0x00;
	}

	// Division is to be used
	else if (opt->div_enable == 1) {
		new_settings = 1<<AVR32_PM_CKSEL_CPUDIV;
		new_settings |= ((opt->divider)-1);
	}

	// Error. div_enable >1
	else
		return INVALID_ARGUMENT;

	pm->cksel &=  ~((AVR32_PM_CKSEL_CPUDIV_MASK|AVR32_PM_CKSEL_CPUSEL_MASK)<<offset);
	pm->cksel |= (new_settings<<offset);

	pm_wait_for_lock(AVR32_PM_ISR_CKRDY);

	return SUCCESS;

} /* End pm_set_clock_domain_scaler() */

/* Return the value of the scaling register */
int pm_read_scaling_register(void)
{
	volatile avr32_pm_t *pm = (void *) AVR32_PM_ADDRESS;

	return pm->cksel;
}

/* Start a single module clock part of a clock domain */
int pm_unmask_module_clock( unsigned int clock )
{
	avr32_pm_t *pm = (void *)AVR32_PM_ADDRESS;

	switch (clock % 32){
		case PM_CPU_DOMAIN:
			/* Check whether the clock is out of bounds */
			if( (clock/32) > PM_CPU_DOMAIN_SIZE )
				return INVALID_ARGUMENT;
			else{
				pm->cpumask |= ( 1<<(clock/32) );
				break;
			}
		case PM_HSB_DOMAIN:
			/* Check whether the clock is out of bounds */
			if( (clock/32) > PM_HSB_DOMAIN_SIZE )
				return INVALID_ARGUMENT;
			else{
				pm->hsbmask |= ( 1<<(clock/32) );
				break;
			}
		case PM_PBA_DOMAIN:
			/* Check whether the clock is out of bounds */
			if( (clock/32) > PM_PBA_DOMAIN_SIZE )
				return INVALID_ARGUMENT;
			else{
				pm->pbamask |= ( 1<<(clock/32) );
				break;
			}
		case PM_PBB_DOMAIN:
			/* Check whether the clock is out of bounds */
			if( (clock/32) > PM_PBB_DOMAIN_SIZE )
				return INVALID_ARGUMENT;
			else{
				pm->pbbmask |= ( 1<<(clock/32) );
				break;
			}
		default:
			return INVALID_ARGUMENT;
	}
	return 0;
} /* End pm_start_clock() */



/* Stop a single module clock part of a clock domain */
int pmmask_module_clock( unsigned int clock )
{
	avr32_pm_t *pm = (void *)AVR32_PM_ADDRESS;

	switch (clock % 32){
		case PM_CPU_DOMAIN:
			/* Check whether the clock is out of bounds */
			if( (clock/32) > PM_CPU_DOMAIN_SIZE )
				return INVALID_ARGUMENT;
			else{
				pm->cpumask &= ~( 1<<(clock/32) );
				break;
			}
		case PM_HSB_DOMAIN:
			/* Check whether the clock is out of bounds */
			if( (clock/32) > PM_HSB_DOMAIN_SIZE )
				return INVALID_ARGUMENT;
			else{
				pm->hsbmask &= ~( 1<<(clock/32) );
				break;
			}
		case PM_PBA_DOMAIN:
			/* Check whether the clock is out of bounds */
			if( (clock/32) > PM_PBA_DOMAIN_SIZE )
				return INVALID_ARGUMENT;
			else{
				pm->pbamask &= ~( 1<<(clock/32) );
				break;
			}
		case PM_PBB_DOMAIN:
			/* Check whether the clock is out of bounds */
			if( (clock/32) > PM_PBB_DOMAIN_SIZE )
				return INVALID_ARGUMENT;
			else{
				pm->pbbmask &= ~( 1<<(clock/32) );
				break;
			}
		default:
			return INVALID_ARGUMENT;
	}
	return 0;
} /* End pm_stop_clock */

/* Unmask all module clocks */
void pm_unmask_all_module_clocks ( void )
{
	avr32_pm_t *pm = (void *)AVR32_PM_ADDRESS;

	pm->cpumask = 0x00000000;
	pm->hsbmask = 0x00000000;
	pm->pbamask = 0x00000000;
	pm->pbbmask = 0x00000000;
	pm_wait_for_lock(AVR32_PM_ISR_MSKRDY);
}

/* Mask all module clocks */
void pmmask_all_module_clocks ( void )
{
	avr32_pm_t *pm = (void *)AVR32_PM_ADDRESS;

	pm->cpumask = 0xFFFFffff;
	pm->hsbmask = 0xFFFFffff;
	pm->pbamask = 0xFFFFffff;
	pm->pbbmask = 0xFFFFffff;

	pm_wait_for_lock(AVR32_PM_ISR_MSKRDY);
}

int pm_read_module_frequency(int module)
{
	int clk, domain, divider;

	clk = pm_read_mclk();
	divider = pm_read_clock_domain_scaler(PM_HSB_DOMAIN);
	domain = module/32;
	
	if ( domain == PM_HSB_DOMAIN )
		divider = divider;
	else if( domain==PM_PBA_DOMAIN )
		divider = divider * pm_read_clock_domain_scaler(PM_PBA_DOMAIN);
	else if( domain==PM_PBB_DOMAIN )
		divider = divider * pm_read_clock_domain_scaler(PM_PBB_DOMAIN);
	else
		return INVALID_ARGUMENT;

	/* CHECK WHETHER THE CLOCK IS MASKED */

	return clk/divider;
}

/* Enable interrupt */
int pm_interrupt_enable( unsigned int source )
{
	avr32_pm_t *pm = (void *)AVR32_PM_ADDRESS;

	if( source > (PM_INT_SOURCES-1) )
		return INVALID_ARGUMENT;
	else{
		pm->ier |= (1 << source);
		return 0;
	}
}



/* Disable interrupt*/
int pm_interrupt_disable( unsigned int source )
{
	avr32_pm_t *pm = (void *)AVR32_PM_ADDRESS;

	if( source > (PM_INT_SOURCES-1) )
		return INVALID_ARGUMENT;
	else{
		pm->idr |= (1 << source);
		return 0;
	}
}



/* Status of interrupt*/
int pm_interrupt_status( unsigned int source )
{
	avr32_pm_t *pm = (void *)AVR32_PM_ADDRESS;

	if ( source > (PM_INT_SOURCES-1) )
		return INVALID_ARGUMENT;
	else
		return (pm->isr & source)>>source;
}



/* Clear interrupt */
int pm_interrupt_clear( unsigned int source )
{
	avr32_pm_t *pm = (void *)AVR32_PM_ADDRESS;

	if( source > (PM_INT_SOURCES-1) )
		return INVALID_ARGUMENT;
	else{
		pm->icr |= (1 << source);
		return 0;
	}
}



/* Clear interrupt */
int pm_interruptmask( unsigned int source )
{
	avr32_pm_t *pm = (void *)AVR32_PM_ADDRESS;

	if( source > (PM_INT_SOURCES-1) )
		return INVALID_ARGUMENT;
	else{
		return (pm->ier & source ) >> source ;
	}
}



/* Modify the generic clock */
int pm_generic_clock_control( struct gen_clk_opt_t *opt, unsigned char genclk)
{
	avr32_pm_t *pm = (void *)AVR32_PM_ADDRESS;

	/* Check whether div-field is larger than 8 bits */
	if( opt->div > 0xFF)
		return INVALID_ARGUMENT;
	/* Check for valid single bits */
	if ( ((opt->diven)|(opt->cen)|(opt->pllsel)|(opt->oscsel)|(genclk > 1)) > 1 )
		return INVALID_ARGUMENT;
	else
		pm->gcctrl[genclk] = ( (opt->div << AVR32_PM_GCCTRL0_DIV_OFFSET)| \
			(opt->diven << AVR32_PM_GCCTRL0_DIVEN_OFFSET)|\
			(opt->cen << AVR32_PM_GCCTRL0_CEN_OFFSET)|\
			(opt->pllsel << AVR32_PM_GCCTRL0_PLLSEL_OFFSET)|\
			(opt->oscsel << AVR32_PM_GCCTRL0_OSCSEL_OFFSET) );
	
	return 0;
} /* End pm_generic_clock_control() */

