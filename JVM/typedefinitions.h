/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
#ifndef __TYPEDEFINITIONS__
#define __TYPEDEFINITIONS__
#include "limits.h"

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

typedef struct
{
    u4 status :2;                                 // 0 -> free for further use; 1 -> allcated static class object; 2-> allocated other object; 3-> allocated arrays
    u4 rootCheck :1;                              // 0 -> 1-> root object (opstack or static) or free or "object in object" !!!
    u4 mutex :1;                                  // mutex for object
    u4 length :16;                                // max 8k 4 byte alloated slots for object!!elements for an object,array on the heap, including heapObjMarker
// expandable
    u4 magic :4;                                  //  um Objects und prim types auf dem heap zu unterscheiden !!!!!
    u4 xxx :8;                                    // reducable
} heapObjectMarker;

#define classNumber arrayLength

typedef struct
{
    u4 pos :16;                                   // on heap (reataive to base) or for CP-string cN refbyte in CP
// 2008 if pos==0 or classnumber ==255 or magic+1 -> CP-String!!
    u4 xxx :4;
    u4 classNumber :8;                            // or arrayLength for arrayobjects
    u4 magic :4;
} stackObjectInfo;

typedef union
{
    u4 UInt;
    s4 Int;
    f4 Float;
    u1 bytes[4];
    u2 characters[2];
    stackObjectInfo stackObj;
    heapObjectMarker heapObjMarker;
} slot;

#ifndef TINYBAJOS_MULTITASKING
typedef struct ThreadControlBlock
{
    slot* opStackBase;
    u2* methodStackBase;
    u2 methodSpPos;                               // relative to methodStackBase
    u1 tid;
    slot obj;                                     // thread creating object
    u4* pPriority;                                // pointer to priority on heap , for main thread global memory range
    u1 numTicks;
    u1 state;                                     // THREADNOTBLOCKED, THREADMUTEXBLOCKED, THREADWAITBLOCKED, THREADWAITAWEAKENED
    struct ThreadControlBlock* pred;
    struct ThreadControlBlock* succ;
    slot isMutexBlockedOrWaitingForObject;
    slot hasMutexLockForObject[MAXLOCKEDTHREADOBJECTS];
    u1 lockCount[MAXLOCKEDTHREADOBJECTS];
} ThreadControlBlock;

typedef struct ThreadPriorityList
{
    ThreadControlBlock* cb;
    u1 count;
} ThreadPriorityList;
#endif

#define methodStartPC (methodInfoStart+6)

#define CONSTANTPOOLCOUNT(cN)   ((u2) *cs[cN].classFileStartAddress+8)

typedef struct
{
    char* classFileStartAddress;                  // Speicheradresse fuer classfile-bytes
    u2 classFileLength;                           // << 64K
    u2 magic;                                     // kann weg
    u2 minor_version;                             // kann weg
    u2 major_version;                             // kann weg
    u2 constant_pool_count;                       // kann weg
    u2* constant_pool;                            //cp_info constant_pool[constant_pool_count-1];
    u2 access_flags;
    u2 this_class;
    u2 super_class;
    u2 interfaces_count;
    u2 interfaces;                                //u2 interfaces[interfaces_count];
    u2 fields_count;
    u2* field_info;                               // field_info fields[fields_count];
// nur ConstantValue attribute
    u2 methods_count;
    u2* method_info;                              //method_info methods[methods_count];
    u2 attributes_count;
    u2* attribute_info;                           //attribute_info attributes[attributes_count];
    functionForNativeMethodType* nativeFunction;
    slot classInfo;
} classStructure;
#endif
