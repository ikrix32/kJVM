package java.lang;

/* Class-specific differences to J2SE 1.4.2:
 * No from String methods: Boolean(String), valueOf(String)
 * */

public final class Boolean {
	public final static Boolean TRUE = new Boolean(true);
	public final static Boolean FALSE = new Boolean(false);

	private boolean boolval;

	public Boolean(boolean val) {
		boolval = val;
	}

	public static String toString(boolean val) {
		return val ? "true" : "false";
	}

	public String toString() {
		return toString(boolval);
	}

	public boolean equals(Object obj) {
		return obj instanceof Boolean && ((Boolean) obj).boolval == this.boolval;
	}

	public boolean booleanValue() {
		return boolval;
	}

	public static Boolean valueOf(boolean b) {
		return b ? Boolean.TRUE : Boolean.FALSE;
	}
}
