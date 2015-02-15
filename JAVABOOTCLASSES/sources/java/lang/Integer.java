package java.lang;

/**
 * Minimal Integer implementation that supports converting an int to a String.
 *
 */

public final class Integer {
	/**
	 * The smallest value of type <code>int</code>. The constant value of this
	 * field is <tt>-2147483648</tt>.
	 */
	public static final int MIN_VALUE = 0x80000000;

	/**
	 * The largest value of type <code>int</code>. The constant value of this
	 * field is <tt>2147483647</tt>.
	 */
	public static final int MAX_VALUE = 0x7fffffff;

	/**
	 * The value of the Integer.
	 *
	 * @serial
	 */

	private int value;

	/**
	 * Constructs a newly allocated <code>Integer</code> object that represents
	 * the primitive <code>int</code> argument.
	 *
	 * @param value
	 *            the value to be represented by the <code>Integer</code>.
	 */
	public Integer(int value) {
		this.value = value;
	}

	public Integer(String s) {
		this.value = parseInt(s, 10);
	}

	public static int parseInt(String s) {
		return parseInt(s, 10);
	}

	public static int parseInt(String s, int radix) {
		if (s == null) {
			throw new IllegalArgumentException();
		}
		int len = s.length();
		if (len == 0) {
			throw new IllegalArgumentException();
		}
		int pos = 0;
		int val = 0;
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
		return val;
	}

	/**
	 * Returns a new String object representing the specified integer. The
	 * argument is converted to signed decimal representation and returned as a
	 * string, exactly as if the argument and radix <tt>10</tt> were given as
	 * arguments to the toString(int, int) method.
	 *
	 * @param i
	 *            an integer to be converted.
	 * @return a string representation of the argument in base&nbsp;10.
	 */
	public static synchronized String toString(int i) {

		if (i == Integer.MIN_VALUE) {
			return "-2147483648";
		}

		char sign = 0;
		char buf[] = new char[12];
		if (i < 0) {
			sign = '-';
			i = -i;
		}

		int charPos = 12;
		for (;;) {
			int q = i / 10;
			buf[--charPos] = (char) ((int) '0' + (i - (q * 10)));
			i = q;
			if (i == 0)
				break;
		}

		if (sign != 0) {
			buf[--charPos] = sign;
		}

		return new String(buf, charPos, 12 - charPos);
	}

	/**
	 * Returns a String object representing this Integer's value. The value is
	 * converted to signed decimal representation and returned as a string.
	 *
	 * @return a string representation of the value of this object in
	 *         base&nbsp;10.
	 */
	public String toString() {
		return toString(value);
	}

	public static Integer valueOf(String s, int radix) {
		return new Integer(Integer.parseInt(s, radix));
	}

	public static Integer valueOf(String s) {
		return new Integer(Integer.parseInt(s));
	}

	public int intValue() {
		return value;
	}

	public boolean equals(Object obj) {
		return obj instanceof Integer && ((Integer) obj).value == this.value;
	}
}
