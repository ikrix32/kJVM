#ifndef __LIMITS__
#define __LIMITS__

#include "config.h"

#define MAXCLASSES          50                    //max 0xFF - stackObjectInfo.classNumber has only 8 bits
#define MAXBYTECODE         128000//52768         // 16384 !!! class file bytes < 16 kB

#define MAXHEAP             420                   // number of u4 entries for heap-elements

#define MAX_ARRAY_SIZE      0xFF                  //stackObjectInfo.arrayLength has only 8 bits

#define OPSTACKSIZE             (328 + 96)        // u4
#define METHODSTACKSIZE         164               // u2

#define MAXTHREADS              46
#define MAXLOCKEDTHREADOBJECTS  24

//#define MAXNATIVEMETHODS        80
#define NUMBEROFINTERRUPTS  1

#endif                                            //__LIMITS__
