package tests;

public class ThreadTest extends Thread{
	int counter =  0;
	int tid = 0;
	
	public ThreadTest(int id){
		tid = id;
	}
	public void run() {
		System.out.println("Start Thread Execution.");
		
		while(counter < 100){
			increment();
		}
		System.out.println("Thread Executin ended.");
	}
	
	public synchronized void increment(){
		counter++;
		if(counter % 10 == 0){
			if(tid == 1)
				Runtime.getRuntime().gc();
			System.out.println("i ="+counter);
		}
	}
	
	public static void runTest() {
		System.out.println("Main started.");
		ThreadTest t = new ThreadTest(0);
		t.start();
		t.setPriority(MIN_PRIORITY);
		
		ThreadTest t1 = new ThreadTest(1);
		t1.start();
		System.out.println("Main ended.");
	}
}
