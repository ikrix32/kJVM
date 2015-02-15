/* This header file is part of the ATMEL AVR32-SoftwareFramework-AT32AP7000-1.0.0 Release */

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

#ifndef _pm_h_
#define _pm_h_

#include "compiler.h"
#include <avr32/io.h>
#include "atngw100.h"
#include "ap7_errno.h"

/* The timeguard used for polling in ticks */
#define PM_TIMEOUT 100000

/* Define "not supported" for the power manager features */
#define PM_NOT_SUPPORTED (-10000)

/* For PLLs that doesn't lock */
#define PM_LOCK_ERROR (-10001)

/* osc opt defs */
#define PM_OSC_OPT_CRYSTAL	1
#define PM_OSC_OPT_CLOCK	2

#if defined(__AVR32_AP7000__)|| defined(__AT32AP7000__)

/* CPUMASK */
#define PM_CPU_DOMAIN 0
#define PM_CPU_DOMAIN_SIZE 1
#define PM_CPU_PICO 0

/* HSB */
#define PM_HSB_DOMAIN 1
#define PM_HSB_DOMAIN_SIZE 11
#define PM_HSB_HEBI 32
#define PM_HSB_PBA 33
#define PM_HSB_PBB 34
#define PM_HSB_HRAMC 35
#define PM_HSB_HSBBRIDGE 36
#define PM_HSB_HISI 37
#define PM_HSB_USB 38
#define PM_HSB_LCDC 39
#define PM_HSB_MACB0 40
#define PM_HSB_MACB1 41
#define PM_HSB_DMAC 42

/* PBA */
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

#endif // #if defined(__AVR32_AP7000__)|| defined(__AT32AP7000__)

/*!
 * \brief The different clock sources of the power manager
 */
enum pm_clock_source_e {
  PM_OSC0=0,
  PM_OSC1=1,
  PM_OSC32=2,
  PM_PLL0=3,
  PM_PLL1=4
};


/*!
 * \brief Generic clock genration options
 */
typedef struct {
  //! The divider from the clock source
  unsigned char divider;
  //! Which clock source to use: \ref pm_clock_source_e
  int clock_source;
} pm_gen_clk_opt_t;


/*!
 * \brief Oscilator startup options
 */
typedef struct {
  //! The oscillator id number
  unsigned char osc_id;
  //! Specify the startup time for the oscillator
  unsigned char startup;
  //! Set the oscillator mode; clock or crystal
  unsigned char mode;
} pm_osc_opt_t;


/*!
 * \brief Pll settings. The new frequency will be set according to the following
 * formula: pll = osc * mul / div
 */
typedef struct {
  //! The pll id number
  unsigned char pll_id;
  //! The oscillator id number to use as a reference
  unsigned char osc_id;
  //! Multiplier
  unsigned char mul;
  //! Divider
  unsigned char div;
  //! The number of cycles after pll lock before status register is updated
  unsigned char count;
  //! Extended pll options. Consult the part specific datasheet for settings
  unsigned char options;
  //! Set to true to wait actively for pll lock
  Bool wait_for_lock;
} pm_pll_opt_t;


/*!
 * \brief The different sources of the power manager that can be calibrated
 */
enum pm_calibration_src_e {
  PM_RC_OSCILLATOR,
  PM_BOD_CONTROL,
  PM_BANDGAP_CALIBRATION,
  PM_VOLTAGE
};


/*! \brief Resets the power manager and enables the default startup clock
 *
 * \return Status.
 *   \retval 0  Reset successful.
 *   \retval <0 Error reseting the power manager.
 */
int pm_reset(void);


/*! \brief Returns cause for the last reset
 *
 * \return Status.
 *   \retval >0 The reset cause
 *   \retval =0 Unknown reset cause
 */
unsigned int pm_get_reset_cause(void);


/*! \brief Starts a specific oscillator. Used for both internal and external osc.
 *
 * \param opt The oscillator options (\ref pm_osc_opt_t)
 *
 * \return Status.
 *   \retval 0  Oscillator start successfull.
 *   \retval <0 Error starting the oscillator.
 */
int pm_start_osc(const pm_osc_opt_t *opt);


/*! \brief Stops a specific oscillator
 *
 * \param osc_id ID number of the oscillator that is to be stopped
 *
 * \return Status.
 *   \retval 0  Oscillator successfully stopped.
 *   \retval <0 An error occured when stopping the oscillator.
 */
int pm_stop_osc(int osc_id);


/*! \brief Setup and configure a specific pll
 *
 * \param opt The pll setting options (\ref pm_pll_opt_t)
 *
 * \return Status.
 *   \retval >=0 Success. Actual pll frequency returned
 *   \retval <0  Not able to set pll according to input.
 */
int pm_start_pll(pm_pll_opt_t *opt);


/*! \brief Setup and configure a specific pll to a match target frequency
 *
 * \param pll_id Id number of the pll to set up.
 * \param freq The desired frequency
 * \param wait_for_lock Assert to poll for lockbit.
 *
 * \return Status.
 *   \retval >=0 Success. Actual pll frequency returned
 *   \retval <0  An error occured when stopping the oscillator.
 */
int pm_start_pll_freq(int pll_id, unsigned int freq, Bool wait_for_lock);


/*! \brief Stop a pll
 *
 * \param pll_id Id number of the pll to stop
 *
 * \return Status.
 *   \retval 0  Success. The pll is stopped
 *   \retval <0 An error occured when trying to stop pll
 */
int pm_stop_pll(int pll_id);


/*! \brief Gets the frequency in hz for the specified clock source
 *
 * \param clock_source The clock source to use. See \ref pm_clock_source_e
 *
 * \return Status.
 *   \retval >=0 Success. The running frequency is return
 *   \retval <0  Unable to read clock source frequency
 */
