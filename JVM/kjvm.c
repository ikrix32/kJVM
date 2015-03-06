//
//  kJVM
//
//  Created by Florin Cristian on 15/02/15.
//  Copyright (c) 2015 Florin Cristian. All rights reserved.
//

#include <stdio.h>
//#include <stdlib.h>

#ifdef AVR8
#include <avr/io.h>
#include <avr/pgmspace.h>
#endif

#include "definitions.h"

#define __DEF_GLOBALS__
#include "kjvm.h"
#include "heap.h"
#include "stack.h"
#include "classfile.h"
#include "classloader.h"
#include "scheduler.h"
#include "interpreter.h"
#include "platform.h"

#if !(AVR32LINUX || LINUX || AM || CH || XPLAIN || NGW100||STK1000||EVK1100|| EVK1104)
#error You need a valid target device: AVR32LINUX||LINUX || AM || CH || XPLAIN || NGW100||STK1000||EVK1100|| EVK1104
#endif


/* all class files stored for linux in DS (malloc)*/
void vm_init() /* read, analyze classfiles and fill structures*/
{
    classLoader_init();

    heapInit();

#ifndef TINYBAJOS_MULTITASKING
    createThread();                           /* for main*/
    opStackBase = currentThreadCB->opStackBase;
    opStackSetSpPos(0);
    methodStackBase = currentThreadCB->methodStackBase;
    methodStackSetSpPos(0);
#else
    opStackInit(&opStackBase);
    opStackSetSpPos(0);
    methodStackInit(&methodStackBase);
    methodStackSetSpPos(0);
#endif

    loadBootstrapClasses();

    DEBUGPRINTHEAP;


    PRINTF("kjvm starting\n");
#ifdef AVR8
#ifndef TINYBAJOS_PRINTF
    PRINTF(PSTR("SP: %x cs: %x cFB: %x hB: %x oPSB: %x mSB: %x \n"),
                256 * SPH+SPL,cs,AVR8_FLASH_JAVA_BASE, heapBase, opStackBase, methodStackBase);
#endif
#endif

}

// classId containing the clinit or main method
s1 vm_run(const u1 classId){
    //PRINTF("start clinit from classsId %d\n",classId);
    cN = classId;
    mN = FIND_METHOD_BYNAME(cN,"main", 4, "([Ljava/lang/String;)V", 22);
    if (mN != INVALID_METHOD_ID)
    {
        //PRINTF("  -> run <main> :\n");
        opStackPush(toSlot((u4) 0));           /* args parameter to main (should be a string array)*/
        opStackSetSpPos(findMaxLocals(cN,mN));
        interpreter_run(); /*  run main*/
#ifndef TINYBAJOS
        return 0;
#endif
    }

    PRINTF("no clinit/main method found");
    return -1;
}

#ifndef TINYBAJOS_ERROREXIT
    void errorExitFunction(int nr, const char *format, ...)
    {
        va_list list;
        va_start(list, format);
#ifdef AVR8
        vfprintf_P(stdout, format, list);         //vprintf_P does not exist in current avr-libc
#else
        vprintf(format, list);
#endif
        va_end(list);
        exit(nr);
    }
#endif
