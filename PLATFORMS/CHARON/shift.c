#ifndef _DEV_BOARD_SHIFT_C
#define _DEV_BOARD_SHIFT_C

#include <stdio.h>
#include <avr/io.h>
#include "deprecated.h"
#include "shift.h"

/*
 * Send data to 8x shift LED shift interface.
 */
void DevBoardShiftLedOut(u_char val)
{
    u_char i = 8;

    sbi(SHIFT_DDR, SHIFT_LED_SET_DDR);            /* parallel write -> output pin */
    sbi(SHIFT_DDR, SHIFT_CLK_DDR);                /* serial clock -> output pin */
    sbi(SHIFT_DDR, SHIFT_OUT_DDR);                /* serial data stream -> output pin */

    cbi(SHIFT_PORT, SHIFT_LED_SET);
    cbi(SHIFT_PORT, SHIFT_CLK);

    while (i--)                                   /* send data to 74595, MSB (most significant bit) first */
    {
        if (val & (1 << i))
            sbi(SHIFT_PORT, SHIFT_OUT);
        else
            cbi(SHIFT_PORT, SHIFT_OUT);

        DevBoardShiftDelay(SHIFT_IO_DELAY);       /* small delay between two I/O operations */
        sbi(SHIFT_PORT, SHIFT_CLK);               /* 0 -> 1 data valid */
        DevBoardShiftDelay(SHIFT_IO_DELAY);       /* small delay between two I/O operations */
        cbi(SHIFT_PORT, SHIFT_CLK);
    }

    sbi(SHIFT_PORT, SHIFT_LED_SET);               /* 0 -> 1 parallel write */
}


/*
 * Send data to 2x16 LCD shift interface.
 */
void DevBoardShiftLcdOut(u_char val)
{
    u_char i = 8;

    sbi(SHIFT_DDR, SHIFT_LCD_SET_DDR);            /* parallel write -> output pin */
    sbi(SHIFT_DDR, SHIFT_CLK_DDR);                /* serial clock -> output pin */
    sbi(SHIFT_DDR, SHIFT_OUT_DDR);                /* serial data stream -> output pin */

    cbi(SHIFT_PORT, SHIFT_LCD_SET);
    cbi(SHIFT_PORT, SHIFT_CLK);

    while (i--)                                   /* send data to 74595, MSB (most significant bit) first */
    {
        if (val & (1 << i))
            sbi(SHIFT_PORT, SHIFT_OUT);
        else
            cbi(SHIFT_PORT, SHIFT_OUT);

        DevBoardShiftDelay(SHIFT_IO_DELAY);       /* small delay between two I/O operations */
        sbi(SHIFT_PORT, SHIFT_CLK);               /* 0 -> 1 data valid */
        DevBoardShiftDelay(SHIFT_IO_DELAY);       /* small delay between two I/O operations */
        cbi(SHIFT_PORT, SHIFT_CLK);
    }

    sbi(SHIFT_PORT, SHIFT_LCD_SET);               /* 0 -> 1 parallel write */
}


/*
 * OBSOLETE: Used on dev. board's version 2.8 to access shift io peripherials.
 */
void DevBoardShiftWordOut(u_int val)
{
    u_char i = 16;

    sbi(SHIFT_DDR, SHIFT_LED_SET_DDR);            /* parallel write -> output pin */
    sbi(SHIFT_DDR, SHIFT_CLK_DDR);                /* serial clock -> output pin */
    sbi(SHIFT_DDR, SHIFT_OUT_DDR);                /* serial data stream -> output pin */

    cbi(SHIFT_PORT, SHIFT_LED_SET);
    cbi(SHIFT_PORT, SHIFT_CLK);

    while (i--)                                   /* send data to 74595, MSB (most significant bit) first */
    {
        if (val & (1 << i))
            sbi(SHIFT_PORT, SHIFT_OUT);
        else
            cbi(SHIFT_PORT, SHIFT_OUT);

        DevBoardShiftDelay(SHIFT_IO_DELAY);       /* small delay between two I/O operations */
        sbi(SHIFT_PORT, SHIFT_CLK);               /* 0 -> 1 data valid */
        DevBoardShiftDelay(SHIFT_IO_DELAY);       /* small delay between two I/O operations */
        cbi(SHIFT_PORT, SHIFT_CLK);
    }

    sbi(SHIFT_PORT, SHIFT_LED_SET);               /* 0 -> 1 parallel write */
}


/*
 * Read data from 8x shift input interface.
 */
u_char DevBoardShiftByteIn(void)
{
    u_char in_byte = 0, i = 7;

    sbi(SHIFT_DDR, SHIFT_IN_LOAD_DDR);            /* shift/!load set as output pin */
    sbi(SHIFT_DDR, SHIFT_CLK_DDR);                /* clock set as output pin */
    cbi(SHIFT_DDR, SHIFT_IN_DDR);                 /* serial data set as input pin */

    cbi(SHIFT_PORT, SHIFT_CLK);                   /* set clock low  */
    cbi(SHIFT_PORT, SHIFT_IN_LOAD);               /* set load low  -> parallel in */
    DevBoardShiftDelay(SHIFT_IO_DELAY);           /* small delay between two I/O operations */
    sbi(SHIFT_PORT, SHIFT_IN_LOAD);               /* set load high -> serial out */

    /* read bit, MSB(most significant bit) first */bit_is_set(SHIFT_PIN, SHIFT_IN) ? (in_byte
        |= 1)
        : (in_byte |= 0);

    while (i--)
    {
        sbi(SHIFT_PORT, SHIFT_CLK);               /* 0 -> 1 next databit on serial output is valid */

        DevBoardShiftDelay(SHIFT_IO_DELAY);       /* small delay between two I/O operations */
        in_byte <<= 1;                            /* shift one bit left */

                                                  /* read next bit */
        bit_is_set(SHIFT_PIN, SHIFT_IN) ? (in_byte |= 1) : (in_byte |= 0);

        cbi(SHIFT_PORT, SHIFT_CLK);
        DevBoardShiftDelay(SHIFT_IO_DELAY);
    }

    return (in_byte);                             /* return received byte */
}


/*
 * Delay by executing a given number of NOPs.
 */
void DevBoardShiftDelay(long nops)
{
    volatile long i;

    for (i = 0; i < nops; i++)
#ifdef __IMAGECRAFT__
        asm("nop\n");
#else
    asm volatile ("nop\n\t"::);
#endif
}
#endif

/* ---------------------------- End Of File ------------------------------ */
