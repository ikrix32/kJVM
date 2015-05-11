package tests;

public class HeapTest
{

	 public static void runTest() {
		 Runtime runtime = Runtime.getRuntime();
		 //System.out.println("Heap test: Alloc integer.");
		 int x = 0;
		 System.out.println(x);
		 runtime.gc();
		 //System.out.println("Heap test: Alloc integer array.");
		 int intArray[] = new int[100];
		 //System.out.println(intArray[0]);
		 runtime.gc();
		 System.out.println("intArray[0]="+intArray[0]);
		 //System.out.println("Heap test: Alloc InstanceA.");
		 InheritanceA a = new InheritanceA();
		 runtime.gc();
		 //System.out.println("Heap test: Alloc InheritanceA array.");
		 InheritanceA inh[] = new InheritanceA[20];
		 inh[0] = a;
		 runtime.gc();
		 //System.out.println("Heap test: Alloc integer array[255].");
		 int iA[] = new int[255];
		 iA[0] = 0;
		 //System.out.println("Heap test: length:"+iA.length); 
		 runtime.gc();
	 }
}
