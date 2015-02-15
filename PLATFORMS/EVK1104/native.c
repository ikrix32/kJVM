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
#include "atmel/adc.h"
#include "atmel/evk1104.h"
#include "atmel/qt60168.h"
#include "atmel/et024006dhu.h"
#include "atmel/led.h"
#ifndef WITHMON
#include "atmel/compiler.h"
#include "atmel/gpio.h"
#include "atmel/rtc.h"
#include "atmel/intc.h"
#include "atmel/usart.h"
#endif
#include "platform.h"

#  define EXAMPLE_ADC_LIGHT_CHANNEL           0
#  define EXAMPLE_ADC_LIGHT_PIN               AVR32_ADC_AD_0_PIN
#  define EXAMPLE_ADC_LIGHT_FUNCTION          AVR32_ADC_AD_0_FUNCTION

volatile avr32_adc_t *adc = &AVR32_ADC;           /* ADC IP registers address*/
/* Assign the on-board sensors to their ADC channel.*/
unsigned short adc_channel_light = EXAMPLE_ADC_LIGHT_CHANNEL;

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
    return usart_getchar(EXAMPLE_USART);
}


void conOut(char c)
{
    while (!usart_tx_ready(EXAMPLE_USART))
        ;
    usart_write_char(EXAMPLE_USART, (int) c);
}


char conStat()
{
    if (usart_test_hit(EXAMPLE_USART))
    {
        opStackPush((slot) (u4) 66);
        return 1;
    }                                             /* input available*/
    if (!usart_tx_ready(EXAMPLE_USART))
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

/*
 ## Author: H.-Christian Hecht, CoMedServ GmbH, IT 2006
 ## Method: int currentTimeMillis(void);
## Parameters: none
## Return Value: int, get the value of the timer
*/
#ifndef WITHMON

#define EXAMPLE_TARGET_PBACLK_FREQ_HZ FOSC0       // PBA clock target frequency, in Hz
void usart1Init()
{
    static const gpio_map_t USART_GPIO_MAP =
    {
        {
            EXAMPLE_USART_RX_PIN,
            EXAMPLE_USART_RX_FUNCTION
        }
        ,
        {
            EXAMPLE_USART_TX_PIN,
            EXAMPLE_USART_TX_FUNCTION
        }
    };

// USART options.
    static const usart_options_t USART_OPTIONS =
    {
        .baudrate = 57600,
        .charlength = 8, .paritytype = USART_NO_PARITY, .stopbits =
        USART_1_STOPBIT, .channelmode = USART_NORMAL_CHMODE
    };
// Assign GPIO to USART.
    gpio_enable_module(USART_GPIO_MAP,
        sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));

// Initialize USART in RS232 mode.
    usart_init_rs232(EXAMPLE_USART, &USART_OPTIONS,
        EXAMPLE_TARGET_PBACLK_FREQ_HZ);
}


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

char initDisplay()
{

/* START init Display */
    delay_init(FOSC0);
    et024006_Init(FOSC0, FOSC0);
    gpio_set_gpio_pin(ET024006DHU_BL_PIN);
    et024006_DrawFilledRect(0, 0, ET024006_WIDTH, ET024006_HEIGHT, WHITE);
/* END init Dispay */
    return 0;

}


char clearDisplay()
{
    et024006_DrawFilledRect(0, 0, ET024006_WIDTH, ET024006_HEIGHT,
        opStackGetValue(local + 1).UInt);
    return 0;
}


char drawPixel()
{
//public native void drawPixel(int x, int y, int color);
    et024006_DrawPixel(opStackGetValue(local + 1).UInt,
        opStackGetValue(local + 2).UInt, opStackGetValue(local + 3).UInt);
    return 0;
}


char drawHorizLine()
{
//public native void drawHorizLine(int x, int y, int length, int color );
    et024006_DrawHorizLine(opStackGetValue(local + 1).UInt,
        opStackGetValue(local + 2).UInt, opStackGetValue(local + 3).UInt,
        opStackGetValue(local + 4).UInt);
    return 0;
}


