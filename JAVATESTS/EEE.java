/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
import java.lang.*;

public class EEE extends Thread {
	int m = 6;
	int t;
	char c = '0';
	boolean bool = true;
	float x = Math.PI;
	int[] ii = new int[3];

	public static void main(String[] args) {
		System.out.println(52);
		EEE d = new EEE();

		d.handle();
		d.start();
		for (int i = 10; i < 20; i++) {
			System.out.println(i);
		}
		d.ii[0] = 666;
		d.ii[1] = 777;
		d.ii[2] = 888;

		System.out.println(d.m + 4);
		System.out.println(7.8f + Math.PI);
		System.out.println(d.ii[0]);
		System.out.println(d.ii[1]);
		System.out.println(d.ii[2]);

		System.out.println(8.8f);
		System.out.println(Math.PI + " hallo\n");
		System.out.println(Math.cos(Math.PI));
		d.m = 3;
		d.t = 99;
		d.m = d.getIntVar(5);
		d.bool = true;
		System.out.println(d.m);
		System.out.println(d.x);
		/*
		 * while( d.c!='r') { d.c=System.out.nativeCharIn();
		 * System.out.nativeCharOut(++d.c); }
		 */
		int i = 23;
		for (i = 600; i < 605; i++) {
			boolean isprime = true;
			for (int k = 2; (k * k) <= i; k++)
				if ((i % k) == 0) {
					isprime = false;
				}
			if (isprime == true)
				System.out.println(i);
		}

	}

	public int getIntVar(int var) {
		return var + m;
	}

	public int getIntAdd(int var1, int var2) {

		return var1 + var2;
	}

	public void run() {
		// while (true) {
		System.out.println("\nich renne\n");
		int i = 4;
		System.out.println(i);
		while (i < 5) {
			System.out.println(i++);
		}
		try {
			i = 2;
			i = 5 % i;
			System.out.println(i % i);
			System.out.println("ich gebe aus ");
		} catch (MyArithmeticException e) {
			handle();
			e.init();
			System.out.println(e.i);

			// System.out.println(e.i);

		}

		System.out.println(" nach try catch");
		for (i = 112100; i < 112102; i++) {
			boolean isprime = true;
			for (int k = 2; (k * k) <= i; k++)
				if ((i % k) == 0) {
					isprime = false;
				}
			if (isprime == true)
				System.out.println(i);
		}
		// System.out.println(" is prime");
	}

	public void handle() {
		for (int i = 200; i < 210; i++)
			System.out.println(i);
	}
}

class MyArithmeticException extends ArithmeticException {
	int i;

	public void init() {
		i = 3344;
	}
}
