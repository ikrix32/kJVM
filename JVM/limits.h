/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
#ifndef __LIMITS__
#define __LIMITS__

#include "config.h"

#ifdef AVR8
#define MAXCLASSES          20
#define MAXBYTECODE             0x5000            //!!! class file bytes in flash!!
#ifdef CH
#define MAXHEAP             0x1800                // u4 entries for heap-elements
#define MAXHEAPOBJECTLENGTH         0x100
#define MAXTHREADS          8
#define MAXLOCKEDTHREADOBJECTS      32
#define OPSTACKSIZE             0x80              // u4
#define METHODSTACKSIZE         0x80              // u2
#define MAXNATIVEMETHODS        60
#endif
#ifdef AM
#define MAXHEAP             0x200                 // u4 entries for heap-elements
#define MAXHEAPOBJECTLENGTH         0x40
#define MAXTHREADS          4
#define MAXLOCKEDTHREADOBJECTS      16
#define OPSTACKSIZE             64                // u4
#define METHODSTACKSIZE         48                // u2
#define MAXNATIVEMETHODS        40
#endif
#ifdef XPLAIN
#define MAXHEAP             0x180                 // u4 entries for heap-elements
#define MAXHEAPOBJECTLENGTH         0x40
#define MAXTHREADS          6
#define MAXLOCKEDTHREADOBJECTS      4
#define OPSTACKSIZE             48                // u4
#define METHODSTACKSIZE         24                // u2
#define MAXNATIVEMETHODS        40
#endif                                            //XPLIAQN
#endif                                            //AVR8
#if (!AVR8)
#define MAXCLASSES          45                    //30
#define MAXBYTECODE             52768             // 16384 !!! class file bytes < 16 kB
#define MAXHEAP             6000                  // number of u4 entries for heap-elements
#define MAXHEAPOBJECTLENGTH         96
#define MAXTHREADS          46
#define MAXLOCKEDTHREADOBJECTS      24
#define OPSTACKSIZE             (328+96)          // u4
#define METHODSTACKSIZE         164               // u2
#define MAXNATIVEMETHODS        80
#endif                                            //!AVR8
#endif                                            //__LIMITS__
