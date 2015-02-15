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
// ..., [arg0, [arg1 ...]] -> ...

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../JVM/definitions.h"
#include "../../JVM/typedefinitions.h"
#include "../../JVM/bajvm.h"
#include "../../JVM/stack.h"
#include "../../JVM/classfile.h"
#include "../../JVM/scheduler.h"
#include "../../JVM/heap.h"
#include "native.h"
#include "platform.h"

char nativeCharOut()
{
    char val = opStackGetValue(local + 1).UInt;
    printf("%c", val);
    return 0;
}


char nativeCharIn()
{
    char ch = conIn();
    opStackPush((slot) (u4) ch);
    return 1;
}


char currentTimeMillis()
{
    opStackPush((slot) (u4) timerMilliSec);
    return 1;
}


char nativeExit(int status)
{
    asm("  lda.w   pc, 0");
//asm("  mov   pc, 0");
//goto *0x00000000;
    return 0;
}
