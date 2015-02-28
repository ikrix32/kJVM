package java.lang;

//bh
import java.io.*;
import platform.*;

/**
 * System utilities.
 */
public final class System {
	public final static PlatForm platform = new PlatForm();
	public static final PrintStream out = new PrintStream(platform);
	public static final InputStream in = new InputStream(platform);
	public static final PrintStream err = out;

	private System() {
	}

	//Copies one array to another.
	static void arraycopy(char[] src, int srcOffset, char[] dest,
			int destOffset, int length) {
		for (int i = 0; i < length; i++)
			dest[i + destOffset] = src[i + srcOffset];
	}

	//
	public static void exit(int code) {
		platform.exit(code);
	}

	//
	public static int currentTimeMillis() {
		return platform.currentTimeMillis();
	}
}
//*/