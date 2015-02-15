/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
import java.graphics.*;

//Bsp 6, 7 ModelView
public class D {
	AffineMatrix mv = new AffineMatrix();
	Polyline quadrat = new Polyline();
	DisplayZBuffer dp = new DisplayZBuffer();
	float alpha = 0f; // Beispiel für eine zeitlich sich ändernde V.
	float deltaAlpha = 10f; // Änderung von alpha/Zeitscheibe

	public static void main(String[] argv) {
		D mv = new D();

		while (true) {
			mv.drawWorld();
		}
	}

	// keine Verwendung von "Punktfarben" mehr
	// auf Primitives (polylines) wirkt die globale Farbe
	// realisiert durch : public Point mult(Point p)// p=m*p in AffineMatrix
	// stepp=0.99999 in Dda.java (2) verhindert Rundungsfehler!!

	D() {
		quadrat.add(1f, 1f, 0f); // Einheitsquadrat z=0
		quadrat.add(1f, -1f, 0f);
		quadrat.add(-1f, -1f, 0f);
		quadrat.add(-1f, 1f, 0f);
	}

	public void drawWorld() {
		dp.clearDisplay();
		dp.font.setFontIndex(0);
		dp.font.setValues();
		dp.font.setCursor(80, 10);
		dp.font.drawString("AffineMatrix Test");
		dp.font.setFontIndex(1);
		dp.font.setValues();
		dp.font.setCursor(240, 5);
		dp.font.drawString("BAJOS");

		mv.loadIdentity();
		// init, scale, rotate, um Hauptachse, translate
		mv.translate(128f, 128f, 0f);
		mv.scale(20f, 20f, 20f);
		mv.rotate(3f, 2f, 1f, 10f * alpha);
		drawUnitCube(); // Kantenlänge 2 im Ursprung!!

		alpha += deltaAlpha;
	}

	public void drawUnitCube() { // Einheitswuerfel
		mv.pushMatrix();
		mv.translate(0f, 0f, 1f);
		drawGlobalFilledQuadrat(0f, 255f, 255f);
		mv.popMatrix();

		mv.pushMatrix();
		mv.translate(0f, 0f, -1f);
		mv.rotate(1f, 0f, 0f, 180f);
		drawGlobalFilledQuadrat(30f, 255f, 255f);
		mv.popMatrix();

		mv.pushMatrix();
		mv.translate(1f, 0f, 0f);
		mv.rotate(0f, 1f, 0f, 90f);
		drawGlobalFilledQuadrat(60f, 255f, 255f);
		mv.popMatrix();

		mv.pushMatrix();
		mv.translate(-1f, 0f, 0f);
		mv.rotate(0f, 1f, 0f, -90f);
		drawGlobalFilledQuadrat(90f, 255f, 255f);
		mv.popMatrix();

		mv.pushMatrix();
		mv.translate(0f, 1f, 0f);
		mv.rotate(1f, 0f, 0f, -90f);
		drawGlobalFilledQuadrat(120f, 255f, 255f);
		mv.popMatrix();

		mv.pushMatrix();
		mv.translate(0f, -1f, 0f);
		mv.rotate(1f, 0f, 0f, 90f);
		drawGlobalFilledQuadrat(150f, 255f, 255f);
		mv.popMatrix();
	}

	public void drawGlobalFilledQuadrat(float hue, float saturation, float value) {
		DisplayZBuffer.setHSBColor(hue, saturation, value);
		filledDraw(quadrat);
	}

	/*
	 * Bsp 6 public void filledDraw(Polyline pl) { dp.scanFill(mv.mult(pl)); }
	 */

	// Bsp 7
	public void filledDraw(Polyline pl) {// back face culling
		if (((mv.mult(pl).pointAt(0).getX() - mv.mult(pl).pointAt(1).getX())
				* (mv.mult(pl).pointAt(2).getY() - mv.mult(pl).pointAt(1)
						.getY()) - (mv.mult(pl).pointAt(0).getY() - mv.mult(pl)
				.pointAt(1).getY())
				* (mv.mult(pl).pointAt(2).getX() - mv.mult(pl).pointAt(1)
						.getX())) >= 0f)
			return;
		dp.scanFill(mv.mult(pl));
	}

	public void polylineDraw(Polyline pl) {
		dp.drawPolyline(mv.mult(pl));
	}

	public void pointDraw(Point p) {
		dp.drawPoint(mv.mult(p));
	}
}