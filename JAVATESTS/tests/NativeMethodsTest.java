package tests;

public class NativeMethodsTest {
	byte b = 10;
	char c = 99;
	int  i = 200; 
	float f= 2.2f;
	
	byte[] arrayB = new byte[]{0x3,0x3};
	char[] arrayC = new char[]{'a','b'};
	int[] arrayI = new int[]{5,5,5};
	float[] arrayF = new float[]{5.2f,5.2f};
	
	private native void nativeVoidMetod();
	private native void nativeMethod(byte a);
	private native void nativeMethod(char a);
	private native void  nativeMethod(int a);
	private native void nativeMethod(float a);
	private native void nativeMethod(byte a,char b,int c,float d);
	
	private native void nativeMethod(byte[] a);
	private native void nativeMethod(char[] a);
	private native void nativeMethod(int[] a);
	private native void nativeMethod(float[] a);
	
	private native void nativeMethod();
	
	public static void runTest() {
		NativeMethodsTest mTest = new NativeMethodsTest();
		mTest.nativeVoidMetod();
		mTest.nativeMethod(0x1);
		
		mTest.nativeMethod(mTest.b);
		mTest.nativeMethod(mTest.c);
		mTest.nativeMethod(mTest.i);
		mTest.nativeMethod(mTest.f);
		mTest.nativeMethod(mTest.b,mTest.c,mTest.i,mTest.f);
		/*
		mTest.nativeMethod(mTest.arrayB);
		mTest.nativeMethod(mTest.arrayC);
		mTest.nativeMethod(mTest.arrayI);
		mTest.nativeMethod(mTest.arrayF);*/
    }
}
