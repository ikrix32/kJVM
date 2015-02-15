/*************************************************************************\ 
 *           Development board - DS182X 1-Wire Digital Thermometer         *
 *-------------------------------------------------------------------------*
 * Description : The DS1822 digital thermometer provides 9- to 12-bit      *
 *               centigrade temperature measurements and has an alarm      *
 *               function with NV user-programmable upper and lower trigger*
 *               points. The DS1822 communicates over a 1-Wire bus that by *
 *               definition requires only one data line (and ground) for   *
 *               communication with a central microprocessor.              *
 *                                                                         *
 *               It has an operating temperature range of -55�C to +125�C  *
 *               and is accurate to +-2.0�C over the range of -10�C to     *
 *               +85�C. In addition, the DS1822 can derive power directly  *
 *               from the data line ("parasite power"), eliminating the    *
 *               need for an external power supply.                        *
 *-------------------------------------------------------------------------*
 * Author(s)   : CEBA & www.DFSOFT.cz & www.HW.cz                          *
 * Developed   : xx.03.2003                Last Update : 07.11.2003        *
 * Version     : 1.1.0                                                     *
 *-------------------------------------------------------------------------*
 * Compiler    : AVR-GCC                   Version : 3.3                   *
 * Source file : ds182x.c                                                  *
 * Licence     : www.HW.cz - GNU GPL                                       *
 *-------------------------------------------------------------------------*
 * Target system : Charon II. - ATmega128, Quartz 14.7456 MHz              *
 *                              UART - 19200,N,8,1                         *
 *                 DS1822 1-Wire Digital Thermometer                       *
 *                                                                         *
 * Pin assignment:                                                         *
 *   Thermometer DS1822                        Charon II. - dev. board     *
 *     1  GND                                      GND                     *
 *     2  DQ Data In/Out                           PD0                     *
 *     3  Vcc                                      Vcc                     *
 *-------------------------------------------------------------------------*
 * History :                                                               *
 * 07.11.03 - added support for DS18B20 devices - CEBA                     *
 * 06.11.03 - added support for DS1820, fixed bug with sign bit - CEBA     *
 * 27.04.03 - source code ported from x51 platform by - CEBA               *
 *                                                                         *
 * FIXME :                                                                 *
 * - test DS1822 timing with scope                                         *
\*************************************************************************/
#ifndef _DEV_BOARD_DS182X_C
#define _DEV_BOARD_DS182X_C

#include "ds182x.h"

/* forward definitions of local functions */
static void   writeByte(u_char);
static u_char readByte(void);
static void   writeBit(u_char);
static u_char readBit(void);
static u_char reset(void);
void delay(u_char cnt);
u_char buildCrc(u_char crc, u_char inp);
u_char searchNext(u_char *ptr);

#define enterCritical()    NutEnterCritical()     /* disable all interrupts */
#define exitCritical()     NutExitCritical()      /* reenable interrupts */

u_char romData[8*MAXCHIP];                        /* storage to hold memory pattern */
u_char lastZero;                                  /* last detected zero bit */
/*      Convert temperature()
 *      This function converts value from raw into an integer data format.
 *      Convqersion is chosegn based on an 8-bit family code.
 *      Assume: 0x10 - DS1820,
 *              0x10 - DS18S20 - software compatible with the DS1820.
 *              0x22 - DS1822
 *              0x28 - DS18B20 - software compatible with the DS1822.
 */
void convertTemperature(u_char idx, u_int *temp, u_int *frac)
{
    switch(romData[8*idx])
    {
        case 0x10:      if(*temp & 0x01) *frac = 5000;
/* 4 decimal char precision */
        else *frac = 0;
        *temp >>= 1;
        break;
        case 0x22:
        case 0x28:      *frac = ((*temp & (1 << 3)) ? 10000/(1 << 1) : 0) +
            ((*temp & (1 << 2)) ? 10000/(1 << 2) : 0) +
            ((*temp & (1 << 1)) ? 10000/(1 << 3) : 0) +
            ((*temp & (1 << 0)) ? 10000/(1 << 4) : 0);
        *temp >>= 4;
        break;
        default:        *temp = 85;
        *frac = 0;
    }
    if(*temp & 0x80)                              /* check the sign bit */
    {
        *temp &= 0x007F;                          /* clear sign bits */
        *temp |= 0x8000;                          /* set sign bit */
    }
}


