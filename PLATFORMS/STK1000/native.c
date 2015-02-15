/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
// C-functions for native methods
// native void method -> C-function ret value 0
// native non void method -> c-cunction ret value 1 (ret value on java -opStack)
// remember:
// invokespecial Operand Stack
// ..., objectref, [arg0, [arg1 ...]] -> ...
// invokestatic: Operand Stack
// ..., [arg0, [arg1 ...]] -> ..

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
#include "lib2d.h"
#include "atmel/bmplib.h"
#include "fontlib.h"
#include "atmel/usart.h"

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


char drawPointRGB()
{
//public native void drawPointRGB(int x,int y, int color);
    lcdSetPixelRGB(opStackGetValue(local + 1).UInt,
        opStackGetValue(local + 2).UInt, opStackGetValue(local + 3).UInt);
    return 0;
}


char drawFillRectRGB()
{
//public native void drawFillRectRGB(int x1, int y1, int x2, int y2, int color);
    lcdFillRect(opStackGetValue(local + 1).UInt,
        opStackGetValue(local + 2).UInt, opStackGetValue(local + 3).UInt,
        opStackGetValue(local + 4).UInt, opStackGetValue(local + 5).UInt);

    return 0;
}


char drawRectangleRGB()
{
//public native void drawRectangleRGB(int x1, int y1, int x2, int y2, int fgcolor, int bkcolor);
    lcdRectangle(opStackGetValue(local + 1).UInt,
        opStackGetValue(local + 2).UInt, opStackGetValue(local + 3).UInt,
        opStackGetValue(local + 4).UInt, opStackGetValue(local + 5).UInt,
        opStackGetValue(local + 6).UInt);
    return 0;
}


char clearScreenRGB()
{
//public native void clearScreenRGB(int color);
    lcdClearScreen(opStackGetValue(local + 1).UInt);
    return 0;
}


char drawRoundRectRGB()
{
//public native void drawRoundRectRGB(int x1, int y1, int x2, int y2, int a, int b, int fgcolor, int bkcolor);
    lcdRoundRect(opStackGetValue(local + 1).UInt,
        opStackGetValue(local + 2).UInt, opStackGetValue(local + 3).UInt,
        opStackGetValue(local + 4).UInt, opStackGetValue(local + 5).UInt,
        opStackGetValue(local + 6).UInt, opStackGetValue(local + 7).UInt,
        opStackGetValue(local + 8).UInt);
    return 0;
}


char drawEllipseRGB()
{
//public native void drawEllipseRGB(int x1, int y1, int x2, int y2, int fgcolor, int bkcolor);
    lcdEllipse(opStackGetValue(local + 1).UInt,
        opStackGetValue(local + 2).UInt, opStackGetValue(local + 3).UInt,
        opStackGetValue(local + 4).UInt, opStackGetValue(local + 5).UInt,
        opStackGetValue(local + 6).UInt);
    return 0;
}


char drawCircleRGB()
{
//public native void drawCircleRGB(int x, int y, int r, int fgcolor, int bkcolor);
    lcdCircle(opStackGetValue(local + 1).UInt, opStackGetValue(local + 2).UInt,
        opStackGetValue(local + 3).UInt, opStackGetValue(local + 4).UInt,
        opStackGetValue(local + 5).UInt);
    return 0;
}


char drawLineRGB()
{
//public native void drawLineRGB(int x1, int y1, int x2, int y2, int color);
    lcdLine(opStackGetValue(local + 1).UInt, opStackGetValue(local + 2).UInt,
        opStackGetValue(local + 3).UInt, opStackGetValue(local + 4).UInt,
        opStackGetValue(local + 5).UInt);
    return 0;
}


