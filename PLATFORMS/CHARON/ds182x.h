#ifndef _DEV_BOARD_DS182X_H
#define _DEV_BOARD_DS182X_H

// From ethernut

#define COMPRESSDISABLE
#define COMPRESSREENABLE

#ifdef __IMAGECRAFT__
#define __IMAGECRAFT__
// bh defined R30 ->R0
#define NutEnterCritical() \
{ \
    asm("in R30, 0x3F\n" \
    "cli\n" \
    "push R30\n"); \
}

#define NutExitCritical() \
{ \
    asm("pop R30\n" \
    "out 0x3F, R30\n"); \
}

#else

#define NutEnterCritical_nt() \
asm volatile( \
"in  __tmp_reg__, __SREG__" "\n\t" \
"cli"                       "\n\t" \
"push __tmp_reg__"          "\n\t" \
)

#define NutExitCritical_nt() \
asm volatile( \
"pop __tmp_reg__"           "\n\t" \
"out __SREG__, __tmp_reg__" "\n\t" \
)

#ifdef NUTTRACER_CRITICAL
#define NutEnterCritical() \
NutEnterCritical_nt(); \
TRACE_ADD_ITEM_PC(TRACE_TAG_CRITICAL_ENTER);

#define NutExitCritical() \
TRACE_ADD_ITEM_PC(TRACE_TAG_CRITICAL_EXIT); \
NutExitCritical_nt()
#else
#define NutEnterCritical() \
NutEnterCritical_nt();

#define NutExitCritical() \
NutExitCritical_nt()
#endif
#endif

#define NutJumpOutCritical() NutExitCritical()

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// end - from ethernut

#include <string.h>

#ifndef __IMAGECRAFT__
#include <avr/io.h>
#include <avr/pgmspace.h>
#endif

//#include <sys/atom.h>
#include <stdio.h>
#include "types.h"

#define DQ           PORTD0                       /* 1-wire thermometer is connected to PD0 */

#define DS_PORT      PORTD
#define DS_PIN       PIND
#define DS_DDR       DDRD

#define MAXSEARCH   20                            /* max # of attempts to find devices on a 1-wire bus */
#define MAXCHIP     1

#define MULTIDEVICE 0
/* set 1 if you have more than one device on a 1-wire bus
 * set 0 if you have only one device on a 1-wire bus
 */

#ifndef _NOP
#define _NOP() asm volatile ("nop\n\t"::)
#endif

void convertTemperature(u_char idx, u_int *temp, u_int *frac);
u_int readTemperature(u_char idx);
void sampleTemperature(u_char idx);
u_char init(void);
void scan(void);

/*
 *  Auxilliary routines
 *
 *    - normally used only internally
 *    - can be used e.g. if device inventory required
 */
u_char TM_Search_next(u_char *ptr);
u_char TM_Search_rom(u_char *ptr);
u_char TM_Read_rom(u_char *ptr);
void TM_Delay(u_char cnt);
u_char TM_Crc(u_char crc, u_char inp);
#endif
