/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
import java.util.*;

public class EVK1104a {
	public static void main(String[] args) {
		char c;
		int n = 0;

		// START Example for Display usage

		// First examples using drawFillRect
		System.platform.initDisplay();
		System.out
				.println("press any key for funny Display or e for exit anc c to clean Display");
		while ((c = System.in.read()) != 'e') {
			if (c == 'c')
				System.platform.drawFillRect(0, 0, 320, 240, 0xFFFF);
			else {
				// 320 240 Display
				for (int i = 0; i < 100; i++) {
					Random rand = new Random();
					int color = rand.nextInt() % 0xFFFF;
					if (color < 0)
						color = color * -1;
					int x = rand.nextInt() % 320;
					if (x < 0)
						x = x * -1;
					int y = rand.nextInt() % 240;
					if (y < 0)
						y = y * -1;
					System.platform.drawFillRect(x, y, 20, 20, color);
				}
			}
		}

		// End Example

		System.platform.drawHorizLine(10, 10, 50, 0xFA30);
		System.platform.drawFillRect(20, 20, 30, 30, 0x0FB4);
		System.platform.drawVertLine(60, 60, 60, 0x00FF);
		for (int i = 1; i < 10; i++) {
			System.platform.drawPixel(i, i * i, 0x0000);
			System.platform.drawPixel(i - 1, i * i - 1, 0x0000);
			System.platform.drawPixel(i - 1, i * i + 1, 0x0000);
			System.platform.drawPixel(i + 1, i * i + 1, 0x0000);
			System.platform.drawPixel(i + 1, i * i - 1, 0x0000);
		}
		int x;
		// while((c=System.in.read()) != 'e'){
		while (true) {
			// System.platform.drawFillRect(0,0,320,240,0xFFFF);
			x = (System.platform.getButtons() & 0xffff);
			// System.out.println("Button: " + x);
			// DISPLAY 240 x 320 pixel
			int bitmask = 0x0000;
			// if (((x & 0x0002) == 2) || ((x & 0x0004) == 4) || ((x & 0x0008)
			// == 8)) {bitmask = (bitmask | 0x03);} // 1. Quadrant
			// if (((x & 0x0010) == 16) || ((x & 0x0020) == 32) || ((x & 0x0100)
			// == 256)) {bitmask = (bitmask | 0xC0);} // 4. Quadrant
			// if (((x & 0x0200) == 512) || ((x & 0x0400) == 1024) || ((x &
			// 0x0800) == 258)) {bitmask = (bitmask | 0x30);} // 3. Quadrant
			// if (((x & 0x1000) == 4096) || ((x & 0x2000) == 8192) || ((x &
			// 0x0001) == 1)) {bitmask = (bitmask | 0x0C);} // 2. Quadrant
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
				System.platform.drawFillRect(30, 10, 40, 40, 0xF0F0);
			} else {
				System.platform.drawFillRect(30, 10, 40, 40, 0xFFFF);
			}
			if ((x & 0x0080) != 0) {
				System.platform.drawFillRect(90, 10, 40, 40, 0xF0F0);
			} else {
				System.platform.drawFillRect(90, 10, 40, 40, 0xFFFF);
			}
			if ((x & 0x4000) != 0) {
				System.platform.drawFillRect(190, 10, 40, 40, 0xF0F0);
			} else {
				System.platform.drawFillRect(190, 10, 40, 40, 0xFFFF);
			}
			if ((x & 0x8000) != 0) {
				System.platform.drawFillRect(230, 10, 40, 40, 0xF0F0);
			} else {
				System.platform.drawFillRect(230, 10, 40, 40, 0xFFFF);
			}

			System.platform.drawFilledCircle(160, 120, 50, 0xF0F0, bitmask);
			System.platform.drawFilledCircle(160, 120, 50, 0xFFFF,
					bitmask ^ 0xffff);

			// if (x==0) {System.platform.drawFillRect(0,0,320,240,0x0F0F);}
			// if (x==6) {System.platform.drawFillRect(0,0,320,240,0xFFFF);}
			// if (x==7) {System.platform.drawFillRect(0,0,320,240,0x0000);}
			// if (x==14) {System.platform.drawFillRect(0,0,320,240,0xF0F0);}
			// if (x==15) {System.platform.drawFillRect(0,0,320,240,0x123F);}
			// if (x==8) {System.platform.drawFillRect(0,0,320,240,0xABC0);}
		}

		// System.out.println("end test");
		// END Example for Display usage

	}
}
