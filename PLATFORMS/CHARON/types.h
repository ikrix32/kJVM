#ifndef _SYS_TYPES_H_
#define _SYS_TYPES_H_

/*
 * Copyright (C) 2001-2003 by egnite Software GmbH. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 *
 * -
 * Portions Copyright (C) 2000 David J. Hudson <dave@humbug.demon.co.uk>
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You can redistribute this file and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software Foundation;
 * either version 2 of the License, or (at your discretion) any later version.
 * See the accompanying file "copying-gpl.txt" for more details.
 *
 * As a special exception to the GPL, permission is granted for additional
 * uses of the text contained in this file.  See the accompanying file
 * "copying-liquorice.txt" for details.
 * -
 * Portions Copyright (c) 1983, 1993 by
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * $Log: types.h,v $
 * Revision 1.8  2003/02/04 18:00:54  harald
 * Version 3 released
 *
 * Revision 1.7  2002/08/08 17:25:12  harald
 * *** empty log message ***
 *
 * Revision 1.6  2002/06/26 17:29:30  harald
 * First pre-release with 2.4 stack
 *
 */

//#include <compiler.h>

/*!
 * \file sys/types.h
 * \brief Nut/OS type declarations.
 */

#ifdef __cplusplus
extern "C"
{
#endif

//#define __SFR_OFFSET 0
#define SFR_IO_ADDR(sfr) ((sfr) - __SFR_OFFSET)
#define SFR_MEM_ADDR(sfr) (sfr)
#define SFR_IO_REG_P(sfr) ((sfr) < 0x40 + __SFR_OFFSET)

/*!
 * \weakgroup xgNutOS
 */
/*@{*/

/*! \brief Unsigned 8-bit value. */
    typedef unsigned char u_char;
/*! \brief Unsigned 16-bit value. */
    typedef unsigned short u_short;
/*! \brief Unsigned 16-bit value. */
    typedef unsigned int u_int;
/*! \brief Unsigned 32-bit value */
    typedef unsigned long u_long;
/*! \brief Unsigned 64-bit value */
    typedef unsigned long long u_longlong;
/*! \brief Void pointer */
    typedef void * HANDLE;

#ifdef DEBUG
#define dbg_printf  printf
//#define dbg_printf(string,arg...) printf(string, ##arg); fflush(stdout);
#else
#define dbg_printf
#endif

#ifdef __IMAGECRAFT__

#define __byte_swap2(val) \
(((val & 0xff) << 8) | \
((val & 0xff00) >> 8))

#define __byte_swap4(val) \
(((val & 0xff) << 24) | \
((val & 0xff00) << 8) | \
((val & 0xff0000) >> 8) | \
((val & 0xff000000) >> 24))

#else

/*
 * Conversion of 16 bit value to network order.
 */
    static inline u_short __byte_swap2(u_short val)
    {
        asm volatile(
            "mov __tmp_reg__, %A0\n\t"
            "mov %A0, %B0\n\t"
            "mov %B0, __tmp_reg__\n\t"
            : "=r" (val)
            : "0" (val)
            );
        return val;
    }

/*
 * Conversion of 32 bit value to network order.
 */
    static inline u_long __byte_swap4(u_long val)
    {
        asm volatile(
            "mov __tmp_reg__, %A0\n\t"
            "mov %A0, %D0\n\t"
            "mov %D0, __tmp_reg__\n\t"
            "mov __tmp_reg__, %B0\n\t"
            "mov %B0, %C0\n\t"
            "mov %C0, __tmp_reg__\n\t"
            : "=r" (val)
            : "0" (val)
            );
        return val;
    }
#endif

/*!
 * \brief Convert short value from host to network byte order.
 */
#define htons(x) __byte_swap2(x)

/*!
 * \brief Convert long value from host to network byte order.
 */
#define htonl(x) __byte_swap4(x)

/*!
 * \brief Convert short value from network to host byte order.
 */
#define ntohs(x) __byte_swap2(x)

/*!
 * \brief Convert long value from network to host byte order.
 */
#define ntohl(x) __byte_swap4(x)

/*@}*/

#ifdef __cplusplus
}
#endif
#endif
