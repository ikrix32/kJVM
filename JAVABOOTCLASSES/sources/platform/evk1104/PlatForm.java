/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/

package platform;
public class PlatForm	{
public native char nativeCharIn();
public native void nativeCharOut(char c);
public native int conStat(); // inkey
public native void exit (int status);
public native int currentTimeMillis();
/*Grafik-Methoden*/
public native void initDisplay();
public native void clearDisplay(int color);
public native void drawPixel(int x, int y, int color);
public native void drawHorizLine(int x, int y, int length, int color );
public native void drawVertLine(int x, int y, int length, int color);
public native void drawLine(int x1, int y1, int x2, int y2, int color);
public native void drawFillRect(int x, int y, int width, int height, int color);
public native void drawFilledCircle(int x, int y, int radius, int color, int quadrants);
public native void drawChar(char s, int font, int x, int y, int color, int bgColor, int index);
/*Touch-Methoden*/
public native int getButtons();
public native boolean isButtonPressed(int id);
/*LED-Methoden*/
public native void setLEDon(int led);
public native void setLEDoff(int led);
/*ADC*/
public native void adcInit ();
public native void adcEnable ();
public native void adcDisable ();
public native int adcGetValue ();
public PlatForm()	{}
}

