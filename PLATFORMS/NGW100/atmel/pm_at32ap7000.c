/* This source file is part of the ATMEL AVR32-SoftwareFramework-AT32AP7000-1.0.0 Release */

/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Power Manager driver.
 *
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 *****************************************************************************/

/* Copyright (C) 2006-2008, Atmel Corporation All rights reserved.
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

#include "pm_at32ap7000.h"
#include "atngw100.h"
#include "ap7_errno.h"
#include "compiler.h"

int pm_reset(void)
{
  volatile avr32_pm_t *pm = &AVR32_PM;
  unsigned int status = 0;
  int disable_module_mask = 0xFFFFffff;

  pm_set_mclk_source(PM_OSC0);

  pm->cksel = 0x00000000;
  status |= pm_wait_for_status(AVR32_PM_ISR_CKRDY);

  pm->cpumask = disable_module_mask;
  status |= pm_wait_for_status(AVR32_PM_ISR_MSKRDY);

  pm->hsbmask = disable_module_mask;
  status |= pm_wait_for_status(AVR32_PM_ISR_MSKRDY);

  pm->pbamask = disable_module_mask;
  status |= pm_wait_for_status(AVR32_PM_ISR_MSKRDY);

  pm->pbbmask = disable_module_mask;
  status |= pm_wait_for_status(AVR32_PM_ISR_MSKRDY);

  pm->idr = 0x7f; /* disable all interrupts*/
  pm->icr = 0x7f; /* clear all interrupts */

  return status;
}


unsigned int pm_get_reset_cause(void)
{
  volatile avr32_pm_t *pm = &AVR32_PM;

  return pm->rcause;
}


int pm_start_osc(const pm_osc_opt_t *opt)
{
  return PM_NOT_SUPPORTED;
}


int pm_stop_osc(int osc_id)
{
  return PM_NOT_SUPPORTED;
}


int pm_start_pll(pm_pll_opt_t *opt)
{
  volatile avr32_pm_t *pm = &AVR32_PM;
  unsigned int pllreg;
  int retval;

  /* Ensure that mclk is not driven by PLL */
  if ((opt->pll_id == 0) && (pm_get_mclk_source() == PM_PLL0))
    pm_set_mclk_source(PM_OSC0);

  if ((opt->mul == 0) | (opt->div == 0) | (opt->osc_id > 1))
    return INVALID_ARGUMENT;

  pllreg = opt->count << AVR32_PM_PLLCOUNT_OFFSET
           | (opt->mul - 1) << AVR32_PM_PLLMUL_OFFSET
           | (opt->div - 1) << AVR32_PM_PLLDIV_OFFSET
           | 4 << AVR32_PM_PLLOPT_OFFSET
           | opt->osc_id << AVR32_PM_PLLOSC_OFFSET
           | 1 << AVR32_PM_PLLEN_OFFSET;
  
  if (opt->pll_id == 0)
    pm->pll0 = pllreg;
  else if (opt->pll_id == 1)
    pm->pll1 = pllreg;
  else
    return INVALID_ARGUMENT;

  if (opt->wait_for_lock) {
    retval = pm_wait_for_clock_source_lock(opt->pll_id == 0 ? PM_PLL0 : PM_PLL1);
    if (retval != 0)
      return retval;
  }

  return pm_get_clock_source_freq_hz(opt->pll_id == 0 ? PM_PLL0 : PM_PLL1);
}


int pm_start_pll_freq(int pll, unsigned int freq, Bool wait_for_lock)
{
  return PM_NOT_SUPPORTED;
}


int pm_stop_pll(int pll)
{
  return PM_NOT_SUPPORTED;
}


int pm_wait_for_clock_source_lock(int source)
{
  unsigned int status;

  switch (source) {
    case PM_OSC0:
    case PM_OSC1:
      return 0;
    case PM_PLL0:
      status = 1 << AVR32_PM_ISR_LOCK0;
      break;
    case PM_PLL1:
      status = 1 << AVR32_PM_ISR_LOCK1;
      break;
    default:
      return INVALID_ARGUMENT;
  }

  return pm_wait_for_status(status);
}


int pm_get_clock_source_freq_hz(int clock_source)
{
  volatile avr32_pm_t *pm = &AVR32_PM;
  unsigned int mul, div, osc;

  switch (clock_source) {
    case PM_OSC0:
      return FOSC0;
    case PM_OSC1:
      return FOSC1;
    case PM_PLL0:
      mul = ((pm->pll0 & AVR32_PM_PLL0_PLLMUL_MASK) >> AVR32_PM_PLL0_PLLMUL_OFFSET) + 1;
      div = ((pm->pll0 & AVR32_PM_PLL0_PLLDIV_MASK) >> AVR32_PM_PLL0_PLLDIV_OFFSET) + 1;
      if((pm->pll0 & AVR32_PM_PLL0_PLLOSC_MASK) >> AVR32_PM_PLL0_PLLOSC_OFFSET) /* Oscillator #1 */
        osc = FOSC1;
      else
        osc = FOSC0;
      break;
    case PM_PLL1:
      mul = ((pm->pll1 & AVR32_PM_PLL1_PLLMUL_MASK) >> AVR32_PM_PLL1_PLLMUL_OFFSET) + 1;
      div = ((pm->pll1 & AVR32_PM_PLL1_PLLDIV_MASK) >> AVR32_PM_PLL1_PLLDIV_OFFSET) + 1;
      if((pm->pll1 & AVR32_PM_PLL1_PLLOSC_MASK) >> AVR32_PM_PLL1_PLLOSC_OFFSET) /* Oscillator #1 */
        osc = FOSC1;
      else
        osc = FOSC0;
      break;
    default:
      return INVALID_ARGUMENT;
  }

  return (mul * osc) / div;
}


