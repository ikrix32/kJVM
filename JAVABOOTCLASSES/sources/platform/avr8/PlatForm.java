/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
//@authors  22.02.2008; Anna Maria Damm; FHW-BA Berlin; Bayer Schering Pharma AG
//	 		H.-Christian Hecht, FHW-BA Berlin; CoMedServ GmbH
//			Adrian Lang,FHW-BA Berlin; Fritz-Haber-Institut
//			Stephan Bauer; FHW-BA Berlin; Bayer Schering Pharma AG
//	    13.02.2009; Sebastian Starroske; FHW-BA Berlin; Bayer Schering Pharma AG
//	    09.03.2009; Friedrich Große; FHW-BA Berlin; Deutsche Post IT-Services
//			Rainer Kirchhoff ; FHW-BA Berlin; MSA Auer GmbH

package platform;

public class PlatForm	{
public native char nativeCharIn();
public native void nativeCharOut(char c);
public native void exit (int status);
public native int  currentTimeMillis();
public native void charLCDOut(char c);
public native void controlLCD(int control);
public native float getTemperature();
public native int getAnalogueValue(int channel, int mode);
public native byte nativeConStat();
public native void nativeSetBarGraph(int value);
public native byte nativeGetDIL();
public native void nativeSetPort(char port, byte value);
public native void nativeSetIOPort(char port, byte value);
public native byte nativeGetPort(char port);
public native byte nativeGetIOPort(char port);
public PlatForm()	{}
}
