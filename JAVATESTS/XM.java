/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */

public class XM {
	public static void main(String[] args) {
		char c;
		int n = 0;
		for (n = 0; n < 20; n++)
			System.out.println("a " + n + " b");

		// not q!!!
		System.out.println("currentTimeMillis Test, type any key -> e for end");
		while ((c = System.in.read()) != 'e')
			System.out.println(System.currentTimeMillis());

		System.out.println("currentTimeMillis Test in a \"for loop\"");
		for (c = 0; c < 4; c++)
			System.out.println(System.currentTimeMillis());

		System.out
				.println("1 sec blinking led an arduinoMega -> type any char for closing application");
		System.platform.nativeSetData(0x24,
				(byte) (System.platform.nativeGetData((byte) 0x24) | 0x80));
		// DDRB PB7 output ->LED
		int currentTime = System.currentTimeMillis();

		while (System.platform.nativeConStat() == 0) {
			while ((System.currentTimeMillis() - currentTime) < 1000)
				;
			System.platform.nativeSetData(0x25,
					(byte) (System.platform.nativeGetData(0x25) ^ 0x80)); // PORTB
																			// toggle
			currentTime += 1000;
		}

		System.in.read(); // dummy read
	}
}
