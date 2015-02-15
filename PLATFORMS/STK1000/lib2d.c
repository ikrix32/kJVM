/* GLCD Library f�r's Nokia 6100
   copyright 2004 Hagen Reddmann

Umsetzung f�r STK1000, Christian Hecht, Berufsakademie Berlin

*/

#include "atmel/at32stk1000.h"
#include "lib2d.h"
#include <float.h>

/*#include "usart.h"
#include <string.h>

volatile avr32_usart_t *usart2 = &AVR32_USART1;
void print(volatile struct avr32_usart_t * usart, char *str);

void printfloat(float f)
{
uint8_t buf[8];
snprintf(buf,8,"%8f",f);
print(usart2, buf);
print(usart2, "\n");
}
*/

void lcdSetPixelRGB2(const lcdCoord_t x, const lcdCoord_t y, const u1 r, const u1 g, const u1 b)
{
    if((x < SCREEN_WIDTH) && ( y < SCREEN_HEIGHT))
    {
        u4 * framePtr=(u4 *) (AVR32_EBI1_ADDRESS|0xA0000000);

        *(framePtr+y*640+x)=(((b << 16) & 0xff0000) | ((g << 8) & 0x00ff00) | (r & 0x000000ff));
    }
}


void lcdSetPixelRGB(const lcdCoord_t x, const lcdCoord_t y, const lcdColor_t color)
{
    if((x < SCREEN_WIDTH) && ( y < SCREEN_HEIGHT))
    {
        u4 * framePtr=(u4 *) (AVR32_EBI1_ADDRESS|0xA0000000);

        *(framePtr+y*640+x)=color;
    }
}


void lcdSetPixelHSB(const lcdCoord_t x, const lcdCoord_t y, lcdHSBColor_t hue, lcdHSBColor_t saturation, lcdHSBColor_t brightness)
{
    hue /=256;
    saturation /=256;
    brightness /=256;

/*hsb to rgb
----------
Input:   hue, saturation, and brightness as floats scaled from 0.0 to 1.0
Output:  red, green, and blue as floats scaled from 0.0 to 1.0*/

    lcdHSBColor_t red, green, blue;

    if (brightness == 0.0)
    {                                             // safety short circuit again
        red   = 0.0;
        green = 0.0;
        blue  = 0.0;
        lcdSetPixelRGB2(x, y, red*256, green*256, blue*256);
        return;
    }

    if (saturation == 0.0)
    {                                             // grey
        red   = brightness;
        green = brightness;
        blue  = brightness;
        lcdSetPixelRGB2(x, y, red*256, green*256, blue*256);
        return;
    }

    lcdHSBColor_t domainOffset;                   // hue mod 1/6
    if (hue < 1.0/6)
    {                                             // red domain; green ascends
        domainOffset = hue;
        red   = brightness;
        blue  = brightness * (1.0 - saturation);
        green = blue + (brightness - blue) * domainOffset * 6;
    }
    else if (hue < 2.0/6)
    {                                             // yellow domain; red descends
        domainOffset = hue - 1.0/6;
        green = brightness;
        blue  = brightness * (1.0 - saturation);
        red   = green - (brightness - blue) * domainOffset * 6;
    }
    else if (hue < 3.0/6)
    {                                             // green domain; blue ascends
        domainOffset = hue - 2.0/6;
        green = brightness;
        red   = brightness * (1.0 - saturation);
        blue  = red + (brightness - red) * domainOffset * 6;
    }
    else if (hue < 4.0/6)
    {                                             // cyan domain; green descends
        domainOffset = hue - 3.0/6;
        blue  = brightness;
        red   = brightness * (1.0 - saturation);
        green = blue - (brightness - red) * domainOffset * 6;
    }
    else if (hue < 5.0/6)
    {                                             // blue domain; red ascends
        domainOffset = hue - 4.0/6;
        blue  = brightness;
        green = brightness * (1.0 - saturation);
        red   = green + (brightness - green) * domainOffset * 6;
    }
    else
    {                                             // magenta domain; blue descends
        domainOffset = hue - 5.0/6;
        red   = brightness;
        green = brightness * (1.0 - saturation);
        blue  = red - (brightness - green) * domainOffset * 6;
    }

    lcdSetPixelRGB2(x, y, red*256, green*256, blue*256);
    return;
}


