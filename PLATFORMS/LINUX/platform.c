#include <stdio.h>
#include <stdlib.h>
#ifndef NRF51
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
#include "definitions.h"
#include "kjvm.h"
#include "classfile.h"
#include "heap.h"
#include "platform.h"

const u1 java_lang_ObjectBin[] =
#include "java_lang_Object.h"

const u1 platformBin[] =
#include "java_PlatForm.h"

const u1 java_lang_BooleanBin[] =
#include "java_lang_Boolean.h"

const u1 java_lang_ByteBin[] =
#include "java_lang_Byte.h"

const u1 java_lang_CharacterBin[] =
#include "java_lang_Character.h"

const u1 java_lang_ShortBin[] =
#include "java_lang_Short.h"

const u1 java_lang_IntegerBin[] =
#include "java_lang_Integer.h"

const u1 java_lang_FloatBin[] =
#include "java_lang_Float.h"

const u1 java_lang_CharSequenceBin[] =
#include "java_lang_CharSequence.h"

const u1 java_lang_StringBin[] =
#include "java_lang_String.h"

const u1 java_lang_StringBufferBin[] =
#include "java_lang_StringBuffer.h"

const u1 java_lang_StringBuilderBin[] =
#include "java_lang_StringBuilder.h"

const u1 java_lang_ErrorBin[] =
#include "java_lang_Error.h"

const u1 java_lang_ThrowableBin[] =
#include "java_lang_Throwable.h"

const u1 java_lang_ExceptionBin[] =
#include "java_lang_Exception.h"

const u1 java_lang_NullPointerExceptionBin[] =
#include "java_lang_NullPointerException.h"

const u1 java_lang_ArithmeticExceptionBin[] =
#include "java_lang_ArithmeticException.h"

const u1 java_lang_ArrayIndexOutOfBoundsExceptionBin[] =
#include "java_lang_ArrayIndexOutOfBoundsException.h"

const u1 java_lang_RuntimeExceptionBin[] =
#include "java_lang_RuntimeException.h"

const u1 java_lang_IllegalArgumentExceptionBin[] =
#include "java_lang_IllegalArgumentException.h"

const u1 java_lang_ClassCastExceptionBin[] =
#include "java_lang_ClassCastException.h"

const u1 java_lang_InterruptedExceptionBin[] =
#include "java_lang_InterruptedException.h"

const u1 java_lang_RuntimeBin[] =
#include "java_lang_Runtime.h"

const u1 java_lang_ThreadBin[] =
#include "java_lang_Thread.h"

const u1 java_lang_mathBin[] =
#include "java_lang_Math.h"

const u1 java_io_InStreamBin[] =
#include "java_io_InStream.h"

const u1 java_io_OutStreamBin[] =
#include "java_io_OutStream.h"

const u1 java_lang_SystemBin[] =
#include "java_lang_System.h"

const u1* bootStrapBinaries[] =
{   java_lang_ObjectBin,
    platformBin,
    java_lang_BooleanBin,
    java_lang_ByteBin,
    java_lang_CharacterBin,
    java_lang_ShortBin,
    java_lang_IntegerBin,
    java_lang_FloatBin,
    java_lang_CharSequenceBin,
    java_lang_StringBin,
    java_lang_StringBufferBin,
    java_lang_StringBuilderBin,
    java_lang_ErrorBin,
    java_lang_ThrowableBin,
    java_lang_ExceptionBin,
    java_lang_NullPointerExceptionBin,
    java_lang_ArithmeticExceptionBin,
    java_lang_ArrayIndexOutOfBoundsExceptionBin,
    java_lang_RuntimeExceptionBin,
    java_lang_IllegalArgumentExceptionBin,
    java_lang_ClassCastExceptionBin,
    java_lang_InterruptedExceptionBin,
    java_lang_RuntimeBin,
    java_lang_ThreadBin,
    java_lang_mathBin,
    java_io_InStreamBin,
    java_io_OutStreamBin,
    java_lang_SystemBin
};

void initHW()
{
}


/* all class files stored for linux in DS (malloc)*/

void initVM(int argc, char* argv[])               /* read, analyze classfiles and fill structures*/
{
    u4 length;
    printf("Size of java obj %lu\n",sizeof(java_lang_ObjectBin));
#if (AVR32LINUX||LINUX)
    classFileBase=(char*)malloc((size_t) MAXBYTECODE);
    if (classFileBase==NULL)
        errorExit(-1,"malloc error while trying to allocate %d bytes for class files.\n", MAXBYTECODE);
#endif

    heapInit();                                   /* linux avr8 malloc , others hard coded!*/
    length = 0;
#if LINUX|| AVR32LINUX
    if (argc > MAXCLASSES)
        errorExit(-1,"ERROR: trying to load %d classes, MAXCLASSES is %d\n", argc, MAXCLASSES);
    for (cN=0; cN < argc; cN++)
    {
        cs[cN].classFileStartAddress = classFileBase + length;
        cs[cN].classFileLength=readClassFile(argv[cN + 1], cs[cN].classFileStartAddress);
        analyzeClass( &cs[cN]);
        length += cs[cN].classFileLength;
        if (length > MAXBYTECODE)
        {
            printf("MAXBYTECODE reached!\n"); exit(-1);
        }
        numClasses = cN + 1;
    }
#endif
    DEBUGPRINTHEAP;
}


u2 readClassFile(char* fileName, char* addr)
{
#if LINUX||AVR32LINUX
#ifndef NRF51
    int fd;
    u2 classFileLength=-(u2)((long)addr%(1<<16))-1;
    if ((fd=open(fileName,O_RDONLY))==-1)
        perror(fileName);
    while (read(fd,addr++,1));
    return classFileLength+=(long)addr;
#else
    return 0;
#endif
#endif
}


void linuxExit(int n)
{
#ifdef DEBUGOPSTACK
    printf(" maxOperandenStack: x%x",maxOpStack);
#endif                                        //DEBUOPSTACK
#ifdef DEBUGMETHODSTACK
    printf(" maxMethodStack: x%x\n",maxMethodStack);
#endif                                        //DEBUGMETHODSTACK
    exit(n);
}
