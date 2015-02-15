/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
//import java.graphics.*;

public class Charon {

	public static void main(String[] args) {
		System.out.println("thats a Charon board");
		System.out.println("LCD-Test type e for \"end\"");
		System.platform.controlLCD(0x00000000); // clear
		char c;
		// not q!!!

		while ((c = System.in.read()) != 'e') {
			System.platform.charLCDOut(c);
			System.out.println(System.currentTimeMillis());
		}
		System.out.println("currentTimeMillis Test in a \"for loop\"");
		for (c = 0; c < 4; c++)
			System.out.println(System.currentTimeMillis());
		System.out.println("1 sec blinking leds on charon");
		System.platform.nativeSetPort((char) 0x31, (byte) 0xff);
		int currentTime = System.currentTimeMillis();
		int tries = 0;
		System.out.println("give a number: ");
		int myChoice = hexIn();
		int secretNumber = currentTime & 0x000000ff;
		while (myChoice != secretNumber) {
			if (myChoice > secretNumber)
				System.platform.nativeSetPort((char) 0x32, (byte) 0xfe);
			else
				System.platform.nativeSetPort((char) 0x32, (byte) 0x7f);
			tries++;
			System.out.println("wrong " + tries + " next try");
			myChoice = hexIn();
			System.out.println(myChoice);
		}
		System.out.println("Hurra");
		while (System.platform.nativeConStat() == 0) {
			while ((System.currentTimeMillis() - currentTime) < 1000)
				;
			System.platform.nativeSetPort((char) 0x32,
					(byte) (System.platform.nativeGetPort((char) 0x32) ^ 0xff));
			// PORTB toggle
			currentTime += 1000;
		}
	}

	static int hexIn() { // 0-9 A-F
		int retL, retH = 0;
		retH = System.in.read();
		retH = (retH < 'F') ? retH : retH - 0x20;
		retH = (retH <= '9') ? (retH - '0') : retH - 'A' + 10;
		retL = System.in.read();
		retL = (retL < 'F') ? retL : retL - 0x20;
		retL = (retL <= '9') ? (retL - '0') : retL - 'A' + 10;
		return (retL + 16 * retH) & 0x000000ff;
	}
}
