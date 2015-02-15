/*
 * by Christopher-Eyk Hrabia
 */
import java.util.concurrent.*;

public class SpinLockTest extends Thread {

	int nr;

	private Lock mylock;

	public SpinLockTest(int nr, Lock lock) {
		this.nr = nr;
		this.mylock = lock;
	}

	public void run() {

		for (int i = 0; i < 10; i++) {
			while (!mylock.tryLock()) {
				System.out.println("Spinwait! " + this.nr);
			}
			for (int j = 0; j < 3; j++) {
				System.out.println("My nr: " + this.nr + " My prio: "
						+ getPriority() + " Counter: " + i);
			}
			mylock.unlock();
		}
	}
}
