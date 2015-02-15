
 /*! \name USART Settings
 */

#  define EXAMPLE_USART               (&AVR32_USART1)
#  define EXAMPLE_USART_RX_PIN        AVR32_USART1_RXD_0_0_PIN
#  define EXAMPLE_USART_RX_FUNCTION   AVR32_USART1_RXD_0_0_FUNCTION
#  define EXAMPLE_USART_TX_PIN        AVR32_USART1_TXD_0_0_PIN
#  define EXAMPLE_USART_TX_FUNCTION   AVR32_USART1_TXD_0_0_FUNCTION
#  define EXAMPLE_USART_CLOCK_MASK    AVR32_USART1_CLK_PBA
#  define EXAMPLE_PDCA_CLOCK_HSB      AVR32_PDCA_CLK_HSB
#  define EXAMPLE_PDCA_CLOCK_PB       AVR32_PDCA_CLK_PBA
/*
#  define EXAMPLE_USART               (&AVR32_USART0)
#  define EXAMPLE_USART_RX_PIN        AVR32_USART0_RXD_0_PIN
#  define EXAMPLE_USART_RX_FUNCTION   AVR32_USART0_RXD_0_FUNCTION
#  define EXAMPLE_USART_TX_PIN        AVR32_USART0_TXD_0_PIN
#  define EXAMPLE_USART_TX_FUNCTION   AVR32_USART0_TXD_0_FUNCTION
*/


/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
#include <stdio.h>
#include "evk1104.h"


#include "usart.h"

char	conIn();
char 	myConIn();
char 	conInWithoutESC();
char 	hexConIn();
char	byteConIn(char* pc);
char	wordConIn(unsigned short int* pi);
char 	addressConIn(char** addr);
void	conOut(char c);
char	conInEcho();
void	hexConOut(char c);
void	byteConOut(char c);
void	wordConOut(unsigned int addr);
char 	ascii2Hex(char c);	

void clearScreen();

int __attribute__((weak)) _read (int file, char * ptr, int len);
int __attribute__((weak)) _write (int file, char * ptr, int len); 
void stdIOInit();
void usart0Init();
void usart1Init();
void initTimer();