void lcdFillRect(lcdCoord_t x1, lcdCoord_t y1, lcdCoord_t x2, lcdCoord_t y2, const lcdColor_t color)
{
    lcdCoord_t i, j;

    if (x1 > x2) lcdSwapCoord(x1, x2);
    if (y1 > y2) lcdSwapCoord(y1, y2);

    for (i=y1; i<=y2; i++)
    {
        for (j=x1; j<=x2; j++)
        {
            lcdSetPixelRGB(j, i, color);
        }
    }
}


lcdColor_t RGB2Color(const u1 r, const u1 g, const u1 b)
{
    return ((b << 16) & 0xff0000) | ((g << 8) & 0x00ff00) | (r & 0x000000ff);
}


void lcdRectangle(lcdCoord_t x1, lcdCoord_t y1, lcdCoord_t x2, lcdCoord_t y2, const lcdColor_t fgcolor, const lcdColor_t bkcolor)
{
    if (x1 > x2) lcdSwapCoord(x1, x2);
    if (y1 > y2) lcdSwapCoord(y1, y2);

    if (fgcolor != bkcolor)
    {
        if (fgcolor != LCD_COLOR_NONE)
        {
            lcdFillRect(x1, y1, x2, y1, fgcolor);
            lcdFillRect(x1, y2, x2, y2, fgcolor);
            lcdFillRect(x1, y1, x1, y2, fgcolor);
            lcdFillRect(x2, y1, x2, y2, fgcolor);
        }
        x1++;
        y1++;
        x2--;
        y2--;
    }
    if ((bkcolor != LCD_COLOR_NONE) && (x1 <= x2) && (y1 <= y2))
    {
        lcdFillRect(x1, y1, x2, y2, bkcolor);
    }
}


void lcdClearScreen(const lcdColor_t color)
{
    lcdRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color, color);
}


void lcdSetPixel4(const lcdCoord_t x1, const lcdCoord_t y1, const lcdCoord_t x2, const lcdCoord_t y2, const lcdColor_t color)
{
    lcdSetPixelRGB(x1, y1, color);
    lcdSetPixelRGB(x2, y1, color);
    lcdSetPixelRGB(x1, y2, color);
    lcdSetPixelRGB(x2, y2, color);
}


void lcdLine2(const lcdCoord_t x1, const lcdCoord_t y1, const lcdCoord_t x2, const lcdCoord_t y2, const lcdColor_t color)
{
    lcdFillRect(x1, y1, x2, y1, color);
    lcdFillRect(x1, y2, x2, y2, color);
}


void lcdRoundRect(lcdCoord_t x1, lcdCoord_t y1, lcdCoord_t x2, lcdCoord_t y2, lcdCoord_t a, lcdCoord_t b, const lcdColor_t fgcolor, const lcdColor_t bkcolor)
{
    if (x1 > x2) lcdSwapCoord(x1, x2);
    if (y1 > y2) lcdSwapCoord(y1, y2);
    lcdCoord_t t;
    t = x2 - x1;
    t >>= 1;
    if (a > t) a = t;
    t = y2 - y1;
    t >>= 1;
    if (b > t) b = t;
    x1 += a;
    x2 -= a;
    if (fgcolor != LCD_COLOR_NONE) lcdLine2(x1, y1, x2, y2, fgcolor);
    u2 as = a * a, bs = b * b;
    s4 dx = 0, dy = as, ee = bs;
    dy *= b;
    ee += as / 4;
    ee -= dy;
    dy += dy;
    while (dx < dy)
    {
        if (fgcolor != LCD_COLOR_NONE) lcdSetPixel4(x1, y1, x2, y2, fgcolor);
        x1--;
        x2++;
        if (ee >= 0)
        {
            y1++;
            y2--;
            dy -= as;
            dy -= as;
            ee -= dy;
            if (bkcolor != LCD_COLOR_NONE) lcdLine2(x1, y1, x2, y2, bkcolor);
        }
        dx += bs;
        dx += bs;
        ee += bs;
        ee += dx;
    }
    s4 tt = as;
    tt -= bs;
    tt *= 3;
    tt >>= 1;
    tt -= dx;
    tt -= dy;
    tt >>= 1;
    ee += tt;
    while (y1 <= y2)
    {
        if (bkcolor != LCD_COLOR_NONE) lcdLine2(x1, y1, x2, y2, bkcolor);
        if (fgcolor != LCD_COLOR_NONE) lcdSetPixel4(x1, y1, x2, y2, fgcolor);
        y1++;
        y2--;
        if (ee < 0)
        {
            x1--;
            x2++;
            a++;
            dx += bs;
            dx += bs;
            ee += dx;
        }
        dy -= as;
        dy -= as;
        ee += as;
        ee -= dy;
    }
}


