package tests;

public class MainTests
{

	public static void main(String args[])
	{
		Runtime runtime = Runtime.getRuntime();
		System.out.println("Starting Tests, jvm: total memory:"+runtime.totalMemory()+", free memory:"+runtime.freeMemory());
		//*
		HelloWorld.runTest();
		System.out.println("Free Memory:"+runtime.freeMemory());
		Arithmetic.runTest();
		System.out.println("Free Memory:"+runtime.freeMemory());
		ICMP.runTest();
		System.out.println("Free Memory:"+runtime.freeMemory());
		Fibonacci.runTest();
		System.out.println("Free Memory:"+runtime.freeMemory());
		SelfInstance.runTest();
		System.out.println("Free Memory:"+runtime.freeMemory());
		StaticTest.runTest();
		System.out.println("Free Memory:"+runtime.freeMemory());
		StringAndHeapTest.runTest();
		System.out.println("Free Memory:"+runtime.freeMemory());
		Switch.runTest();
		System.out.println("Free Memory:"+runtime.freeMemory());
		ArraySize.runTest();
		System.out.println("Free Memory:"+runtime.freeMemory());
		Inheritance.runTest();
		System.out.println("Free Memory:"+runtime.freeMemory());
		FloatTest.runTest();
		System.out.println("Free Memory:"+runtime.freeMemory());
		QuickSort.runTest();
		System.out.println("Free Memory:"+runtime.freeMemory());
		Erathostenes.runTest();
		System.out.println("Free Memory:"+runtime.freeMemory());
		DivByZero.runTest();
		System.out.println("Free Memory:"+runtime.freeMemory());
		//ThreadTest.runTest();
		System.out.println("Free Memory:"+runtime.freeMemory());
		NativeMethodsTest.runTest();/*/
		HeapTest.runTest();
		System.out.println("Free Memory:"+runtime.freeMemory());*/
	}
}
