/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
//@authors  22.02.2008; Anna Maria Damm; FHW-BA Berlin; Bayer Schering Pharma AG
//	 					H.-Christian Hecht, FHW-BA Berlin; CoMedServ GmbH
//						Adrian Lang,FHW-BA Berlin; Fritz-Haber-Institut
//						Stephan Bauer; FHW-BA Berlin; Bayer Schering Pharma AG
package platform;

public class PlatForm	{
public native char nativeCharIn();
public native void nativeCharOut(char c);
//public native byte getButtons();
//public native void setOnBoardLEDs(byte status);
//public native void charLCDOut(char c);
//public native void controlLCD(int control);
public native void exit (int status);
public native int currentTimeMillis();
//public native float getTemperature();
/*Grafik-Methoden*/
public native void drawPointRGB(int x,int y, int color);
public native void drawFillRectRGB(int x1, int y1, int x2, int y2, int color);
public native void drawRectangleRGB(int x1, int y1, int x2, int y2, int fgcolor, int bkcolor);
public native void clearScreenRGB(int color);
public native void drawRoundRectRGB(int x1, int y1, int x2, int y2, int a, int b, int fgcolor, int bkcolor);
public native void drawEllipseRGB(int x1, int y1, int x2, int y2, int fgcolor, int bkcolor);
public native void drawCircleRGB(int x, int y, int r, int fgcolor, int bkcolor);
public native void drawLineRGB(int x1, int y1, int x2, int y2, int color);

//public native void drawPointHSB(Point p);
public native void drawPointHSB(float x, float y, float h, float s, float b);
public native void drawLineHSB(float x1, float y1, float h1, float s1, float b1, float x2, float y2, float h2, float s2, float b2);
public native void drawTriangleFill(float x1, float y1, float h1, float s1, float b1, float x2, float y2, float h2, float s2, float b2, float x3, float y3, float h3, float s3, float b3);
public native void clearZBuffer();
public native void drawPointHSBZBuffer(float x, float y, float z, float h, float s, float b);
public native void drawLineHSBZBuffer(float x1, float y1, float z1, float h1, float s1, float b1, float x2, float y2, float z2, float h2, float s2, float b2);

/*Schrift-Methoden*/
public native void setFont(int index);
public native void setFontWindow(int x1, int y1, int x2, int y2);
public native void setFontAutoLineFeed(int value);
public native void setFontFixedFont(int value);
public native void setFontCursor(int x, int y);
public native void setFontColor(int fgcolor, int bkcolor);
public native int drawChar(char c);
public native int drawCharAt(int x, int y, char c, int fgcolor, int bkcolor);
public native int getCharWidth(char c);
public PlatForm()	{}
}
