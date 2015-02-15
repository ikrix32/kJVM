/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
public class FFF {// extends Thread {
	static String aaa = new String("aaa");
	int nn;

	FFF(int n) {
		this.nn = n;
	}

	public static void main(String[] args) {

		FFF f = new FFF(3);
		System.out.println("hello");
		for (int j = 0; j < 100; j++)
			for (int i = 0; i < 28; i++) {
				System.out.println(10000 * j + f.add(i));
			}
	}

	public int add(int n) {
		FFF f = new FFF(n);
		if (n == 0)
			return 0;
		return (n + add(n - 1));
	}

}