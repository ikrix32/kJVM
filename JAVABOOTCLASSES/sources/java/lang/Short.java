package java.lang;

public final class Short {
	public static final short MIN_VALUE = -32768;
	public static final short MAX_VALUE =  32767;

	private short value;

	public Short(short value) {
		this.value = value;
	}

	public Short(String s) {
		this.value = parseShort(s, 10);
	}
	
	public static short parseShort(String s) {
		return parseShort(s, 10);
	}

	public static short parseShort(String s, int radix) {
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
		return (short) val;
}
   
	public static synchronized String toString(short i) {
	
		if (i == Short.MIN_VALUE) {
			return "-32768";
		}
	
		char sign = 0;
		char buf [] = new char [12];
		if (i < 0) { 
			sign = '-';
			i = (short) -i;
		}

		short charPos = 12;
		for (;;) {
			short q = (short) (i/10);
			buf [--charPos] = (char) ((short) '0' + (i-(q*10)));
			i = q;
			if (i == 0) break;
		}
	
		if (sign != 0) {
			buf [--charPos] = sign;
		}

		return new String ( buf, charPos, 12 - charPos);
	}

	public String toString()  {
		return toString(value);
	}

	public static Short valueOf(String s, short radix) {
		return new Short(Short.parseShort(s, radix));
	}

	public static Short valueOf(String s) {
		return new Short(Short.parseShort(s));
	}

	public short shortValue() {
		return value;
	}

	public boolean equals(Object obj) {
		return obj instanceof Short && ((Short) obj).value == this.value;
	}
}

 
