#ifndef __LIMITS__
#define __LIMITS__

#include "config.h"

#define MAXCLASSES          45                    //30
#define MAXBYTECODE         128000//52768                 // 16384 !!! class file bytes < 16 kB
#define MAXHEAP             60000//6000                  // number of u4 entries for heap-elements
#define MAXHEAPOBJECTLENGTH         960//96
#define MAXTHREADS          46
#define MAXLOCKEDTHREADOBJECTS      24
#define OPSTACKSIZE             (328+96)          // u4
#define METHODSTACKSIZE         164               // u2
#define MAXNATIVEMETHODS        80
#define NUMBEROFINTERRUPTS  1

#endif                                            //__LIMITS__
