/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/

package platform;

public class PlatForm	{
public native char nativeCharIn();
public native void nativeCharOut(char c);
public native void exit (int status);
public native int currentTimeMillis();
public PlatForm()	{}
}
