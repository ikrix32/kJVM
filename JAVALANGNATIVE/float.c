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

extern u1 local;
/*
char floatToCharArray()//300bytes
{
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
    slot mySlot;
    mySlot.stackObj.arrayLength = (u2) 8;// char array length
    //mySlot.stackObj.type=STACKNEWARRAYOBJECT;

    const u2 heapPos = heapAllocElement(8,HEAPALLOCATEDARRAY,&mySlot.stackObj);
    opStackPush(mySlot);

    for (u1 i = 0; i < 8; i++)
        heapSetElement(toSlot((u4)(*(buf + i))), heapPos + i);

    return 1;
}*/


char typeConvert()
{
    opStackPush(opStackGetValue(local));
    return 1;
}


/* char arr to float* /
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
    sscanf(buf, "%f", &f);
#endif
    opStackPush(toSlot( f ));
    return 1;
}*/

