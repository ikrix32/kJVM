class Buffer {
	int count = 0; // num Items in buffer
	int BUFSIZE;
	int out = 0, in = 0;
	int[] buffer;
	int progress = 0;

	public Buffer(int bufsize) {
		BUFSIZE = bufsize;
		buffer = new int[BUFSIZE];
	}

	public synchronized void produce(int item) {
		while (count == BUFSIZE) {
			try {
				wait();
			} catch (/* Interrupted */Exception e) {
			}
		}
		++count;
		if (buffer[in] != 0)
			System.out.println("produce error " + buffer[in]);
		buffer[in] = item;
		in = (in + 1) % BUFSIZE;
		// System.out.println("produce "+in+" "+count);
		notify();// All();
	}

	public synchronized int consume() {
		while (count == 0) {
			try {
				wait();
			} catch (/* Interrupted */Exception e) {
			}
		}
		--count;
		int item = buffer[out];
		if (item != 77)
			System.out.println("consume error " + item);
		buffer[out] = 0;
		out = (out + 1) % BUFSIZE;
		if ((progress % 100) == 0)
			System.out.println("progress " + progress);
		progress++;
		notify();// All();
		return item;
	}
}

public class ProducerConsumer extends Thread {
	public static Buffer buf;
	char dispatch;

	public void run() {
		System.out.println(dispatch);
		if (dispatch == 'p')
			while (true) {
				buf.produce(77);
			}
		else
			while (true) {
				buf.consume();
			}
	}

	public ProducerConsumer(char c) {
		dispatch = c;
	}

	public static void main(String args[]) {
		ProducerConsumer.buf = new Buffer(4);
		ProducerConsumer p = new ProducerConsumer('p');
		ProducerConsumer p1 = new ProducerConsumer('p');
		ProducerConsumer c = new ProducerConsumer('c');
		p.start();
		p1.start();
		c.start();
		p.run();
	}
}
