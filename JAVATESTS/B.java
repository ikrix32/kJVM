/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
import java.graphics.*;

public class B {

	public static void main(String[] args) {
		DisplayHSB dp = new DisplayHSB();
		dp.font.setCursor(100, 10);
		dp.font.drawString("Grafik Test HSB");
		dp.font.setFontIndex(1);
		dp.font.setValues();
		dp.font.setCursor(240, 5);
		dp.font.drawString("BAJOS");

		Point p0, p1, p2;
		p0 = new Point(50, 120);
		p0.HSBColor(0, 255, 255);
		p1 = new Point(230, 160);
		p1.HSBColor(127, 255, 255);
		p2 = new Point(70, 180);
		p2.HSBColor(255, 255, 255);

		dp.triangleFill(p0, p1, p2);

		// Bsp 3 HSB, kein ZBuffer
		Polyline pl = new Polyline();
		DisplayHSB.setHSBColor(0, 255, 255);
		pl.add(50, 29);
		pl.add(70, 40);
		DisplayHSB.setHSBColor(80, 255, 255);
		pl.add(72, 70);
		pl.add(49, 47);
		DisplayHSB.setHSBColor(160, 255, 255);
		pl.add(10, 98);
		DisplayHSB.setHSBColor(100, 255, 255);
		pl.add(7, 32);
		dp.drawPolyline(pl);

		// ScanFill + triangleFill Bsp 4 HSB, kein ZBuffer
		DisplayHSB.setHSBColor(120, 255, 255);
		pl = new Polyline();
		pl.add(150, 70);
		DisplayHSB.setHSBColor(120, 255, 255);
		pl.add(170, 100);
		DisplayHSB.setHSBColor(120, 255, 128);
		pl.add(172, 130);
		DisplayHSB.setHSBColor(120, 255, 60);
		pl.add(149, 148);
		DisplayHSB.setHSBColor(220, 255, 255);
		pl.add(110, 110);
		DisplayHSB.setHSBColor(220, 255, 128);
		pl.add(117, 92);
		dp.scanFill(pl);

		System.out.println(" ENDE");

		while (true) {

		}

	}
}
