package java.io;

import platform.*;
import java.lang.*;

public class InputStream {
	PlatForm platform;

	public InputStream(PlatForm platform) {
		this.platform = platform;
	}

	public char read() {
		return platform.nativeCharIn();

	}
}