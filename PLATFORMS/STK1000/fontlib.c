/* GLCD Library f�r's Nokia 6100
   copyright 2004 Hagen Reddmann

Umsetzung f�r STK1000, Christian Hecht, Berufsakademie Berlin

*/

#include "fontlib.h"

void lcdSelectFontFromArray(const u2 index)
{
    if (index < sizeof(lcd_Fonts))
        lcdSelectFont(lcd_Fonts[index], lcdFontRead);
}


void lcdSelectFont(const void * data, const lcdFontFunc_t callback)
{
    lcd_FontData = (lcdFontData_t)(data);
    lcd_FontRead = callback;
    lcd_FontWidth = lcd_FontRead(lcd_FontData + 2);
    lcd_FontHeight = lcd_FontRead(lcd_FontData + 3);
    lcd_FontBitsPixel = lcd_FontRead(lcd_FontData + 4);
    lcd_FontFirstChar = lcd_FontRead(lcd_FontData + 5);
    lcd_FontLastChar = lcd_FontRead(lcd_FontData + 6);
}


void initFont(void)
{
//hier die benoetigten Schriften einbinden, es koennen auch harte Speicheradressen sein
    lcd_Fonts[0] = arial16;
    lcd_Fonts[1] = bajosfont;

//Farb-Mappings, wegen dynamischer Bits / Pixel (1, 2, 3, 4)
//nach Wichtigkeit geordnet
    lcd_Colors[0] = LCD_COLOR_NONE;               //Hintergrundfarbe, NONE = Transparent
    lcd_Colors[1] = LCD_COLOR_BLACK;              //Vordergrundfarbe
    lcd_Colors[2] = LCD_COLOR_WHITE;
    lcd_Colors[3] = LCD_COLOR_RED;
    lcd_Colors[4] = LCD_COLOR_GREEN;
    lcd_Colors[5] = LCD_COLOR_BLUE;
    lcd_Colors[6] = LCD_COLOR_YELLOW;
    lcd_Colors[7] = LCD_COLOR_GRAY;
    lcd_Colors[8] = LCD_COLOR_MAROON;
    lcd_Colors[9] = LCD_COLOR_NAVY;
    lcd_Colors[10] = LCD_COLOR_PURPLE;
    lcd_Colors[11] = LCD_COLOR_TEAL;
    lcd_Colors[12] = LCD_COLOR_SILVER;
    lcd_Colors[13] = LCD_COLOR_LIME;
    lcd_Colors[14] = LCD_COLOR_FUCHSIA;
    lcd_Colors[15] = LCD_COLOR_AQUA;

//LCD_COLOR_OLIVE musse wegen NONE weichen

    lcd_FontData=0;                               //Schrift-DATA -> keine Schrift geladen

    lcd_Flags.AutoLineFeed=0;                     //Automatischer umbruch, wenn Zeile voll?
    lcd_Flags.FixedFont=0;                        //Feste Schriftbreite?
//lcdSetRect(lcd_Clip, 1, 1, SCREEN_WIDTH, SCREEN_HEIGHT);
                                                  //Schrift-Fenster
    lcdSetRect(lcd_Window, 0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1);

//Schrift Cursor
    lcd_Cursor.X = lcd_Window.X1;
    lcd_Cursor.Y = lcd_Window.Y1;
}


void lcdNewLine(void)
{

    lcd_Cursor.X = lcd_Window.X1;
    lcd_Cursor.Y += lcd_FontHeight;
    if (lcd_Cursor.Y + lcd_FontHeight > lcd_Window.Y2)
    {
        lcd_Cursor.Y = lcd_Window.Y1;
    }
}


u2 lcdFontRead(const lcdFontData_t index)
{
//return(pgm_read_byte(index));

    return(*index);
}


