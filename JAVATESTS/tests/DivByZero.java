package tests;

public class DivByZero {

	public static void main(String args[]) {
		try{
			int x = 10 / 0;
		}catch(ArithmeticException ex){
			System.out.println("Exception "+ex.toString());
		}
	}
}
