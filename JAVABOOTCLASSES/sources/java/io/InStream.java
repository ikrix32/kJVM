package java.io;

import platform.*;
import java.lang.*;

public class InStream {
	PlatForm platform;

	public InStream(PlatForm platform) {
		this.platform = platform;
	}

	public char read() {
		return platform.nativeCharIn();

	}
}