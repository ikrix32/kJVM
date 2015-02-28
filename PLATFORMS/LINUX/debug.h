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

#define DEBUGPRINT(format, ...) avr8Printf((format),  ## __VA_ARGS__)
#define DEBUGPRINTLN(format, ...) avr8Printf((format),  ## __VA_ARGS__); avr8Printf("\n");

#define DEBUGPRINTE(x,f)    avr8Printf(#x ": " "%"#f" ",x)
#define PRINTE(x,f)  avr8Printf(#x ": " "%"#f" ",x)
#define DEBUGPRINTSTACK {\
    int i;\
    for (i= opStackGetSpPos() > 8 ? -8 : -opStackGetSpPos() ; i < 0 ; i++) \
    { \
        avr8Printf("%08x ",((opStackGetValue(((opStackGetSpPos()+i)<0)?0:(opStackGetSpPos()+i)))).UInt);\
    } \
    avr8Printf(":|_|stack\n"); \
}
/*
 VT102Attribute('0', COLOR_WHITE);avr8Printf(" ");\
 if (i==(opStackGetSpPos()))VT102Attribute ('4', COLOR_GREEN); else VT102Attribute('0', COLOR_WHITE);\
 */
#define DEBUGPRINTLOCALS {\
    u1 i;\
    avr8Printf("|.| local:");\
    for (i=0; i < 8 && i < local; i++)\
        avr8Printf(" %08x",opStackGetValue(local+i).UInt);\
    avr8Printf("\n"); \
}

#define DEBUGPRINTHEAP
/*{\
    u2 i,j;\
    avr8Printf("|#|  heap:\n");\
    for (j=0; j <33; j+=8){\
        for (i=0; i < 8; i++)\
            avr8Printf(" %8x",(*(heapBase+i+j)).UInt);avr8Printf("\n");}\
    }\
    avr8Printf("\n");\
 }
 */

#define DEBUGPRINTSTRING(p,l) {\
u2 _i;\
    for (_i=0; _i < (l); _i++)\
        avr8Printf("%c",*((u1*)(p)+_i));\
    avr8Printf(" "); \
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
#undef DDEBUG_VM
#endif

#ifdef DEBUG_VM
#define DEBUGPRINTLN_OPC(format, ...) avr8Printf((format),  ## __VA_ARGS__)
#define DEBUGPRINT_OPC(format, ...) avr8Printf((format),  ## __VA_ARGS__)
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
