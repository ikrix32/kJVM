/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
//@authors  22.02.2008; Anna Maria Damm; FHW-BA Berlin; Bayer Schering Pharma AG
//	 		H.-Christian Hecht, FHW-BA Berlin; CoMedServ GmbH
//			Adrian Lang,FHW-BA Berlin; Fritz-Haber-Institut
//			Stephan Bauer; FHW-BA Berlin; Bayer Schering Pharma AG
//			Felix Fehlberg; FHW-BA Berlin; Berliner Volksbank eG
//			Steffen Kalisch; FHW-BA Berlin; SANA IT Services GmbH

package platform;
public class PlatForm	{
public native char nativeCharIn();
public native void nativeCharOut(char c);
public native int conStat(); // inkey
public native byte getButtons();
public native void setOnBoardLEDs(byte status);
public native void charLCDOut(char c);
public native void controlLCD(int control);
public native void exit (int status);
public native int currentTimeMillis();
public native char ser1In();
public native void ser1Out(char c);
public native char ser1Stat();
public native void ser1RTS(char c);// -12 V 0??
public native void ser1DTR(char c);// +12 V 1??
public native void pwmStart(int channel, int pulseLength, int frequency);
public native void pwmStop (int channel);
public native int adcGetValue (int channel, int mode);
public PlatForm()	{}
}
