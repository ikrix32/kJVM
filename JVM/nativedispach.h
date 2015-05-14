//
//  native_dispach.h
//  kJVM
//
//  Created by Florin Cristian on 16/03/15.
//  Copyright (c) 2015 Florin Cristian. All rights reserved.
//

#ifndef kJVM_nativedispach_h
#define kJVM_nativedispach_h
typedef s1 byte;
typedef struct byteArray
{
    byte* values;
    u1  length;
} byteArray;

typedef struct charArray
{
    char* values;
    u1    length;
} charArray;

typedef struct shortArray
{
    s2*  values;
    u1   length;
} shortArray;

typedef struct intArray
{
    int* values;
    u1   length;
} intArray;

typedef struct floatArray
{
    float*  values;
    u1      length;
} floatArray;

//u1 nativeDispath(const u1 isInvokeStatic,const char* methodName,const char* methodDescription);
u1 nativeDispath(const u1 isInvokeStatic,const u2 classId,const u2 methodId,const char* methodName,const char* methodDescription);
#endif
