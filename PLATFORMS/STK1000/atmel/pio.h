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
 * $Revision: 1.5 $
 * $RCSfile: pio.h,v $
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

#ifndef _pio_h_
#define _pio_h_

#ifdef __GNUC__
	#include <avr32/io.h>
#elif __ICCAVR32__
	#include <avr32/ioap7000.h>
#else
	#error No known compiler used
#endif

/*!
 * A type definitions of pins and module connectivity.
 * First column is the pin number, the second is pio connectivity
 */
typedef char avr32_piomap_t[][2];

#define PIO_INVALID_ARGUMENT -1
#define PIO_SUCCESS           0

/*!
 * This function will return the baseaddress for a port
 * \param *port The port number
 * \return The port's baseaddress
 */
volatile avr32_pio_t *pioGetHandle(int port);

/*!
 * This function will put a single pin under a module's control
 * \param *pin The pin number
 * \param *function The PIO module which to enable
 * \return PIO_SUCCESS or PIO_INVALID_ARGUMENT
 */
int pio_setup_pin(int pin, int function);

/*!
 * This function will enable a module pin for a given set of pins and respective modules
 * \param *piomap A list of pins and pio connectivity
 * \param size The number of pins in *piomap
 * \return nothing
 */
void pio_enable_module(avr32_piomap_t piomap, int size);

#endif
