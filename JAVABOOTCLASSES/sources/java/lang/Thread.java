package java.lang;

/**
 * A thread of execution (or task). Now handles priorities, daemon threads and
 * interruptions.
 */
public abstract class Thread {
	/**
	 * The minimum priority that a thread can have. The value is 0.
	 */
	public final static int MIN_PRIORITY = 1;

	/**
	 * The priority that is assigned to the primordial thread. The value is 5.
	 */
	public final static int NORM_PRIORITY = 5;

	/**
	 * The maximum priority that a thread can have. The value is 10.
	 */

	public final static int MAX_PRIORITY = 10;

	// Extra instance state follows:

	private String name; // field 0
	private int priority = NORM_PRIORITY; // field 1
	private boolean isAlive = false; // field 2

	public native void start();

	public static native void yield();

	// bh public static native void sleep (long aMilliseconds) throws
	// InterruptedException;
	public static native void sleep(int aMilliseconds)
			throws InterruptedException;

	public static native Thread currentThread();

	/**
	 * Set the interrupted flag. If we are asleep we will wake up and an
	 * InterruptedException will be thrown.
	 */
	public native void interrupt();

	public static native boolean interrupted();

	public final native boolean isInterrupted();

	/**
	 * Set the daemon flag. If a thread is a daemon thread its existence will
	 * not prevent a JVM from exiting.
	 */
	// bh public final native boolean isDaemon();
	// bh public final native void setDaemon(boolean on);

	private native void nativeSetPriority(int priority);

	public final native void join() throws InterruptedException;

	// bh public final native void join(long timeout) throws
	// InterruptedException;
	// public final void join(int timeout) {
	// jointimeout1(timeout);
	// }
	// public native void jointimeout1(int timeout);//throws
	// InterruptedException;

	public Thread() {
		this("");
	}

	public Thread(String name) {
		this.name = name;
	}

	// bh a thread is alive after start and upto deleteThread
	public final boolean isAlive() {
		return isAlive;
	}

	public abstract void run();

	// create thread, register pointer to priority in native treadControlBlock,
	// execute run, set isALive

	public final int getPriority() {
		return priority;
	}

	/**
	 * Set the priority of this thread. Higher number have higher priority. The
	 * scheduler will always run the highest priority thread in preference to
	 * any others. If more than one thread of that priority exists the scheduler
	 * will time-slice them. In order for lower priority threas to run a higher
	 * priority thread must cease to be runnable. i.e. it must exit, sleep or
	 * wait on a monitor. It is not sufficient to just yield.
	 * <P>
	 * Threads inherit the priority of their parent. The primordial thread has
	 * priority NORM_PRIORITY.
	 *
	 * @param priority
	 *            must be between MIN_PRIORITY and MAX_PRIORITY.
	 */
	public final void setPriority(int priority) {
		//if (isAlive)
		//	nativeSetPriority(priority);
		//else
		this.priority = ((priority > 0) && (priority <= MAX_PRIORITY)) ? priority
					: NORM_PRIORITY;
	}

	/**
	 * Returns a string representation of this thread, including the thread's
	 * name, priority, and thread group.
	 *
	 * @return a human-readable String representing this Thread
	 */
	public String toString() {
		return ("Thread " + name + ",Prio: " + priority);
	}

	/**
	 * Get this Thread's name.
	 *
	 * @return this Thread's name
	 */
	public final String getName() {
		return name;
	}

}
