/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief SPI bus example driver.
 *
 * This file defines a usefull set of functions for the SPI interface on
 * AVR32 devices. The driver handles normal polled usage and direct memory
 * access (PDC) usage.
 *
 * - Compiler:           IAR EWAAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with a SPI module can be used.
 * - AppNote:            AVR32105 - Master and Slave SPI Driver
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support email: avr32@atmel.com
 *
 * $Name: RELEASE_1_0 $
 * $Revision: 1.9 $
 * $RCSfile: spi.c,v $
 * $Date: 2006/05/09 11:25:42 $
 *****************************************************************************/

/* Copyright (c) 2006, Atmel Corporation All rights reserved.
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

#include "spi.h"

int getBaudDiv(struct spi_options_t * options, int cpuHz);


/*! \brief Resets the SPI controller
 *
 *  \param spi Pointer to the correct avr32_spi_t struct
 */
void spi_reset(volatile avr32_spi_t * spi)
{
	spi->cr = AVR32_SPI_CR_SWRST_MASK;
}


/*! \brief Initialize the SPI in slave mode
 *
 *  \param spi Pointer to the correct avr32_spi_t struct
 *  \param bits Number of bits in each transmitted character (8 - 16)
 *  \param spi_mode When the read and write bits; Clock polarity and phase
 *
 *  \return Status
 *    \retval SPI_OK on success
 *    \retval SPI_ERROR_ARGUMENT when invalid arguments are passed
 */
int spi_initSlave(volatile avr32_spi_t * spi,
		unsigned char bits,
		unsigned char spi_mode)
{
	/* Reset */
	spi->cr = (1<<AVR32_SPI_CR_SWRST_OFFSET);

	/* The number of bits and clock polarity/phase have to be set in
	   csr0 for the SPI to communcate in slave mode */
	if (bits > 16 || bits < 8) {
		return SPI_ERROR_ARGUMENT;
	}

	spi->csr0 = (bits - 8) << AVR32_SPI_CSR0_BITS_OFFSET;

	switch(spi_mode) {
		case 0:
			spi->csr0 |= (1 << AVR32_SPI_CSR0_NCPHA_OFFSET);
		case 1:
			break;
		case 2:
			spi->csr0 |= (1<<AVR32_SPI_CSR0_NCPHA_OFFSET);
		case 3:
			spi->csr0 |= (1<<AVR32_SPI_CSR0_CPOL_OFFSET);
			break;
		default: /* Not in legal range */
			return SPI_ERROR_ARGUMENT;
	}

	return SPI_OK;
}


/*! \brief Setup the SPI in a test mode where the transmitter is
 *         connected to the receiver
 *
 *  \param spi Pointer to the correct avr32_spi_t struct
 *
 *  \return Status
 *    \retval SPI_OK on success
 */
int spi_initTest(volatile avr32_spi_t * spi)
{
	/* Reset */
	spi->cr = (1<<AVR32_SPI_CR_SWRST_OFFSET);
	/* Master Mode */
	spi->mr |= (1<<AVR32_SPI_MR_MSTR_OFFSET);
	/* Local Loopback */
	spi->mr |= (1<<AVR32_SPI_MR_LLB_OFFSET);

	return SPI_OK;
}


/*! \brief Setup the SPI module in master mode
 *
 *  \param spi Pointer to the correct avr32_spi_t struct
 *  \param options Pointer struct containing setup for a chip register
 *
 *  \return Status
 *    \retval SPI_OK on success
 *    \retval SPI_ERROR_ARGUMENT when invalid arguments are passed
 */
int spi_initMaster(volatile avr32_spi_t * spi, struct spi_options_t * options)
{
	if (options->modfdis > 1) {
		return SPI_ERROR_ARGUMENT;
	}

	if (options->fdiv > 1) {
		return SPI_ERROR_ARGUMENT;
	}

	/* Reset */
	spi->cr = AVR32_SPI_CR_SWRST_MASK;

	/* Master Mode */
	spi->mr = AVR32_SPI_MR_MSTR_MASK|
		(options->fdiv<<AVR32_SPI_MR_FDIV_OFFSET)|
		(options->modfdis<<AVR32_SPI_MR_MODFDIS_OFFSET);

	return SPI_OK;
}


