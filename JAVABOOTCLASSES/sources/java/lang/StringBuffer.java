package java.lang;

/**
 * An expandable string of characters. Actually not very expandable! 09/25/2001
 * added number formatting thanks to Martin E. Nielsen. You must ensure that the
 * buffer is large enough to take the formatted number.
 * <P>
 * 
 * @author <a href="mailto:martin@egholm-nielsen.dk">Martin E. Nielsen</a>
 */
public class StringBuffer implements CharSequence {

	char[] characters;
	int curPos = 0;

	public StringBuffer() {
		this(16);
	}

	public StringBuffer(String aString) {
		characters = aString.toCharArray();
		curPos = characters.length;
	}

	public StringBuffer(int length) {
		characters = new char[length];
	}

	public String substring(int start) {
		checkIndex(start);
		return new String(characters, start, characters.length - start);
	}

	public char charAt(int i) {
		checkIndex(i);
		return characters[i];
	}

	public int length() {
		return curPos;
	}

	public int capacity() {
		return characters.length;
	}

	public void ensureCapacity(int target) {
		if (characters.length < target) {
			char[] newbuf = new char[Math.max(target, characters.length * 2)];
			System.arraycopy(characters, 0, newbuf, 0, characters.length);
			characters = newbuf;
		}
	}

	public void setLength(int newLength) {
		if (newLength < 0) {
			throw new ArrayIndexOutOfBoundsException();
		}
		if (newLength < curPos) {
			curPos = newLength;
		} else {
			ensureCapacity(newLength);
			for (++curPos; curPos <= newLength; ++curPos) {
				characters[curPos] = '\00';
			}
		}
	}

	public void setCharAt(int index, char ch) {
		checkIndex(index);
		characters[index] = ch;
	}

	protected void checkIndex(int index) {
		if (index < 0 || index > curPos) {
			throw new ArrayIndexOutOfBoundsException();
		}
	}

	private void checkRange(int lower, int upper) {
		if (lower < 0 || lower > upper || upper > curPos) {
			throw new ArrayIndexOutOfBoundsException();
		}
	}

	public char[] getChars() {
		return characters;
	}

	public void getChars(int srcBegin, int srcEnd, char[] dst, int dstBegin) {
		checkRange(srcBegin, srcEnd);
		if (dstBegin < 0 || dstBegin + (srcEnd - srcBegin) > dst.length) {
			throw new ArrayIndexOutOfBoundsException();
		}
		System.arraycopy(characters, srcBegin, dst, dstBegin, srcEnd - srcBegin);
	}

	public String substring(int start, int end) {
		checkRange(start, end);
		return new String(characters, start, end - start);
	}

	public CharSequence subSequence(int start, int end) {
		return substring(start, end);
	}

	public String toString() {
		return new String(characters, 0, curPos);
	}

}
