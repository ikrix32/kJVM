import java.lang.*;
import java.util.concurrent.*;

public class LockSimple extends Thread {

	public static int foo = 0;

	public static void main(String[] args) {
		System.out.println("Started! ");
		LockSimple h = new LockSimple();
		h.start();
		for (int i = 0; i < 20; i++) {
			Lock.lock(h);
			System.out.println("Value: " + LockSimple.foo++);
			Lock.unlock(h);
		}
	}

	public void run() {
		for (int i = 0; i < 20; i++) {
			Lock.lock(this);
			foo++;
			Lock.unlock(this);
		}

	}
}
