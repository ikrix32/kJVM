package java.io;
import platform.PlatForm;
import java.lang.*;

public class OutStream {
	PlatForm platform;

	public OutStream(PlatForm platform) {
		this.platform = platform;
	}

	public synchronized void print(char c) {
		platform.nativeCharOut(c);
	}

	public synchronized void print(String s) {
		for (int i = 0 ; i < s.length() ; i++) {
			print(s.charAt(i));
		}
	}

	public synchronized void print(int n) {
		print(Integer.toString(n));
	}

	public synchronized void print(float f) {
		print(Float.toString(f));
	}


	public synchronized void print(boolean b) {
		print(Boolean.toString(b));
	}

	public synchronized void print(Object obj) {
		print(obj.toString());
	}

	public synchronized void println() {
		print('\n');
	}

	public synchronized void println(char c) {
		print(c);
		println();
	}

	public synchronized void println(String s) {
		print(s);
		println();
	}

	public synchronized void println(int n) {
		print(n);
		println();
	}

	public synchronized void println(float f) {
		print(f);
		println();
	}

	public synchronized void println(boolean b) {
		print(b);
		println();
	}

	public synchronized void println(Object obj) {
		print(obj);
		println();
	}
}
