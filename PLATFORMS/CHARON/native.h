/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
/* ************************************************************************ */

char nativeCharOut();
char nativeCharIn();
char getButtons();
char setOnBoardLEDs();
char charLCDOut();
char controlLCD();
char currentTimeMillis();
char nativeExit();
char getTemperature();
char getAnalogueValue();
u2 adc_ReadChannel(u1 channel);
char nativeConStat();
char nativeSetBarGraph();
char nativeGetDIL();
char nativeSetPort();
char nativeGetPort();
char nativeSer1CharIn();
char nativeSer1CharOut();
char nativeSer1Stat();

void uart1_putchar(u1 c);
u1 uart1_getchar();
