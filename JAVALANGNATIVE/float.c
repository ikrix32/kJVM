/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
/* C-functions for native methods*/
/* native void method -> C-function ret value 0*/
/* native non void method -> c-function ret value 1 (ret value on java -opStack)*/
/* remember:*/
/* invokespecial Operand Stack*/
/* ..., objectref, [arg0, [arg1 ...]] -> ...*/
/* invokestatic: Operand Stack*/
/* ..., [arg0, [arg1 ...]] -> ...*/

//#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"
#include "typedefinitions.h"
#include "kjvm.h"
#include "classfile.h"
#include "stack.h"
#include "heap.h"
#include "float.h"

//
// Simple and fast atof (ascii to float) function.
//
// - Executes about 5x faster than standard MSCRT library atof().
// - An attractive alternative if the number of calls is in the millions.
// - Assumes input is a proper integer, fraction, or scientific format.
// - Matches library atof() to 15 digits (except at extreme exponents).
// - Follows atof() precedent of essentially no error checking.
//
// 09-May-2009 Tom Van Baak (tvb) www.LeapSecond.com
#define white_space(c) ((c) == ' ' || (c) == '\t')
#define valid_digit(c) ((c) >= '0' && (c) <= '9')

double atof (const char *p)
{
    int frac;
    double sign, value, scale;

    // Skip leading white space, if any.
    while (white_space(*p) ) {
        p += 1;
    }

    // Get sign, if any.

    sign = 1.0;
    if (*p == '-') {
        sign = -1.0;
        p += 1;

    } else if (*p == '+') {
        p += 1;
    }

    // Get digits before decimal point or exponent, if any.

    for (value = 0.0; valid_digit(*p); p += 1) {
        value = value * 10.0 + (*p - '0');
    }

    // Get digits after decimal point, if any.

    if (*p == '.') {
        double pow10 = 10.0;
        p += 1;
        while (valid_digit(*p)) {
            value += (*p - '0') / pow10;
            pow10 *= 10.0;
            p += 1;
        }
    }

    // Handle exponent, if any.

    frac = 0;
    scale = 1.0;
    if ((*p == 'e') || (*p == 'E')) {
        unsigned int expon;

        // Get sign of exponent, if any.

        p += 1;
        if (*p == '-') {
            frac = 1;
            p += 1;

        } else if (*p == '+') {
            p += 1;
        }

        // Get digits of exponent, if any.

        for (expon = 0; valid_digit(*p); p += 1) {
            expon = expon * 10 + (*p - '0');
        }
        if (expon > 308) expon = 308;

        // Calculate scaling factor.

        while (expon >= 50) { scale *= 1E50; expon -= 50; }
        while (expon >=  8) { scale *= 1E8;  expon -=  8; }
        while (expon >   0) { scale *= 10.0; expon -=  1; }
    }

    // Return signed and scaled floating point result.
    
    return sign * (frac ? (value / scale) : (value * scale));
}

char floatToCharArray()//300bytes
{
    slot mySlot;
    const f4 f = opStackGetValue(local).Float;          // the float
    char buf[8];

    for (u1 i = 0; i < 8; ++i)
    {
        buf[i] = 0;
    }
#ifdef AVR8
    dtostrf( f, 7, 3, buf );
#else
    snprintf(buf, 8, "%f", f);//krix was 7
#endif
    u2 heapPos = getFreeHeapSpace(8 + 1);         // char arr length + marker
    mySlot.stackObj.pos = heapPos;
    mySlot.stackObj.magic = OBJECTMAGIC;
    //mySlot.stackObj.type=STACKNEWARRAYOBJECT;
    mySlot.stackObj.arrayLength = (u1) 8;         // char array length
    opStackPush(mySlot);
    HEAPOBJECTMARKER(heapPos).status = HEAPALLOCATEDARRAY;
    HEAPOBJECTMARKER(heapPos).magic = OBJECTMAGIC;
    HEAPOBJECTMARKER(heapPos++).mutex = MUTEXNOTBLOCKED;
    for (u1 i = 0; i < 8; i++)
    {
        heapSetElement(toSlot((u4)(*(buf + i))), heapPos++);
    }
    return 1;
}


char typeConvert()
{
    opStackPush(opStackGetValue(local));
    return 1;
}


/* char arr to float*/
char nativeParseFloat()//500bytes
{
    const slot mySlot = opStackGetValue(local);         // the char array
    char buf[mySlot.stackObj.arrayLength];
    f4 f;

    for (u1 i = 0; i < mySlot.stackObj.arrayLength; i++)
        buf[i] = (u1) heapGetElement(mySlot.stackObj.pos + i + 1).UInt;
    buf[mySlot.stackObj.arrayLength] = 0;
#ifdef AVR8
    f = strtod(buf,NULL);
#else
    //sscanf(buf, "%f", &f);
     f = atof(buf);
#endif
    opStackPush(toSlot( f ));
    return 1;
}

