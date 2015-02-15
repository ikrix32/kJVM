/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
import java.graphics.*;

public class A {

	public static void main(String[] args) {
		Display dp = new Display();
		dp.font.setCursor(100, 10);
		dp.font.drawString("Grafik Test");
		dp.font.setFontIndex(1);
		dp.font.setValues();
		dp.font.setCursor(240, 5);
		dp.font.drawString("BAJOS");

		dp.drawRectangle(10, 100, 60, 160);

		dp.setColor(0xff0000); // blau -- BGR
		dp.drawFillRect(40, 20, 60, 40);

		dp.setColor(0x00ff00); // gruen...
		dp.drawRoundRect(70, 70, 100, 100, 10, 20);

		dp.setColor(0x0000ff); // rot
		dp.drawEllipse(110, 110, 70, 220);
		dp.drawCircle(160, 160, 30);
		dp.drawLine(1, 1, 310, 230);

		System.out.println(" ENDE");

		while (true) {

		}

	}
}