char drawVertLine()
{
//public native void drawVertLine(int x, int y, int length, int color);
    et024006_DrawVertLine(opStackGetValue(local + 1).UInt,
        opStackGetValue(local + 2).UInt, opStackGetValue(local + 3).UInt,
        opStackGetValue(local + 4).UInt);
    return 0;
}


char drawLine()
{
//public native void drawLine(int x1, int y1, int x2, int y2, int color);
    et024006_DrawLine(opStackGetValue(local + 1).UInt,
        opStackGetValue(local + 2).UInt, opStackGetValue(local + 3).UInt,
        opStackGetValue(local + 4).UInt, opStackGetValue(local + 5).UInt);
    return 0;
}


char drawFillRect()
{
//public native void drawFillRectRGB(int x, int y, int width, int height, int color);
    et024006_DrawFilledRect(opStackGetValue(local + 1).UInt,
        opStackGetValue(local + 2).UInt, opStackGetValue(local + 3).UInt,
        opStackGetValue(local + 4).UInt, opStackGetValue(local + 5).UInt);

    return 0;
}


char drawFilledCircle()
{
// public native void drawFilledCircle(int x, int y, int radius, int color, int quadrants);
    et024006_DrawFilledCircle(opStackGetValue(local + 1).UInt,
        opStackGetValue(local + 2).UInt, opStackGetValue(local + 3).UInt,
        opStackGetValue(local + 4).UInt, opStackGetValue(local + 5).UInt);
    return 0;
}


char drawChar()
{

    const unsigned char *font;
    unsigned char mask = 0, xfont, yfont, font_size;
    const unsigned char *data;
    int index = opStackGetValue(local + 7).UInt;

//get Font-Size
    if (opStackGetValue(local + 2).UInt == 1)
        font = (const unsigned char *) &FONT6x8;
    else if (opStackGetValue(local + 2).UInt == 2)
        font = (const unsigned char *) &FONT8x8;
    else
        font = (const unsigned char *) &FONT8x16;

//get Font offset by String position
    data = font;                                  // point to the start of the font table
    xfont = *data;                                // get font x width

//Char into String
    char c[2] = { opStackGetValue(local + 1).UInt, 0 };

//x offset
    int x = opStackGetValue(local + 3).UInt + (index * xfont);

    et024006_PrintString(c, font, x, opStackGetValue(local + 4).UInt,
        opStackGetValue(local + 5).UInt, opStackGetValue(local + 6).UInt);
    return 0;

}


char getButtons()
{
// public native int getButtons();
// opStackPush((slot) (u4) getButtons());
    opStackPush((slot) (u4) qt60168_report_all_key());
// opStackPush((u8) qt60168_report_all_key());
    return 1;
}


char isButtonPressed()
{
    opStackPush(
        (slot) (u4) qt60168_is_key_pressed(opStackGetValue(local + 1).UInt));
    return 1;
}


char setLEDon()
{
// public native void setLEDon(int LED);
    LED_On(opStackGetValue(local + 1).UInt);
    return 0;
}


char setLEDoff()
{
// public native void setLEDoff(int LED);
    LED_Off(opStackGetValue(local + 1).UInt);
    return 0;
}


char adcInit()
{

/* GPIO pin/adc-function map.*/
    static const gpio_map_t ADC_GPIO_MAP =
    {
        {
            EXAMPLE_ADC_LIGHT_PIN,
            EXAMPLE_ADC_LIGHT_FUNCTION
        }
        ,
    };

/* switch to oscillator 0*/
/*	pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);*/
/* Assign and enable GPIO pins to the ADC function.*/
    gpio_enable_module(ADC_GPIO_MAP,
        sizeof(ADC_GPIO_MAP) / sizeof(ADC_GPIO_MAP[0]));
/* configure ADC*/
    adc_configure(adc);
    return 0;

}


char adcEnable()
{
    adc_enable(adc, adc_channel_light);
    return 0;
}


char adcDisable()
{
    adc_disable(adc, adc_channel_light);
    return 0;
}


char adcGetValue()
{
    int rw;
    signed short adc_value_light = -1;

    adc_start(adc);
    adc_value_light = adc_get_value(adc, adc_channel_light);
    rw = adc_value_light;
    opStackPush((slot)(s4)(rw));
    return 1;
}


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
