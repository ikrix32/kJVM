 package java.util.concurrent;

 /*
 * by Christopher-Eyk Hrabia
 */
public class Lock {

	public native void lock();

	public native void unlock();

	public native boolean tryLock();

	public native static void lock(Object obj);

	public native static void unlock(Object obj);

	public Lock(){}

}