/*! \brief How and when are the slave chip selected (master mode only)
 *
 *  \param spi Pointer to the correct avr32_spi_t struct
 *  \param variable_ps Target slave is selected in transfer register \
 *         for every character to transmit
 *  \param pcs_decode The four chip select lines are decoded externaly; \
 *         values 0 to 15 can be given to select_chip()
 *  \param delay Delay in MCK cyles (or NxMCK with FDIV) between chip selects
 *
 *  \return Status
 *    \retval SPI_OK on success
 *    \retval SPI_ERROR_ARGUMENT when invalid arguments are passed
 */
int spi_selectionMode(volatile avr32_spi_t * spi,
		unsigned char variable_ps,
		unsigned char pcs_decode,
		unsigned char delay)
{
	if (variable_ps > 1) {
		return SPI_ERROR_ARGUMENT;
	}

	if (pcs_decode > 1) {
		return SPI_ERROR_ARGUMENT;
	}

	spi->mr |=
		(variable_ps<<AVR32_SPI_MR_PS_OFFSET)|
		(pcs_decode<<AVR32_SPI_MR_PCSDEC_OFFSET)|
		(delay<<AVR32_SPI_MR_DLYBCS_OFFSET);

	return SPI_OK;
}


/*! \brief Select slave chip
 *
 *  \param spi Pointer to the correct avr32_spi_t struct
 *  \param chip Slave chip number (normal: 0-3, \
 *         extarnaly decoded signal: 0-15)
 *
 *  \return Status
 *    \retval SPI_OK on success
 *    \retval SPI_ERROR_ARGUMENT when invalid arguments are passed
 */
int spi_selectChip(volatile avr32_spi_t * spi, unsigned char chip)
{
	/* Assert all lines; no peripheral is selected */
	spi->mr |= (0xF << AVR32_SPI_MR_PCS_OFFSET);

	if ((spi->mr & (1<<AVR32_SPI_MR_PCSDEC_OFFSET)) != 0) {
		/* The signal is decoded; allow up to 16 chips */
		if (chip > 15) {
			return SPI_ERROR_ARGUMENT;
		}

		spi->mr |= chip << AVR32_SPI_MR_PCS_OFFSET;
	} else {
		if (chip > 3) {
			return SPI_ERROR_ARGUMENT;
		}

		spi->mr &= ~(1<<(AVR32_SPI_MR_PCS_OFFSET + chip));
	}

	return SPI_OK;
}


/*! \brief Set options for a specific slave chip. The baudrate field
 *         has to be written before transfer in master mode. Four
 *         similar registers exist, one for each slave. When using
 *         encoded slave addressing, reg=0 sets options for slave 0-3,
 *         reg=1 for slave 4-7 and so on.
 *
 *  \param spi Pointer to the correct avr32_spi_t struct
 *  \param options Pointer struct containing setup for a chip register
 *  \param cpuHz clock speed in Hz for the peripheral bus
 *
 *  \return Status
 *    \retval SPI_OK on success
 *    \retval SPI_ERROR_ARGUMENT when invalid arguments are passed
 */
