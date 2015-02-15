/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
import java.graphics.*;

//ZBuffer Bsp 5 HSB
public class C {
	DisplayZBuffer dp = new DisplayZBuffer();
	int alpha = 0; // Beispiel für eine zeitlich sich ändernde V.
	int deltaAlpha = 20; // Änderung von alpha/Zeitscheibe

	public static void main(String[] argv) {
		C mv = new C();

		while (true) {
			mv.drawWorld();
		}
	}

	public void drawWorld() {
		dp.clearDisplay();
		dp.font.setFontIndex(0);
		dp.font.setValues();
		dp.font.setCursor(100, 10);
		dp.font.drawString("ZBuffer Test");
		dp.font.setFontIndex(1);
		dp.font.setValues();
		dp.font.setCursor(240, 5);
		dp.font.drawString("BAJOS");

		Polyline pl = new Polyline();
		DisplayZBuffer.setHSBColor(80, 255, 255);
		pl.add(50, 20, 60);
		DisplayZBuffer.setHSBColor(80, 255, 200);
		pl.add(70, 30, 60);
		DisplayZBuffer.setHSBColor(80, 255, 160);
		pl.add(77, 60, 60);
		DisplayZBuffer.setHSBColor(80, 255, 120);
		pl.add(40, 67, 60);
		DisplayZBuffer.setHSBColor(80, 255, 80);
		pl.add(40, 88, 60);
		DisplayZBuffer.setHSBColor(80, 255, 60);
		pl.add(7, 22, 60);

		Polyline tr = new Polyline();
		DisplayZBuffer.setHSBColor(140, 255, 255);
		tr.add(45, 10, 25);
		DisplayZBuffer.setHSBColor(140, 255, 255);
		tr.add(90, 15, 30);
		DisplayZBuffer.setHSBColor(140, 255, 255);
		tr.add(60, 80, (60 + alpha));

		dp.scanFill(pl);
		dp.scanFill(tr);

		alpha += deltaAlpha;
	}
}