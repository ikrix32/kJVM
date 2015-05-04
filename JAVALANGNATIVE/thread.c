/*
 * FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
/* C-functions for native methods*/
/* native void method -> C-function ret value 0*/
/* native non void method -> c-function ret value 1 (ret value on java -opStack)*/
/* remember:*/
/* invokespecial Operand Stack*/
/* ..., objectref, [arg0, [arg1 ...]] -> ...*/
/* invokestatic: Operand Stack*/
/* ..., [arg0, [arg1 ...]] -> ...*/
#include "config.h"

#ifndef TINYBAJOS_MULTITASKING
//#include <stdio.h>
//#include <stdlib.h>
#include "definitions.h"
#include "typedefinitions.h"
#include "kjvm.h"
#include "classfile.h"
#include "stack.h"
#include "scheduler.h"
#include "heap.h"
#include "stack.h"
#include "thread.h"

char start()
{
    cN = opStackGetValue(local).stackObj.classNumber;
    createThread();
    return 0;
}


char yield()                                      //by ceh
{
    //force scheduling!
    currentThreadCB->numTicks = 0;
    return 0;
}


char currentThread()
{
    opStackPush(currentThreadCB->obj);
    return 1;
}


char nativeSetPriority()                          //by ceh
{
    return 0;
}


//to do
char interrupt()
{
    return 0;
}


char interrupted()
{
    return 1;
}


char isInterrupted()
{
    return 1;
}


char ksleep()//krix
{
    return 0;
}


char join()
{
    return 0;
}


char jointimeout()
{
    return 0;
}


char nativeSleep()
{
    return 0;
}
#endif
