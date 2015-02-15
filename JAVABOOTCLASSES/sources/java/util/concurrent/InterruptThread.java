package java.util.concurrent;

import java.lang.Thread;

/**
 * InterruptThread for realtime behavior of bajos
 */
public abstract class InterruptThread extends Thread	{

	public static native void sei();

	public static native void cli();

	public native void initInterrupt(byte vectorNr);

	public static native void removeInterrupt(byte vectorNr);

	public static native void forceInterrupt(byte vectorNr);

	public InterruptThread(){
		setPriority(MAX_PRIORITY);
		start();	
	}

	public void run()	{
		Lock.lock(this);
		while(true){
			Lock.lock(this);
			isr();		
		}
	}

 	public abstract void isr();
 
}
