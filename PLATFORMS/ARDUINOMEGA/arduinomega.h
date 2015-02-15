#define baud        57600                         //115200
#define OCR1ATIME   (F_CPU/1000)
#if baud == 115200
#define bdteiler    (F_CPU/(16*baud))             // Baud-Divider
#else
#define bdteiler    (F_CPU/(16*baud)-1)           // Baud-Divider
#endif
#define     MYUDR       UDR0                      // usart data reg 0/1
#define     MYUDRE      UDRE0
#define     MYUCSRA     UCSR0A                    // usart control/status reg A,B,C 0,1
#define     MYUCSRB     UCSR0B
#define     MYUCSRC     UCSR0C
#define     MYUCSZ      UCSZ00
#define     MYRXC       RXC0
#define     MYUBRRH     UBRR0H                    // usart baud rate reg H,L 0,1
#define     MYUBRRL     UBRR0L
#define     MYUSBS      USBS0
#define     MYRXEN      RXEN0
#define     MYTXEN      TXEN0
/*
 #ifdef ARDUINOMEGA
 #define		MYOCIE2		OCIE2A
#define		MYOCF2		OCF2A
#define		MYEEWE		EEPE
#define		MYEEMWE		EEMPE
#define		MYOCR2		OCR2A
#define		MYTIMSK		TIMSK2
#define		MYTCCR2VAL	1
#define		MYTCCR2		TCCR2B
#define		MYTIFR		TIFR2
*/