void lcdEllipse(const lcdCoord_t x1, const lcdCoord_t y1, const lcdCoord_t x2, const lcdCoord_t y2, const lcdColor_t fgcolor, const lcdColor_t bkcolor)
{
    lcdRoundRect(x1, y1, x2, y2, (x2 - x1) / 2, (y2 - y1) / 2, fgcolor, bkcolor);
}


void lcdCircle(const lcdCoord_t x, const lcdCoord_t y, const lcdCoord_t r, const lcdColor_t fgcolor, const lcdColor_t bkcolor)
{
    lcdRoundRect(x - r, y - r, x + r, y + r, r, r, fgcolor, bkcolor);
}


void lcdLine(lcdCoord_t x1, lcdCoord_t y1, lcdCoord_t x2, lcdCoord_t y2, const lcdColor_t color)
{

    if ((x1 == x2) | (y1 == y2))
    {
        lcdFillRect(x1, y1, x2, y2, color);
    }
    else
    {
        lcdCoord_t dx, dy, sx, sy;

        if (x1 < x2)
        {
            dx = x2 - x1;
            sx = +1;
        }
        else
        {
            dx = x1 - x2;
            sx = -1;
        }

        if (y1 < y2)
        {
            dy = y2 - y1;
            sy = +1;
        }
        else
        {
            dy = y1 - y2;
            sy = -1;
        }

        u1 swap = (dx > dy) ? 0 : 1;

        if (swap)
        {
            lcdSwapCoord(x1, y1);
            lcdSwapCoord(dx, dy);
            lcdSwapCoord(sx, sy);
        }

        s4 e = dy - dx + dy;
        lcdCoord_t x3 = x1;
        lcdCoord_t y3 = y1;
        x2 = x1;
        y2 = y1;
/* Diese Loop ist suboptimal. Man k�nnte mehr rausholen wenn man die Hardware besser ausnutzen w�rde, aber
   dann w�re diese Routine eben nicht mehr Hardwareunabh�ngig so wie jetzt. */
        lcdCoord_t i;
        for (i = 0; i <= dx; i++)
        {
            if ((x1 != x2) & (y1 != y2))
            {
                if (swap)
                {
                    lcdSwapCoord(x2, y2);
                    lcdSwapCoord(x3, y3);
                }
                lcdFillRect(x2, y2, x3, y3, color);
                x2 = x1;
                y2 = y1;
            };
            x3 = x1;
            y3 = y1;
            while (e >= 0)
            {
                y1 += sy;
                e  -= dx;
                e  -= dx;
            }
            x1 += sx;
            e  += dy;
            e  += dy;
        }
        if (swap)
        {
            lcdSwapCoord(x2, y2);
            lcdSwapCoord(x3, y3);
        }
        lcdFillRect(x2, y2, x3, y3, color);
    }
}


//-------------------------------------------------------------------------------------------------------
//Erweiterte Grafikmethoden

lcdHSBCoord_t lcdAbs(const lcdHSBCoord_t a)
{
    if (a>0)
        { return a;}
        else
            { return (a*-1);}
}