int spi_setupChipReg(volatile avr32_spi_t * spi,
		struct spi_options_t * options,
		unsigned int cpuHz)
{
	unsigned long csr;

	if (options->bits > 16 || options->bits < 8) {
		return SPI_ERROR_ARGUMENT;
	}

	if (options->stay_act > 1) {
		return SPI_ERROR_ARGUMENT;
	}

	int baudDiv = getBaudDiv(options, cpuHz);

	if (baudDiv < 0) {
		return -baudDiv;
	}

	/* Will use CSR0 offsets; these are the same for CSR0 - CSR3 */
	csr = ((options->bits - 8)<<AVR32_SPI_CSR0_BITS_OFFSET)|
		(baudDiv<<AVR32_SPI_CSR0_SCBR_OFFSET)|
		(options->spck_delay<<AVR32_SPI_CSR0_DLYBS_OFFSET)|
		(options->trans_delay<<AVR32_SPI_CSR0_DLYBCT_OFFSET)|
		(options->stay_act<<AVR32_SPI_CSR0_CSAAT_OFFSET);

	switch(options->spi_mode) {
		case 0:
			csr |= (1<<AVR32_SPI_CSR0_NCPHA_OFFSET);
		case 1:
			break;
		case 2:
			csr |= (1<<AVR32_SPI_CSR0_NCPHA_OFFSET);
		case 3:
			csr |= (1<<AVR32_SPI_CSR0_CPOL_OFFSET);
			break;
		default: /* Not in legal range */
			return SPI_ERROR_ARGUMENT;
	}

	switch(options->reg) {
		case 0:
			spi->csr0 = csr;
			break;
		case 1:
			spi->csr1 = csr;
			break;
		case 2:
			spi->csr2 = csr;
			break;
		case 3:
			spi->csr3 = csr;
			break;
		default:
			return SPI_ERROR_ARGUMENT;
	}

	return SPI_OK;
}


/*! \brief Enables a disabled SPI
 *
 *  \param spi Pointer to the correct avr32_spi_t struct
 *
 *  \return Nothing
 */
void spi_enable(volatile avr32_spi_t * spi)
{
	spi->cr = (1<<AVR32_SPI_CR_SPIEN_OFFSET);
}


/*! \brief Disables the SPI, ensures that nothing is transferred while
 *         setting up
 *         buffers. (Data loss is risked when used on a slave)
 *  \param spi Pointer to the correct avr32_spi_t struct
 *
 *  \return Nothing
 */
void spi_disable(volatile avr32_spi_t * spi)
{
	spi->cr = (1<<AVR32_SPI_CR_SPIDIS_OFFSET);
}


/*! \brief Writes one block of data to the selected (fixed select) slave, will
 *         block program execution until timeout occurs if transmitter is busy
 *
 *  \param spi Pointer to the correct avr32_spi_t struct
 *  \param data The block to write
 *
 *  \return Status
 *    \retval SPI_OK on success
 *    \retval SPI_TIMEOUT on timeout
 */
int spi_write(volatile avr32_spi_t * spi, unsigned short data)
{
	unsigned int timeout = SPI_TIMEOUT;

	while ((spi->sr & AVR32_SPI_SR_TXEMPTY_MASK) == 0 && timeout > 0) {
		--timeout;
	}

	if (timeout == 0) {
		return SPI_ERROR_TIMEOUT;
	}

	spi->tdr = data & 0x0000FFFF;

	return SPI_OK;
}


/*! \brief Selects a slave and writes one block of data to this, will block
 *         program execution until timeout occurs if transmitter is busy.
 *  \param spi Pointer to the correct avr32_spi_t struct
 *  \param data The block to write
 *  \param pcs Slave select (bit0 -> ncs_line0, bit1 -> ncs_line1, ...)
 *    \arg SPI_NPCS0
 *    \arg SPI_NPCS1
 *    \arg SPI_NPCS2
 *    \arg SPI_NPCS3
 *  \param lastxfer Last transfer to slave
 *    \arg 0 when there is more transfers to the slave
 *    \arg 1 when there is no more transfers to the slave
 *
 *  \return Status
 *    \retval SPI_OK on success
 *    \retval SPI_ERROR_ARGUMENT when invalid arguments are passed
 */
