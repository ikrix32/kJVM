/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "lijjjjjjjecense.terms" for information on usage and redistribution of this file.
 */

public class EVK1100 {

	public static void main(String[] args) {
		System.out.println("thats a EVK1100 board lcd test end press e");
		System.platform.controlLCD(0x00000000); // clear
		System.platform.controlLCD(0x00010000); // set cur pos 0,0
		char c;
		// do not press q!!!
		while ((c = System.in.read()) != 'e')
			System.platform.charLCDOut(c);
		System.out.println("millisec test");
		for (c = 0; c < 4; c++)
			System.out.println(System.currentTimeMillis());

		System.out.println("jetzt Button-Test end press n");
		while (System.platform.conStat() != 66) {
			int b = System.platform.getButtons() & 0xff;
			System.platform.controlLCD(0); // clear
			System.platform
					.charLCDOut((char) (((b >>> 4) < 10) ? ((b >>> 4) + '0')
							: ((b >>> 4) + 'A' - 10)));
			System.platform
					.charLCDOut((char) (((b % 16) < 10) ? ((b % 16) + '0')
							: ((b % 16) + 'A' - 10)));
		}

		c = 0xff;
		System.out.println("jetzt LED-Test end press e");
		while (System.in.read() != 'e') {
			System.platform.setOnBoardLEDs((byte) c);
			if (c > 0xf) {
				c >>>= 1;
				continue;
			}
			if (c > 3) {
				c -= 4;
				continue;
			}
			c--;
			c &= 0xff;
		}
		;
		System.out.println("jetzt PWM-Test end test e\n give channel");
		System.out.println("  0    1    2    3    4    5    6");
		System.out.println("PB19 PB20 PB21 PB22 PB27 PB28 PB18");

		// @author Felix Fehlberg / Berliner Volksbank / IT07
		int channel = System.in.read() - '0';// 0; //default channel
		int pulseLength = 20; // default pulse length
		int frequency = 40; // default frequency
		// start output with default settings
		// On EVK1100, PWM0 is pin number 51 (PB19) Also, LED? is connected to
		// PB21. //Thus, you should also see LED? blink.
		System.platform.pwmStart(channel, pulseLength, frequency);
		while ((c = System.in.read()) != 'e') {
			// stop output
			// System.platform.pwmStop(channel);
			// if the character is '-' then increase pulse lenght
			// (decrease period of high)
			if ((c == '-') & (pulseLength < frequency))
				pulseLength = pulseLength + 1;
			// if the character is '+' then decrease pulse lenght
			// (increase period of high)
			if ((c == '+') & (pulseLength > 0))
				pulseLength = pulseLength - 1;
			// if the character is 'n' then decrease frequency
			if ((c == 'n') & (pulseLength < frequency))
				frequency = frequency - 1;
			// if the character is 'm' then increase frequency
			if ((c == 'm'))
				frequency = frequency + 1;
			// start output with new settings
			System.platform.pwmStart(channel, pulseLength, frequency);
		}
		// stop output after exitting
		System.platform.pwmStop(channel);

		// added 13.03.2009 by: Steffen Kalisch, FHW-BA Berlin
		// Sana IT-Services GmbH
		//
		// Call function
		// first parameter = cannel
		// 1 = TEMP
		// 2 = LIGHT
		// 3 = POTI
		// second parameter = mode (currently unused)
		System.out.println("BAJOS - Sensor readout (ADC) end press e");
		System.out.println("######################### type s");
		while ((c = System.in.read()) != 'e') {
			System.out.println("Please enter the channel:");
			System.out.println("Channel 1 = temperature");
			System.out.println("Channel 2 = light");
			System.out.println("Channel 3 = potentiometer");

			switch (c) {
			case '1':
				System.out.println("temp:"
						+ System.platform.adcGetValue(c - '0', 0));
				break;
			case '2':
				System.out.println("light:"
						+ System.platform.adcGetValue(c - '0', 0));
				break;
			case '3':
				System.out.println("poti:"
						+ System.platform.adcGetValue(c - '0', 0));
			}
		}
	}
}
