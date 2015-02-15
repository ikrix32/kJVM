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

#ifndef _MT481C2M32B2TG_H_
#define _MT481C2M32B2TG_H_

#include "sdram.h"

/*
These values are device specific. Consult the datasheet for
the given device for more information
*/

/*!
 * The number of columns for this sdram.
 */
// number of cols = 256  ->  8 col bits
#define mt481c2m32b2tg_cols  8

/*!
 * The number of rows for this sdram.
 */
// number of rows = 2048 -> 11 row bits
#define mt481c2m32b2tg_rows  11

/*!
 * The number of banks for this sdram.
 */
// number of banks = 4   ->  2 bank bits
#define mt481c2m32b2tg_banks 2


/*!
 * The refresh rate. NOTE: This is dependant on the clock frequency
 */
// Refresh rate
// Use 15.6 uS = 15600 ns.
// With cpu hz 20 MHz = 50 ns clock cycle :	
// Refresh rate = 15600 ns / 50 ns = 312 clock cycles
// Generic formula for chip (156 * (CPU_HZ / 1000) ) / 10000
#define  mt481c2m32b2tg_tr 312


/*!
 * The CAS latency for this sdram.
 */
// CAS Latency: 3
// Should be 3 for Speed grade -5 part (up to 200Mhz)
#define mt481c2m32b2tg_cas 3

/*!
 * The write recovery time. NOTE: This setting is dependant on the clock frequency
 */
// Min: 2ck
#define mt481c2m32b2tg_twr 2

/*!
 * Minimal ACTIVE to ACTIVE command period. NOTE: This setting is dependant on the clock frequency
 */

// Min 55 ns -> 55 / 50 ~=  2
#define mt481c2m32b2tg_trc 7

/*!
 * Minimal PRECHARGE command period. NOTE: This setting is dependant on the clock frequency
 */
// 15 ns -> 15 / 50 ~= 1
#define mt481c2m32b2tg_trp 2
/*!
 * Minimal ACTIVE to READ or WRITE delay. NOTE: This setting is dependant on the clock frequency
 */
//15 ns -> 15 / 50 ~= 1
#define mt481c2m32b2tg_trcd 2

/*!
 * Minimal ACTIVE to PRECHARGE command. NOTE: This setting is dependant on the clock frequency.
 */
// 42 ns -> 42 / 50 ~= 1
#define mt481c2m32b2tg_tras 5

/*!
 *Minimal exit SELF REFREASH to ACTIVE command. NOTE: This setting is dependant on the clock frequency
 */
// 55 ns -> 55 / 50 ~= 2
#define mt481c2m32b2tg_txsr 5

/*!
 * This function will initialize the mt481c2m32b2tg sdram.
 * \param *info The mt481c2m32b2tg hardware specific info
 * \return Nothing
 */
void mt481c2m32b2tg_init( const sdram_info *info );

#endif
