/*
 * by Christopher-Eyk Hrabia
 */
public class SchedulerTest extends Thread {

	SchedulerTest(String name) {
		super(name);
	}

	public void run() {
		System.out.println("time in ms: " + System.currentTimeMillis() % 10000
				+ " " + currentThread());

	}

	public static void main(String args[]) {
		SchedulerTest t1 = new SchedulerTest("T1");
		// SchedulerTest t2=new SchedulerTest("T2");
		// SchedulerTest t3=new SchedulerTest("T3");
		// SchedulerTest t4=new SchedulerTest("T4");
		// SchedulerTest t5=new SchedulerTest("T5");
		t1.setPriority(6);
		// t2.setPriority(7);
		// t4.setPriority(6);
		// t5.setPriority(6);

		t1.start();
		// t2.start();
		// t3.start();
		// t4.start();
		// t5.start();
		System.out.println("hier drin warte ich - oder auch nicht");
	}

}