int pm_get_clock_source_freq_hz(int clock_source);


/*! \brief Wait actively for a clock source to successfully lock
 *
 * \param clock_source The clock source to wait for. See \ref pm_clock_source_e
 *
 * \return Status.
 *   \retval 0  Success.
 *   \retval <0 Unable to lock clock source
 */
int pm_wait_for_clock_source_lock(int clock_source);


/*! \brief Wait for status bit(s) to be set
 *
 * \param status Status bit mask to wait for
 *
 * \return Status.
 *   \retval 0  Success. The status bit(s) are set
 *   \retval <0 An error occured when waiting for status
 */
int pm_wait_for_status(unsigned int status);


/*! \brief Enable power manager interrupts
 *
 * \param mask the interrupts to enable
 *
 * \return Status.
 *   \retval 0  Success. The interrupts set in the mask is enabled
 *   \retval <0 Unable to set the desired mask.
 */
int pm_enable_interrupt(unsigned int mask);


/*! \brief Disable power manager interrupts
 *
 * \param mask the interrupts to disable
 *
 * \return Status.
 *   \retval =0 Success. The interrupts set in the mask is disabled
 *   \retval <0 Unable to set the desired mask.
 */
int pm_disable_interrupt(unsigned int mask);


/*! \brief Read the enable power manager interrupts
 *
 * \return Status.
 *   \retval The enabled interrupts.
 */
unsigned int pm_get_enabled_interrupts(void);


/*! \brief Read the interrupt status of the power manager
 *
 * \return Status.
 *   \retval The interrupts which have been trigged
 */
unsigned int pm_get_interrupt_status(void);


/*! \brief Clear raised interrupts from the power manager.
 *
 * \param mask The interrupts to clear.
 *
 * \return Status.
 *   \retval >0 The interrupts which have been cleared.
 *   \retval <0 Unable to clear the desired interrupts.
 */
int pm_clear_interrupt_status(unsigned int mask);


/*! \brief Calibrate the hardware
 *
 * \param source The source to calibrate
 * \param calibration_value Consult the datasheet for your device for vaules.
 *
 * \return Status.
 *   \retval =0 Bandgap successfully calibrated.
 *   \retval <0 Unable to calibrate bandgap.
 */
int pm_calibrate(int source, unsigned int calibration_value);


/*! \brief Calibrate and set the BOD
 *
 * \param bod_control Specify behaviour upon BOD event
 * \param hysteresis Enable/disable hysteresis on the BOD
 * \param level Consult the datasheet for your device for vaules.
 *
 * \return Status.
 *   \retval =0 Bandgap successfully calibrated.
 *   \retval <0 Unable to calibrate the BOD.
 */
int pm_set_bod_settings(unsigned int bod_control, Bool hysteresis, unsigned char level);


/*! \brief Read Bod settings
 *
 * \return Status.
 *   \retval >=0 Success. BOD settings returned.
 *   \retval <0 Error reading BOD.
 */
int pm_get_bod_settings(unsigned int bod_control, Bool hysteresis, unsigned char level);


/*! \brief Set the main clock.
 *
 * \param clock_source The clock to use as the main clock. See \ref pm_clock_source_e
 *
 * \return Status.
 *   \retval 0  Success.
 *   \retval <0 An error occured when trying to set the main clock
 */
int pm_set_mclk_source(int clock_source);


/*! \brief Read source of the main clock.
 *
 * \return source.
 *   \retval The source of the main clock. See \ref pm_clock_source_e
 */
int pm_get_mclk_source(void);


/*! \brief Read the current cpu speed
 *
 * \return freq.
 *   \retval The current frequency of the main clock
 */
int pm_get_mclk_freq_hz(void);


/*! \brief Read the clock domain scaler
 *
 * \param clock_domain The clock domain in question
 *
 * \return Status.
 *   \retval >0 Success. The divider is returned
 *   \retval <0 An error occured.
 */
int pm_read_clock_domain_scaler(unsigned int clock_domain);


/*! \brief Set the relative divider for a clock domain
 *
 * \param clock_domain The clock domain to alter
 * \param divider The scaler to set. Valid values are 1, 2, 4, 8, 16, 32, 64, 128, 256
 *
 * \return Status.
 *   \retval >=0 Success. The divider is returned
 *   \retval <0 An error occured.
 */
int pm_set_clock_domain_scaler(unsigned int clock_domain, int divider);


/*! \brief Read the running frequency of a module.
 *
 * \param module The module to read.
 *
 * \return Status.
 *   \retval >0 Success. The running frequency is returned.
 *   \retval <1 An error occured.
 */
int pm_read_module_freq_hz(unsigned int module);


/*! \brief Enable the main clock of a specified module
 *
 * \param module The module to start
 *
 * \return Status
 *   \retval 0  Success.
 *   \retval <0 An error occured.
 */
int pm_enable_module(unsigned int module);


/*! \brief Disable a module
 *
 * \param module The module to shut down.
 *
 * \return Status
 *   \retval 0  Success.
 *   \retval <0 An error occured.
 */
int pm_disable_module(unsigned int module);


/*! \brief Setup a genclock
 *
 * \param clock The generick clock number to setup
 * \param opt The settings for the clock
 *
 * \return Status
 *   \retval 0 Success.
 *   \retval <0 An error occured.
 */
int pm_start_generic_clock(unsigned int clock, const pm_gen_clk_opt_t *opt);


/*! \brief Stop a generic clock
 *
 * \param clock The generic clock source to try to stop
 *
 * \return Status
 *   \retval 0 Success.
 *   \retval <0 Unable to stop generic clock
 */
int pm_stop_generic_clock(unsigned int clock);

#endif /* _pm_h_ */

