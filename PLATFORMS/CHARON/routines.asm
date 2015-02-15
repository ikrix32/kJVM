#include <avr/io.h>
#include <avr/interrupt.h>

.global getU1Flash
.global strncmpFlashFlash
.global strncmpRamFlash
.global getCharsFromFlash
.global printStringFromFlash

#ifndef WITHMON
.global startUart0
#endif

.extern	conOut
/* RAMPZ ist set to 1 in platform.c init()*/
// assume : Z ist free; RAMPZ never changed
getU1Flash:	movw	ZL,r24
			elpm	r24,Z
			clr	r25
			ret

#define	s1	r24
#define	s2	r22
#define	sLength r20	; 8 bit in r20 !!
#define	char1	r26
#define	char2	r27
strncmpFlashFlash:		;(const char *s1, const char *s2, size_t n) {}
				;	r24,r25     r22, r23        r20,r21
				; assume Z is free !!
				; java flash strings strings in upper half of flash 
				; assume RAMPZ == 1 ( set forever in platform.c )
				; assume 1<= n <= 255
				; ret 0 -> strings equal otherwise 1 !!!
				; not compatible to unix strncmp !!
strncmpFlashFlash3:	movw	ZL,s1
			elpm	char1,Z+
			movw	s1,ZL
			movw	ZL,s2
			elpm	char2,Z+
			movw	s2,ZL
			cp	char1,char2
			brne	strncmpFlashFlash1	; the end
			dec	r20
			brne	strncmpFlashFlash3
			clr	r24
			rjmp	strncmpFlashFlash2
strncmpFlashFlash1:	ldi	r24,1
strncmpFlashFlash2:	clr	r25
			ret

strncmpRamFlash:
strncmpRamFlash3:	movw	ZL,s1
			ld	char1,Z+	; in sram
			movw	s1,ZL
			movw	ZL,s2
			elpm	char2,Z+
			movw	s2,ZL
			cp	char1,char2
			brne	strncmpRamFlash1	; the end
			dec	r20
			brne	strncmpRamFlash3
			clr	r24
			rjmp	strncmpRamFlash2
strncmpRamFlash1:	ldi	r24,1
strncmpRamFlash2:	clr	r25
			ret
// from r24 , n r22, dest r20
getCharsFromFlash:	movw	ZL,r24
			movw	XL,r20
getCharsFromFlash1:	elpm	r21,Z+
			st	X+,r21
			dec	r22
			brne	getCharsFromFlash1
			ret
// from r24, n r22, n!==0 !!!!
printStringFromFlash:	movw	ZL,r24
printStringFromFlash1:	elpm	r24,Z+
			call 	0xf006
			dec	r22
			brne	printStringFromFlash1
			ret