u2 lcdDrawChar(const char c)
{

    if (c == '\n')
    {
        lcdNewLine();
        return(0);
    }
// ist das Zeichen im Font definiert und auch ein Font selektiert ??
    if ((c < lcd_FontFirstChar) | (c > lcd_FontLastChar) | (lcd_FontData == 0))
    {
        return(0);
    }

// ist das Zeichen im Font deklariert aber nicht definiert ??
    u2 charwidth = lcd_FontRead(lcd_FontData + FONT_HEADER_SIZE + c - lcd_FontFirstChar);
    if (charwidth == 0) {return(0);}

// alle Zeichen kleiner #128 haben rechts eine implizite Leerspalte
    u2 width = charwidth;
    if (c < 128) {width++;}

// wenn wir den Zeichensatz mit fester Breite zeichnen sollen dann setze die Width entsprechend
    if ((lcd_Flags.FixedFont == 1) & (width <= lcd_FontWidth))
    {
        width = lcd_FontWidth;
    }

// passt das Zeichen noch in das Zeichen-Fenster ?? falls nein eventuell ein Linefeed einf�gen
    if ((lcd_Flags.AutoLineFeed == 1) & (lcd_Cursor.X + width >= lcd_Window.X2))
    {
        lcdNewLine();
    }

// ist das Zeichen �berhaupt noch ansatzweise sichtbar ?
//    if ((lcd_Cursor.X > lcd_Clip.X2) | (lcd_Cursor.Y > lcd_Clip.Y2)) {
//      return(0);
//    }

    lcdCoord_t x = lcd_Cursor.X;
    lcdCoord_t y = lcd_Cursor.Y;
    lcd_Cursor.X += width;

// ist das Zeichen �berhaupt noch ansatzweise sichtbar ?
//    if ((x > lcd_Clip.X2) | (y + lcd_FontHeight < lcd_Clip.Y1)) {
//      return(0);
//    }

    u2 i = 0;

// falls FixedFont m�ssen die Zeichen innerhalb lcd_FontWidth zentriert werden, aber nur
// wenn das Zeichen schmaler als lcd_FontWidth ist. Wir f�llen dann auch gleich den rechten
// und linken Rand des Zeichens, ebenfalls nur wenn die Hintergrundfarbe nicht transparent ist.
    if ((lcd_Flags.FixedFont == 1) & (width > charwidth))
    {
        i = (width - charwidth) / 2;
        if (lcdBkColor != LCD_COLOR_NONE)
        {
//lcdDoFillRect(x, y, x + i -1, y + lcd_FontHeight -1, lcdBkColor);
            lcdFillRect(x, y, x + i -1, y + lcd_FontHeight -1, lcdBkColor);
        }
        x += i;
    }
    i = width - charwidth - i;
    if ((i > 0) & (lcdBkColor != LCD_COLOR_NONE))
    {
//lcdDoFillRect(x + charwidth, y, x + charwidth + i -1, y + lcd_FontHeight -1, lcdBkColor);
        lcdFillRect(x + charwidth, y, x + charwidth + i -1, y + lcd_FontHeight -1, lcdBkColor);
    }

// m�ssen die eigentlichen Zeichendaten gezeichnet werden ??
//    if ((x > lcd_Clip.X2) | (x + charwidth < lcd_Clip.X1)) {
//      return(width);
//    }

    lcdFontData_t data;                           // *Font Daten
    u2 bitspixel = lcd_FontBitsPixel & 0x7F;      // Bits pro Pixel
    u2 bitsmask = 0xFF >> (8 - bitspixel);        // Bitmaske f�r Farbe
    u4 bits;                                      // enth�lt die aktuelle Bits des Zeichens
    u2 bitscount;                                 // aktuelle Anzahl an Bits in Bits
    u2 table[4];                                  // RLE L�ngen Tabelle

// ALLE Lesevorg�nge in den Font-Speicher sind so aufgebaut das sie ganze Bl�cke von
// Bytes sequentiell erledigen. Dies ist wichtig und sehr gut optimiert auf zB. I2C EEPROMS.

    if (lcd_FontBitsPixel & 0x80)
    {
// komprimierte Fonts

// Hole das BytePadding die RLE L�ngentabelle und berechne den Byteindex zu den Daten.
// Dieser Lesevorgang hat einen zus�tzliche Overhead von 4 bytes bei komprimierten Fonts.
// Allerdings wird dieser Overhead bei weitem in der eigentlichen Fontzeichendarstellungsschleife
// wieder wett gemacht !
// Bei komprimierten Fonts kann kein linkes Clipping vorgenommen werden :(
        data = lcd_FontData + FONT_HEADER_SIZE + lcd_FontLastChar - lcd_FontFirstChar +1;
        u2 padding = lcd_FontRead(data++);
        table[0] = 1;
        table[1] = lcd_FontRead(data++);
        table[2] = lcd_FontRead(data++);
        table[3] = lcd_FontRead(data++);
        u8 index = 0;
        for (i = lcd_FontFirstChar; i < c; i++)
        {
            index += lcd_FontRead(data++);
        }
        index *= padding;
        data = lcd_FontData + FONT_HEADER_SIZE + (lcd_FontLastChar - lcd_FontFirstChar +3) * 2 + index;
        bits = lcd_FontRead(data++);
        bitscount = 8;
    }
    else
    {
// unkomprimierte Fonts

// berechne nun den Bitindex zu den Bitdaten im Font
        data = lcd_FontData + FONT_HEADER_SIZE;
        u8 index = 0;
        for (i = lcd_FontFirstChar; i < c; i++)
        {
            index += lcd_FontRead(data++);
        }
// linkes Clipping ber�cksichtigen, nur bei unkomprimierten Fonts m�glich
//      if (x < lcd_Clip.X1) {
//        lcdCoord_t t = lcd_Clip.X1 - x;
//        index += t;
//        charwidth -= t;
//        x = lcd_Clip.X1;
//      }
        index *= lcd_FontHeight * bitspixel;

// positioniere data auf's erste Byte mit Fontdaten zum Zeichen und hole die ersten Bits
        bitscount = index % 8;
        index /= 8;
        data = lcd_FontData + FONT_HEADER_SIZE + lcd_FontLastChar - lcd_FontFirstChar +1 + index;
        bits = lcd_FontRead(data++) >> bitscount;
        bitscount = 8 - bitscount;
    }

// wir senden die Bitdaten der einzelnen Pixel jeweils Spaltenweise in Y Richtung
    u2 pixelcount = 0;                            // Anzahl der zu setzenden Pixel
    u2 pixelcolor = 0;                            // zu setzende Pixelfarbe max 8 Bit
    u2 clipinvalid = 1;                           // G�ltigkeit der Adresszeiger im PCF8833
    lcdCoord_t clipY = 0;                         // letzte gesetzte Y-Adresse im PCF8833
                                                  // unterste Pixelzeile des Zeichens
    lcdCoord_t stopY = lcd_Cursor.Y + lcd_FontHeight -1;
    lcdCoord_t stopX = x + charwidth -1;

// rechtes Clipping ber�cksichtigen
//    if (stopX > lcd_Clip.X2) {
//      stopX = lcd_Clip.X2;
//    }
    if (stopX > SCREEN_WIDTH-1)
    {
        stopX = SCREEN_WIDTH-1;
    }

    for (; x <= stopX; x++)
    {
        if (clipY |= lcd_Cursor.Y)
        {
            clipinvalid = 1;
        }
        for (y = lcd_Cursor.Y; y <= stopY; y++)
        {
            if (pixelcount == 0)
            {
                if (bitscount <= 8)
                {
                    bits |= lcd_FontRead(data++) << bitscount;
                    bitscount += 8;
                }
                if (lcd_FontBitsPixel & 0x80)
                {
                    pixelcount = table[bits & 3];
                    bits >>= 2;
                    bitscount -= 2;
                }
                else
                {
                    pixelcount++;
                }

                pixelcolor = bits & bitsmask;
                if (bitspixel > COLOR_TABLE_BITS)
                {
                    if (pixelcolor>0)
                    {
//Farbtabelle zu klein, keim Mapping m�glich -> alle Farben werden schwarz
                        pixelcolor=1;
                    }
                }
                bits >>= bitspixel;
                bitscount -= bitspixel;
            }
            pixelcount--;
//if ((lcd_Colors[pixelcolor] != LCD_COLOR_NONE) & (y >= lcd_Clip.Y1) & (y <= lcd_Clip.Y2) & (x >= lcd_Clip.X1)) {
            if ((lcd_Colors[pixelcolor] != LCD_COLOR_NONE) & (y < SCREEN_HEIGHT))
            {
                if (clipinvalid)
                {
//lcdSetAddr(x, y, lcd_Cursor.X, stopY);
                    clipY = y;
                    clipinvalid = 0;
                }
//lcdPutPixel(pixelcolor);
                lcdSetPixelRGB(x, y, lcd_Colors[pixelcolor]);
            }
            else
            {
                clipinvalid = 1;
            }
        }
    }

    return(width);
}