void lcdDDALine(const lcdLinePoint_t a, const lcdLinePoint_t e)
{                                                 // digital differential alg
    lcdHSBCoord_t numSteps, dx, dy;
    lcdHSBColor_t dh, ds, db;
    lcdHSBCoord_t x, y;
    lcdHSBColor_t h, s, b;
    lcdHSBCoord_t step=.9999;                     // selten: einige pkte werden 2* gezeichnet, andere nicht
// schwierigkeiten mit step=1. , 1.99999-> 1, 1.99999+1.-> 3.
// -> löcher in line

    x = a.x;
    y = a.y;
    h = a.h;
    s = a.s;
    b = a.b;

    numSteps=e.x-a.x;

    if (lcdAbs(numSteps)>=lcdAbs(e.y-a.y))
    {
        if (e.x>=a.x)
            dx  =step;
        else
        {
            dx  =-step;
            numSteps    =-numSteps;
        }
        dy=(e.y-a.y)/numSteps;
    }
    else
    {
        numSteps=e.y-a.y;
        if (e.y>=a.y)
            dy  =step;
        else
        {
            dy  =-step;
            numSteps    =-numSteps;
        }
        dx=(e.x-a.x)/numSteps;
    }

//if (numSteps< 1.) {drawPoint(e);return;}	//Endpunktfarbe
    dh = (e.h - a.h)/numSteps;
    ds = (e.s - a.s)/numSteps;
    db = (e.b - a.b)/numSteps;

    do
    {
//lcdSetPixelRGB2(x, y, h, s, b);
        lcdSetPixelHSB(x, y, h, s, b);
        x+=dx;
        y+=dy;
        h+=dh;
        s+=ds;
        b+=db;
    }
    while  ((--numSteps) >= -.5);
}


void lcdTriangleFill(const lcdLinePoint_t p0, const lcdLinePoint_t p1, const lcdLinePoint_t p2)
{
// to do: sort points !!!
// assume: points are sorted by rising y

    lcdLinePoint_t upperPoint;
    lcdLinePoint_t middlePoint;
    lcdLinePoint_t bottomPoint;
    lcdLinePoint_t pa;
    lcdLinePoint_t pe;

    upperPoint=p0;
    middlePoint=p1;
    bottomPoint=p2;

    lcdHSBCoord_t deltaX1,deltaH1,deltaS1,deltaB1;
    lcdHSBCoord_t deltaX2,deltaH2,deltaS2,deltaB2;

    lcdCoord_t y;
    for(y=upperPoint.y; y<middlePoint.y; y++)
    {
        deltaX1 = (middlePoint.x-upperPoint.x)/(middlePoint.y-upperPoint.y)*(y-upperPoint.y)+upperPoint.x;
        deltaX2 = (bottomPoint.x-upperPoint.x)/(bottomPoint.y-upperPoint.y)*(y-upperPoint.y)+upperPoint.x;
        deltaH1 = (middlePoint.h-upperPoint.h)/(middlePoint.y-upperPoint.y)*(y-upperPoint.y)+upperPoint.h;
        deltaS1 = (middlePoint.s-upperPoint.s)/(middlePoint.y-upperPoint.y)*(y-upperPoint.y)+upperPoint.s;
        deltaB1 = (middlePoint.b-upperPoint.b)/(middlePoint.y-upperPoint.y)*(y-upperPoint.y)+upperPoint.b;
        deltaH2 = (bottomPoint.h-upperPoint.h)/(bottomPoint.y-upperPoint.y)*(y-upperPoint.y)+upperPoint.h;
        deltaS2 = (bottomPoint.s-upperPoint.s)/(bottomPoint.y-upperPoint.y)*(y-upperPoint.y)+upperPoint.s;
        deltaB2 = (bottomPoint.b-upperPoint.b)/(bottomPoint.y-upperPoint.y)*(y-upperPoint.y)+upperPoint.b;

        pa.x=deltaX1;
        pa.y=y;
        pa.h=deltaH1;
        pa.s=deltaS1;
        pa.b=deltaB1;

        pe.x=deltaX2;
        pe.y=y;
        pe.h=deltaH2;
        pe.s=deltaS2;
        pe.b=deltaB2;

        lcdDDALine(pa, pe);
    }

    for(y=middlePoint.y; y<bottomPoint.y; y++)
    {
        deltaX1 = (bottomPoint.x-middlePoint.x)/(bottomPoint.y-middlePoint.y)*(y-middlePoint.y)+middlePoint.x;
        deltaX2 = (bottomPoint.x-upperPoint.x)/(bottomPoint.y-upperPoint.y)*(y-upperPoint.y)+upperPoint.x;
        deltaH1 = (bottomPoint.h-middlePoint.h)/(bottomPoint.y-middlePoint.y)*(y-middlePoint.y)+middlePoint.h;
        deltaS1 = (bottomPoint.s-middlePoint.s)/(bottomPoint.y-middlePoint.y)*(y-middlePoint.y)+middlePoint.s;
        deltaB1 = (bottomPoint.b-middlePoint.b)/(bottomPoint.y-middlePoint.y)*(y-middlePoint.y)+middlePoint.b;
        deltaH2 = (bottomPoint.h-upperPoint.h)/(bottomPoint.y-upperPoint.y)*(y-upperPoint.y)+upperPoint.h;
        deltaS2 = (bottomPoint.s-upperPoint.s)/(bottomPoint.y-upperPoint.y)*(y-upperPoint.y)+upperPoint.s;
        deltaB2 = (bottomPoint.b-upperPoint.b)/(bottomPoint.y-upperPoint.y)*(y-upperPoint.y)+upperPoint.b;

        pa.x=deltaX1;
        pa.y=y;
        pa.h=deltaH1;
        pa.s=deltaS1;
        pa.b=deltaB1;

        pe.x=deltaX2;
        pe.y=y;
        pe.h=deltaH2;
        pe.s=deltaS2;
        pe.b=deltaB2;

        lcdDDALine(pa, pe);
    }
}


