package java.util;

public class LinkedList implements List {
	private Element first;
	private Element last;
	private int size;

	private class Element {
		Object data;
		Element next;
		Element previous;

		Element(Object data) {
			this.data = data;
		}
	}

	public LinkedList() {
		first = null;
		last = null;
		size = 0;
	}

	public Object getFirst() {
		if (isEmpty()) throw new NoSuchElementException();
		return first.data;
	}

	public Object getLast() {
		if (isEmpty()) throw new NoSuchElementException();
		return last.data;
	}

	public Object removeFirst() {
		if (isEmpty()) throw new NoSuchElementException();
		Element tmp = first;
		first = first.next;
		if (first != null) {
			first.previous = null;
		} else {
			last = null;
		}
		--size;
		return tmp.data;
	}

	public Object removeLast() {
		if (isEmpty()) throw new NoSuchElementException();
		Element tmp = last;
		last = last.previous;
		if (last != null) {
			last.next = null;
		} else {
			first = null;
		}
		--size;
		return tmp.data;
	}

	public void addFirst(Object o) {
		Element elem = new Element(o);
		elem.next = first;
		if (first != null) {
			first.previous = elem;
		} else {
			last = elem;
		}
		first = elem;
		++size;
	}

	public void addLast(Object o) {
		Element elem = new Element(o);
		elem.previous = last;
		if (last != null) {
			last.next = elem;
		} else {
			first = elem;
		}
		last = elem;
		++size;
	}

	public boolean add(Object o) {
		addLast(o);
		return true;
	}

	public boolean contains(Object o) {
		Element elem = first;
		while (elem != null) {
			if (elem.data == o || (elem.data != null && elem.data.equals(o))) return true;
			elem = elem.next;
		}
		return false;
	}

	public boolean remove(Object o) {
		Element elem = first;
		while (elem != null) {
			if (elem.data == o || (elem.data != null && elem.data.equals(o))) {
				removeElement(elem);
				return true;
			} 
			elem = elem.next;
		}
		return false;
	}

	public int size() {
		return size;
	}

	public boolean isEmpty() {
		return size == 0;
	}

	public void clear() {
		Element elem = first;
		while (elem != null) {
			Element tmp = elem;
			elem = elem.next;
			tmp.previous = null;
			tmp.next = null;
		}
		first = null;
		last = null;
		size = 0;
	}

	private void removeElement(Element elem) {
		if (elem.next != null) {
			elem.next.previous = elem.previous;
		}
		if (elem.previous != null) {
			elem.previous.next = elem.next;
		}
		if (elem == first) {
			first = elem.next;
		}
		if (elem == last) {
			last = elem.previous;
		}
		--size;
	}

	private void insertElement(Element after, Element newelem) {
		after = after != null ? after : first;
		if (after != null && after.previous != null) {
			after.previous.next = newelem;
		}
		if (after == first) {
			first = newelem;
		}
		++size;
	}

	private Element getElement(int index) {
		Element elem = first;
		for ( ; index > 0 ; --index) { elem = elem.next; }
		return elem;
	}

	private void checkIndex(int index) {
		if (index < 0 || index >= size()) {
			throw new ArrayIndexOutOfBoundsException();
		}
	}

	public Object get(int index) {
		checkIndex(index);
		return getElement(index).data;
	}

	public Object set(int index, Object element) {
		checkIndex(index);
		Element elem = getElement(index);
		Object old = elem.data;
		elem.data = element;
		return old;
	}

	public void add(int index, Object element) {
		checkIndex(index);
		insertElement(getElement(index), new Element(element));
	}

	public Object remove(int index) {
		checkIndex(index);
		Element elem = getElement(index);
		removeElement(elem);
		return elem.data;
	}

	public int indexOf(Object o) {
		Element elem = first;
		for(int i = 0 ; elem != null ; ++i) {
			if (elem.data == o || (elem.data != null && elem.data.equals(o))) {
				return i;
			} 
			elem = elem.next;
		}
		return -1;
	}

	public int lastIndexOf(Object o) {
		Element elem = last;
		for (int i = size ; elem != null ; --i) {
			if (elem.data == o || (elem.data != null && elem.data.equals(o))) {
				return i;
			} 
			elem = elem.previous;
		}
		return -1;
	}

	public Object[] toArray() {
		Object[] buff = new Object[size];
		Element elem = first;
		for (int i = 0 ; i < size && elem != null ; ++i) {
			buff[i] = elem.data;
			elem = elem.next;
		}
		return buff;
	}

	public Object[] toArray(Object[] a) {
		if (a.length < size) {
			return toArray();
		} else {
	    Element elem = first;
  	  for (int i = 0 ; i < size && elem != null ; ++i) {
    	 	a[i] = elem.data;
      	elem = elem.next;
    	}
			if (size < a.length) {
				a[size+1] = null;
			}
			return a;
		}
	}
}
