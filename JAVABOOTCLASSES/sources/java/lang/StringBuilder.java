package java.lang;

/**
 * bh: StringBuilder.java for java 1.6 !!!
 */
public final class StringBuilder extends StringBuffer {
	public StringBuilder delete(int start, int end) {
		checkIndex(start);
		if (start > end) {
			throw new ArrayIndexOutOfBoundsException();
		}
		if (end >= curPos) {
			end = curPos;
		} else {
			System.arraycopy(characters, end, characters, start, curPos - end);
		}
		curPos -= end - start;
		return this;
	}

	public StringBuilder deleteCharAt(int index) {
		checkIndex(index);
		System.arraycopy(characters, index + 1, characters, index, curPos
				- index);
		--curPos;
		return this;
	}

	public StringBuilder replace(int start, int end, String str) {
		checkIndex(start);
		if (start > end) {
			throw new ArrayIndexOutOfBoundsException();
		}
		char[] buf = new char[start + str.length() + curPos - end];
		System.arraycopy(characters, 0, buf, 0, start);
		System.arraycopy(characters, end, buf, str.length() + start, curPos
				- end);
		str.getChars(0, str.length(), buf, start);
		characters = buf;
		curPos = buf.length;
		return this;
	}

	public StringBuilder append(String s) {
		return append(s.toCharArray());
	}

	public StringBuilder append(Object aObject) {
		return append(aObject != null ? aObject.toString() : "null");
	}

	public StringBuilder append(StringBuffer sb) {
		if (sb != null) {
			ensureCapacity(curPos + sb.curPos);
			System.arraycopy(sb.characters, 0, characters, curPos, sb.curPos);
			curPos += sb.curPos;
			return this;
		} else {
			return append("null");
		}
	}

	public StringBuilder append(boolean aBoolean) {
		return append(Boolean.toString(aBoolean));
	}

	public StringBuilder append(char aChar) {
		ensureCapacity(curPos + 1);
		characters[curPos++] = aChar;
		return this;
	}

	public StringBuilder append(char[] aChars) {
		return append(aChars, 0, aChars.length);
	}

	public StringBuilder append(char[] str, int offset, int len) {
		ensureCapacity(curPos + len);
		System.arraycopy(str, offset, characters, curPos, len);
		curPos += len;
		return this;
	}

	public StringBuilder append(int aInt) {
		return append(Integer.toString(aInt));
	}

	public StringBuilder append(float aFloat) {
		return append(Float.toString(aFloat));
	}

	/*
	 * public StringBuffer insert(int index, char[] str, int offset, int len) {
	 * checkIndex(index); if (offset < 0 || len < 0 || offset + len >
	 * str.length) { throw new ArrayIndexOutOfBoundsException(); }
	 * ensureCapacity(curPos+len); char[] buf = new char[curPos + len];
	 * System.arraycopy(characters, 0, buf, 0, index); System.arraycopy(str,
	 * offset, buf, index, len); System.arraycopy(characters, index + 1, buf,
	 * index + len + 1, curPos - index); characters = buf; curPos += len; return
	 * this; }
	 */

	public StringBuffer insert(int index, String str) {
		checkIndex(index);
		int len = str.length();
		ensureCapacity(curPos + len);
		char[] buf = new char[curPos + len];
		System.arraycopy(characters, 0, buf, 0, index);
		str.getChars(0, len, buf, index);
		System.arraycopy(characters, index + 1, buf, index + len + 1, curPos
				- index);
		characters = buf;
		curPos += len;
		return this;
	}

	public StringBuffer reverse() {
		char[] buf = new char[curPos];
		for (int i = 0; i < curPos; ++i) {
			buf[i] = characters[curPos - i];
		}
		characters = buf;
		return this;
	}
}