int pm_wait_for_status(unsigned int status)
{
  unsigned int timeout = PM_TIMEOUT;

  while ((pm_get_interrupt_status() & status) != status) {
    timeout--;
    if (timeout==0)
      return -1;
  }

  pm_clear_interrupt_status(status);

  return 0;
}


int pm_enable_interrupt(unsigned int mask)
{
  volatile avr32_pm_t *pm = &AVR32_PM;

  pm->ier = mask;

  return pm->ier;
}


int pm_disable_interrupt(unsigned int mask)
{
  volatile avr32_pm_t *pm = &AVR32_PM;

  pm->idr = mask;

  return pm->idr;
}


unsigned int pm_get_enabled_interrupts(void)
{
  volatile avr32_pm_t *pm = &AVR32_PM;

  return pm->imr;
}


unsigned int pm_get_interrupt_status(void)
{
  volatile avr32_pm_t *pm = &AVR32_PM;
  return pm->isr;
}


int pm_clear_interrupt_status(unsigned int mask)
{
  volatile avr32_pm_t *pm = &AVR32_PM;
  
  pm->icr=mask;

  return ~(pm->isr & mask);	
}


int pm_calibrate(int source, unsigned int calibration_value)
{
  return PM_NOT_SUPPORTED;
}


int pm_set_bod_settings(unsigned int bod_control, Bool hysteresis, unsigned char level)
{
  return PM_NOT_SUPPORTED;
}


int pm_get_bod_settings(unsigned int bod_control, Bool hysteresis, unsigned char level)
{
  return PM_NOT_SUPPORTED;
}


int pm_set_mclk_source(int clock_source)
{
  volatile avr32_pm_t *pm = &AVR32_PM;

  if ((clock_source == PM_OSC0))
    pm->mcctrl = (0 << AVR32_PM_PLLSEL_OFFSET);
  else if (clock_source == PM_PLL0)
    pm->mcctrl = (1 << AVR32_PM_PLLSEL_OFFSET);
  else
    return INVALID_ARGUMENT;

  return pm_wait_for_clock_source_lock(clock_source);
}


int pm_get_mclk_source(void)
{
  unsigned int source;
  volatile avr32_pm_t *pm = &AVR32_PM;

  source = pm->mcctrl & AVR32_PM_PLLSEL_MASK;

  if(source==0)
    return PM_OSC0;
  else if(source==AVR32_PM_PLLSEL_MASK)
    return PM_PLL0;
  else
    return INVALID_ARGUMENT;
}


int pm_get_mclk_freq_hz(void)
{
  unsigned int f_hz;
  int source;

  /* Get mclk module source */
  source = pm_get_mclk_source();

  /* check wheter oscillator 0 is used */
  if( source == PM_OSC0 )
    f_hz = FOSC0;
  /* check wheter pll 0 is used */
  else if ( source == PM_PLL0 )
    f_hz = pm_get_clock_source_freq_hz(PM_PLL0);
  else
    return INVALID_ARGUMENT;

  return f_hz;
}


int pm_read_module_domain_scaler(unsigned int module_domain)
{
  volatile avr32_pm_t *pm = &AVR32_PM;
  unsigned int divider, offset, cksel;

  /* Get offset for divider */
  if(module_domain == PM_PBB_DOMAIN)
    offset = AVR32_PM_CKSEL_PBBSEL_OFFSET;
  else if(module_domain == PM_PBA_DOMAIN)
    offset = AVR32_PM_CKSEL_PBASEL_OFFSET;
  else if(module_domain == PM_HSB_DOMAIN)
    offset = AVR32_PM_CKSEL_HSBSEL_OFFSET;
  else if(module_domain == PM_CPU_DOMAIN)
    offset = AVR32_PM_CKSEL_CPUSEL_OFFSET;
  else
    return INVALID_ARGUMENT;

  cksel = pm->cksel >> offset;
  // Check if prescaler is enabled
  if (cksel & AVR32_PM_CKSEL_CPUDIV_MASK)
    divider = 1 << ((cksel & AVR32_PM_CKSEL_CPUSEL_MASK) + 1);
  else
    divider = 1;

  return divider;
}


