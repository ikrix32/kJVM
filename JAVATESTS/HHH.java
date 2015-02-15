/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */

import java.lang.*;

public class HHH extends Thread {

	public static void main(String[] args) {
		HHH d = new HHH();
		d.start();
		while (true)
			d.out1("ha");
	}

	public synchronized void out1(String s) {
		System.out.print("hu");
		System.out.println(s);
	}

	public void run() {
		while (true)
			out1("ho");
	}
}