u2 lcdCharWidth(const char c)
{

    if ((c < lcd_FontFirstChar) | (c > lcd_FontLastChar) | (lcd_FontData == 0))
    {
        return(0);                                // character not defined
    }
    else
    {
        u2 i = *(lcd_FontData + FONT_HEADER_SIZE + c - lcd_FontFirstChar);
        if (i > 0)
        {
            if (c < 128)
            {
                i++;
            }
            if ((lcd_Flags.FixedFont) & (i < lcd_FontWidth))
            {
                i = lcd_FontWidth;
            }
        }
        return(i);
    }
}


u4 lcdPrint(const char *addr)
{

    u2 c;
    u4 i = 0;
    while ((c = *addr++))
    {
        i += lcdDrawChar(c);
    }
    return(i);
}


/*u4 lcdPrint_P(const char *addr) {

    u2 c;
u4 i = 0;
while ((c = pgm_read_byte(addr++))) {
i += lcdDrawChar(c);
}
return(i);
}*/

u4 lcdCharsWidth(const char *addr)
{

    u2 c;
    u4 i = 0;

    while ((c = *addr++))
    {
        i += lcdCharWidth(c);
    }
    return(i);
}


/*u4 lcdCharsWidth_P(const char *addr) {

    u2 c;
u4 i = 0;

while ((c = pgm_read_byte(addr++))) {
i += lcdCharWidth(c);
}
return(i);
}*/

u4 lcdDrawStringAt(const lcdCoord_t x, const lcdCoord_t y, const char *addr, const lcdColor_t fgcolor, const lcdColor_t bkcolor)
{
    lcd_Colors[0] = bkcolor;                      //Hintergrundfarbe, NONE = Transparent
    lcd_Colors[1] = fgcolor;                      //Vordergrundfarbe

//Schrift Cursor
    lcd_Cursor.X = x;
    lcd_Cursor.Y = y;

    return lcdPrint(addr);
}


u2 lcdDrawCharAt(const lcdCoord_t x, const lcdCoord_t y, const char c, lcdColor_t fgcolor, const lcdColor_t bkcolor)
{
    lcd_Colors[0] = bkcolor;                      //Hintergrundfarbe, NONE = Transparent
    lcd_Colors[1] = fgcolor;                      //Vordergrundfarbe

//Schrift Cursor
    lcd_Cursor.X = x;
    lcd_Cursor.Y = y;

    return lcdDrawChar(c);
}