int pm_set_clock_domain_scaler(unsigned int clock_domain, int divider)
{
  volatile avr32_pm_t *pm = &AVR32_PM;
  unsigned int offset;
  int divsel;

  // Count trailing zeros
  divsel = ctz(divider);

  if ((divsel > 8) || ((1 << divsel) != divider))
    return INVALID_ARGUMENT;

  if (divsel > 0)
    divsel = (divsel - 1) | AVR32_PM_CKSEL_CPUDIV_MASK;

  // Get offset for bitfields
  switch (clock_domain){
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
  } /* end switch */

  pm->cksel = (pm->cksel & ~((AVR32_PM_CKSEL_CPUDIV_MASK | AVR32_PM_CKSEL_CPUSEL_MASK) << offset))
              | (divsel << offset);

  pm_wait_for_status(AVR32_PM_ISR_CKRDY);

  return 0;
} /* End pm_set_clock_domain_scaler() */


int pm_read_module_freq_hz(unsigned int module)
{
  unsigned int clk, domain;
  int divider;

  clk = pm_get_mclk_freq_hz();

  domain = module / 32;
  divider = pm_read_module_domain_scaler(domain);
  if (divider < 0)
    return divider;

  /* CHECK WHETHER THE CLOCK IS MASKED */

  return clk / divider;
}


int pm_enable_module(unsigned int module)
{
  volatile avr32_pm_t *pm = &AVR32_PM;

  switch (module % 32){
    case PM_CPU_DOMAIN:
      /* Check whether the module is out of bounds */
      if( (module/32) > PM_CPU_DOMAIN_SIZE )
        return INVALID_ARGUMENT;
      else{
        pm->cpumask |= ( 1<<(module/32) );
        break;
      }
    case PM_HSB_DOMAIN:
      /* Check whether the module is out of bounds */
      if( (module/32) > PM_HSB_DOMAIN_SIZE )
        return INVALID_ARGUMENT;
      else{
        pm->hsbmask |= ( 1<<(module/32) );
        break;
      }
    case PM_PBA_DOMAIN:
      /* Check whether the module is out of bounds */
      if( (module/32) > PM_PBA_DOMAIN_SIZE )
        return INVALID_ARGUMENT;
      else{
        pm->pbamask |= ( 1<<(module/32) );
        break;
      }
    case PM_PBB_DOMAIN:
      /* Check whether the module is out of bounds */
      if( (module/32) > PM_PBB_DOMAIN_SIZE )
        return INVALID_ARGUMENT;
      else{
        pm->pbbmask |= ( 1<<(module/32) );
        break;
      }
    default:
      return INVALID_ARGUMENT;
  }
  return 0;
}


int pm_disable_module(unsigned int module)
{
  volatile avr32_pm_t *pm = &AVR32_PM;

  switch (module % 32){
    case PM_CPU_DOMAIN:
      /* Check whether the module is out of bounds */
      if( (module/32) > PM_CPU_DOMAIN_SIZE )
        return INVALID_ARGUMENT;
      else{
        pm->cpumask &= ~( 1<<(module/32) );
        break;
      }
    case PM_HSB_DOMAIN:
      /* Check whether the module is out of bounds */
      if( (module/32) > PM_HSB_DOMAIN_SIZE )
        return INVALID_ARGUMENT;
      else{
        pm->hsbmask &= ~( 1<<(module/32) );
        break;
      }
    case PM_PBA_DOMAIN:
      /* Check whether the module is out of bounds */
      if( (module/32) > PM_PBA_DOMAIN_SIZE )
        return INVALID_ARGUMENT;
      else{
        pm->pbamask &= ~( 1<<(module/32) );
        break;
      }
    case PM_PBB_DOMAIN:
      /* Check whether the module is out of bounds */
      if( (module/32) > PM_PBB_DOMAIN_SIZE )
        return INVALID_ARGUMENT;
      else{
        pm->pbbmask &= ~( 1<<(module/32) );
        break;
      }
    default:
      return INVALID_ARGUMENT;
  }
  return 0;
}


int pm_start_generic_clock(unsigned int clock, const pm_gen_clk_opt_t *opt)
{
  volatile avr32_pm_t *pm = &AVR32_PM;
  unsigned int reg = 1 << AVR32_PM_GCCTRL0_CEN_OFFSET;

  if (opt->divider > 1)
    reg |= (((opt->divider >> 1) - 1) << AVR32_PM_GCCTRL0_DIV_OFFSET) | (1 << AVR32_PM_GCCTRL0_DIVEN_OFFSET);

  if ( (opt->clock_source == PM_OSC0) || (opt->clock_source == PM_OSC1) )
    reg |= opt->clock_source << AVR32_PM_GCCTRL0_OSCSEL_OFFSET;
  else if ( (opt->clock_source == PM_PLL0) || (opt->clock_source == PM_PLL1) )
    reg |= ((opt->clock_source-3) << AVR32_PM_GCCTRL0_OSCSEL_OFFSET) | (1 << AVR32_PM_GCCTRL0_PLLSEL_OFFSET);
  else
    return INVALID_ARGUMENT;

  pm->gcctrl[clock] = reg;

  return 0;
}


int pm_stop_generic_clock(unsigned int clock)
{
  volatile avr32_pm_t *pm = &AVR32_PM;

  pm->gcctrl[clock] &= ~(1 << AVR32_PM_GCCTRL0_CEN_OFFSET);

  return 0;
}

