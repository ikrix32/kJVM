/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */
/* for educational purposes,...*/
/* version 0.1 - 1.10.07*/
/* version 0.2 - 15.3.08*/
/* version 0.3 - 15.1.09*/
/* version 0.4 - 15.6.09*/
/* version 0.5 - 15.1.10 */
/* version 0.6.- 15.6.10 */
/********************************************************************************************
 Erweiterungen von:
 2006	Matthias Böhme und Jakob Fahrenkrug, Ausbildungsbetrieb: Bayer-Schering Pharma AG
2008	Anna Maria Damm, Stephan Bauer, Bayer Schering Pharma AG
Hans-Christian Hecht, CoMedServ GmbH
Adrian Mikosch Lang, Fritz-Haber-Institut
Christopher Hartl, Alcatel-Lucent AG
Jascha Radziewski, DP-IT-Service GmbH
2009	Rainer Kirchhoff, MSA-Auer
Friedrich Grosse, Deutsche Post IT
Sebastian Staroske, Joerdis Loesser, Bayer Schering Pharma AG
Chris Engel, CONTROL
Felix Fehlberg, Berliner Volksbank
Steffen Kalisch, COMED
2010	Christopher-Eyk Hrabia BTC AG
students of informatics at the HWR-Berlin/Berufsakademie
********************************************************************************************/
/* speed is not our primary goal!!!*/
/* no double, no long*/
/* no complete access flags evaluation*/
/* no utf8 but ascii*/
/* ignore some attributes*/
/* no classloader*/
/* no ...*/
/* and errors ........................................................................*/
/*
 but include are:
 garbage collector
threads
synchronized constructs
native methods interface
exception handling
scheduler
*/
/*AVR8(arduinoMega and CharonII) EVK1100 EVK1104 NGW100 STK1000 LINUX AVR32-LINUX XMEGA -> Target Systems*/

#include <stdio.h>
#include <stdlib.h>

#ifdef AVR8
#include <avr/io.h>
#include <avr/pgmspace.h>
#endif

#include "definitions.h"

#define __DEF_GLOBALS__
#include "bajvm.h"
#include "heap.h"
#include "stack.h"
#include "classfile.h"
#include "interpreter.h"
#include "scheduler.h"
#include "stack.h"

#if !(AVR32LINUX||LINUX || AM || CH || XPLAIN || NGW100||STK1000||EVK1100|| EVK1104)
#error You need a valid target device: AVR32LINUX||LINUX || AM || CH || XPLAIN || NGW100||STK1000||EVK1100|| EVK1104
#endif

#ifdef TINYBAJOS
void main() __attribute__ ((noreturn));
void main()
{
#else
    int main(int argc, char* argv[])
    {
#endif
        initHW();
#ifdef LINUX
        initVM(argc-1,argv);
#else
        initVM();
#endif

        PRINTF("Bajos starting\n");
	
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

#ifdef AVR8
#ifndef TINYBAJOS_PRINTF
        printf_P(PSTR("SP: %x cs: %x cFB: %x hB: %x oPSB: %x mSB: %x \n"),
            256*SPH+SPL,cs,AVR8_FLASH_JAVA_BASE, heapBase, opStackBase, methodStackBase);
#endif
#endif

        PRINTF("start clinit");

        for (cN = 0; cN < numClasses; cN++)
            if (findMethodByName("<clinit>", 8, "()V", 3))
        {
            opStackPush(cs[cN].classInfo);
            opStackSetSpPos(findMaxLocals());
            run();
        }
        for (cN = 0; cN < numClasses; cN++)
            if (findMethodByName("main", 4, "([Ljava/lang/String;)V", 22))
        {

            PRINTF("  -> run <main> :\n");

            opStackPush(toSlot((u4) 0));           /* args parameter to main (should be a string array)*/
            opStackSetSpPos(findMaxLocals());
            run();                                /*  run main*/
#ifndef TINYBAJOS
            return 0;
#endif
        }

        ERROREXIT(0, "\nno main found %d", numClasses);
#ifndef TINYBAJOS
        return 1;
#endif
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