char drawPointHSB()
{
/*slot mySlot=opStackGetValue(local+1);       // in static methods the first Arg on Stack
 // otherwise local+1
 // mySlot is a reference to heap object (Point p)

lcdSetPixelHSB(heapGetElement(mySlot.stackObj.pos+1).Float,	//x
heapGetElement(mySlot.stackObj.pos+2).Float,	//y
heapGetElement(mySlot.stackObj.pos+4).Float,	//h
heapGetElement(mySlot.stackObj.pos+5).Float,	//s
heapGetElement(mySlot.stackObj.pos+6).Float);	//b
*/

//public native void drawPointHSB(float x, float y, float h, float s, float b);
    lcdSetPixelHSB(opStackGetValue(local + 1).Float,
        opStackGetValue(local + 2).Float, opStackGetValue(local + 3).Float,
        opStackGetValue(local + 4).Float, opStackGetValue(local + 5).Float);

    return 0;
}


char drawLineHSB()
{
/*	slot mySlot=opStackGetValue(local+1);       // in static methods the first Arg on Stack
 // otherwise local+1
 // mySlot is a reference to heap object (Point p)

lcdLinePoint_t p1;
p1.x=heapGetElement(mySlot.stackObj.pos+1).Float;	//x
p1.y=heapGetElement(mySlot.stackObj.pos+2).Float;	//y
p1.h=heapGetElement(mySlot.stackObj.pos+4).Float;	//h
p1.s=heapGetElement(mySlot.stackObj.pos+5).Float;	//s
p1.b=heapGetElement(mySlot.stackObj.pos+6).Float;	//b

mySlot=opStackGetValue(local+2);
lcdLinePoint_t p2;
p2.x=heapGetElement(mySlot.stackObj.pos+1).Float;	//x
p2.y=heapGetElement(mySlot.stackObj.pos+2).Float;	//y
p2.h=heapGetElement(mySlot.stackObj.pos+4).Float;	//h
p2.s=heapGetElement(mySlot.stackObj.pos+5).Float;	//s
p2.b=heapGetElement(mySlot.stackObj.pos+6).Float;	//b

lcdDDALine(p1, p2);
*/

//public native void drawLineHSB(float x1, float y1, float h1, float s1, float b1, float x2, float y2, float h2, float s2, float b2);
    lcdLinePoint_t p1;
    p1.x = opStackGetValue(local + 1).Float;      //x
    p1.y = opStackGetValue(local + 2).Float;      //y
    p1.h = opStackGetValue(local + 3).Float;      //h
    p1.s = opStackGetValue(local + 4).Float;      //s
    p1.b = opStackGetValue(local + 5).Float;      //b

    lcdLinePoint_t p2;
    p2.x = opStackGetValue(local + 6).Float;      //x
    p2.y = opStackGetValue(local + 7).Float;      //y
    p2.h = opStackGetValue(local + 8).Float;      //h
    p2.s = opStackGetValue(local + 9).Float;      //s
    p2.b = opStackGetValue(local + 10).Float;     //b

    lcdDDALine(p1, p2);
    return 0;
}


