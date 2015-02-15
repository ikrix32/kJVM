package java.lang;

/* Class-specific differences to J2SE 1.4.2:
 * No decode() method
 * See also README for general differences in BAJOSBOOT
 * */

public final class Byte {
	public final static byte MAX_VALUE = 127;
	public final static byte MIN_VALUE = -128;

	private byte byteval;

	public Byte(byte val) {
		this.byteval = val;
	}

	public Byte(String s) {
		this(Byte.parseByte(s, 10));
	}

	public static String toString(byte val) {
		if (val == Byte.MIN_VALUE) { /*
									 * because the -val trick doesn't work with
									 * -128 since 128 is not in range
									 */
			return "-128";
		}

		if (val == 0) {
			return "0";
		}

		char sign = 0;
		char buf[] = new char[4];
		if (val < 0) {
			sign = '-';
			val = (byte) -val;
		}

		byte charPos = 4;
		while (val != 0) {
			byte q = (byte) (val / 10);
			buf[--charPos] = (char) ((int) '0' + val - (q * 10));
			val = q;
		}

		if (sign != 0) {
			buf[--charPos] = sign;
		}

		return new String(buf, charPos, 4 - charPos);
	}

	public String toString() {
		return toString(byteval);
	}

	public static byte parseByte(String s) {
		return parseByte(s, 10);
	}

	public static byte parseByte(String s, int radix) {
		if (s == null) {
			throw new IllegalArgumentException();
		}
		int len = s.length();
		if (len == 0) {
			throw new IllegalArgumentException();
		}
		int pos = 0;
		int val = 0; // has to be int since 128 does not fit in a byte, yet may
						// occur while parsing "-128"
		boolean neg = (s.charAt(0) == '-');
		if (neg) {
			if (len == 1) {
				throw new IllegalArgumentException();
			}
			++pos;
		}
		for (; pos < len; ++pos) {
			int nextdigit = Character.digit(s.charAt(pos), radix);
			if (nextdigit == -1) {
				throw new IllegalArgumentException();
			}
			val = val * radix + nextdigit;
		}
		val = neg ? -val : val;
		if (val < MIN_VALUE || val > MAX_VALUE) {
			throw new IllegalArgumentException();
		}
		return (byte) val;
	}

	public static Byte valueOf(String s) {
		return new Byte(Byte.parseByte(s, 10));
	}

	public boolean equals(Object obj) {
		return obj instanceof Byte && ((Byte) obj).byteval == this.byteval;
	}

	public byte byteValue() {
		return byteval;
	}

}
