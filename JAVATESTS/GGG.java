/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */

import java.lang.*;

public class GGG extends Thread {
	char c;
	int n;

	public GGG(char c, int n) {
		this.c = c;
		this.n = n;
	}

	public static void main(String[] args) {
		GGG d = new GGG('d', 3);
		GGG e = new GGG('e', 2);
		GGG f = new GGG('f', 1);
		// d.start();
		e.start();
		// while (true)
		f.run();
	}

	public void run() {
		for (int i = 0; i < n; i++) {
			System.out.print('a');
			System.out.println(c);
		}
	}
}