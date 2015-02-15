package java.util;

public class Stack extends Vector {

	public Stack() {
	}

	public Object push(Object item) {
		addElement(item);
		return item;	
	}

	public Object pop() {
		return remove(elementCount - 1);
	}

	public Object peek() {
		return elementAt(elementCount - 1);
	}

	public boolean empty() {
		return elementCount == 0;
	}

	public int search(Object o) {
		for (int k = elementCount - 1 ; k >= 0 ; --k) {
			if (o == null ? elementData[k] == null : elementData[k].equals(o)) {
				return elementCount - k;
			}
		}
		return -1;
	}
}
