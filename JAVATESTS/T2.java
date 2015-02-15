/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
public class T2 extends T1 {

	public void doit() {
		System.out.println("t2\t");
	}

	public static void main(String[] args) {
		T2 t2 = new T2();
		T1 t1 = new T1();
		t1.doit();
		t2.doit();
		t1 = t2;
		t1.doit();
	}
}