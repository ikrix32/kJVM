package tests;
/*
  Inheritance.java

  (c) 2005 Till Harbaum <till@harbaum.org>
*/

class Inheritance {

  static void methodA() {
    System.out.println("Inheritance.methodA()");
  }

  public static void main(String[] args) {
    InheritanceA inheritanceA = new InheritanceB();
    InheritanceB inheritanceB = new InheritanceB();
    InheritanceA inheritanceC = new InheritanceA();

    System.out.println("Inheritance test");
    
    methodA();

    inheritanceA.methodB();
    inheritanceB.methodB();

    System.out.println("inheritanceA.varA = " + inheritanceA.varA);
    System.out.println("inheritanceA.varB = " + inheritanceA.varB);
    System.out.println("inheritanceB.varB = " + inheritanceB.varB);
    
    boolean isAClass = inheritanceC instanceof InheritanceA;
    System.out.println("inheritanceC is instance of InheritanceA:"+isAClass);
    
    boolean isBClass = inheritanceC instanceof InheritanceB;
    System.out.println("inheritanceC is instance of InheritanceB:"+isBClass);
    
    inheritanceA = (InheritanceA)inheritanceB;
    System.out.println("Type cast succedded!!");
  }
}
