package tests;
/*
  StaticTest.java
 */

class StaticTest {
  static int a = 32;
  static int b = 11;
  static String c = "Nase";

  public static void runTest() {
    System.out.println("StaticTest");
    System.out.println("a = " + a);
    System.out.println("b = " + b);
    a = 22;
    System.out.println("a = " + a);
    System.out.println("Nase = " + c);
  }
}

     
