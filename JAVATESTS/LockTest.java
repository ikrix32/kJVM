/*
 * by Christopher-Eyk Hrabia
 */
import java.util.concurrent.*;

public class LockTest extends Thread {

	int nr;

	private Lock mylock;

	public LockTest(int nr, Lock lock) {
		this.nr = nr;
		this.mylock = lock;
	}

	public void run() {

		for (int i = 0; i < 10; i++) {
			mylock.lock();
			for (int j = 0; j < 3; j++) {
				System.out.println("My nr: " + this.nr + " My prio: "
						+ getPriority() + " Counter: " + i);
			}
			mylock.unlock();
		}
	}

	public static void main(String args[]) {
		Lock lock = new Lock();
		LockTest t1 = new LockTest(1, lock);
		LockTest t2 = new LockTest(2, lock);
		SpinLockTest t3 = new SpinLockTest(3, lock);
		t1.start();
		t2.start();
		t3.start();
	}
}
