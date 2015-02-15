/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
/* C-functions for native methods*/
/* native void method -> C-function ret value 0*/
/* native non void method -> c-cunction ret value 1 (ret value on java -opStack)*/
/* remember:*/
/* invokespecial Operand Stack*/
/* ..., objectref, [arg0, [arg1 ...]] -> ...*/
/* invokestatic: Operand Stack*/
/* ..., [arg0, [arg1 ...]] -> ..*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../JVM/definitions.h"
#include "../../JVM/bajvm.h"
#include "../../JVM/stack.h"
#include "../../JVM/classfile.h"
#include "../../JVM/scheduler.h"
#include "../../JVM/heap.h"
#include "native.h"
#ifndef WITHMON
#define EVK1100           1                       //!< AT32UC3A EVK1100 board.
#define EVK1100ES   EVK1100                       //           1   //!< AT32UC3A EVK1100 board.
#include "atmel/evk1100.h"
#include "atmel/compiler.h"
#include "atmel/gpio.h"
#include "atmel/evk1100.h"
#include "atmel/dip204.h"
#include "atmel/rtc.h"
#include "atmel/intc.h"
#include "atmel/usart.h"
#include "atmel/pwm.h"
#include "atmel/adc.h"
#endif
#include "platform.h"

/* its the evk1100*/
/*DEFINES FOR EVK1100 (Steffen Kalisch)*/
/* Connection of the temperature sensor*/
#  define EXAMPLE_ADC_TEMPERATURE_CHANNEL     0
#  define EXAMPLE_ADC_TEMPERATURE_PIN         AVR32_ADC_AD_0_PIN
#  define EXAMPLE_ADC_TEMPERATURE_FUNCTION    AVR32_ADC_AD_0_FUNCTION
/* Connection of the light sensor*/
#  define EXAMPLE_ADC_LIGHT_CHANNEL           2
#  define EXAMPLE_ADC_LIGHT_PIN               AVR32_ADC_AD_2_PIN
#  define EXAMPLE_ADC_LIGHT_FUNCTION          AVR32_ADC_AD_2_FUNCTION
/* Connection of the potentiometer*/
#  define EXAMPLE_ADC_POTENTIOMETER_CHANNEL   1
#  define EXAMPLE_ADC_POTENTIOMETER_PIN       AVR32_ADC_AD_1_PIN
#  define EXAMPLE_ADC_POTENTIOMETER_FUNCTION  AVR32_ADC_AD_1_FUNCTION

char nativeCharOut()
{
    char val = opStackGetValue(local + 1).UInt;
    printf("%c", val);
    return 0;
}


char nativeCharIn()
{
    char ch = conIn();
    opStackPush((slot) (u4) ch);
    return 1;
}


#ifndef WITHMON
char conIn()
{
    return usart_getchar(&AVR32_USART0);
}


void conOut(char c)
{
    while (!usart_tx_ready(&AVR32_USART0))
        ;
    usart_write_char(&AVR32_USART0, (int) c);
}


char conStat()
{
    if (usart_test_hit(&AVR32_USART0))
    {
        opStackPush((slot) (u4) 66);
        return 1;
    }                                             /* input available*/
    if (!usart_tx_ready(&AVR32_USART0))
    {
        opStackPush((slot) (u4) 77);
        return 1;
    }                                             /* no input, transmit ready*/
    opStackPush((slot) (u4) 0);                   /* no char available, transmit no ready*/
    return 1;
}


#else
char conStat()
{
    opStackPush((slot)(u4)ConStat());
    return 1;
}
#endif

#ifndef WITHMON
/* Robert Vietzke HMI Berlin-Wannsee 2009*/
/*gpio_clr_gpio_pin(unsigned int pin)*/
/*gpio_set_gpio_pin(unsigned int pin)*/
#define MYRTS AVR32_PIN_PA08
#define MYDTR AVR32_PIN_PB25

