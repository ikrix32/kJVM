package java.util;

import java.lang.NullPointerException;
import java.lang.ArrayIndexOutOfBoundsException;
import java.lang.IllegalArgumentException;

public class Vector implements List {

	protected Object[] elementData;
	protected int elementCount;
	protected int capacityIncrement;

	public Vector(int initialCapacity, int capacityIncrement) {
		if (initialCapacity < 0) {
			throw new IllegalArgumentException();
		}
		elementData = new Object[initialCapacity];
		for (int k = 0 ; k < initialCapacity ; ++k) {
			elementData[k] = null;
		}
		this.capacityIncrement = capacityIncrement;
		elementCount = 0;
	}

	public Vector(int initialCapacity) {
		this(initialCapacity,0);
	}

	public Vector() {
		this(10,0);
	}

/*	public Vector(Collection c) {
		this();
		if (c == null) {
			throw new NullPointerException();
		}
		Iterator it = c.iterator();
		while (it.hasNext()) {
			ensureCapacity(elementCount + 1);
			elementData[++elementCount] = it.next();	
		}
	}*/

	public Object clone() {
		Vector newobj = new Vector(elementData.length, capacityIncrement);
		newobj.elementData = elementData;
		newobj.elementCount = elementCount;
		return newobj;
	}

	public void clear() {
		for (int k = 0 ; k < elementCount ; ++k) {
			elementData[k] = null;
		}
		elementCount = 0;
	}

	public void removeAllElements() {
		clear();
	}

	public void copyInto(Object[] anArray) {
		if (anArray == null)
			throw new NullPointerException();
		if (anArray.length < elementCount)
			throw new ArrayIndexOutOfBoundsException();

		for (int k = 0 ; k < elementData.length ; ++k) {
			anArray[k] = elementData[k];
		}
	}

	public void trimToSize() {
		if (elementData.length > elementCount) {
			Object[] newarray = new Object[elementCount];
			for (int k = 0 ; k < elementData.length ; ++k) {
				newarray[k] = elementData[k];
			}
			elementData = newarray;
		}
	}

	public int size() {
		return elementCount;
	}

	public int capacity() {
		return elementData.length;
	}

	public boolean isEmpty() {
		return elementCount == 0;
	}

	public boolean contains(Object elem) {
		for ( int k = 0 ; k < elementData.length ; ++k) {
			if (elementData[k] == null ? elem == null : elementData[k].equals(elem)) return true;
		}
		return false;
	}

/* enumeration haben wir (noch) nicht
	public Enumeration elements() {
	}*/

	public int indexOf(Object elem) {
		return indexOf(elem, 0);
	}

	public int indexOf(Object elem, int index) {
		for ( int k = index ; k < elementCount ; ++k) {
			if (elementData[k] == null ? elem == null : elementData[k].equals(elem)) return k;
		}
		return -1;
	}

	public int lastIndexOf(Object elem) {
		return lastIndexOf(elem, elementCount);
	}

	public int lastIndexOf(Object elem, int index) {
		for ( int k = index ; k >= 0 ; --k) {
			if (elementData[k].equals(elem)) return k;
		}
		return -1;
	}

	public Object elementAt(int index) {
		if (index < 0 || index >= elementCount)
			throw new ArrayIndexOutOfBoundsException();
		return elementData[index];
	}

	public Object firstElement() {
		if (elementCount == 0)
			throw new NoSuchElementException();
		return elementData[0];
	}

	public Object lastElement() {
		if (elementCount == 0)
			throw new NoSuchElementException();
		return elementData[elementCount - 1];
	}

	public void setElementAt(Object obj, int index) {
		if (index < 0 || index >= elementCount)
			throw new ArrayIndexOutOfBoundsException();
		elementData[index] = obj;		
	}

	public void removeElementAt(int index) {
		if (index < 0 || index >= elementCount)
			throw new ArrayIndexOutOfBoundsException();
		for (int k = index ; k < elementCount ; ++k) {
			elementData[k] = elementData[k+1];
		}
		--elementCount;
	}

	public void insertElementAt(Object obj, int index) {
		if (index < 0 || index >= elementCount)
			throw new ArrayIndexOutOfBoundsException();
		
		for (int k = elementCount - 1; k >= index ; --k) {
			elementData[k+1] = elementData[k];
		}
		elementData[index] = obj;
		++elementCount;
	}

	public Object get(int index) {
		return elementAt(index);	
	}

	public Object set(int index, Object element) {
		if (index < 0 || index >= elementCount)
			throw new ArrayIndexOutOfBoundsException();
		Object oldelem = elementData[index];
		elementData[index] = element;
		return oldelem;
	}

	public void addElement(Object obj) {
		if (elementCount == elementData.length) {
			ensureCapacity(elementCount + 1);
		}
		elementData[elementCount++] = obj;
	}

	public boolean add(Object o) {
		addElement(o);
		return true;	
	}

	public void ensureCapacity(int minCapacity) {
		if (elementData.length < minCapacity) {
			Object[] newElementData = new Object[elementData.length
					+ Math.max(
					minCapacity,
					elementData.length +
					(capacityIncrement <= 0 ? elementData.length : capacityIncrement))];
			for (int k = 0 ; k < elementData.length ; ++k) {
				newElementData[k] = elementData[k];
			}
			elementData = newElementData;
		}
	}

	public void setSize(int newSize) {
		if (newSize < 0)
			throw new ArrayIndexOutOfBoundsException();
		ensureCapacity(newSize);
		for (int k = elementCount - 1 ; k < newSize ; ++k) {
			elementData[k] = null;
		}
		elementCount = newSize;
	}

	public boolean remove(Object obj) {
		for (int k = 0 ; k < elementCount ; ++k) {
			if (obj == null ? elementData[k] == null : obj.equals(elementData[k])) {
				removeElementAt(k);
				return true;
			}
		}
		return false;
	}

	public Object remove(int index) {
		if (index < 0 || index >= elementCount)
			throw new ArrayIndexOutOfBoundsException();

		Object temp = elementData[index];
		for (int k = index ; k < elementCount ;	++k) {
			elementData[k] = elementData[k+1];
		}
		--elementCount;
		return temp;
	}

	public Object[] toArray() {
		Object[] newarray = new Object[elementCount];
		for (int k = 0 ; k < elementCount ; ++k) {
			newarray[k] = elementData[k];
		}
		return newarray;
	}

	public Object[] toArray(Object[] arr) {
		Object[] newarray = arr;
		if (arr.length < elementCount) {
			newarray = new Object[elementCount];
		}
		for (int k = 0 ; k < elementCount ; ++k) {
			newarray[k] = elementData[k];
		}
		for (int k = elementCount ; k < arr.length ; ++k) {
			newarray[k] = null;
		}
		return newarray;
	}

	protected void removeRange(int fromIndex, int toIndex) {

		for (int k = toIndex ; k < elementCount ; ++k) {
			elementData[k] = elementData[k+(toIndex-fromIndex)];
		}
		elementCount -= (toIndex-fromIndex);
	}
}
