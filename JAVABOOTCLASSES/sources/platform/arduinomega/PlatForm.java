/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
//@authors  22.02.2008; Anna Maria Damm; FHW-BA Berlin; Bayer Schering Pharma AG
//	 		H.-Christian Hecht, FHW-BA Berlin; CoMedServ GmbH
//			Adrian Lang,FHW-BA Berlin; Fritz-Haber-Institut
//			Stephan Bauer; FHW-BA Berlin; Bayer Schering Pharma AG
//	    13.02.2009; Sebastian Starroske; FHW-BA Berlin; Bayer Schering Pharma AG
//	    09.03.2009; Friedrich Grosse; FHW-BA Berlin; Deutsche Post IT-Services
//			Rainer Kirchhoff ; FHW-BA Berlin; MSA Auer GmbH

package platform;

public class PlatForm	{
public native char nativeCharIn();
public native void nativeCharOut(char c);
public native void exit (int status);
public native byte currentTimeMillis();
public native byte nativeConStat();
public native void nativeSetData(int addr, byte value);	// sdram
public native byte nativeGetData(int addr);				// sdram
public PlatForm()	{}
}
