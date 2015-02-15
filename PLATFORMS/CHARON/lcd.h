/*************************************************************************\ 
 *                  LCD - Liquid Crystal Display                           *
 *-------------------------------------------------------------------------*
 * Description : Header file for programming of LCD display based on       *
 *               HITACHI controller HD44780.                               *
 *-------------------------------------------------------------------------*
 * Author      : Ceba & www.HW.cz          Email       : ceba@hw.cz        *
 * Developed   : 06.02.2002                Last Update : 12.04.2003        *
 * Version     : 0.1.0                                                     *
 *-------------------------------------------------------------------------*
 * Compiler     : ANY                      Version: ANY                    *
 * Source file  : LCD.H                                                    *
 *-------------------------------------------------------------------------*
 * Target system : Charon II. - ATmega128, Quartz 14.7456 MHz              *
 *                 LCD SC1602A - 4.bit mode                                *
 *-------------------------------------------------------------------------*
 *   Instruction     D7  D6  D5  D4  D3  D2  D1  D0                        *
 *   ==============================================                        *
 *   Display clear   0   0   0   0   0   0   0   1                         *
 *   Cursor home     0   0   0   0   0   0   1   *                         *
 *   Entry Mode Set  0   0   0   0   0   1  I/D  S                         *
 *   Display On/Off  0   0   0   0   1   D   C   B                         *
 *   Curs/Disp shift 0   0   0   1  S/C R/L  *   *                         *
 *   Function Set    0   0   1   DL  N   F   *   *                         *
 *   CG RAM addr set 0   1   ---------Acg---------                         *
 *   DD RAM addr set 1   -------------Add---------                         *
 *                                                                         *
 *   Meaning:                                                              *
 *   *     - nonvalid bit                                                  *
 *   Acg   - CG RAM address (CHARACTER GENERATOR)                          *
 *   Add   - DD RAM address (DATA DISPLAY)                                 *
 *   AC    - adress counter                                                *
 *                                                                         *
 *   I/D   - 1-increment, 0-decrement                                      *
 *   S     - 1-display shift, 0-no display shift                           *
 *   D     - 1-display ON, 0-display OFF                                   *
 *   C     - 1-cursor ON, 0-cursor OFF                                     *
 *   B     - 1-blink ON, 0-blink OFF                                       *
 *   S/C   - 1-display shift, 0-cursor movement                            *
 *   R/L   - 1-right shift, 0-left shift                                   *
 *   DL    - 1-8 bits data transfer, 0-4 bits data transfer                *
 *   N     - 1-1/16 duty, 0-1/8 or 1/11 duty                               *
 *   F     - 1-5x10 dot matrix, 0-5x7 dot matrix                           *
 *   BF    - 1-internal operation in progress, 0-display ready             *
 *                                                                         *
\*************************************************************************/
#ifndef _DEV_BOARD_LCD_H
#define _DEV_BOARD_LCD_H

//#include <stdio.h>                     /* standard I/O functions */
//#include "types.h"
#include "shift.h"

#define LCD_RS  0x01                              /* register select */
#define LCD_RW  0x02                              /* read/write */
#define LCD_EN  0x04                              /* chip enable */

/* --- LCD commands --- */

#define cmd_lcd_init            0x38

#define cmd_lcd_clear           0x01
#define cmd_lcd_home            0x02

#define cmd_cur_shift_on        0x06
#define cmd_cur_shift_off       0x07

#define cmd_lcd_on              0x0E
#define cmd_lcd_off             0x0A

#define cmd_cur_on              0x0E
#define cmd_cur_off             0x0C

#define cmd_cur_blink_on        0x0F
#define cmd_cur_blink_off       0x0E

#define cmd_cur_left            0x10
#define cmd_cur_right           0x14

#define cmd_scr_left            0x18
#define cmd_scr_right           0x1C

#define cmd_set_cgram_addr      0x40
#define cmd_set_ddram_addr      0x80

/* --- prototypes --- */

void DelayUs(int i);
void DelayMs(int i);

void LCD_Init( void );
void LCD_WR_CGRAM( u_char char_val, char *char_buf );
void LCD_SendVal( u_char val, u_char RS );
void    LCD_SendCmd(u_char val);

//bh#define LCD_SendCmd( val )  LCD_SendVal( val, 0 )       /* send command */
#define LCD_SendData( val ) LCD_SendVal( val, 1 ) /* send data */
//#define LCD_Write( val )    DevBoardShiftWordOut( val ) /* send serial data */
                                                  /* send serial data */
#define LCD_Write( val )    DevBoardShiftLcdOut( val )

void LCD_Putch( char val );
void LCD_Puts( char *str );
void LCD_Wait( void );
void LCD_Cursor_YX( u_char row, u_char column );
#endif

/* ---------------------------- End Of File ------------------------------ */
