/*************************************************************************\ 
 *               Development board -  LCD interface                        *
 *-------------------------------------------------------------------------*
 * Description :                                                           *
 *-------------------------------------------------------------------------*
 * Author      : Ceba & www.HW.cz          Email       : ceba@hw.cz        *
 * Developed   : 12.04.2003                Last Update : 12.04.2003        *
 * Version     : 0.1.0                                                     *
 *-------------------------------------------------------------------------*
 * Compiler    : AVR-GCC                   Version : 3.3                   *
 * Source file : lcd.c                                                     *
 * Licence     : www.HW.cz - GNU GPL                                       *
 *-------------------------------------------------------------------------*
 * Target system : Charon II. - ATmega128, Quartz 14.7456 MHz              *
 *                              UART0 - 9600,N,8,1                         *
 *-------------------------------------------------------------------------*
 * History :                                                               *
 * 12.04.03 - lcd code ported from x51 platform                            *
 *            FIXME: recalculate correct delay values                      *
\*************************************************************************/
#ifndef _DEV_BOARD_LCD_C
#define _DEV_BOARD_LCD_C

#include <avr/io.h>
#include "lcd.h"

void LCD_WR_CGRAM(u_char char_val, char *char_buf)
{
    u_char i = 8;

    char_val <<= 3;                               /* The character definition byte start at offset char_val * 8. */
    LCD_SendCmd(cmd_set_cgram_addr | char_val);   /* Set CGRAM address. */

    while (i--)
        LCD_SendData(*char_buf++);                /* Send CGRAM data (character definition). */

    LCD_SendCmd(cmd_set_ddram_addr);              /* Set DDRAM address. */
}


/* --- move LCD cursor to X,Y position --- */
void LCD_Cursor_YX(u_char row, u_char column)
{
    u_char i;

    switch (row)
    {
        case 0:
            i = 0x80 + column;
            break;
        case 1:
            i = 0xC0 + column;
            break;
        case 2:
            i = 0x90 + column;
            break;
        case 3:
            i = 0xD0 + column;
            break;
        default:
            i = 0x80 + column;
            break;
    }

    LCD_SendCmd(i);
}


/* --- send character to LCD --- */
void LCD_Putch(char val)
{
/* FIXME: add support for formatting output characters
 static u_char xy = 0;

if(val = '\n')
{ if(++xy > 1) xy = 0;
LCD_Cursor_YX( xy, 0 );
}
else LCD_SendData( val );
*/

    LCD_SendData(val);
}


/* --- display string on LCD --- */
void LCD_Puts(char *str)
{
    while (*str)
        LCD_Putch(*str++);
}


void DelayUs(int j)
{
/* 1 nop cca 70ns ... quartz 14.756MHz
 * 15 * 70ns = 1050ns = 1.05 us
 */

    while (j--)
        DevBoardShiftDelay(15);
}


void DelayMs(int i)
{
    while (i--)
        DelayUs(10);                              /* FIXME: correct delay value ??? //1000 */
}


/* --- wait until LCD finish current operation --- */
void LCD_Wait(void)
{
    DelayMs(10);                                  //5); /* 10 */
}


/* --- send data to LCD --- */
void LCD_SendVal(u_char val, u_char RS)
{
    if (RS)                                       /* RS ... 0 (command), RS ... 1 (data) */
    {
        LCD_Write(LCD_RS | LCD_EN | (val & 0xF0));/* high nibble */
        LCD_Write(LCD_RS | (val & 0xF0));         /* high nibble */

        LCD_Write(LCD_RS | LCD_EN | (val << 4));  /* low nibble */
        LCD_Write(LCD_RS | (val << 4));           /* low nibble */
    }
    else
    {
        LCD_Write(LCD_EN | (val & 0xF0));         /* high nibble */
        LCD_Write((val & 0xF0));                  /* high nibble */

        LCD_Write(LCD_EN | (val << 4));           /* low nibble */
        LCD_Write((val << 4));                    /* low nibble */
    }

    LCD_Wait();                                   /* The busy flag must be checked after the 4-bit data */
/* has been transferred twice. */
}


/* --- Initializing by Instruction - 4-bit initialization --- */
void LCD_Init(void)                               /* all delays are vital */
{
    DelayMs(100);                                 /* power on delay - wait more than 15 ms */

    LCD_Write(0);                                 /* set RS, RW and EN low */

    LCD_Write(LCD_EN | 0x30);                     /* lcd enable high */
    LCD_Write(0x30);                              /* lce enable low */
    DelayMs(5);                                   /* wait more than 4.1 ms */

    LCD_Write(LCD_EN | 0x30);                     /* lcd enable high */
    LCD_Write(0x30);                              /* lcd enable low */
    DelayUs(100);                                 /* wait more than 100us */

    LCD_Write(LCD_EN | 0x30);                     /* lcd enable high */
    LCD_Write(0x30);                              /* lcd enable low */
    DelayUs(100);                                 /* wait more than 100us */

    LCD_Write(LCD_EN | 0x20);                     /* lcd enable high */
    LCD_Write(0x20);                              /* lcd enable low */
    DelayUs(100);                                 /* wait more than 100us */

    LCD_SendCmd(0x28);                            /* 4 bit mode, 1/16 duty, 5x8 font */
    LCD_SendCmd(0x08);                            /* display off */
    LCD_SendCmd(0x01);                            /* display clear */
    LCD_SendCmd(0x06);                            /* entry mode */
    LCD_SendCmd(0x0E);                            /* display on, blinking cursor off */
}


void LCD_SendCmd(u_char val)
{
    LCD_SendVal(val, 0);
}                                                 /* send command */
#endif
