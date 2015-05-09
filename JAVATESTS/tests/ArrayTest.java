package tests;

public class ArrayTest {
	public static int a = 2;
	public static int b = 3;
	public static int c = 7;
	static byte array[] = { 20, 32, 44 };

	private static byte[] multiply(final byte[] arr, int z) {
		byte[] result = new byte[arr.length]; 
		for(int i = 0; i < arr.length;i++)
			result[i] =(byte)(arr[i] * z);
		return result;
	}

	private static byte[] subtract(final byte[] arr, int w) {
		byte[] result = new byte[arr.length]; 
		for(int i = 0; i < arr.length;i++)
			result[i] =(byte)(arr[i] - w);
		return result;
	}

	private static byte[] divide(final byte[] arr, int n) {
		byte[] result = new byte[arr.length]; 
		for(int i = 0; i < arr.length;i++)
			result[i] =(byte)(arr[i] / n);
		return result;
	}
	
	private static String arrayToString(final byte[] arr){
		String s = "[";
		for(int i = 0; i < arr.length;i++){
			s += arr[i];
			if(i < arr.length - 1)
				s += ",";
		}
		s += "]";
		return s;
	}

	public static void runTest() {
		byte[] arr = multiply(array, a);
		System.out.println(""+arrayToString(array)+" * "+a+" = "+arrayToString(arr));
		arr = subtract(array, b);
		System.out.println(""+arrayToString(array)+" - "+b+" = "+arrayToString(arr));
		arr = divide(array, c);
		System.out.println(""+arrayToString(array)+" / "+c+" = "+arrayToString(arr));
	}
}