/*    Read_temperature()
 *    This function expects a previous call to Search_rom() which
 *    fills an array of ROM patterns of found devices.
 *  Arguments:
 *    idx is index to this array.
 *  Returns:
 *    0xFFFF (-1) Device not present
 *    0x8000      Error during read (improper CRC)
 *    > 0         Temperature value
 */
u_int readTemperature(u_char idx)
{
    u_char cnt;
    u_char crc;
    u_int temper;
    u_char tempBuf[9];
    enterCritical();                              /* disable interrupts */
/* Read previously sampled value ("packet" which last byte is CRC of previous) */
    if (reset())
    {
        exitCritical();                           /* reenable interrupts */
        return -1;                                /* device not present (e.g. unplugged) */
    }

#if !MULTI_DEVICE
    writeByte(0xCC);                              /* skip ROM */
#else
    u_char *ptr_tmp=&tm_romdta[8*idx];            //bh
    writeByte(0x55);                              /* match ROM */
    cnt = 8;
    do writeByte(*ptr_tmp++);
    while (--cnt);
#endif
    writeByte(0xBE);                              /* read Scratch Pad */
    for(cnt=0; cnt <= 8; cnt++) tempBuf[cnt] = readByte();
    exitCritical();                               /* reenable interrupts */
/* check if read byte stream has correct CRC */
    crc = 0;
    for(cnt=0; cnt<sizeof(tempBuf); cnt++) crc = buildCrc(crc, tempBuf[cnt]);
    temper = ((tempBuf[1] & 0x0F) << 8) | tempBuf[0];
    if (crc) temper |= 0x8000;                    /* return error (conversion can be pending) */
    return temper;
}


/*      sampleTemperature()
 *    This function initiates temperature conversion.
 *  Arguments:
 *     idx == 0xFF   skip ROM and initiate conversion on all devices
 */
void sampleTemperature(u_char idx)
{
    u_char *ptr_tmp = &romData[8*idx];
    u_char cnt;
/* Issue command to sample temperature to be prepared for next read */
    enterCritical();                              /* disable interrupts */
    reset();
    if (idx == 0xFF)    writeByte(0xCC);          /* skip ROM */
    else
    {
        writeByte(0x55);                          /* match ROM */
        cnt = 8;
        do   writeByte(*ptr_tmp++);
        while (--cnt);
    }
    writeByte(0x44);                              /* start conversion */
    exitCritical();                               /* reenable interrupts */
}


#if !MULTI_DEVICE
/*  Read_rom()
 *    If there is a single device on a 1-wire bus
 *    its ROM code can be read by this function.
 *    If more devices are present logical AND of their ROM will be read.
 *    Currently not used. Will be removed from link automatically.
 */
u_char readRom(u_char *ptr)
{
    u_char stat = 0,
        cnt = 8,
        crc = 0,
        *fam_ptr = ptr;
    enterCritical();                              /* disable interrupts */
    if (!reset())
    {
        writeByte(0x33);
        do  *ptr++ = readByte();                  /* read ROM code */
        while(--cnt);
        exitCritical();                           /* reenable interrupts */
        cnt = 8;
        ptr = fam_ptr;
        do   crc = buildCrc(crc, *ptr++);         /* calculate CRC */
        while(--cnt);
        if ( !crc && *fam_ptr != 0) stat = 1;     /* reading successfully finished */
    }
    else  exitCritical();                         /* reenable interrupts */
    return stat;
}
#endif

/*  Search_rom()
 *    This function scans all devices up to MAXCHIP on 1-wire "network".
 *    It fills passed array with ROM patterns of found devices.
 *    This array is supposed to be used with Read_temperature().
 */
