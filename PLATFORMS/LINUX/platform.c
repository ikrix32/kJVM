#include <stdio.h>
//#include <stdlib.h>
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
#include "classloader.h"

#ifdef BOOTSTRAP_BINARIES

#ifdef ENABLE_KCLASS_FORMAT
static const char* microkernel[] =
#include "mikrokernel_dbg.h"
#endif

static const u1 java_lang_ObjectBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Object_k.h"
#else
#include "java_lang_Object.h"
#endif

static const u1 platformBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_PlatForm_k.h"
#else
#include "java_PlatForm.h"
#endif

static const u1 java_lang_BooleanBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Boolean_k.h"
#else
#include "java_lang_Boolean.h"
#endif

static const u1 java_lang_ByteBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Byte_k.h"
#else
#include "java_lang_Byte.h"
#endif

static const u1 java_lang_CharacterBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Character_k.h"
#else
#include "java_lang_Character.h"
#endif

static const u1 java_lang_ShortBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Short_k.h"
#else
#include "java_lang_Short.h"
#endif

static const u1 java_lang_IntegerBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Integer_k.h"
#else
#include "java_lang_Integer.h"
#endif

static const u1 java_lang_FloatBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Float_k.h"
#else
#include "java_lang_Float.h"
#endif

static const u1 java_lang_CharSequenceBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_CharSequence_k.h"
#else
#include "java_lang_CharSequence.h"
#endif

static const u1 java_lang_StringBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_String_k.h"
#else
#include "java_lang_String.h"
#endif

static const u1 java_lang_StringBufferBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_StringBuffer_k.h"
#else
#include "java_lang_StringBuffer.h"
#endif

static const u1 java_lang_StringBuilderBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_StringBuilder_k.h"
#else
#include "java_lang_StringBuilder.h"
#endif

static const u1 java_lang_ErrorBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Error_k.h"
#else
#include "java_lang_Error.h"
#endif

static const u1 java_lang_ThrowableBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Throwable_k.h"
#else
#include "java_lang_Throwable.h"
#endif

static const u1 java_lang_ExceptionBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Exception_k.h"
#else
#include "java_lang_Exception.h"
#endif

static const u1 java_lang_NullPointerExceptionBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_NullPointerException_k.h"
#else
#include "java_lang_NullPointerException.h"
#endif

static const u1 java_lang_ArithmeticExceptionBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_ArithmeticException_k.h"
#else
#include "java_lang_ArithmeticException.h"
#endif

static const u1 java_lang_ArrayIndexOutOfBoundsExceptionBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_ArrayIndexOutOfBoundsException_k.h"
#else
#include "java_lang_ArrayIndexOutOfBoundsException.h"
#endif

static const u1 java_lang_RuntimeExceptionBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_RuntimeException_k.h"
#else
#include "java_lang_RuntimeException.h"
#endif

static const u1 java_lang_IllegalArgumentExceptionBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_IllegalArgumentException_k.h"
#else
#include "java_lang_IllegalArgumentException.h"
#endif

static const u1 java_lang_ClassCastExceptionBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_ClassCastException_k.h"
#else
#include "java_lang_ClassCastException.h"
#endif

static const u1 java_lang_InterruptedExceptionBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_InterruptedException_k.h"
#else
#include "java_lang_InterruptedException.h"
#endif

static const u1 java_lang_RuntimeBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Runtime_k.h"
#else
#include "java_lang_Runtime.h"
#endif

static const u1 java_lang_ThreadBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Thread_k.h"
#else
#include "java_lang_Thread.h"
#endif

static const u1 java_lang_mathBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Math_k.h"
#else
#include "java_lang_Math.h"
#endif

static const u1 java_io_InStreamBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_io_InputStream_k.h"
#else
#include "java_io_InputStream.h"
#endif

static const u1 java_io_OutStreamBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_io_PrintStream_k.h"
#else
#include "java_io_PrintStream.h"
#endif

static const u1 java_lang_SystemBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_System_k.h"
#else
#include "java_lang_System.h"
#endif

static const u1* bootstrapBinaries[] =
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
    java_lang_SystemBin,
};

static const u4 bootstrapBinariesSize[] =
{   sizeof(java_lang_ObjectBin),
    sizeof(platformBin),
    sizeof(java_lang_BooleanBin),
    sizeof(java_lang_ByteBin),
    sizeof(java_lang_CharacterBin),
    sizeof(java_lang_ShortBin),
    sizeof(java_lang_IntegerBin),
    sizeof(java_lang_FloatBin),
    sizeof(java_lang_CharSequenceBin),
    sizeof(java_lang_StringBin),
    sizeof(java_lang_StringBufferBin),
    sizeof(java_lang_StringBuilderBin),
    sizeof(java_lang_ErrorBin),
    sizeof(java_lang_ThrowableBin),
    sizeof(java_lang_ExceptionBin),
    sizeof(java_lang_NullPointerExceptionBin),
    sizeof(java_lang_ArithmeticExceptionBin),
    sizeof(java_lang_ArrayIndexOutOfBoundsExceptionBin),
    sizeof(java_lang_RuntimeExceptionBin),
    sizeof(java_lang_IllegalArgumentExceptionBin),
    sizeof(java_lang_ClassCastExceptionBin),
    sizeof(java_lang_InterruptedExceptionBin),
    sizeof(java_lang_RuntimeBin),
    sizeof(java_lang_ThreadBin),
    sizeof(java_lang_mathBin),
    sizeof(java_io_InStreamBin),
    sizeof(java_io_OutStreamBin),
    sizeof(java_lang_SystemBin),
};
#endif

void initHW()
{
}

void loadBootstrapClasses(){
#ifdef BOOTSTRAP_BINARIES
    const int noBins = sizeof(bootstrapBinariesSize)/sizeof(bootstrapBinariesSize[0]);
    for (int i = 0; i < noBins; i++)
        classLoader_loadClass(bootstrapBinaries[i], bootstrapBinariesSize[i]);

    for (int i = 0; i < numClasses; i++)
        classLoader_clinitClass(i);
#endif
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

u2 readClassBin(const u1* bin,const u4 binSize, char* addr)
{
#if LINUX||AVR32LINUX
    u2 classFileLength=-(u2)((long)addr%(1<<16))-1;
    for (int i = 0; i < binSize; i++) {
        addr[0] = (char)bin[0];
        addr++;bin++;
    }
    return classFileLength += (long)addr;
#else
    return 0;
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
