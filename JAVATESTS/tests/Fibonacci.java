package tests;
/*
  Fibonacci.java
 */

class Fibonacci {

  static int fib(int n) {
    if(n < 2) 
      return 1;
    else
      return fib(n-2) + fib(n-1);
  } 

  public static void main(String[] args) {
    //for(int i=0;i<=20;i++)
	  float x = -1.0005f;
      System.out.println("Fibonacci of "+20+" is "+fib(20)+" aa:"+x);
  }
}

     
