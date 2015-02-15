/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
// C-functions for native methods
// native void method -> C-function ret value 0
// native non void method -> c-cunction ret value 1 (ret value on java -opStack)
// remember:
// invokespecial Operand Stack
// ..., objectref, [arg0, [arg1 ...]] -> ...
// invokestatic: Operand Stack
// ..., [arg0, [arg1 ...]] -> ..

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "../../JVM/typedefinitions.h"
#include "../../JVM/definitions.h"
#include "../../JVM/bajvm.h"
#include "../../JVM/stack.h"
#include "../../JVM/classfile.h"
#include "../../JVM/scheduler.h"
#include "../../JVM/heap.h"
//#include "native.h"

// insert and update here arrays for classes with native methods
// array length ->  at least up to last native method < methods_count
// lock at methods in the *.java or *.class file in increasing order
// if method is non native -> insert NULL, otherwise pointer to nativce C-function
void conOut(char val)
{
    printf("%c", val);
}


char conIn()
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}


char nativeCharOut()
{
    char val = opStackGetValue(local + 1).UInt;
    printf("%c", val);
    return 0;
}


char nativeCharIn()
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    opStackPush((slot) (u4) ch);
    return 1;
}


char nativeExit()
{
    exit(opStackGetValue(local).UInt);
    return 0;
}


char currentTimeMillis()
{
    struct timeval timerstart;
    gettimeofday(&timerstart, NULL);
    opStackPush(
        (slot)(u4)(
        (timerstart.tv_sec * 1000 + timerstart.tv_usec / 1000)
        & 0x7FFFFFFF));
    return 1;
}
