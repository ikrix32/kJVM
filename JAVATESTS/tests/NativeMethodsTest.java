package tests;

public class NativeMethodsTest {
	byte b;
	char c;
	int  i; 
	float f;
	
	byte[] arrayB = new byte[]{0x3,0x3};
	char[] arrayC = new char[]{'a','b'};
	int[] arrayI = new int[]{5,5,5};
	float[] arrayF = new float[]{5.2f,5.2f};
	
	private native void nativeVoidMetod();
	private native byte nativeMethod(byte a);
	private native char nativeMethod(char a);
	private native int  nativeMethod(int a);
	private native float nativeMethod(float a);
	private native void nativeMethod(byte a,char b,int c,float d);
	
	private native void nativeMethod(byte[] a);
	private native void nativeMethod(char[] a);
	private native void nativeMethod(int[] a);
	private native void nativeMethod(float[] a);
	
	private native byte[] nativeMethod();
	
	public static void main(String[] args) {
		NativeMethodsTest mTest = new NativeMethodsTest();
		mTest.nativeVoidMetod();
		mTest.nativeMethod(0x1);
		
		mTest.nativeMethod(mTest.b);
		mTest.nativeMethod(mTest.c);
		mTest.nativeMethod(mTest.i);
		mTest.nativeMethod(mTest.f);
		mTest.nativeMethod(mTest.b,mTest.c,mTest.i,mTest.f);
		
		mTest.nativeMethod(mTest.arrayB);
		mTest.nativeMethod(mTest.arrayC);
		mTest.nativeMethod(mTest.arrayI);
		mTest.nativeMethod(mTest.arrayF);
    }
}