int spi_variableSlaveWrite(volatile avr32_spi_t * spi, unsigned short data,
		unsigned char pcs, unsigned char lastxfer)
{
	unsigned int timeout = SPI_TIMEOUT;

	if (pcs > 15 || lastxfer > 1)
		return SPI_ERROR_ARGUMENT;

	while ((spi->sr & AVR32_SPI_SR_TXEMPTY_MASK) == 0 && --timeout) {
	}

	if (timeout == 0) {
		return SPI_ERROR_TIMEOUT;
	}

	spi->tdr = (data << AVR32_SPI_TDR_TD_OFFSET) |
		(pcs << AVR32_SPI_TDR_PCS_OFFSET) |
		(lastxfer << AVR32_SPI_TDR_LASTXFER_OFFSET);

	return SPI_OK;
}


/*! \brief Check if there is data in the receive register
 *
 *  \param spi Pointer to the correct avr32_spi_t struct
 *
 *  \return Status
 *    \retval 1 when there is data in RDR
 *    \retval 0 when there is not data in RDR
 */
unsigned char spi_readRegisterFullCheck(volatile avr32_spi_t * spi)
{
	if ((spi->sr & AVR32_SPI_SR_RDRF_MASK) != 0) {
		return 1;
	} else {
		return 0;
	}
}


/*! \brief Read one block from the selected (fixed select) slave, will block
 *         program execution until timeout occurs if no data is received.
 *
 *  \param spi pointer to the correct avr32_spi_t struct
 *  \param data the received block of data (8 to 16 bits)
 *
 *  \return status or error code
 *    \retval SPI_OK on success
 *    \retval SPI_ERROR_TIMEOUT on timeout waiting for data
 */
int spi_read(volatile avr32_spi_t * spi, unsigned short * data)
{
	unsigned int timeout = SPI_TIMEOUT;

	do {
		--timeout;
	} while ((spi->sr & AVR32_SPI_SR_RDRF_MASK) == 0 && timeout > 0);

	if (timeout == 0) {
		return SPI_ERROR_TIMEOUT;
	}

	*data = spi->rdr & 0x0000FFFF;

	return SPI_OK;
}


/*! \brief Get status information from the SPI
 *
 *  \param spi Pointer to the correct avr32_spi_t struct
 *
 *  \return Status
 *    \retval SPI_OK on success
 *    \retval SPI_ERROR_OVERRUN Overrun error has occurred
 *    \retval SPI_ERROR_MODE_FAULT Mode fault (has been addressed as slave while
 *            in master mode)
 *    \retval SPI_ERROR_OVERRUN_AND_MODE_FAULT Overrun error and mode
 *            fault error has occurred
 */
unsigned char spi_getStatus(volatile avr32_spi_t * spi)
{
	unsigned char ret = 0;

	if ((spi->sr & (1<<AVR32_SPI_SR_OVRES_OFFSET)) != 0) {
		ret = SPI_ERROR_OVERRUN;
	}

	if ((spi->sr & (1<<AVR32_SPI_SR_MODF_OFFSET)) != 0) {
		ret += SPI_ERROR_MODE_FAULT;
	}

	if (ret == (SPI_ERROR_OVERRUN + SPI_ERROR_MODE_FAULT)) {
		return SPI_ERROR_OVERRUN_AND_MODE_FAULT;
	}
	else if (ret > 0) {
		return ret;
	} else {
		return SPI_OK;
	}
}


/*! \brief Calculate the baudrate division
 *
 *  \param options Pointer to a spi_options_t struct
 *  \param cpuHz the clock frequency in Hz for the peripheral bus
 *
 *  \return division factor or error code
 *    \retval >= 0 on success
 *    \retval  < 0 on error
 */
int getBaudDiv(struct spi_options_t * options, int cpuHz) {
	int baudDiv = 0;

	if (options->fdiv == 0) {
		baudDiv = cpuHz / (options->baudrate);
	} else {
		baudDiv = cpuHz / (32 * options->baudrate);
	}

	if (baudDiv > 255 || baudDiv <= 0) {
		return -SPI_ERROR_ARGUMENT;
	}

	return baudDiv;
}

