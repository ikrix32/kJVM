/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
import java.io.IOException;
import java.util.*;

public class EVK1104 {
	public static void main(String[] args) throws IOException {
		char c;
		// not q!!!
		System.out.println("currentTimeMillis Test, type any key -> e for end");
		while ((c = (char) System.in.read()) != 'e')
			System.out.println(System.currentTimeMillis());
		System.out.println("currentTimeMillis Test in a \"for loop\"");
		for (c = 0; c < 4; c++)
			System.out.println(System.currentTimeMillis());

		System.out.println("end test");

	}
}
