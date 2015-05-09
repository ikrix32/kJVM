package tests;
/*
  StringAndHeapTest.java
 */

class StringAndHeapTest {

  static void test(int a) {
    System.out.println("test: "+a);
  }

  public static void runTest() {
    int i = 12;

    System.out.println("A String");
    System.out.println("A String + Number: "+ i);
    System.out.println("A String + Number: "+ i + " + String");
    System.out.println("" +i + " A number + String");

    for(int j=0;j<6;j++) {
      System.out.println("TEST "+ i + " + TEST " + j + " TEST");
    }

    test(i);
  }
}

     
