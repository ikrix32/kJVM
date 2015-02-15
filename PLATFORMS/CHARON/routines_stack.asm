#include <avr/io.h>

.extern opSp
.extern opStackBase
.extern methodStackBase
.extern methodSp

/*
 * ASM replacements for stack.c
 * look at stack.c to see c implementations
 */


//Push operant on operant stack
.global opStackPush
opStackPush:		//load global pointer
			lds	ZL,opSp
			lds	ZH,opSp+1
			//store 32bit where pointer points to
			st	Z+,r22
			st	Z+,r23	
			st	Z+,r24	
			st	Z+,r25	
			//Pointer increased 32BIT write back to Ram
			sts	opSp+1,ZH
			sts	opSp,ZL
			ret

//Pop operant from operant stack and return 32Bit value
.global opStackPop
opStackPop:		//load global pointer
			lds	ZL,opSp
			lds	ZH,opSp+1
			//decrement pointer 32bit
			sbiw	ZL,0x04
			//write global pointer back
			sts	opSp+1,ZH
			sts	opSp,ZL
			//load 32bit data where pointer points to
			ld	r22,Z
			ldd	r23,Z+1	
			ldd	r24,Z+2	
			ldd	r25,Z+3	
			ret

//get the pretop from stack without global changes
.global opStackPeek
opStackPeek:
			lds	ZL,opSp
			lds	ZH,opSp+1
			sbiw	ZL, 0x04	; 4
			ld	r22, Z
			ldd	r23, Z+1	; 0x01
			ldd	r24, Z+2	; 0x02
			ldd	r25, Z+3	; 0x03
			ret

//write 32 bit var on pretop of operant stack
.global opStackPoke
opStackPoke:
			lds	ZL,opSp
			lds	ZH,opSp+1
			sbiw	ZL, 0x04	; 4
			st	Z, r22
			std	Z+1, r23	; 0x01
			std	Z+2, r24	; 0x02
			std	Z+3, r25	; 0x03
			ret


.global opStackSetValue
// pos in r24,r45
// val in r20,r21,r22,r23
opStackSetValue:
lds	ZL, opStackBase
lds	ZH, opStackBase+1
//multiplicate pos with 4 because it is a 32 Bit (4Byte) Integer position
add	r24, r24
adc	r25, r25
add	r24, r24
adc	r25, r25
add	ZL, r24
adc	ZH, r25
st	Z, r20
std	Z+1, r21	
std	Z+2, r22	
std	Z+3, r23	
ret

.global opStackGetValue
// pos in r24,r45
// val in r20,r21,r22,r23
opStackGetValue:
lds	ZL, opStackBase
lds	ZH, opStackBase+1
//multiplicate pos with 4 because it is a 32 Bit (4Byte) Integer position
add	r24, r24
adc	r25, r25
add	r24, r24
adc	r25, r25
add	ZL, r24
adc	ZH, r25
ld	r22,Z
ldd	r23,Z+1	
ldd	r24,Z+2	
ldd	r25,Z+3	
ret

.global opStackGetSpPos
opStackGetSpPos:
lds	r18, opStackBase
lds	r19, opStackBase+1
lds	r24, opSp
lds	r25, opSp+1
sub	r24, r18
sbc	r25, r19
asr	r25
ror	r24
asr	r25
ror	r24
ret

.global opStackSetSpPos
opStackSetSpPos:
lds	r18, opStackBase
lds	r19, opStackBase+1
//muliplication by 4 because of 32BIT Slots
add	r24, r24
adc	r25, r25
add	r24, r24
adc	r25, r25
add	r24, r18
adc	r25, r19
sts	opSp+1, r25
sts	opSp, r24
ret



//Push method on operant stack
.global methodStackPush
methodStackPush:		//load global pointer
			lds	ZL,methodSp
			lds	ZH,methodSp+1
			//store 16bit where pointer poits to
			st	Z+,r24
			st	Z+,r25		
			//Pointer increased 32BIT write back to Ram
			sts	methodSp+1,ZH
			sts	methodSp,ZL
			ret

//Pop operant from method stack and return 32Bit value
.global methodStackPop
methodStackPop:		//load global pointer
			lds	ZL,methodSp
			lds	ZH,methodSp+1
			//decrement pointer 32bit
			sbiw	ZL,0x02
			//write global pointer back
			sts	methodSp+1,ZH
			sts	methodSp,ZL
			//load 32bit data where pointer points to
			ld	r24,Z
			ldd	r25,Z+1	
			ret

//get the pretop from stack without global changes
.global methodStackPeek
methodStackPeek:
			lds	ZL,methodSp
			lds	ZH,methodSp+1
			sbiw	ZL, 0x02	
			ld	r24, Z
			ldd	r25, Z+1
			ret

.global methodStackGetSpPos
methodStackGetSpPos:
			lds	r18, methodStackBase
			lds	r19, methodStackBase+1
			lds	r24, methodSp
			lds	r25, methodSp+1
			sub	r24, r18
			sbc	r25, r19
			asr	r25
			ror	r24
			ret

.global methodStackSetSpPos
methodStackSetSpPos:
			lds	r18, methodStackBase
			lds	r19, methodStackBase+1
			//muliplication by 4 because of 32BIT Slots
			add	r24, r24
			adc	r25, r25
			add	r24, r18
			adc	r25, r19
			sts	methodSp+1, r25
			sts	methodSp, r24
			ret

.global methodStackEmpty
methodStackEmpty:
			lds	r18, methodSp
			lds	r19, methodSp+1
			lds	r24, methodStackBase
			lds	r25, methodStackBase+1
			cp	r18, r24
			cpc	r19, r25
			brne	methodNotEqual
			ldi	r24, 0x01
			ret
methodNotEqual:		ldi	r24, 0x00
			ret	