char drawTriangleFill()
{
/*	slot mySlot=opStackGetValue(local+1);       // in static methods the first Arg on Stack
 // otherwise local+1
 // mySlot is a reference to heap object (Point p)

lcdLinePoint_t p1;
p1.x=heapGetElement(mySlot.stackObj.pos+1).Float;	//x
p1.y=heapGetElement(mySlot.stackObj.pos+2).Float;	//y
p1.h=heapGetElement(mySlot.stackObj.pos+4).Float;	//h
p1.s=heapGetElement(mySlot.stackObj.pos+5).Float;	//s
p1.b=heapGetElement(mySlot.stackObj.pos+6).Float;	//b

mySlot=opStackGetValue(local+2);
lcdLinePoint_t p2;
p2.x=heapGetElement(mySlot.stackObj.pos+1).Float;	//x
p2.y=heapGetElement(mySlot.stackObj.pos+2).Float;	//y
p2.h=heapGetElement(mySlot.stackObj.pos+4).Float;	//h
p2.s=heapGetElement(mySlot.stackObj.pos+5).Float;	//s
p2.b=heapGetElement(mySlot.stackObj.pos+6).Float;	//b

mySlot=opStackGetValue(local+3);
lcdLinePoint_t p3;
p3.x=heapGetElement(mySlot.stackObj.pos+1).Float;	//x
p3.y=heapGetElement(mySlot.stackObj.pos+2).Float;	//y
p3.h=heapGetElement(mySlot.stackObj.pos+4).Float;	//h
p3.s=heapGetElement(mySlot.stackObj.pos+5).Float;	//s
p3.b=heapGetElement(mySlot.stackObj.pos+6).Float;	//b

lcdTriangleFill(p1, p2, p3);
*/

//public native void drawTriangleFill(float x1, float y1, float h1, float s1, float b1, float x2, float y2, float h2, float s2, float b2, float x3, float y3, float h3, float s3, float b3);

    lcdLinePoint_t p1;
    p1.x = opStackGetValue(local + 1).Float;      //x
    p1.y = opStackGetValue(local + 2).Float;      //y
    p1.h = opStackGetValue(local + 3).Float;      //h
    p1.s = opStackGetValue(local + 4).Float;      //s
    p1.b = opStackGetValue(local + 5).Float;      //b

    lcdLinePoint_t p2;
    p2.x = opStackGetValue(local + 6).Float;      //x
    p2.y = opStackGetValue(local + 7).Float;      //y
    p2.h = opStackGetValue(local + 8).Float;      //h
    p2.s = opStackGetValue(local + 9).Float;      //s
    p2.b = opStackGetValue(local + 10).Float;     //b

    lcdLinePoint_t p3;
    p3.x = opStackGetValue(local + 11).Float;     //x
    p3.y = opStackGetValue(local + 12).Float;     //y
    p3.h = opStackGetValue(local + 13).Float;     //h
    p3.s = opStackGetValue(local + 14).Float;     //s
    p3.b = opStackGetValue(local + 15).Float;     //b

    lcdTriangleFill(p1, p2, p3);
    return 0;
}


char clearZBuffer()
{
    lcdInitZBuffer();
    return 0;
}


char drawPointHSBZBuffer()
{
/*slot mySlot=opStackGetValue(local+1);       // in static methods the first Arg on Stack
 // otherwise local+1
 // mySlot is a reference to heap object (Point p)

lcdLinePoint_t p;
p.x=heapGetElement(mySlot.stackObj.pos+1).Float;	//x
p.y=heapGetElement(mySlot.stackObj.pos+2).Float;	//y
p.z=heapGetElement(mySlot.stackObj.pos+3).Float;	//z
p.h=heapGetElement(mySlot.stackObj.pos+4).Float;	//h
p.s=heapGetElement(mySlot.stackObj.pos+5).Float;	//s
p.b=heapGetElement(mySlot.stackObj.pos+6).Float;	//b

lcdDrawZBufferPoint(p);*/

//public native void drawPointHSBZBuffer(float x, float y, float z, float h, float s, float b);
    lcdLinePoint_t p;
    p.x = opStackGetValue(local + 1).Float;       //x
    p.y = opStackGetValue(local + 2).Float;       //y
    p.z = opStackGetValue(local + 3).Float;       //z
    p.h = opStackGetValue(local + 4).Float;       //h
    p.s = opStackGetValue(local + 5).Float;       //s
    p.b = opStackGetValue(local + 6).Float;       //b

    lcdDrawZBufferPoint(p);
    return 0;
}


