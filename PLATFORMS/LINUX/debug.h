//
//  debug.h
//  kJVM
//
//  Created by Florin Cristian on 28/02/15.
//  Copyright (c) 2015 Florin Cristian. All rights reserved.
//

#ifndef kJVM_debug_h
#define kJVM_debug_h

#ifdef DEBUG
#define OUTSTREAM stdout

#define DEBUGPRINT(format, ...) PRINTF ((format),  ## __VA_ARGS__)
#define DEBUGPRINTLN(format, ...) PRINTF ((format),  ## __VA_ARGS__); PRINTF("\n");

#define DEBUGPRINTE(x,f)    PRINTF(#x ": " "%"#f" ",x)
#define PRINTE(x,f)         PRINTF(#x ": " "%"#f" ",x)
#define DEBUGPRINTSTACK {\
    int i;\
    for (i= opStackGetSpPos() > 8 ? -8 : -opStackGetSpPos() ; i < 0 ; i++) \
    { \
        PRINTF("%08x ",((opStackGetValue(((opStackGetSpPos()+i)<0)?0:(opStackGetSpPos()+i)))).UInt);\
    } \
    PRINTF(":|_|stack\n"); \
}

#define DEBUGPRINTLOCALS {\
    u1 i;\
    PRINTF("|.| local:");\
    for (i=0; i < 8 && i < local; i++)\
        PRINTF(" %08x",opStackGetValue(local+i).UInt);\
    PRINTF("\n"); \
}

#define DEBUGPRINTHEAP heapPrintMemory

#define DEBUGPRINTSTRING(p,l) {\
u2 _i;\
    for (_i=0; _i < (l); _i++)\
        PRINTF("%c",*((u1*)(p)+_i));\
    PRINTF(" "); \
}

#define DEBUGPRINTLNSTRING(p,l) {DEBUGPRINTSTRING(p,l); avr8Printf("\n");}

#define DEBUGPRINTF(format, ...) PRINTF(format, ## __VA_ARGS__)

#else

#define DEBUGPRINT(format, ...)
#define DEBUGPRINTLN(format, ...)
#define DEBUGPRINTE(x,f)
#define DEBUGPRINTSTACK
#define DEBUGPRINTHEAP
#define DEBUGPRINTLOCALS
#define DEBUGPRINTSTRING(p,l)
#define DEBUGPRINTLNSTRING(p,l)
#define OUTSTREAM stderr

#define DEBUGPRINTF(format, ...)

#endif

#ifndef DEBUG
#undef DEBUG_VM
#endif

#ifdef DEBUG_VM
#define DEBUGPRINTLN_OPC(format, ...) PRINTF((format),  ## __VA_ARGS__)
#define DEBUGPRINT_OPC(format, ...) PRINTF((format),  ## __VA_ARGS__)
#else
#define DEBUGPRINTLN_OPC(format, ...)
#define DEBUGPRINT_OPC(format, ...)
#endif

#ifdef DEBUG_CLASS_LOADING
#define DEBUG_CL_PRINTF(format, ...) PRINTF(format, ## __VA_ARGS__)
#else
#define DEBUG_CL_PRINTF(format, ...)
#endif

#endif
