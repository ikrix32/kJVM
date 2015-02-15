/* This source file is part of the ATMEL AVR32-SoftwareFramework-AT32AP7000-1.0.0 Release */

/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief pio driver functions
 *
 * This file contains basic pio driver functions.
 *
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with a PIO module can be used.
 *                       The example is written for AP7000 and STK1000.
 * - AppNote:            AVR32111 - Using the AVR32 PIO Controller
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support email: avr32@atmel.com
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

/*! \mainpage
 * \section intro Introduction
 * This is the documentation for the data structures, functions, variables,
 * defines, enums, and typedefs in the software for application note AVR32111.
 * It also gives an examples of the usage of the PIO driver
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC for AVR32 and IAR Systems compiler
 * for AVR32. Other compilers may or may not work.
 *
 * \section deviceinfo Device Info
 * All AVR32 devices with a PIO module can be used. This example has been tested
 * with the following setup:
 *
 * - STK1000 development kit
 * - AP7000 top board for STK1000 development kit
 *
 * \section setupinfo Setup Information
 * CPU speed: <i>20 MHz</i>
 *
 *
 * - The header \b SW6 must be jumped to \b GPIO for the signals to be routed correctly
 * - The header \b SW4 must be jumped to \b GPIO for the signals to be routed correctly
 *
 * \section applicationUsage Application Usage
 *
 * The example is dip-switch lights. It's toggles the leds according to which 
 * corresponding dip-switch which is pressed. You must connect a cable from \b J3 
 * to \b LED and \b J1 to J25 for the correct behaviour.
 *
 * When recompiling a project, make sure to run \b make \b clean before recompiling
 *
 * \section files Main Files
 * - pio.c: PIO driver source
 * - pio.h: PIO driver header
 * - pio_example.c: PIO example
 *
 * \section contactinfo Contact Info
 * For more info about Atmel AVR32 visit
 * <A href="http://www.atmel.com/products/AVR32/" >Atmel AVR32</A> \n
 * <A href="http://www.atmel.com/dyn/products/app_notes.asp?family_id=682">
 * AVR32 Application Notes</A>\n
 * Support mail: avr32@atmel.com
*/
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

int gpio_enable_module_pin(int pin, int function)
{
  return pio_setup_pin(pin, function);
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

void pio_enable_module(const gpio_map_t piomap, int size)
{
  int i;

  for(i=0; i<size; i++){
    pio_setup_pin(piomap->pin, piomap->function );
    piomap++;
  }
}

void gpio_enable_module(const gpio_map_t piomap, int size)
{
  pio_enable_module(piomap, size);
}
