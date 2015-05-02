#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#ifdef AVR8
#include <avr/pgmspace.h>
#endif

#include "typedefinitions.h"
// RAMPZ ever 1 -> therefore 0x8000+0x3000=0xb000 (words) java class base
// 0x10000 + 0x06000 in bytes
// bamo128 -> 0xF000 to 0xFFFF in words
#define AVR8_FLASH_JAVA_BASE    0x0000            // +rampz;16k boot classes in flash	-> 0x8000 in words
#define AVR8_FLASH_APP_BASE     0xcc00            // +rampz;6k application classes	-> 0xe600
// UC3A is evk1100
#define UC3A_FLASH_BASE         0x80000000        // internal 512 KB flash
//#define UC3A_FLASH_JAVA_BASE	0x80040000        // boot classes in flash
#define UC3A_FLASH_JAVA_BASE    0x80038000        // boot classes in flash
#define UC3A_SDRAM_BASE     0xD0000000
#define UC3A_SDRAM_JAVA_BASE    0xD0200000        // application classes, heap, opstack methodstack
#define STK1000_FLASH_BASE  0                     // external 8 MB flash
#define STK1000_FLASH_JAVA_BASE 0x00040000        // boot classes in flash
#define STK1000_SDRAM_BASE  0x10000000            // external SDRAM 8MB
#define STK1000_SDRAM_JAVA_BASE (STK1000_SDRAM_BASE+0x00600000)

#define STK1000_SDRAM_LCD_BASE  STK1000_SDRAM_BASE

#define NGW_FLASH_BASE      0                     // external 8 MB flash
#define NGW_FLASH_JAVA_BASE 0x00040000            // boot classes in flash
#define NGW_SDRAM_BASE      0x10000000            // external SDRAM
#define NGW_SDRAM_JAVA_BASE     (NGW_SDRAM_BASE+0x00600000)

#define HEAPFREESPACE           0
#define HEAPALLOCATEDSTATICCLASSOBJECT  1
#define HEAPALLOCATEDNEWOBJECT      2
#define HEAPALLOCATEDARRAY      3
#define OBJECTMAGIC         0xA
#define CPSTRINGMAGIC           (OBJECTMAGIC+1)
#define MUTEXNOTBLOCKED         1
#define MUTEXBLOCKED            0

#define THREADNOTBLOCKED        0
#define THREADMUTEXBLOCKED      1
#define THREADWAITBLOCKED       2
#define THREADWAITAWAKENED      3
#define GARBAGEFUSIONTRIALS     20

#ifdef AVR8                                       // change all avr8 string to flash strings gives more data ram space for java!!
    #ifdef TINYBAJOS
	#ifndef TINYBAJOS_PRINTF
	    #define	PRINTF(format, ...)
	#else
	    #define PRINTF(format, ...) printf_P(PSTR(format),  ## __VA_ARGS__)
	#endif
    #else
	#define PRINTF(format, ...) printf_P(PSTR(format),  ## __VA_ARGS__)
    #endif
#else
    #define PRINTF(format, ...) printf(format,  ## __VA_ARGS__)
#endif

#ifdef TINYBAJOS_ERROREXIT
    #define ERROREXIT(nr, format, ...) exit((nr))
#else
    #ifdef AVR8                                       // change all avr8 string to flash strings gives more data ram space for java!!
	#define ERROREXIT(nr, format, ...) errorExitFunction ((nr), PSTR((format)),  ## __VA_ARGS__)
    #else
	#define ERROREXIT(nr, format, ...) errorExitFunction ((nr), (format),  ## __VA_ARGS__)
    #endif
#endif

//#ifdef AVR8
//    #define STRNCMPRAMFLASH strncmpRamFlash
//    #define STRNCMPFLASHFLASH strncmpFlashFlash
//#else
    #define STRNCMPRAMFLASH     stringsNotEquals
    #define STRNCMPFLASHFLASH   stringsNotEquals
//#endif



#ifdef AVR8                                       // change all avr8 string to flash strings gives more data ram space for java!!
    #define avr8Printf( format, ...) printf_P (PSTR((format)),  ## __VA_ARGS__)
#else
    #define avr8Printf( format, ...) printf ((format),  ## __VA_ARGS__)
#endif

#ifndef TINYBAJOS_ERROREXIT
#ifdef AVR8                                       // change all avr8 string to flash strings gives more data ram space for java!!
#define errorExit(nr, format, ...) errorExitFunction ((nr), PSTR((format)),  ## __VA_ARGS__)
#else
#define errorExit(nr, format, ...) errorExitFunction ((nr), (format),  ## __VA_ARGS__)
#endif
#else
#define errorExit(nr, format, ...) exit((nr))
#endif

#ifndef TINYBAJOS_PRINTF
#define verbosePrintf( format, ...) avr8Printf ((format),  ## __VA_ARGS__)
#else
#define verbosePrintf( format, ...)
#endif

