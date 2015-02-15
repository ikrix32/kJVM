/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
import java.util.*;
import platform.*;

public class EVK1104b {
	public static void main(String[] args) {

		int x;
		char c;
		int y = 0;
		Display display = new Display();

		System.out
				.println("ACD-Test... end with 'e', other keys -> give value out");
		ADC adc = new ADC();
		adc.enable();
		while (true) {
			System.out.println(adc.getValue());
			if ((System.in.read()) == 'e')
				break;
		}
		adc.disable();

		System.out.println("ADC Test end");

		System.out.println("LCD-Test end with 'e' ");

		while (true) {
			y++;
			display.setColor(0xF0F0);
			display.setBackgroundColor(0x0000);
			// Rects
			display.drawFillRect(0, 0, 40, 40);
			display.drawRect(45, 0, 40, 40, 2);
			display.drawFillRect(100, 0, 40, 40, 0xFFFF);
			display.drawRect(155, 0, 40, 40, 2, 0xFFFF, 0x0F0F);
			display.drawRect(210, 0, 40, 40, 2, 0x0F0F);

			// Circles
			display.drawFillCircle(25, 70, 25);
			display.drawCircle(80, 70, 25, 2);
			display.drawFillCircle(135, 70, 25, 0xFFFF);
			display.drawCircle(190, 70, 25, 2, 0xFFFF, 0x0F0F);
			display.drawCircle(245, 70, 25, 2, 0x0F0F);

			// Texts
			display.drawString("LCD Text", 1, 0, 100);
			display.drawString("steht hier", 2, 65, 100);
			display.drawString("auf dem", 3, 155, 100);
			display.drawString("EVK1104", 1, 0, 120, 0xFFFF, 0x0F0F);
			display.drawString("Display", 2, 65, 120, 0xFFFF, 0x0F0F);
			display.drawString("in tollen", 3, 145, 120, 0xFFFF, 0x0F0F);
			display.drawString("Farben", 3, 220, 120, 0x0F0F);

			// Pixels
			display.drawPoint(10, 140);
			display.drawPoint(11, 140);
			display.drawPoint(10, 141);
			display.drawPoint(11, 141);
			display.drawPoint(20, 140, 0x0F0F);
			display.drawPoint(21, 140, 0x0F0F);
			display.drawPoint(20, 141, 0x0F0F);
			display.drawPoint(21, 141, 0x0F0F);

			// Parcial Circles
			boolean[] circleParts = { true, false, true, false };
			display.drawPartialFillCircle(25, 180, 25, circleParts);
			display.drawPartialCircle(80, 180, 25, 2, 0xCC);
			display.drawPartialFillCircle(135, 180, 25, 0x33, 0xFFFF);
			display.drawPartialCircle(190, 180, 25, 2, 0x33, 0xFFFF, 0x0F0F);
			display.drawPartialCircle(245, 180, 25, 2, 0x33, 0x0F0F);

			// Lines
			display.drawHorizLine(0, 210, 30);
			display.drawHorizLine(40, 210, 30, 0x0F0F);
			display.drawVertLine(0, 215, 20);
			display.drawVertLine(40, 215, 20, 0x0F0F);
			display.drawLine(0, 236, 20, 240);
			display.drawLine(30, 236, 70, 240, 0X0F0F);
			if ((System.in.read()) == 'e')
				break;
		}
		display.clearDisplay();
		y = 0;
		System.out.println("LCD-Test vorbei...");

		System.out.println("LED-Test...");

		System.out
				.println("Test 1: Bit-Zaehler, Druecke eine Taste zum zaehlen, zum Beenden 'e' druecken");
		LEDs leds = new LEDs();
		while (true) {
			// System.out.println((y) + " an-> " + (y^0xf) + "aus!");
			// System.platform.setLEDon(y);
			// System.platform.setLEDoff(y^0xf);
			leds.activateBits(y);

			y = y + 1;
			if (y == 16) {
				y = 0;
			}
			if ((System.in.read()) == 'e')
				break;
		}
		System.out
				.println("Test 2: Manuell, Druecke 1-4 um die entsprechende LED umzuschalten, zum Beenden 'e' druecken");

		while ((c = System.in.read()) != 'e') {
			if (c == '1')
				leds.toggle(1);
			if (c == '2')
				leds.toggle(2);
			if (c == '3')
				leds.toggle(3);
			if (c == '4')
				leds.toggle(4);
		}
		System.out.println("LED-Test vorbei...");
		System.out
				.println("Touch-Wheel/-Button-Test mit Anzeige auf LCD (Button 1 und 4 zum beenden druecken)");
		while (true) {
			display.clearDisplay();
			int bitmask = 0x0000;
			x = (System.platform.getButtons() & 0xffff);
			// int bitmask = 0x0000;

			if ((x & 0x000e) != 0) {
				bitmask = (bitmask | 0x03);
			}
			if ((x & 0x0130) != 0) {
				bitmask = (bitmask | 0xC0);
			}
			if ((x & 0x0e00) != 0) {
				bitmask = (bitmask | 0x30);
			}
			if ((x & 0x3001) != 0) {
				bitmask = (bitmask | 0x0C);
			}
			if ((x & 0x0040) != 0) {
				display.drawFillRect(30, 10, 40, 40);
			}
			if ((x & 0x0080) != 0) {
				display.drawFillRect(90, 10, 40, 40);
			}
			if ((x & 0x4000) != 0) {
				display.drawFillRect(190, 10, 40, 40);
			}
			if ((x & 0x8000) != 0) {
				display.drawFillRect(250, 10, 40, 40);
			}
			/*
			 * if (Touch.isTouched(1)||Touch.isTouched(2)||Touch.isTouched(3))
			 * {bitmask = (bitmask | 0x03);} if
			 * (Touch.isTouched(4)||Touch.isTouched(5)||Touch.isTouched(8))
			 * {bitmask = (bitmask | 0xC0);} if
			 * (Touch.isTouched(9)||Touch.isTouched(10)||Touch.isTouched(11))
			 * {bitmask = (bitmask | 0x30);} if
			 * (Touch.isTouched(12)||Touch.isTouched(13)||Touch.isTouched(0))
			 * {bitmask = (bitmask | 0x0C);} if (Touch.isTouched(6))
			 * display.drawFillRect(30,10,40,40); if (Touch.isTouched(7))
			 * display.drawFillRect(90,10,40,40); if (Touch.isTouched(14))
			 * display.drawFillRect(190,10,40,40); if (Touch.isTouched(15))
			 * display.drawFillRect(230,10,40,40);
			 */
			display.drawPartialFillCircle(160, 120, 50, bitmask);

			// System.platform.setLEDon(y);
			// System.platform.setLEDoff(y^0xff);
			// y += 1;
			// if (y==9) {y=0;}
			// if(Touch.isTouched(6) & Touch.isTouched(15))
			if (x == 0x8040)
				break;
		}
		display.drawPartialCircle(160, 120, 100, 10, 0xF0);
		display.drawVertLine(155, 80, 60);
		display.drawVertLine(156, 80, 60);
		display.drawVertLine(157, 80, 60);
		display.drawVertLine(158, 80, 60);
		display.drawVertLine(159, 80, 60);
		display.drawVertLine(160, 80, 60);
		display.drawVertLine(161, 80, 60);
		display.drawVertLine(162, 80, 60);
		display.drawVertLine(163, 80, 60);
		display.drawVertLine(164, 80, 60);
		display.drawVertLine(165, 80, 60);
		System.out.println("Ende Touch-Wheel/-Button-Test");

		System.out.println("Test beendet");

		// END Example for Display usage

	}
}
