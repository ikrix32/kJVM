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
	
	private native int[] stressTest(byte[] a,char[] b,int[] c,float[] d);
	
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
		
		int[] result  = mTest.stressTest(mTest.arrayB, mTest.arrayC, mTest.arrayI, mTest.arrayF);
		array = "[";
		for(int i = 0; i < result.length;i++){
			array += result[i]+",";
		}
		System.out.print("Return:"+array);
		array = "[byte:";
		for(int i = 0; i < mTest.arrayB.length;i++){
			array += mTest.arrayB[i]+",";
		}
		array += "][char:";
		for(int i = 0; i < mTest.arrayC.length;i++){
			array += mTest.arrayC[i]+",";
		}
		array += "][int:";
		for(int i = 0; i < mTest.arrayI.length;i++){
			array += mTest.arrayI[i]+",";
		}
		array += "][float:";
		for(int i = 0; i < mTest.arrayF.length;i++){
			array += mTest.arrayF[i]+",";
		}
		System.out.print("Native changed arrays:"+array);
    }
}
