package tests;

public class ThreadTest extends Thread{
	public void run() {
		int counter =  0;
		while(counter < 100){
			counter++;
		}
		System.out.println("Thread Executin ended.");
	}
	
	public static void main(String args[]){
		System.out.println("Start Thread Execution.");
		Thread t = new ThreadTest();
		//t.start();
		System.out.println("Main ended.");
	}
}
