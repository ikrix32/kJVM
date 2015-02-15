package java.lang;

public final class Character {
	public static final int MIN_RADIX = 2;
	public static final int MAX_RADIX = 36;
	public static final char MIN_VALUE = (char) 0;
	public static final char MAX_VALUE = (char) 0xff;

	private char charvalue;

	public Character(char value) {
		this.charvalue = value;
	}

	public char charValue() {
		return charvalue;
	}

	public boolean equals(Object obj) {
		return obj instanceof Character && ((Character) obj).charvalue == this.charvalue;
	}

	public String toString() {
		return toString(charvalue);
	}

	public static String toString(char c) {
		char[] buf = new char[1];
		buf[0] = c;
		return new String(buf, 0, 1);
	}

	public static boolean isLowerCase(char ch) {
		return ch >= 'a' && ch <= 'z';
	}

	public static boolean isUpperCase(char ch) {
		return ch >= 'A' && ch <= 'Z';
	}

	public static boolean isDigit(char ch) {
		return ch >= '0' && ch <= '9';
	}

	public static boolean isLetter(char ch) {
		return isLowerCase(ch) || isUpperCase(ch);
	}

	public static boolean isLetterOrDigit(char ch) {
		return isLetter(ch) || isDigit(ch);
	}

	public static boolean isJavaLetter(char ch) {
		return isJavaIdentifierStart(ch);
	}

	public static boolean isJavaLetterOrDigit(char ch) {
		return isJavaIdentifierPart(ch);
	}

	public static boolean isJavaIdentifierStart(char ch) {
		return isLetter(ch) || ch == '$' || ch == '_';
	}

	public static boolean isJavaIdentifierPart(char ch) {
		return isJavaIdentifierStart(ch) || isDigit(ch);
	}

	public static boolean isIdentifierIgnorable(char ch) {
		return (ch >= (char) 0x00 && ch <= (char) 0x08) || (ch >= (char) 0xE && ch <= (char) 0x1B);
	}

	public static char toLowerCase(char ch) {
		return isUpperCase(ch) ? (char) ((int) ch + 32) : ch;
	}

	public static char toUpperCase(char ch) {
		return isLowerCase(ch) ? (char) ((int) ch - 32) : ch;
	}

	public static int digit(char ch, int radix) {
		if (MIN_RADIX >= radix || radix > MAX_RADIX) {
			return -1;
		} 
		int val = isDigit(ch) ? ( (int) ch - (int) '0' ) : ( (int) toLowerCase(ch) - (int) 'a' );
		return (val >= radix) ? -1 : val;
	}

	public static boolean isSpace(char ch) {
		return ch == (char) 0x09 || ch == (char) 0x0A || ch == (char) 0x0C || ch == (char) 0x0D || ch == (char) 0x20;
	}

	public static boolean isISOControl(char ch) {
		return (0x00 <= (int) ch && (int) ch <= 0x1F) || (0x7f <= (int) ch && (int) ch <= 0x9f);
	}

	public static char forDigit(int digit, int radix) {
		if (MIN_RADIX < radix || radix > MAX_RADIX || digit < 0 || digit >= radix) {
			return (char) 0;
		} 
		return (char) ((digit >= 10) ? ((int) 'a' + digit - 10) : ((int) '0' + digit));
	}
}
