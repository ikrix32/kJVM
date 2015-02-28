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

#include <stdio.h>
//#include <stdlib.h>
#include "definitions.h"
#include "typedefinitions.h"
#include "kjvm.h"
#include "classfile.h"
#include "stack.h"
#include "heap.h"
#include "stack.h"
#include "nstring.h"

char nativeStringLength()
{
    const slot mySlot = opStackGetValue(local);
    if (mySlot.stackObj.magic == CPSTRINGMAGIC)
    {
        methodStackPush(cN);
        cN = (u1) (mySlot.stackObj.classNumber);
        opStackPush( toSlot( (u4) getU2(cN, CP( cN, getU2(cN,CP(cN, mySlot.stackObj.pos) + 1)) + 1)));
        cN = methodStackPop();
    } else
        opStackPush(toSlot((u4) 0xffff));
    return 1;
}


char nativeCharAt()
{
    const slot mySlot = opStackGetValue(local);
    if (mySlot.stackObj.magic == CPSTRINGMAGIC)
    {
        methodStackPush(cN);
        cN = (u1) (mySlot.stackObj.classNumber);
        opStackPush(toSlot((u4) getU1(cN,CP(cN,getU2(cN,CP(cN,mySlot.stackObj.pos) + 1)) + 3 + (u2) opStackGetValue(local + 1).UInt)));
        cN = methodStackPop();
    } else
        opStackPush(toSlot((u4) 0));
    return 1;
}

int stringLength(const char* str){
    int len = 0;
    while(str[len] != 0){
        len++;
    }
    return len;
}

char stringsNotEquals(const char* str1,const char* str2,const size_t len){
    //int equals = 0;
    for(int i = 0; i < len;i++){
        if(str1[i] != str2[i]){
            //equals = 1;
            return 1;
            //break;
        }
    }
    return 0;
    //PRINTF("equals:%d,strcmp:%d\n",equals,strncmp(str1,str2,len));
    //return strncmp(str1,str2,len);
}