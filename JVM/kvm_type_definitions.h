//
//  kvm_typedefinitions.h
//  kJVM
//
//  Created by Florin Cristian on 16/05/15.
//  Copyright (c) 2015 Florin Cristian. All rights reserved.
//

#ifndef KVM_TYPE_DEFINITIONS
#define KVM_TYPE_DEFINITIONS

#include "config.h"

typedef char (*functionForNativeMethodType)(void);
typedef unsigned char u1;
typedef signed char s1;
typedef float f4;
#if (LINUX || AVR32UC3A|| AVR32AP7000||AVR32LINUX)
#if ( __i386__ || AVR32UC3A|| AVR32AP7000 || AVR32LINUX )
typedef unsigned short u2;
typedef unsigned int u4;
typedef unsigned long long u8;
typedef signed short s2;
typedef signed int s4;
#endif
#ifdef __x86_64__
typedef unsigned short u2;
typedef unsigned int u4;
typedef unsigned long u8;
typedef signed short s2;
typedef signed int s4;
#endif
#endif
#ifdef AVR8
typedef unsigned int u2;
typedef unsigned long u4;
typedef unsigned long long u8;
typedef signed int s2;                            //mb added "int"
typedef signed long s4;
#endif

#define NULL 0

#define kvm_internal extern
#define kvm_mem_pointer void*

typedef void jvoid;
typedef u1 jboolean;
typedef s1 jbyte;
typedef u1 jchar;//todo - check if java char is signed or unsigned and has 2 bytes size
typedef s2 jshort;
typedef s4 jint;
typedef f4 jfloat;

typedef struct jbooleanArray
{
    jboolean* values;
    u1        length;
} jbooleanArray;

typedef struct jbyteArray
{
    jbyte*  values;
    u1      length;
} jbyteArray;

typedef struct jcharArray
{
    jchar*  values;
    u1      length;
} jcharArray;

typedef struct jshortArray
{
    jshort* values;
    u1      length;
} jshortArray;

typedef struct jintArray
{
    jint* values;
    u1    length;
} jintArray;

typedef struct jfloatArray
{
    jfloat* values;
    u1      length;
} jfloatArray;

typedef enum NativeTypes{
    VOID,
    BOOLEAN,
    BYTE,
    CHAR,
    SHORT,
    INT,
    FLOAT,
    BOOLEANARRAY,
    BYTEARRAY,
    CHARARRAY,
    SHORTARRAY,
    INTARRAY,
    FLOATARRAY,
} NativeType;
#endif