char ser1RTS()
{
/*if (opStackGetValue(local+1).UInt==0)*/
/*AVR32_USART1.cr |=AVR32_USART_CR_RTSDIS_MASK;// rts to 1*/
/*else AVR32_USART1.cr |=AVR32_USART_CR_RTSEN_MASK;*/
    if (opStackGetValue(local + 1).UInt == 0)
        gpio_clr_gpio_pin(MYRTS);
    else
        gpio_set_gpio_pin(MYRTS);
    return 0;
}


char ser1DTR()
{
/*if (opStackGetValue(local+1).UInt==0)*/
/*AVR32_USART1.cr |=AVR32_USART_CR_DTRDIS_MASK;*/
/*else AVR32_USART1.cr |=AVR32_USART_CR_DTREN_MASK;*/
/*printf("jetzt dtr\n");*/
    if (opStackGetValue(local + 1).UInt == 0)
        gpio_clr_gpio_pin(MYDTR);
    else
        gpio_set_gpio_pin(MYDTR);
    return 0;
}


char ser1In()
{
    opStackPush((slot) (u4) usart_getchar(&AVR32_USART1));
    return 1;
}


char ser1Out()
{
    while (!usart_tx_ready(&AVR32_USART1))
        ;
    usart_write_char(&AVR32_USART1, (int) opStackGetValue(local + 1).UInt);
    return 0;
}


char ser1Stat()
{

    return 1;
}


#else
char ser1Stat()
{

    return 1;
}


char ser1Out()
{
    Ser1Out(opStackGetValue(local+1).UInt);
    return 0;
}


char ser1In()
{
    opStackPush((slot)(int)Ser1In());
    return 1;
}


char ser1RTS()
{
    Ser1RTS(opStackGetValue(local+1).UInt);
    return 0;
}


char ser1DTR()
{
    Ser1DTR(opStackGetValue(local+1).UInt);
    return 0;
}
#endif

#ifndef WITHMON
/*
 ## Author: Adrian Lang, Fritz-Haber-Institut, IT 2006
 ## Method: byte getButtons();
## Parameters: none
## Return Value:
##     Button State in 8 Bit array (0: Button not pressed; 1: Button pressed)
##     MSB                        ..                      LSB
##     PB0  PB1  PB2 JoystickPress JSUp JSRight JSDown JSLeft
*/
char getButtons()
{
    int buttons[8] =
    {
        GPIO_PUSH_BUTTON_0, GPIO_PUSH_BUTTON_1,
        GPIO_PUSH_BUTTON_2, GPIO_JOYSTICK_PUSH, GPIO_JOYSTICK_UP,
        GPIO_JOYSTICK_RIGHT, GPIO_JOYSTICK_DOWN, GPIO_JOYSTICK_LEFT
    };
    u1 n = 0;
    u1 ch = 0;
    for (; n < 8; n++)
        gpio_enable_pin_glitch_filter(buttons[n]);

    for (n = 7; n < 255; n--)
        ch = (ch << 1) | !gpio_get_pin_value(buttons[n]);
    opStackPush((slot) (u4) ch);
    return 1;
}


#else
char getButtons()
{
    opStackPush((slot) (u4) GetButtons());
    return 1;
}
#endif

#ifndef WITHMON
/*
 ## Author: Adrian Lang, Fritz-Haber-Institut, IT 2006
 ## Method: void setOnBoardLEDs(char);
## Parameters:
##     char state which is to be set
##     MSB                          ..                         LSB
##     LED1 LED2 LED3 LED4 LED5_red LED5_green LED6_red LED6_green
## Return Value: void
*/
char setOnBoardLEDs()
{
    char state = opStackGetValue(local + 1).UInt;
    int LEDs[8] =
    {
        LED0_GPIO, LED1_GPIO, LED2_GPIO, LED3_GPIO, LED4_GPIO,
        LED5_GPIO, LED6_GPIO, LED7_GPIO
    };
    int n = 0;

    (state & 0x80) ? gpio_clr_gpio_pin(LEDs[0]) : gpio_set_gpio_pin(LEDs[0]);
    for (n = 1; n <= 7; ++n)
    {
        ((state << n) & 0x80) ? gpio_clr_gpio_pin(LEDs[n]) : gpio_set_gpio_pin(
            LEDs[n]);
    }

    return 0;
}


