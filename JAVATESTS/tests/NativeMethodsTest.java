package tests;

public class NativeMethodsTest {
	byte b = 10;
	char c = 99;
	int  i = 200; 
	float f= 2.2f;
	
	byte[] arrayB = new byte[]{0x3,0x2,1,5,78,98};
	char[] arrayC = new char[]{'a','b'};
	int[] arrayI = new int[]{5,6,5};
	float[] arrayF = new float[]{5.2f,5.2f};
	
	private native int  nativeMethod(byte a);
	private native char[] nativeMethod(char a);
	private native void nativeMethod(int a);
	private native void nativeMethod(float a);
	
	private native void nativeMethod(byte[] a);
	private native void nativeMethod(char[] a);
	private native void nativeMethod(int[] a);
	private native void nativeMethod(float[] a);
	
	private native void nativeMethod();
	
	public static void runTest() {
		NativeMethodsTest mTest = new NativeMethodsTest();
		
		int ret = mTest.nativeMethod(mTest.b);
		System.out.println("int return:"+ret);
		char[] text = mTest.nativeMethod(mTest.c);
		String x = "";
		for(int i = 0;i< text.length;i++)
			x += text[i];
		System.out.println("char array return:"+x);
		mTest.nativeMethod(mTest.i);
		mTest.nativeMethod(mTest.f);
		
		mTest.nativeMethod(mTest.arrayB);
		String array = "";
		for(int i = 0; i < mTest.arrayB.length;i++){
			array += mTest.arrayB[i]+",";
		}
		System.out.print("Native changed byteArray:"+array);
		mTest.nativeMethod(mTest.arrayC);
		mTest.nativeMethod(mTest.arrayI);
		mTest.nativeMethod(mTest.arrayF);
    }
}
