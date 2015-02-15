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

#ifndef _SDRAM_H_
#define _SDRAM_H_

#include <avr32/io.h>
#include "errno.h"

/*!
 * SDRAMC operation mode: Normal operation
 */
#define MODE_NORMAL		0

/*!
 * SDRAMC operation mode: NO OP
 */
#define MODE_NOP		1

/*!
 * SDRAMC operation mode: Precharge
 */
#define MODE_PRECHARGE		2

/*!
 * SDRAMC operation mode: Load Mode Register
 */
#define MODE_LOAD_MR		3

/*!
 * SDRAMC operation mode: Auto refresh
 */
#define MODE_AUTOREFRESH	4

/*!
 * SDRAMC operation mode: Extended Load
 */
#define MODE_EXT_LOAD_MR	5

/*!
 * SDRAMC operation mode: Power Down
 */
#define MODE_POWER_DOWN		6

typedef struct sdram_info
{
  /*! \brief physical_address: The base address for the sdram */
  unsigned int physical_address;

  /*! \brief bus_width: 16/32 bus width */
  unsigned char bus_width;

  /*! \brief rows: The number of rows in the SDRAM */
  unsigned int rows;

  /*! \brief cols: The number of columns in the SDRAM */
  int cols;

  /*! \brief banks: The number of banks in the SDRAM */
  int banks;

  /*! \brief cas: The CAS latency*/
  unsigned int cas;

  /*! \brief chip: TWI chip address to communicate with */
  unsigned int twr;

  /*! \brief chip: TWI chip address to communicate with */
  unsigned int trc;

  /*! \brief chip: TWI chip address to communicate with */
  unsigned int trp;

  /*! \brief chip: TWI chip address to communicate with */
  unsigned int trcd;

  /*! \brief chip: TWI chip address to communicate with */
  unsigned int tras;

  /*! \brief chip: TWI chip address to communicate with */
  unsigned int txsr;

}sdram_info;


/*!
 * This function will initialize the AVR32 SDRAM Controller
 * \param *info The setup and timing variables for the controller
 * \return Nothing
 */
int sdram_init(const sdram_info *info);

#endif
