/* This header file is part of the ATMEL AVR32-SoftwareFramework-AT32AP7000-1.0.0 Release */

#ifndef _stk1000_h_
#define _stk1000_h_

#include <avr32/io.h>
#include "led.h"

#define CPU_FAMILY    "AVR32_AP7"
#define CPU_TYPE      "AT32AP7000"
#define MAX_FREQ_HZ   150000000

#define DEFAULT_FREQ  FOSC0
#define DEFAULT_UART  UART_A

#define FOSC32          32768     //!< Osc32 frequency: Hz.
#define OSC32_STARTUP   3         //!< Osc32 startup time: RCOsc periods.

/* Define the on-board oscillators in Hz */
#define FOSC0           20000000  //!< Osc0 frequency: Hz.
#define FOSC1           12000000  //!< Osc1 frequency: Hz.
#define OSC0_STARTUP    3         //!< Osc0 startup time: RCOsc periods.
#define OSC1_STARTUP    3         //!< Osc1 startup time: RCOsc periods.

//! Set onboard SDRAM
#define SDRAM_PART_HDR  "MT481C2M32B2TG/mt481c2m32b2tg.h"

//! Data bus width to use the SDRAM(s)
#define SDRAM_DBW       32



/* Define the peripheral names */
#define UART_A  AVR32_USART1
#define UART_C  AVR32_USART3
#define IRDA    AVR32_USART2

/* Define default MACB */
#define AVR32_MACB      AVR32_MACB0
#define AVR32_MACB_IRQ	AVR32_MACB0_IRQ

/* Using privileged mode P2 to bypass cache*/
#define BOARD_SDRAM_BASE 0xB0000000

//! Number of LEDs.
#define LED_COUNT   8

/*! \name PIO Connections of LEDs
 */
//! @{
#  define LED0_PIO   AVR32_PIOC_P_0_PIN
#  define LED1_PIO   AVR32_PIOC_P_1_PIN
#  define LED2_PIO   AVR32_PIOC_P_2_PIN
#  define LED3_PIO   AVR32_PIOC_P_3_PIN
#  define LED4_PIO   AVR32_PIOC_P_4_PIN
#  define LED5_PIO   AVR32_PIOC_P_5_PIN
#  define LED6_PIO   AVR32_PIOC_P_6_PIN
#  define LED7_PIO   AVR32_PIOC_P_7_PIN
//! @}



/*! \name PWM Channels of LEDs
 */
//! @{
#define LED0_PWM    (-1)
#define LED1_PWM    (-1)
#define LED2_PWM    (-1)
#define LED3_PWM    (-1)
#define LED4_PWM      0
#define LED5_PWM      1
#define LED6_PWM      2
#define LED7_PWM      3
//! @}

/*! \name PWM Functions of LEDs
 */
//! @{
#define LED0_PWM_FUNCTION   (-1)
#define LED1_PWM_FUNCTION   (-1)
#define LED2_PWM_FUNCTION   (-1)
#define LED3_PWM_FUNCTION   (-1)
#define LED4_PWM_FUNCTION   AVR32_PWM_0_FUNCTION
#define LED5_PWM_FUNCTION   AVR32_PWM_1_FUNCTION
#define LED6_PWM_FUNCTION   AVR32_PWM_2_FUNCTION
#define LED7_PWM_FUNCTION   AVR32_PWM_3_FUNCTION
//! @}

/*! \name PWM Functions of LEDs
 */
//! @{
#define LED_MONO0_GREEN LED0
#define LED_MONO1_GREEN LED1
#define LED_MONO2_GREEN LED2
#define LED_MONO3_GREEN LED3

//! @}


#endif
