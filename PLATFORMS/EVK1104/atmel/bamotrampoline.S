/* This file is part of the ATMEL AVR-UC3-SoftwareFramework-1.7.0 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief AVR32 UC3 ISP trampoline.
 *
 * In order to be able to program a project with both BatchISP and JTAGICE mkII
 * without having to take the general-purpose fuses into consideration, add this
 * file to the project and change the program entry point to _trampoline.
 *
 * The pre-programmed ISP will be erased if JTAGICE mkII is used.
 *
 * - Compiler:           GNU GCC for AVR32
 * - Supported devices:  All AVR32UC devices can be used.
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ******************************************************************************/

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 *
 */

#include "../bamo32io.h"
#include "conf_isp.h"

//! @{
//! \verbatim


  // This must be linked @ 0x80000000 if it is to be run upon reset.
  .section  .reset, "ax", @progbits


  .global _trampoline
  .type _trampoline, @function
_trampoline:
  // Jump to program start.
			rjmp    program_start		// 0x80000000
		      	nop
		 	bral	monitor			// 0x80000004
			bral	conIn			// 0x80000008
			bral	conOut			// 0x8000000c

		 	bral	addressConIn		// 0x80000010
		 	bral	hexConIn		// 0x80000014
		 	bral	hexConOut		// 0x80000018
		 	bral	byteConIn		// 0x8000001c

		 	bral	byteConOut		// 0x80000020
		 	bral	wordConIn		// 0x80000024
			bral	wordConOut		// 0x80000028
		 	bral	clearScreen		// 0x8000002c

			bral	getMilliSec		// 0x80000030
			bral	dip204_write_data	// 0x80000034
			bral	controlLCD	 	// 0x80000038
			bral	adcGetValue		// 0x8000003c

			bral	pwmStart		// 0x80000040
			bral	pwmStop			// 0x80000044
			bral	setOnBoardLEDs		// 0x80000048
			bral	getButtons		// 0x8000004c

			bral	conStat			// 0x80000050
			bral	ser1ConOut 		// 0x80000054
			bral	ser1ConIn			// 0x80000058
			bral	ser1RTS			// 0x8000005c

			bral	ser1DTR			// 0x80000060
			bral	ser0ConOut			// 0x80000064
			bral	ser0ConIn			// 0x80000068
			bral	ser0ConStat		// 0x8000006c

			bral	usbConOut		// 0x80000070
			bral	usbConIn		// 0x80000074
			bral	usbConStat		// 0x80000078
			bral	at45dbx_write_open	// 0x8000007c

			bral	at45dbx_write_close	// 0x80000080
			bral	at45dbx_write_byte	// 0x80000084	
			bral	at45dbx_read_open	// 0x80000088
			bral	at45dbx_read_close	// 0x8000008c

			bral	at45dbx_read_byte	// 0x80000090


//  .org  PROGRAM_START_OFFSET
program_start:
  // Jump to the C runtime startup routine.
  lda.w   pc, _stext


//! \endverbatim
//! @}
dip204_write_data:
controlLCD:
adcGetValue:
pwmStart:
pwmStop:
setOnBoardLEDs:
getButtons:
ser1ConOut:
ser1ConIn:
ser1RTS:
ser1DTR:
usbConOut:
usbConIn:
usbConStat:
at45dbx_write_open:
at45dbx_write_close:
at45dbx_write_byte:
at45dbx_read_open:
at45dbx_read_close:
at45dbx_read_byte:
		    lda.w   pc, _stext
                                                                          
                                                          