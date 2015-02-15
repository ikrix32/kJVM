#ifndef _DEV_BOARD_SHIFT_H
#define _DEV_BOARD_SHIFT_H

#ifndef __IMAGECRAFT__
#include <avr/io.h>
#endif

#include "types.h"
#include <string.h>

#define SHIFT_IO_DELAY        1

#define SHIFT_PORT            PORTD
#define SHIFT_PIN             PIND
#define SHIFT_DDR             DDRD

#if 0
/* 74165 */
#define SHIFT_IN_LOAD         PORTD3              /* shift/!load */
#define SHIFT_IN              PORTD4              /* data */
#define SHIFT_CLK             PORTD7              /* serial clock */

/* 74595 */
#define SHIFT_LCD_SET         PORTD1              // parallel write clock (storage register clock)
#define SHIFT_LED_SET         PORTD2              // parallel write clock (storage register clock)
#define SHIFT_OUT             PORTD6              // data
#define SHIFT_IN_LOAD_DDR     DDD3                /* pin direction bit */
#define SHIFT_IN_DDR          DDD4                /* pin direction bit */
#define SHIFT_CLK_DDR         DDD7                /* pin direction bit */

#define SHIFT_LCD_SET_DDR     DDD1                // pin direction bit
#define SHIFT_LED_SET_DDR     DDD2                // pin direction bit
#define SHIFT_OUT_DDR         DDD6                // pin direction bit
#endif

/* 74165 */
#define SHIFT_IN_LOAD         PORTD2              /* shift/!load */
#define SHIFT_IN              PORTD4              /* data */
#define SHIFT_CLK             PORTD7              /* serial clock */

/* 74595 */
#define SHIFT_LCD_SET         PORTD1              // parallel write clock (storage register clock)
#define SHIFT_LED_SET         PORTD5              // parallel write clock (storage register clock)
#define SHIFT_OUT             PORTD6              // data
#define SHIFT_IN_LOAD_DDR     DDD2                /* pin direction bit */
#define SHIFT_IN_DDR          DDD4                /* pin direction bit */
#define SHIFT_CLK_DDR         DDD7                /* pin direction bit */

#define SHIFT_LCD_SET_DDR     DDD1                // pin direction bit
#define SHIFT_LED_SET_DDR     DDD5                // pin direction bit
#define SHIFT_OUT_DDR         DDD6                // pin direction bit
// --- Prototypes --- //

void DevBoardShiftLedOut(u_char val);
void DevBoardShiftLcdOut(u_char val);
void DevBoardShiftWordOut(u_int val);
u_char DevBoardShiftByteIn(void);
void DevBoardShiftDelay(long nops);
void DevBoardShiftLcdOut(u_char val);
#endif