u_char searchRom(u_char *ptr)
{
    u_char  tm_cnt,                               /* number of found devices (thermometers) */
        st;
    u_char  cnt = MAXSEARCH;
    memset(ptr, (u_char)0, 8 * MAXCHIP);
    lastZero = 0;
    tm_cnt = 0;
    do
    {
        st = searchNext(ptr);
        if (st)
        {
            tm_cnt++;
            if ((st == 1) && (tm_cnt < MAXCHIP))
            {
                memcpy(ptr + 8, ptr, 8);
                ptr += 8;
            }
        }
        else
/*  if 1-wire bus is empty no device responds, this prevents endless loop */
        if (!--cnt)       break;
    } while ((st != 2) && (tm_cnt < MAXCHIP));
    return (tm_cnt | ((st != 2) ? 0x80 : 0));
}


/*  Search_next()
 *    Search for a single device.
 */
u_char searchNext(u_char *ptr)
{
    u_char  i = 0, x = 0, lst_dif = lastZero;
    u_char  mask = 0x01;
    u_char  tm_rombit;
    enterCritical();                              /* disable interrupts */
    if (!reset())       writeByte(0xF0);
    else
    {
        exitCritical();                           /* reenable interrupts */
        return 0;
    }
    ptr--;                                        /* adjust pointer to satisfy algorithm within loop below */
    while ((i++ < 64) && (x < 3))                 /* 8 bytes */
    {
        if (!((i-1) % 8))
        {
            mask = 0x01;                          /* mask within a byte */
            ptr++;                                /* skip to next byte if 8 bits passed */
        }
        else            mask <<= 1;               /* next bit */
        x = ((u_char)readBit()) << 1;
        x |= readBit();
        if (!x)
        {
            if (i > lst_dif)                tm_rombit = 0;
            else
            if (i == lst_dif)   tm_rombit = 1;
/* to device where conflict was last time use 1 now (it was 0 for previous device) */
                else                tm_rombit = *ptr & mask;
/* seed with bit value of previously found device */
            if (!tm_rombit) lastZero = i;         /* last bit not set position */
        }
        else
#if 0                                     /* did not worked (against spec ?) */
            tm_rombit = (x & 0x02);               /* or (x & 1) ^ 1 (inverted LSb) */
#else
        tm_rombit = (x & 1) ^ 1;                  /* inverted LSb */
#endif
        if (tm_rombit)  *ptr |= mask;
        else            *ptr &= ~mask;
        writeBit(tm_rombit);
    }
    exitCritical();                               /* reenable interrupts */
    return (x == 3) ? 0 : 1 + (lst_dif == lastZero);
}


/*        buildCrc - calculate CRC (Cyclic Redundancy Check) for
 *           Dallas Semiconductor Touch Memory.
 */
u_char buildCrc(u_char crc, u_char inp)
{
    u_char inp_z = inp,
        i = 8,  carry;
    do
    {
        inp ^= crc;
        crc >>= 1;
        if (inp & 1)
        {
            crc ^= (0x18 >> 1);
            crc |= 0x80;
        }
        carry = inp_z & 1;
        inp_z >>= 1;
        if (carry)            inp_z |= 0x80;
        inp = inp_z;
    } while (--i);
    return crc;
}


/*  WRITE_BYTE - writes a byte to the one-wire bus. */
static void writeByte(u_char val)
{
    u_char cnt = 8;
    do
    {
        writeBit ((val & 1));
        val >>= 1;
    } while (--cnt);
}


/*  READ_BYTE - reads a byte from the one-wire bus.*/
static u_char readByte(void)
{
    u_char cnt = 8;
    u_char value = 0;
    do
    {
        value >>= 1;
        value |= (readBit()) ? 0x80 : 0x00;       /* read one bit */
    } while (--cnt);
    return value;
}


/*  WRITE_BIT - writes a byte to the one-wire bus, passed in u_charval. */
static void writeBit(u_char bitval)
{
    sbi(DS_DDR, DQ);                              /* set DQ pin direction - output */
    cbi(DS_PORT, DQ);                             /* pull DQ low to start timeslot */
    COMPRESSDISABLE;
    _NOP();                                       /* wait 68 ns */
    _NOP();                                       /* wait 68 ns */
    _NOP();                                       /* wait 68 ns */
    COMPRESSREENABLE;
    if( bitval )        sbi(DS_PORT, DQ);         /* set DQ */
    else                cbi(DS_PORT, DQ);
    delay(3);                                     /* hold value for remainder of timeslot */
    sbi(DS_PORT, DQ);                             /* DQ = 1 */
    delay(1);                                     /* finish timeslot */
}