//-------------------------------------------------------------------------------------------------------
//Z-Buffer

void lcdInitZBuffer()
{
    lcdCoord_t x,y;

    for (x=0; x<SCREEN_WIDTH; x++)
        for (y=0; y<SCREEN_HEIGHT; y++)
            insertZBufferAt(x, y, FLT_MAX);
}


lcdHSBCoord_t getZBufferAt(const lcdCoord_t x, const lcdCoord_t y)
{
    lcdHSBCoord_t * zBuf=(lcdHSBCoord_t *) (ZBUFFER_BASE);

    if((x < SCREEN_WIDTH) && ( y < SCREEN_HEIGHT))
        return *(zBuf+y*SCREEN_WIDTH+x);

    return 0;
}


void insertZBufferAt(const lcdCoord_t x, const lcdCoord_t y, const lcdHSBCoord_t value)
{
    lcdHSBCoord_t * zBuf=(lcdHSBCoord_t *) (ZBUFFER_BASE);

    if((x < SCREEN_WIDTH) && ( y < SCREEN_HEIGHT))
        *(zBuf+y*SCREEN_WIDTH+x) = value;
}


void lcdDrawZBufferPoint(const lcdLinePoint_t p)
{
    lcdCoord_t x;
    lcdCoord_t y;
    x=p.x;
    y=p.y;

    if((x < SCREEN_WIDTH) && ( y < SCREEN_HEIGHT))
        if(p.z < getZBufferAt(x, y))
    {
        lcdSetPixelHSB(x, y, p.h, p.s, p.b);

        insertZBufferAt(x, y, p.z);
    }
}


void lcdDDALineZBuffer(const lcdLinePoint_t a, const lcdLinePoint_t e)
{
    lcdHSBCoord_t numSteps, dx, dy, dz;
    lcdHSBColor_t dh, ds, db;
    lcdLinePoint_t p;
    lcdHSBCoord_t step=.9999;                     // selten: einige pkte werden 2* gezeichnet, andere nicht
// schwierigkeiten mit step=1. , 1.99999-> 1, 1.99999+1.-> 3.
// -> löcher in line

    p=a;

    numSteps=e.x-a.x;

    if (lcdAbs(numSteps)>=lcdAbs(e.y-a.y))
    {
        if (e.x>=a.x)
            dx  =step;
        else
        {
            dx  =-step;
            numSteps    =-numSteps;
        }
        dy=(e.y-a.y)/numSteps;
    }
    else
    {
        numSteps=e.y-a.y;
        if (e.y>=a.y)
            dy  =step;
        else
        {
            dy  =-step;
            numSteps    =-numSteps;
        }
        dx=(e.x-a.x)/numSteps;
    }

    if ( numSteps< 1.)                            //lass zBuffer entscheiden
    {
        lcdDrawZBufferPoint(a); lcdDrawZBufferPoint(e); return;
    }
    dh = (e.h - a.h)/numSteps;
    ds = (e.s - a.s)/numSteps;
    db = (e.b - a.b)/numSteps;
    dz = (e.z - a.z)/numSteps;

    do
    {
        lcdDrawZBufferPoint(p);
        p.x+=dx;
        p.y+=dy;
        p.z+=dz;
        p.h+=dh;
        p.s+=ds;
        p.b+=db;
    }
    while  ((--numSteps) >= -.5);
}
