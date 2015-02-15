#include <avr/io.h>

/*
 * ASM implementation for fast interrupt handling
 */

.extern interruptVectors //array
.extern interruptThread //function

.global RESET_vect
RESET_vect:		
			PUSH	r26
			LDI	r26,0
			JMP	GENERALINTERRUPT
.global INT0_vect
INT0_vect:		
			PUSH	r26
			LDI	r26,1
			JMP	GENERALINTERRUPT
.global INT1_vect
INT1_vect:		
			PUSH	r26
			LDI	r26,2
			JMP	GENERALINTERRUPT
.global INT2_vect
INT2_vect:	
			PUSH	r26
			LDI	r26,3
			JMP	GENERALINTERRUPT
.global INT3_vect
INT3_vect:		
			PUSH	r26
			LDI	r26,4
			JMP	GENERALINTERRUPT
.global INT4_vect
INT4_vect:	
			PUSH	r26
			LDI	r26,5
			JMP	GENERALINTERRUPT
.global INT5_vect
INT5_vect:		
			PUSH	r26
			LDI	r26,6
			JMP	GENERALINTERRUPT
.global INT6_vect
INT6_vect:		
			PUSH	r26
			LDI	r26,7
			JMP	GENERALINTERRUPT
.global INT7_vect
INT7_vect:	
			PUSH	r26
			LDI	r26,8
			JMP	GENERALINTERRUPT
.global PCINT0_vect
PCINT0_vect:	
			PUSH	r26
			LDI	r26,9
			JMP	GENERALINTERRUPT
.global PCINT1_vect
PCINT1_vect:		
			PUSH	r26
			LDI	r26,10
			JMP	GENERALINTERRUPT
.global PCINT2_vect 
PCINT2_vect:		
			PUSH	r26
			LDI	r26,11
			JMP	GENERALINTERRUPT
.global WDT_vect
WDT_vect:		
			PUSH	r26
			LDI	r26,12
			JMP	GENERALINTERRUPT
#ifndef WITHMON
.global TIMER2_COMPA_vect
TIMER2_COMPA_vect:
			PUSH	r26
			LDI	r26,13
			JMP	GENERALINTERRUPT
#endif
.global TIMER2_COMPB_vect
TIMER2_COMPB_vect:	
			PUSH	r26
			LDI	r26,14
			JMP	GENERALINTERRUPT
.global TIMER2_OVF_vect
TIMER2_OVF_vect:	
			PUSH	r26
			LDI	r26,15
			JMP	GENERALINTERRUPT
.global TIMER1_CAPT_vect
TIMER1_CAPT_vect:
			PUSH	r26
			LDI	r26,16
			JMP	GENERALINTERRUPT
/* used by time in millis
.global TIMER1_COMPA_vect
TIMER1_COMPA_vect:
			PUSH	r26
			LDI	r26,17
			JMP	GENERALINTERRUPT
*/
.global TIMER1_COMPB_vect
TIMER1_COMPB_vect:	
			PUSH	r26
			LDI	r26,18
			JMP	GENERALINTERRUPT
.global TIMER1_COMPC_vect
TIMER1_COMPC_vect:	
			PUSH	r26
			LDI	r26,19
			JMP	GENERALINTERRUPT
.global TIMER1_OVF_vect
TIMER1_OVF_vect:	
			PUSH	r26
			LDI	r26,20
			JMP	GENERALINTERRUPT
.global TIMER0_COMPA_vect
TIMER0_COMPA_vect:	
			PUSH	r26
			LDI	r26,21
			JMP	GENERALINTERRUPT
.global TIMER0_COMPB_vect
TIMER0_COMPB_vect:	
			PUSH	r26
			LDI	r26,22
			JMP	GENERALINTERRUPT
.global TIMER0_OVF_vect
TIMER0_OVF_vect:
			PUSH	r26
			LDI	r26,23
			JMP	GENERALINTERRUPT	
.global SPI_STC_vect
SPI_STC_vect:	
			PUSH	r26
			LDI	r26,24
			JMP	GENERALINTERRUPT
.global USART0_RX_vect
USART0_RX_vect:	
			PUSH	r26
			LDI	r26,25
			JMP	GENERALINTERRUPT
.global USART0_UDRE_vect
USART0_UDRE_vect:
			PUSH	r26
			LDI	r26,26
			JMP	GENERALINTERRUPT
.global USART0_TX_vect
USART0_TX_vect:
			PUSH	r26
			LDI	r26,27
			JMP	GENERALINTERRUPT
.global ANALOG_COMP_vect
ANALOG_COMP_vect:	
			PUSH	r26
			LDI	r26,28
			JMP	GENERALINTERRUPT
.global ADC_vect
ADC_vect:		
			PUSH	r26
			LDI	r26,29
			JMP	GENERALINTERRUPT
.global EE_READY_vect
EE_READY_vect:	
			PUSH	r26
			LDI	r26,30
			JMP	GENERALINTERRUPT
