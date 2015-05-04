//
//  kJVM
//
//  Created by Florin Cristian on 15/02/15.
//  Copyright (c) 2015 Florin Cristian. All rights reserved.
//

#include <stdlib.h>
#include <stdarg.h>
#define __DEF_GLOBALS__
#include "kjvm.h"
#include "heap.h"
#include "stack.h"
#include "classfile.h"
#include "classloader.h"
#include "interpreter.h"

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
    opStackSetBase(currentThreadCB->opStackBase);
    opStackSetSpPos(0);
    methodStackSetBase(currentThreadCB->methodStackBase);
    methodStackSetSpPos(0);
#else
    opStackInit(&opStackBase);
    opStackSetSpPos(0);
    methodStackInit(&methodStackBase);
    methodStackSetSpPos(0);
#endif

#ifdef USE_MICROKERNEL
    extern void loadMicroKernel(void);
    loadMicroKernel();
#endif

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
#ifdef ENABLE_KMETHOD
    const u1 mainMethodIndex = cs[classId].mainMethodId;
#else
    const u1 mainMethodIndex = findMethodByName(classId,"main", 4, "([Ljava/lang/String;)V", 22);
#endif
    if (mainMethodIndex != INVALID_METHOD_ID)
    {
        //PRINTF("  -> run <main> :\n");
        opStackPush(toSlot((u4) 0));           /* args parameter to main (should be a string array)*/
        interpreter_run(classId,mainMethodIndex); /*  run main*/
#ifndef TINYBAJOS
        return 0;
#endif
    }

    PRINTF("no main method found");
    return -1;
}