/*  READ_BIT - reads a byte from the one-wire bus.      */
static u_char readBit(void)
{
    u_char val;
    sbi(DS_DDR, DQ);                              /* set DQ pin direction - output */
    cbi(DS_PORT, DQ);                             /* pull DQ low to start timeslot */
    COMPRESSDISABLE;
    _NOP();                                       /* wait 68 ns */
    _NOP();                                       /* wait 68 ns */
    _NOP();                                       /* wait 68 ns */
    COMPRESSREENABLE;
    sbi(DS_PORT, DQ);                             /* then return high DQ = 1 */
    delay(1);                                     /* wait 15 us */
    cbi(DS_DDR, DQ);                              /* set DQ pin direction - input */
    sbi(DS_PORT, DQ);                             /* enable AVR internal pull-up resistor */
                                                  /* read DQ status */
    bit_is_set(DS_PIN, DQ) ? (val = 1) : (val = 0);
    delay(4);                                     /* wait for end of timeslot */
    return val;                                   /* return value of DQ line */
}


/*  RESET - the initialization sequence (reset pulse and presence pulse(s)).
 *  Note: All transactions on the 1-Wire bus begin with an initialization sequence.
 */
static u_char reset(void)
{
    u_char presence;
    sbi(DS_DDR, DQ);                              /* set DQ pin direction - output */
    cbi(DS_PORT, DQ);                             /* pull DQ line low */
    delay(32);           /* leave it low for 480us */                           /* allow DQ line to return high */
    cbi(DS_DDR, DQ);                              /* set DQ pin direction - input */
// sbi(DS_PORT, DQ);       /* enable AVR internal pull-up resistor */
    delay(4);                                     /* wait for presence */
/* get presence signal */
    if(bit_is_set(DS_PIN, DQ))   presence = 1; else presence = 0;
//    bit_is_set(DS_PIN, DQ) ? (presence = 1) : (presence = 0);
    delay(20);                                    /* wait for end of timeslot */
    return presence;
/* presence signal returned
 * 0 = presence, 1 = no part
 */
}


/*  delay()
 *  approximately 15us delay - this is vital for DQ !!
 *                             change number of nops to adjust the delay
 */
void delay(u_char cnt)
{
/* 14,7456 MHz ... 1x nop ... 68 ns */
#define delay1us()  _NOP(); \
_NOP(); \
_NOP(); \
_NOP(); \
_NOP(); \
_NOP(); \
_NOP(); \
_NOP(); \
_NOP(); \
_NOP(); \
_NOP(); \
_NOP(); \
_NOP(); \
_NOP()
    COMPRESSDISABLE;
    do
    {                                             /* 1us */
        delay1us();
        delay1us();                               /* 2us */
        delay1us();                               /* 3us */
        delay1us();                               /* 4us */
        delay1us();                               /* 5us */
        delay1us();                               /* 6us */
        delay1us();                               /* 7us */
        delay1us();                               /* 8us */
        delay1us();                               /* 9us */
        delay1us();                               /* 10us */
        delay1us();                               /* 11us */
        delay1us();                               /* 12us */
        delay1us();                               /* 13us */
        delay1us();                               /* 14us */
        delay1us();                               /* 15us */
    }while (--cnt);
    COMPRESSREENABLE;
}


/*    init()
 *    This is here to initialize the 1-Wire bus devices.
 *   Returns:
 *    > 0  ...  Number of appeared thermometeres on the 1-Wire bus.
 *      0  ...  Thermometer not present.
 */
u_char init(void)
{
    u_char tm_cnt = 0;
/* Search_rom() returns 0x80 if thermometer not connected */
    tm_cnt = 0x7F & searchRom(&romData[0]);
    if( tm_cnt )
/* start conversion on all thermometers */
        sampleTemperature(0xFF);
    return tm_cnt;
}
#endif
/* ---------------------------- End Of File ------------------------------ */