#else
char setOnBoardLEDs()
{
    SetOnBoardLEDs( opStackGetValue(local+1).UInt);
    return 0;
}
#endif
#ifndef WITHMON
char charLCDOut()
{
    char c = opStackGetValue(local + 1).UInt;
    dip204_write_data(c);
    return 0;
}


#else
char charLCDOut()
{
    char c = opStackGetValue(local+1).UInt;
    Dip204_write_data(c);
    return 0;
}
#endif
/* 0000 0000 -> clear*/
/* 0000 0001 -> showCursor*/
/* 0000 0002 -> hideCursor*/
/* 0001 xxyy -> setCursor*/
#ifndef WITHMON
char controlLCD()
{
    int control = opStackGetValue(local + 1).UInt;
    switch (control)
    {
        case 0x0000:
            dip204_clear_display();
            CASE 0x0001: dip204_show_cursor();
            CASE 0x0002: dip204_hide_cursor(); break;
/* Parameters of set_cursor_position: lower 2 Byte and higher 2 Byte of control */
        default:
            dip204_set_cursor_position(((control & 0x0000ff00) >> 8) + 1,
                (control & 0x000000ff) + 1);
            break;
    };
    return 0;
}


#else
char controlLCD()
{
    ControlLCD(opStackGetValue(local+1).UInt);
    return 0;
}
#endif

/*
 ## Author: H.-Christian Hecht, CoMedServ GmbH, IT 2006
 ## Method: int currentTimeMillis(void);
## Parameters: none
## Return Value: int, get the value of the timer
*/
#ifndef WITHMON
char currentTimeMillis()
{
    opStackPush((slot) (u4) timerMilliSec);
    return 1;
}


#else
char currentTimeMillis()
{
    opStackPush((slot) (u4) GetMilliSec());
    return 1;
}
#endif

#ifndef WITHMON
char nativeExit()
{
    exit(opStackGetValue(local + 1).UInt);
    return 0;
}


#else
char nativeExit()
{
    Monitor(opStackGetValue(local+1).UInt);

    return 0;
}
#endif

/*			Felix Fehlberg; FHW-BA Berlin; Berliner Volksbank eG*/
#ifndef WITHMON
char pwmStart()
{
    int channel = opStackGetValue(local + 1).UInt;
    int pulseLength = opStackGetValue(local + 2).UInt;
    int frequency = opStackGetValue(local + 3).UInt;
    pwm_opt_t pwm_opt;                            /* PWM option config.*/
    avr32_pwm_channel_t pwm_channel;              /* One channel config.*/
    gpio_enable_module_pin(AVR32_PWM_0_PIN, AVR32_PWM_0_FUNCTION);
/* PWM controller configuration.*/
    pwm_opt.diva = AVR32_PWM_DIVA_CLK_OFF;
    pwm_opt.divb = AVR32_PWM_DIVB_CLK_OFF;
    pwm_opt.prea = AVR32_PWM_PREA_MCK;
    pwm_opt.preb = AVR32_PWM_PREB_MCK;
    pwm_init(&pwm_opt);
    pwm_channel.CMR.calg = PWM_MODE_LEFT_ALIGNED; /* Channel mode.*/
    pwm_channel.CMR.cpol = PWM_POLARITY_LOW;      /* Channel polarity.*/
    pwm_channel.CMR.cpd = PWM_UPDATE_DUTY;        /* Not used the first time.*/
                                                  /* Channel prescaler.*/
    pwm_channel.CMR.cpre = AVR32_PWM_CPRE_MCK_DIV_256;
    pwm_channel.cdty = pulseLength;               /* Channel pulse length, should be < CPRD.*/
    pwm_channel.cprd = frequency;                 /* Channel frequency.*/
    pwm_channel.cupd = 0;                         /* Channel update is not used here.*/
    pwm_channel_init(channel, &pwm_channel);      /* Set channel configuration to channel*/
    pwm_start_channels(1 << channel);             /* Start channel	*/
    return (0);
}


