/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
//by CEH
/*
 * This example is only for arduino and linux, please consider you comment in and out the right lines, see comments
 */
import java.util.concurrent.InterruptThread;

public class InterruptTest extends InterruptThread {

	public void isr() {
		// toggle LED on portb
		// comment in for arduino; comment out for linux!
		System.platform.nativeSetData(0x25,
				(byte) (System.platform.nativeGetData(0x25) ^ 0x80));
		// System.out.println("Interrupt Occured in Java!!"); //comment out for
		// arduino; comment in for linux!
	}

	public static void main(String[] args) {

		cli();
		InterruptTest inter = new InterruptTest();
		inter.initInterrupt((byte) 23);

		initTimerAndLed(); // comment in for arduino; comment out for linux!

		// enable Global Interrupts
		sei();

		for (int i = 0; i < 100000; i++) {
			// comment out for arduino; comment in for linux!
			// InterruptTest.forceInterrupt((byte)23);
		}

		InterruptTest.removeInterrupt((byte) 23);

	}

	// comment in for arduino; comment out for linux!
	public static void initTimerAndLed() {

		byte DDRB = System.platform.nativeGetData(0x24);

		DDRB |= 0x80;

		System.platform.nativeSetData(0x24, DDRB);

		System.platform.nativeSetData(0x44, (byte) 0); // TCCR0A normal
														// increment with
														// overflow,

		byte TCCR0B = System.platform.nativeGetData(0x45);
		// CS02 CS00
		TCCR0B |= (1 << 2) | (1 << 0); // Prescaler 1024
		System.platform.nativeSetData(0x45, TCCR0B);

		byte TIMSK0 = System.platform.nativeGetData(0x6e);
		// enable Overflow Interrupt
		TIMSK0 |= (1 << 0); // TOIE0
		System.platform.nativeSetData(0x6e, TIMSK0);

	}
}