#if (AVR32UC3A || AVR32AP7000)
#define NULLOBJECT          (toSlot(0xfffff000 |  ((u4)OBJECTMAGIC<<8)|((u4)OBJECTMAGIC<<4)|((u4)OBJECTMAGIC)))
#else
#define NULLOBJECT          (toSlot(0x000fffff |  ((u4)OBJECTMAGIC<<28)|((u4)OBJECTMAGIC<<24)|((u4)OBJECTMAGIC<<20)))
#endif

//mb jf
#ifndef TINYBAJOS_EXCEPTION
#define ARRAYINDEXOUTOFBOUNDSEXCEPTION raiseExceptionFromIdentifier(ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION)
#define NEGATIVEARRAYSIZEEXCEPTION raiseExceptionFromIdentifier(NEGATIVE_ARRAY_SIZE_EXCEPTION)
#define NULLPOINTEREXCEPTION raiseExceptionFromIdentifier(NULL_POINTER_EXCEPTION)
#define ARITHMETICEXCEPTION raiseExceptionFromIdentifier(ARITHMETIC_EXCEPTION)
#define CLASSCASTEXCEPTION raiseExceptionFromIdentifier(CLASS_CAST_EXCEPTION)
#define ILLEGALMONITORSTATEEXCEPTION raiseExceptionFromIdentifier(ILLEGAL_MONITOR_STATE_EXCEPTION)

#define DNOTSUPPORTED errorExit(-2, "Double precision primitive data types (double and long) are not supported.\n")
#define CLASSNOTFOUNDERR(classname,classnamelength) {errorExit(-3, "Class '%s' not found.\n", classname);}
#define UNHANDLEDEXCEPTIONERR(exceptionname) errorExit(-4, "Unhandled exception of type '%s'.\n", exceptionname)
#define FIELDNOTFOUNDERR(fieldname, classname) errorExit(-5, "Field '%s' in class '%s' not found.\n", fieldname, classname)
#define METHODNOTFOUNDERR(methodname, classname) errorExit(-6, "Method '%s' in class '%s' not found.\n", methodname, classname)
#define MALLOCERR(count, target) errorExit(-7, "Malloc error while trying to allocate %d bytes for %s\n", count, target)
#else
#define ARRAYINDEXOUTOFBOUNDSEXCEPTION errorExit(-8, "ARRAYINDEXOUTOFBOUNDS\n")
#define NEGATIVEARRAYSIZEEXCEPTION errorExit(-2, "NEGATIVEARRAYSIZE\n")
#define NULLPOINTEREXCEPTION errorExit(-2, "NULLPOINTER\n")
#define ARITHMETICEXCEPTION errorExit(-2, "ARITHMETIC\n")
#define CLASSCASTEXCEPTION errorExit(-2, "CLASSCAST\n")
#define ILLEGALMONITORSTATEEXCEPTION  errorExit(-2, "ILLEGALMONITORSTATE\n")
#define DNOTSUPPORTED errorExit(-2, "Double precision primitive data types (double and long) are not supported.\n")
#define CLASSNOTFOUNDERR(classname,classnamelength) {errorExit(-3, "Class '%s' not found.\n", classname);}
#define UNHANDLEDEXCEPTIONERR(exceptionname) errorExit(-4, "Unhandled exception of type '%s'.\n", exceptionname)
#define FIELDNOTFOUNDERR(fieldname, classname) errorExit(-5, "Field '%s' in class '%s' not found.\n", fieldname, classname)
#define METHODNOTFOUNDERR(methodname, classname) errorExit(-6, "Method '%s' in class '%s' not found.\n", methodname, classname)
#define MALLOCERR(count, target) errorExit(-7, "Malloc error while trying to allocate %d bytes for %s\n", count, target)
#endif

//#define CLASSSTA            (cs[classId].classFileStartAddress)
                                                  // constantpool entry n of class m
#define CP(m,n)             (cs[m].constant_pool[n])
#define BYTECODEREF         ((byte1 << 8) | byte2)
#define HEAPOBJECTMARKER(pos)       ((*(heapBase+(pos))).heapObjMarker)
#define HEAPOBJECT(pos)         (*(heapBase+pos))

#define METHODBASE(classId,methodId)       (cs[classId].method_info[methodId])
#define METHODACCESSFLAG(classId,methodId)     getU2(METHODBASE(classId,methodId))
#define METHODNAMESTR(classId,methodId)        getAddr(cN,CP(classId,getU2(METHODBASE(classId,methodId)+2))  +3)
#define METHODNAMESTRLENGTH(classId,methodId)  getU2(CP(classId,getU2(METHODBASE(classId,methodId)+2))  +1)
#define METHODDESCRSTR(classId,methodId)       getAddr(cN,CP(classId,getU2(METHODBASE(classId,methodId)+4))  +3)
#define METHODDESCRSTRLENGTH(classId,methodId) getU2(CP(classId,getU2(METHODBASE(classId,methodId)+4))  +1)
                                                  // at length(u2)
#define METHODATTRIBUTEBASE(classId,methodId)  (METHODBASE(classId,methodId)+6)
#define FIELDBASE(classId,fieldId)        (cs[classId].field_info[fieldId])