char pwmStop()
{
    int channel = opStackGetValue(local + 1).UInt;
    avr32_pwm_channel_t pwm_channel;              /* One channel config.*/
    pwm_channel_init(channel, &pwm_channel);      /* Set channel configuration to channel*/
    pwm_stop_channels(1 << channel);              /* Stop channel*/
    return (0);
}


#else
char pwmStart()
{
    int channel = opStackGetValue(local+1).UInt;
    int pulseLength = opStackGetValue(local+2).UInt;
    int frequency = opStackGetValue(local+3).UInt;
    PwmStart(channel,pulseLength,frequency);
    return 0;
}


char pwmStop()
{
    int channel = opStackGetValue(local+1).UInt;
    PwmStop(channel);
    return 0;
}
#endif

/* function adcGetValue: do init and loop to display ADC values (Temp / Light / Poti)*/
/* added 13.03.2009 by: Steffen Kalisch, FHW-BA Berlin*/
/* Sana IT-Services GmbH*/
#ifndef WITHMON
char adcGetValue()
{
    int channel = opStackGetValue(local + 1).UInt;
    int rw;
/* GPIO pin/adc-function map.*/
    static const gpio_map_t ADC_GPIO_MAP =
    {
        {
            EXAMPLE_ADC_TEMPERATURE_PIN,
            EXAMPLE_ADC_TEMPERATURE_FUNCTION
        }
        ,
        {
            EXAMPLE_ADC_LIGHT_PIN,
            EXAMPLE_ADC_LIGHT_FUNCTION
        }
        ,
        {
            EXAMPLE_ADC_POTENTIOMETER_PIN,
            EXAMPLE_ADC_POTENTIOMETER_FUNCTION
        }
    };
    volatile avr32_adc_t *adc = &AVR32_ADC;       /* ADC IP registers address*/
    signed short adc_value_temp = -1;
    signed short adc_value_light = -1;
    signed short adc_value_pot = -1;
/* Assign the on-board sensors to their ADC channel.*/
    unsigned short adc_channel_temp = EXAMPLE_ADC_TEMPERATURE_CHANNEL;
    unsigned short adc_channel_light = EXAMPLE_ADC_LIGHT_CHANNEL;
    unsigned short adc_channel_pot = EXAMPLE_ADC_POTENTIOMETER_CHANNEL;
/* switch to oscillator 0*/
/*	pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);*/
/* Assign and enable GPIO pins to the ADC function.*/
    gpio_enable_module(ADC_GPIO_MAP,
        sizeof(ADC_GPIO_MAP) / sizeof(ADC_GPIO_MAP[0]));
/* configure ADC*/
    adc_configure(adc);
/* get value for first adc channel (TEMP)*/
    if (channel == 1)
    {
        adc_enable(adc, adc_channel_temp);
        adc_start(adc);
        adc_value_temp = adc_get_value(adc, adc_channel_temp);
        adc_disable(adc, adc_channel_temp);
        rw = adc_value_temp;
    }
/* get value for second adc channel (LIGHT)*/
    if (channel == 2)
    {
        adc_enable(adc, adc_channel_light);
        adc_start(adc);
        adc_value_light = adc_get_value(adc, adc_channel_light);
        adc_disable(adc, adc_channel_light);
        rw = adc_value_light;
    }
/* get value for third adc channel (POTI)*/
    if (channel == 3)
    {
        adc_enable(adc, adc_channel_pot);
        adc_start(adc);
        adc_value_pot = adc_get_value(adc, adc_channel_pot);
        adc_disable(adc, adc_channel_pot);
        rw = adc_value_pot;
    }
/*   	}*/
    opStackPush((slot)(s4)(rw));
    return (1);
}


#else
char adcGetValue()
{
    int channel = opStackGetValue(local+1).UInt;
    int mode = opStackGetValue(local+2).UInt;
    int rw;
    rw=AdcGetValue(channel, mode);
    opStackPush((slot)(s4)(rw));
    return(1);
}
#endif
