/*This file has been prepared for Doxygen automatic documentation generation.*/
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


#ifndef _CYCLE_COUNTER_H_
#define _CYCLE_COUNTER_H_

#include "compiler.h"


typedef struct
{
  unsigned long time;

  Bool wrap;
} t_cpu_time;


#if __GNUC__
__attribute__((__always_inline__))
#endif
extern __inline__ U32 cpu_ms_2_cy(unsigned long ms, unsigned long fcpu_hz)
{
  return ((unsigned long long)ms * fcpu_hz + 999) / 1000;
}


#if __GNUC__
__attribute__((__always_inline__))
#endif
extern __inline__ U32 cpu_cy_2_ms(unsigned long cy, unsigned long fcpu_hz)
{
  return ((unsigned long long)cy * 1000 + fcpu_hz-1) / fcpu_hz;
}


#if __GNUC__
__attribute__((__always_inline__))
#endif
extern __inline__ U32 cpu_cy_2_us(unsigned long cy, unsigned long fcpu_hz)
{
  return ((unsigned long long)cy * 1000000 + fcpu_hz-1) / fcpu_hz;
}


#if __GNUC__
__attribute__((__always_inline__))
#endif
extern __inline__ void cpu_set_timeout(unsigned long delay, t_cpu_time *cpu_time)
{
  // Use the CPU cycle counter.
  unsigned long delay_start_cycle = Get_system_register(AVR32_COUNT);
  unsigned long delay_end_cycle   = delay_start_cycle + delay;

  if (delay_start_cycle <= delay_end_cycle)
    cpu_time->wrap=FALSE;
  else
    cpu_time->wrap=TRUE;

  cpu_time->time= delay_end_cycle;
}


#if __GNUC__
__attribute__((__always_inline__))
#endif
extern __inline__ unsigned long cpu_is_timeout(t_cpu_time *cpu_time)
{
  unsigned long delay_end_cycle = cpu_time->time;
  // Use the CPU cycle counter.
  if (cpu_time->wrap==FALSE)
  {
    if( (unsigned long)Get_system_register(AVR32_COUNT) < delay_end_cycle )
      return FALSE;
    else
      return TRUE;
  }
  else
  {
    if( (unsigned long)Get_system_register(AVR32_COUNT) > delay_end_cycle )
      return FALSE;
    else
    { // AVR32 counter has wrapped.
      cpu_time->wrap=FALSE;
      return FALSE;
    }
  }
}


#if __GNUC__
__attribute__((__always_inline__))
#endif
extern __inline__ void cpu_delay_ms(unsigned long delay, unsigned long fcpu_hz)
{
  t_cpu_time timer;
  cpu_set_timeout( cpu_ms_2_cy(delay, fcpu_hz), &timer);
  while( !cpu_is_timeout(&timer) );
}


#if __GNUC__
__attribute__((__always_inline__))
#endif
extern __inline__ void cpu_delay_cy(unsigned long delay, unsigned long fcpu_hz)
{
  t_cpu_time timer;
  cpu_set_timeout( delay, &timer);
  while( !cpu_is_timeout(&timer) );
}


#define Get_sys_count()     ( Get_system_register(AVR32_COUNT)        )
#define Set_sys_count(x)    ( Set_system_register(AVR32_COUNT,   (x)) )
#define Get_sys_compare()   ( Get_system_register(AVR32_COMPARE)      )
#define Set_sys_compare(x)  ( Set_system_register(AVR32_COMPARE, (x)) )


#endif // _CYCLE_COUNTER_H_