// change pc
#define METHODCODEBASE(classId,methodId)       (getStartPC(classId,methodId) - 14)
                                                  //start at length(u2)
#define METHODCODEEXCEPTIONBASE(classId,methodId)  (getStartPC(classId,methodId) + getU4(classId,getStartPC(classId,methodId) - 4))
#define METHODEXCEPTIONBASE(classId,methodId)  (cs[classId].method_info[methodId,3])

#define   CONSTANT_Class                    7
#define   CONSTANT_Fieldref                 9
#define   CONSTANT_Methodref                10
#define   CONSTANT_InterfaceMethodref   11
#define   CONSTANT_String                   8
#define   CONSTANT_Integer                  3
#define   CONSTANT_Float                    4
#define   CONSTANT_Long                 5
#define   CONSTANT_Double                   6
#define   CONSTANT_NameAndType          12
#define   CONSTANT_Utf8                     1

#define   CONSTANT_KClass           25
#define   CONSTANT_KFIELD_REF       26
#define   CONSTANT_KMEHOD_REF       27
#define   CONSTANT_KINTERFACE_MEHOD_REF 28
#define   CONSTANT_KNAME_AND_TYPE   29

#define     ACC_PUBLIC              0x0001        //Declared public; may be accessed from outside its package.
#define     ACC_PRIVATE         0x0002            //Declared private; accessible only within the defining class.
#define     ACC_PROTECTED   0x0004                //Declared protected; may be accessed within subclasses.
#define     ACC_STATIC              0x0008        //Declared static.
#define     ACC_FINAL               0x0010        //Declared final; may not be overridden.
#define     ACC_SYNCHRONIZED 0x0020               //Declared synchronized; invocation is wrapped in a monitor lock.
#define     ACC_NATIVE              0x0100        //Declared native; implemented in a language other than Java.
#define     ACC_ABSTRACT        0x0400            //Declared abstract; no implementation is provided.
#define     ACC_STRICT              0x0800        //Declared strictfp; floating-point mode is FP-strict
//mb jf
#define     T_BOOLEAN           0x4               // aaray type boolean
#define     T_CHAR              0x5               // aaray type boolean
#define     T_FLOAT             0x6               // aaray type boolean
#define     T_DOUBLE            0x7               // aaray type boolean
#define     T_BYTE              0x8               // aaray type boolean
#define     T_SHORT             0x9               // aaray type boolean
#define     T_INT               0xa               // aaray type boolean
#define     T_LONG              0xb               // aaray type boolean


#define GET_TAG(classId,x) getU1(classId,CP(classId, x))

#define METHODREF_GET_CLASSINFOID(classId,x) getU2(classId,CP(classId, x) + 1)//1 = sizeof(tag)
#define METHODREF_GET_NAME_AND_TYPEID(classId,x) getU2(classId,CP(classId, x) + 3)//3 = sizeof(tag) + sizeof(classInfoId)

#define FIELDINFO_GET_CLASSINFOID(classId,x) getU2(classId,CP(classId, x) + 1)//1 = sizeof(tag)
#define FIELDINFO_GET_NAME_AND_TYPEID(classId,x) getU2(classId,CP(classId, x) + 3)//3 = sizeof(tag) + sizeof(classInfoId)


#define NAMEANDTYPE_GET_NAMEID(classId,x) getU2(classId,CP(classId, x) + 1)//1 = sizeof(tag)
#define NAMEANDTYPE_GET_DESCRIPTIONID(classId,x) getU2(classId,CP(classId, x) + 3)//1 = sizeof(tag) + sizeof(nameId)

#define CLASSINFO_GET_NAMEID(classId,classInfoId) getU2(classId,CP(classId, classInfoId) + 1)//1 = sizeof(tag)

#define UTF8_GET_LENGTH(classId,id) getU2(classId,CP(classId, id) + 1);// 1 = sizeof(tag)
#define UTF8_GET_STRING(classId,id) (char*)getAddr(classId,CP(classId, id) + 3)//3 = sizeof(tag) + sizeof(classNameLength)

#define PRINT(format, ...)
#define PRINTLN(format, ...)

#define PRINTEXITTHREAD(a,b) {\
    avr8Printf(a,b);\
    if (numThreads == 1) \
    { \
        return; \
    }\
    else {  deleteThread();} \
}

#define  COLOR_BLACK     0                        /* VT102 Color Codes                                  */
#define  COLOR_RED       1
#define  COLOR_GREEN     2
#define  COLOR_BROWN     3
#define  COLOR_BLUE      4
#define  COLOR_MAGENTA   5
#define  COLOR_CYAN      6
#define  COLOR_WHITE     7

#ifdef AM
#define NUMBEROFINTERRUPTS  57                    //SIZE of interruptvectorarray
#else
#define NUMBEROFINTERRUPTS  1
#endif

#define INVALID_CLASS_ID 0xff
#define INVALID_METHOD_ID 0xff
#define INVALID_FIELD_ID 0xff

//#ifdef AVR8
//#define STRNCMP strncmpRamFlash
//#else
#define STRNCMP stringsNotEquals
//#endif

#include "debug.h"
//end of file
#endif
