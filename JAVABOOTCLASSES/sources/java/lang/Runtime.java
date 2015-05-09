package java.lang;

/**
 * Minimalist version of the standard Java Runtime class.
 * 
 * @author Paul Andrews
 */
public class Runtime 
{
	private static Runtime currentRuntime = new Runtime();

	/**
	 * Private so no one but us can create one.
	 */
	private Runtime() {
	}

	/**
	 * Get the single instance of us.
	 */
	public static Runtime getRuntime() {
		return currentRuntime;
	}

	/**
	 * Return the amount of free memory.on the heap
	 *
	 * @return the free memory in bytes
	 */
	public native int freeMemory();//long freeMemory();

	/**
	 * Return the size of the heap in bytes.
	 *
	 * @return the free memory in bytes
	 */
	public native int totalMemory();//long totalMemory();
	/**
	 *	Returns the maximum amount of memory that the Java virtual machine will attempt to use. 
	 *	If there is no inherent limit then the value Long.MAX_VALUE will be returned.
	 *	Returns:
	 * 	the maximum amount of memory that the virtual machine will attempt to use, measured in bytes
	 * 	Since:
	 * 	1.4
	 */
	//public native int maxMemory();//long maxMemory();
	
	/**
	 * 	Runs the garbage collector. Calling this method suggests that the Java virtual machine expend 
	 * 	effort toward recycling unused objects in order to make the memory they currently occupy available for quick reuse.
	 * 	When control returns from the method call, the virtual machine has made its best effort to recycle all discarded objects.
	 *	The name gc stands for "garbage collector". The virtual machine performs this recycling process automatically as needed,
	 *	 in a separate thread, even if the gc method is not invoked explicitly.
	 *	The method System.gc() is the conventional and convenient means of invoking this method.
	 */
	public native void gc();
}
