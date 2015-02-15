/* GLCD Library f�r's Nokia 6100
   copyright 2004 Hagen Reddmann

Umsetzung f�r STK1000, Christian Hecht, Berufsakademie Berlin

*/

#ifndef LIB2D_H_
#define LIB2D_H_

#include "../../JVM/typedefinitions.h"

#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240

#define ZBUFFER_BASE    0x107B0000                //0x0004FFFF Bytes am Ende des SDRAMS fuer den ZBuffer

#define LCD_COLOR_NONE      0x002000              // == tansparent, ein gr�n Ton weniger !!

#define LCD_COLOR_BLACK     0x000000
#define LCD_COLOR_GRAY      0x808080
#define LCD_COLOR_MAROON    0x000080
#define LCD_COLOR_RED       0x0000FF
#define LCD_COLOR_GREEN     0x008000
#define LCD_COLOR_LIME      0x00FF00
#define LCD_COLOR_OLIVE     0x008080
#define LCD_COLOR_YELLOW    0x00FFFF
#define LCD_COLOR_NAVY      0x800000
#define LCD_COLOR_BLUE      0xFF0000
#define LCD_COLOR_PURPLE    0x800080
#define LCD_COLOR_FUCHSIA   0xFF00FF
#define LCD_COLOR_TEAL      0x808000
#define LCD_COLOR_AQUA      0xFFFF00
#define LCD_COLOR_SILVER    0xC0C0C0
#define LCD_COLOR_WHITE     0xFFFFFF

#define lcdSwapCoord(x, y) {lcdCoord_t t = x; x = y; y = t;}
#define lcdCheckCoord(x, y) {if (x > y) {lcdSwapCoord(x, y)}}

//typedef unsigned char uint8_t;
//typedef unsigned short int uint16_t;
//typedef unsigned int uint32_t;
//
//typedef char int8_t;
//typedef short int int16_t;
//typedef int int32_t;

typedef u2 lcdCoord_t;                            // Koordinaten
typedef u4 lcdColor_t;                            // Farbe, 24 Bit, 3* 8 Bit RGB
typedef f4 lcdHSBColor_t;
typedef f4 lcdHSBCoord_t;

typedef struct
{
    lcdHSBCoord_t x;
    lcdHSBCoord_t y;
    lcdHSBCoord_t z;
    lcdHSBColor_t h;
    lcdHSBColor_t s;
    lcdHSBColor_t b;
} lcdLinePoint_t;

void lcdSetPixelRGB(const lcdCoord_t x, const lcdCoord_t y, const lcdColor_t color);
void lcdSetPixelRGB2(const lcdCoord_t x, const lcdCoord_t y, const u1 r, const u1 g, const u1 b);
void lcdSetPixelHSB(const lcdCoord_t x, const lcdCoord_t y, lcdHSBColor_t hue, lcdHSBColor_t saturation, lcdHSBColor_t brightness);
lcdColor_t RGB2Color(const u1 r, const u1 g, const u1 b);
void lcdFillRect(lcdCoord_t x1, lcdCoord_t y1, lcdCoord_t x2, lcdCoord_t y2, const lcdColor_t color);
void lcdRectangle(lcdCoord_t x1, lcdCoord_t y1, lcdCoord_t x2, lcdCoord_t y2, const lcdColor_t fgcolor, const lcdColor_t bkcolor);
void lcdClearScreen(const lcdColor_t color);
void lcdSetPixel4(const lcdCoord_t x1, const lcdCoord_t y1, const lcdCoord_t x2, const lcdCoord_t y2, const lcdColor_t color);
void lcdLine2(const lcdCoord_t x1, const lcdCoord_t y1, const lcdCoord_t x2, const lcdCoord_t y2, const lcdColor_t color);
void lcdRoundRect(lcdCoord_t x1, lcdCoord_t y1, lcdCoord_t x2, lcdCoord_t y2, lcdCoord_t a, lcdCoord_t b, const lcdColor_t fgcolor, const lcdColor_t bkcolor);
void lcdEllipse(const lcdCoord_t x1, const lcdCoord_t y1, const lcdCoord_t x2, const lcdCoord_t y2, const lcdColor_t fgcolor, const lcdColor_t bkcolor);
void lcdCircle(const lcdCoord_t x, const lcdCoord_t y, const lcdCoord_t r, const lcdColor_t fgcolor, const lcdColor_t bkcolor);
void lcdLine(lcdCoord_t x1, lcdCoord_t y1, lcdCoord_t x2, lcdCoord_t y2, const lcdColor_t color);
void lcdDDALine(const lcdLinePoint_t a, const lcdLinePoint_t e);
void lcdTriangleFill(const lcdLinePoint_t p0, const lcdLinePoint_t p1, const lcdLinePoint_t p2);
void lcdInitZBuffer();
void lcdDrawZBufferPoint(const lcdLinePoint_t p);
void lcdDDALineZBuffer(const lcdLinePoint_t a, const lcdLinePoint_t e) ;
lcdHSBCoord_t getZBufferAt(const lcdCoord_t x, const lcdCoord_t y);
void insertZBufferAt(const lcdCoord_t x, const lcdCoord_t y, const lcdHSBCoord_t value);
#endif                                            /*LIB2D_H_*/