char drawLineHSBZBuffer()
{
/*	slot mySlot=opStackGetValue(local+1);       // in static methods the first Arg on Stack
 // otherwise local+1
 // mySlot is a reference to heap object (Point p)

lcdLinePoint_t p1;
p1.x=heapGetElement(mySlot.stackObj.pos+1).Float;	//x
p1.y=heapGetElement(mySlot.stackObj.pos+2).Float;	//y
p1.z=heapGetElement(mySlot.stackObj.pos+3).Float;	//z
p1.h=heapGetElement(mySlot.stackObj.pos+4).Float;	//h
p1.s=heapGetElement(mySlot.stackObj.pos+5).Float;	//s
p1.b=heapGetElement(mySlot.stackObj.pos+6).Float;	//b

mySlot=opStackGetValue(local+2);
lcdLinePoint_t p2;
p2.x=heapGetElement(mySlot.stackObj.pos+1).Float;	//x
p2.y=heapGetElement(mySlot.stackObj.pos+2).Float;	//y
p2.z=heapGetElement(mySlot.stackObj.pos+3).Float;	//z
p2.h=heapGetElement(mySlot.stackObj.pos+4).Float;	//h
p2.s=heapGetElement(mySlot.stackObj.pos+5).Float;	//s
p2.b=heapGetElement(mySlot.stackObj.pos+6).Float;	//b

lcdDDALineZBuffer(p1, p2);
*/

//public native void drawLineHSBZBuffer(float x1, float y1, float z1, float h1, float s1, float b1, float x2, float y2, float z2, float h2, float s2, float b2);
    lcdLinePoint_t p1;
    p1.x = opStackGetValue(local + 1).Float;      //x
    p1.y = opStackGetValue(local + 2).Float;      //y
    p1.z = opStackGetValue(local + 3).Float;      //z
    p1.h = opStackGetValue(local + 4).Float;      //h
    p1.s = opStackGetValue(local + 5).Float;      //s
    p1.b = opStackGetValue(local + 6).Float;      //b

    lcdLinePoint_t p2;
    p2.x = opStackGetValue(local + 7).Float;      //x
    p2.y = opStackGetValue(local + 8).Float;      //y
    p2.z = opStackGetValue(local + 9).Float;      //z
    p2.h = opStackGetValue(local + 10).Float;     //h
    p2.s = opStackGetValue(local + 11).Float;     //s
    p2.b = opStackGetValue(local + 12).Float;     //b

    lcdDDALineZBuffer(p1, p2);
    return 0;
}


char setFont()
{
    lcdSelectFontFromArray(opStackGetValue(local + 1).UInt);
    return 0;
}


char setFontWindow()
{
    lcdSetRect(lcd_Window, opStackGetValue(local + 1).UInt,
        opStackGetValue(local + 2).UInt, opStackGetValue(local + 3).UInt,
        opStackGetValue(local + 4).UInt);         //Schrift-Fenster
    return 0;
}


char setFontAutoLineFeed()
{
    if (opStackGetValue(local + 1).UInt > 0)
        lcd_Flags.AutoLineFeed = 1;
    else
        lcd_Flags.AutoLineFeed = 0;

    return 0;
}


char setFontFixedFont()
{
    if (opStackGetValue(local + 1).UInt > 0)
        lcd_Flags.FixedFont = 1;
    else
        lcd_Flags.FixedFont = 0;
    return 0;
}


char setFontCursor()
{
    lcd_Cursor.X = opStackGetValue(local + 1).UInt;
    lcd_Cursor.Y = opStackGetValue(local + 2).UInt;
    return 0;
}


char setFontColor()
{
//public native void setFontColor(int fgcolor, int bkcolor);
                                                  //Hintergrundfarbe, NONE = Transparent
    lcd_Colors[0] = opStackGetValue(local + 2).UInt;
                                                  //Vordergrundfarbe
    lcd_Colors[1] = opStackGetValue(local + 1).UInt;
    return 0;
}


char drawChar()
{
    opStackPush((slot) (u4) lcdDrawChar(opStackGetValue(local + 1).UInt));
    return 0;
}


char drawCharAt()
{
    opStackPush(
        (slot) (u4) lcdDrawCharAt(opStackGetValue(local + 1).UInt,
        opStackGetValue(local + 2).UInt,
        opStackGetValue(local + 3).UInt,
        opStackGetValue(local + 4).UInt,
        opStackGetValue(local + 5).UInt));
    return 0;
}


char getCharWidth()
{
    opStackPush((slot) (u4) lcdCharWidth(opStackGetValue(local + 1).UInt));
    return 0;
}


char currentTimeMillis()
{
    return 0;
}


;

char nativeExit()
{
//asm("  lda.w   pc, 0");
//asm("  mov   pc, 0");
//goto *0x00000000;
    return 0;
}