.global TIMER3_CAPT_vect
TIMER3_CAPT_vect:
			PUSH	r26
			LDI	r26,31
			JMP	GENERALINTERRUPT
.global TIMER3_COMPA_vect
TIMER3_COMPA_vect:
			PUSH	r26
			LDI	r26,32
			JMP	GENERALINTERRUPT
.global TIMER3_COMPB_vect
TIMER3_COMPB_vect:
			PUSH	r26
			LDI	r26,33
			JMP	GENERALINTERRUPT
.global TIMER3_COMPC_vect
TIMER3_COMPC_vect:
			PUSH	r26
			LDI	r26,34
			JMP	GENERALINTERRUPT
.global TIMER3_OVF_vect
TIMER3_OVF_vect:
			PUSH	r26
			LDI	r26,35
			JMP	GENERALINTERRUPT
.global USART1_RX_vect
USART1_RX_vect:	
			PUSH	r26
			LDI	r26,36
			JMP	GENERALINTERRUPT
.global USART1_UDRE_vect
USART1_UDRE_vect:
			PUSH	r26
			LDI	r26,37
			JMP	GENERALINTERRUPT
.global USART1_TX_vect
USART1_TX_vect:	
			PUSH	r26
			LDI	r26,38
			JMP	GENERALINTERRUPT
.global TWI_vect
TWI_vect:	
			PUSH	r26
			LDI	r26,39
			JMP	GENERALINTERRUPT
.global SPM_READY_vect
SPM_READY_vect:	
			PUSH	r26
			LDI	r26,40
			JMP	GENERALINTERRUPT
.global TIMER4_CAPT_vect
TIMER4_CAPT_vect:
			PUSH	r26
			LDI	r26,41
			JMP	GENERALINTERRUPT
.global TIMER4_COMPA_vect
TIMER4_COMPA_vect:
			PUSH	r26
			LDI	r26,42
			JMP	GENERALINTERRUPT
.global TIMER4_COMPB_vect
TIMER4_COMPB_vect:
			PUSH	r26
			LDI	r26,43
			JMP	GENERALINTERRUPT
.global TIMER4_COMPC_vect
TIMER4_COMPC_vect:
			PUSH	r26
			LDI	r26,44
			JMP	GENERALINTERRUPT
.global TIMER4_OVF_vect
TIMER4_OVF_vect:
			PUSH	r26
			LDI	r26,45
			JMP	GENERALINTERRUPT
.global TIMER5_CAPT_vect
TIMER5_CAPT_vect:
			PUSH	r26
			LDI	r26,46
			JMP	GENERALINTERRUPT
.global TIMER5_COMPA_vect
TIMER5_COMPA_vect:
			PUSH	r26
			LDI	r26,47
			JMP	GENERALINTERRUPT
.global TIMER5_COMPB_vect
TIMER5_COMPB_vect:
			PUSH	r26
			LDI	r26,48
			JMP	GENERALINTERRUPT
.global TIMER5_COMPC_vect
TIMER5_COMPC_vect:
			PUSH	r26
			LDI	r26,49
			JMP	GENERALINTERRUPT
.global TIMER5_OVF_vect
TIMER5_OVF_vect:
			PUSH	r26
			LDI	r26,50
			JMP	GENERALINTERRUPT
.global USART2_RX_vect
USART2_RX_vect:	
			PUSH	r26
			LDI	r26,51
			JMP	GENERALINTERRUPT
.global USART2_UDRE_vect
USART2_UDRE_vect:
			PUSH	r26
			LDI	r26,52
			JMP	GENERALINTERRUPT
.global USART2_TX_vect
USART2_TX_vect:	
			PUSH	r26
			LDI	r26,53
			JMP	GENERALINTERRUPT
.global USART3_RX_vect
USART3_RX_vect:	
			PUSH	r26
			LDI	r26,54
			JMP	GENERALINTERRUPT
.global USART3_UDRE_vect
USART3_UDRE_vect:
			PUSH	r26
			LDI	r26,55
			JMP	GENERALINTERRUPT
.global USART3_TX_vect
USART3_TX_vect:	
			PUSH	r26
			LDI	r26,56
			JMP	GENERALINTERRUPT

//For every Interrupt the same...
GENERALINTERRUPT:	
			PUSH	r27
			IN	r27,_SFR_IO_ADDR(SREG)	
			PUSH	r27
			PUSH	r1
			PUSH	r24
			PUSH	r25
			PUSH	r30
			PUSH	r31
			EOR	r1,r1 //set to 0
			//load pointer to sram pos of arry
			ldi	ZL, lo8(interruptVectors)
			ldi	ZH, hi8(interruptVectors)
			ADD	r26,r26 //x2 for 16bit
			//add array index to pointer
			ADD	ZL,r26
			ADC	ZH,r1	
			//load content where pointer points to
			LD	r24,Z+
			LD	r25,Z
			CALL	interruptThread

			POP	r31
			POP	r30
			POP	r25
			POP	r24
			POP	r1
			POP	r27
			OUT	_SFR_IO_ADDR(SREG),r27
			POP	r27
			POP	r26
			RETI
