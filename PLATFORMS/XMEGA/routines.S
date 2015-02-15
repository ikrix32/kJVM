// safe safer safest
#include <avr/io.h>
#include <avr/interrupt.h>

.global getU1Flash
.global strncmpFlashFlash
.global strncmpRamFlash
.global getCharsFromFlash
.global printStringFromFlash


// assume classfiles in flash between 64k and 128k (bytes)
getU1Flash:		push	ZL
			push	ZH
			movw	ZL,r24
			ldi	r24,1
			in	r25,RAMPZ
			out	RAMPZ,r24
			elpm	r24,Z
			out	RAMPZ,r25
			clr	r25
			pop	ZH
			pop	ZL
			ret

#define	s1	r24
#define	s2	r22
#define	sLength r20	; 8 bit in r20 !!
#define	char1	r26
#define	char2	r27
strncmpFlashFlash:		;(const char *s1, const char *s2, size_t n) {}
				;	r24,r25     r22, r23        r20,r21
				; java flash strings strings in upper half of flash 
				; assume RAMPZ == 1 ( set forever in platform.c )
				; assume 1<= n <= 255
				; ret 0 -> strings equal otherwise 1 !!!
				; not compatible to unix strncmp !!
strncmpFlashFlash3:	push	ZL
			push	ZH
			push	char1
			push	char2
			push	r21
			in	r21,RAMPZ
			ldi	char1,1
			out	RAMPZ,char1
strncmpFlashFlash33:	movw	ZL,s1
			elpm	char1,Z+
			movw	s1,ZL
			movw	ZL,s2
			elpm	char2,Z+
			movw	s2,ZL
			cp	char1,char2
			brne	strncmpFlashFlash1	; the end
			dec	r20
			brne	strncmpFlashFlash33
			clr	r24
			rjmp	strncmpFlashFlash2
strncmpFlashFlash1:	ldi	r24,1
strncmpFlashFlash2:	clr	r25
			out	RAMPZ,r21
			pop	r21
			pop	char2
			pop	char1
			pop	ZH
			pop	ZL
			ret

strncmpRamFlash:
strncmpRamFlash3:	push	ZL
			push	ZH
			push	XL
			push	XH
			push	r16
			push	r17
			push	r21
			in	r21,RAMPZ
      			ldi	r16,1
			out	RAMPZ,r16
strncmpRamFlash33:	movw	XL,s1
			ld	r16,X+	
; in sram damed rampx, what do you do
			movw	s1,XL
			movw	ZL,s2
			elpm	r17,Z+
			movw	s2,ZL
			cp	r16,r17
			brne	strncmpRamFlash1	; the end
			dec	r20
			brne	strncmpRamFlash33
			clr	r24
			rjmp	strncmpRamFlash2
strncmpRamFlash1:	ldi	r24,1
strncmpRamFlash2:	clr	r25
			out	RAMPZ,r21
			pop	r21
			pop	r17
			pop	r16
			pop	XH
			pop	XL
			pop	ZH
			pop	ZL
			ret

// from r24 , n r22, dest r20
getCharsFromFlash:	push	ZL
			push	ZH
			push	XL
			push	XH
			push	r23
			in	r23,RAMPZ
			ldi	XL,1
			out	RAMPZ,XL
			movw	ZL,r24
			movw	XL,r20
getCharsFromFlash1:	elpm	r24,Z+
			st	X+,r24		; rampx !!!!
			dec	r22
			brne	getCharsFromFlash1
			out	RAMPZ,r23
			pop	r23
			pop	XH
			pop	XL
			pop	ZH
			pop	ZL
			ret

// from r24, n r22, n!==0 !!!!
printStringFromFlash:	push	ZL
			push	ZH
			movw	ZL,r24
			in	r25,RAMPZ
			ldi	r24,1
			out	RAMPZ,r24
printStringFromFlash1:	elpm	r24,Z+
			push	r22
			push 	ZL
			push 	ZH
			push	r25
			push	r26
			ldi	r26,0
			out	RAMPZ,r26
			pop	r26
			call 	conOut
			push	r26
			ldi	r26,1
			out	RAMPZ,r26
			pop	r26
			pop	r25
			pop 	ZH
			pop 	ZL
			pop	r22
			dec	r22
			brne	printStringFromFlash1
			out	RAMPZ,r25
			pop	ZH
			pop 	ZL
			ret
.ascii		"end of world"  // really???
.align 1
avr8bootpack: