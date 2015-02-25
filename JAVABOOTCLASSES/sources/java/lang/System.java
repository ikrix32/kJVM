package java.lang;

//bh
import java.io.*;
import platform.*;

/**
 * System utilities.
 */
public final class System {
	public final static PlatForm platform = new PlatForm();
	public static final OutStream out = new OutStream(platform);
	public static final InStream in = new InStream(platform);
	public static final OutStream err = out;

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