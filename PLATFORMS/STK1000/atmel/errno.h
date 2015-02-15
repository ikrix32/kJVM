/* *****************************************************************************
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
#ifndef _ernno_h_
#define _errno_h_

#define SUCCESS 		0

/* Generic errors */
#define ERROR			-1
#define INVALID_ARGUMENT	-2

/* 
 * Module specific error codes can be added here 100 codes are reserved for 
 * each module. Add modules sequentially. A list of the modules ordered 
 * alphabetically should be inserted to ease the usability.
 *
 * LIST OF MODULES
 *
 * Power Manager		100
 * USART			200
 *	
*/

/* Power Manager */
#define PM_LOCK_ERROR		-100
#define PM_MODULE_IS_MASKED	-101

/* USART */
#define USART_SUCCESS 0
#define USART_FAILURE -1
#define USART_INVALID_INPUT 1
#define USART_INVALID_ARGUMENT -1

#define USART_TX_BUSY		-200
#define USART_RX_EMPTY		-201
#define USART_RX_ERROR		-202
#define USART_MODE_FAULT 	-203
#define USART_ADDR_RECEIVED 	-204

#endif
