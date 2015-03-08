#ifdef USE_MICROKERNEL

static const u1 java_lang_ObjectBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Object_k.h"
#else
#include "java_lang_Object.h"
#endif

static const u1 platform_PlatFormBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "platform_PlatForm_k.h"
#else
#include "platform_PlatForm.h"
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

static const u1 java_lang_IllegalMonitorStateExceptionBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_IllegalMonitorStateException_k.h"
#else
#include "java_lang_IllegalMonitorStateException.h"
#endif

static const u1 java_lang_ThreadBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Thread_k.h"
#else
#include "java_lang_Thread.h"
#endif

static const u1 java_lang_RuntimeBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Runtime_k.h"
#else
#include "java_lang_Runtime.h"
#endif

static const u1 java_lang_MathBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_lang_Math_k.h"
#else
#include "java_lang_Math.h"
#endif

static const u1 java_io_InputStreamBin[] =
#ifdef ENABLE_KCLASS_FORMAT
#include "java_io_InputStream_k.h"
#else
#include "java_io_InputStream.h"
#endif

static const u1 java_io_PrintStreamBin[] =
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

static const u1* microkernelBinaries[] =
{	java_lang_ObjectBin,
	platform_PlatFormBin,
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
	java_lang_IllegalMonitorStateExceptionBin,
	java_lang_ThreadBin,
	java_lang_RuntimeBin,
	java_lang_MathBin,
	java_io_InputStreamBin,
	java_io_PrintStreamBin,
	java_lang_SystemBin,
};

static const u4 microKernelBinariesSize[] =
{	sizeof(java_lang_ObjectBin),
	sizeof(platform_PlatFormBin),
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
	sizeof(java_lang_IllegalMonitorStateExceptionBin),
	sizeof(java_lang_ThreadBin),
	sizeof(java_lang_RuntimeBin),
	sizeof(java_lang_MathBin),
	sizeof(java_io_InputStreamBin),
	sizeof(java_io_PrintStreamBin),
	sizeof(java_lang_SystemBin),
};

#ifdef DEBUG_KCLASS
static const char* microkernelClassNames[] =
{	"java/lang/Object",
	"platform/PlatForm",
	"java/lang/Boolean",
	"java/lang/Byte",
	"java/lang/Character",
	"java/lang/Short",
	"java/lang/Integer",
	"java/lang/Float",
	"java/lang/CharSequence",
	"java/lang/String",
	"java/lang/StringBuffer",
	"java/lang/StringBuilder",
	"java/lang/Error",
	"java/lang/Throwable",
	"java/lang/Exception",
	"java/lang/NullPointerException",
	"java/lang/ArithmeticException",
	"java/lang/ArrayIndexOutOfBoundsException",
	"java/lang/RuntimeException",
	"java/lang/IllegalArgumentException",
	"java/lang/ClassCastException",
	"java/lang/InterruptedException",
	"java/lang/IllegalMonitorStateException",
	"java/lang/Thread",
	"java/lang/Runtime",
	"java/lang/Math",
	"java/io/InputStream",
	"java/io/PrintStream",
	"java/lang/System",
};

extern const char* getMicroKernelClassName(const u2 classId){
	return microkernelClassNames[classId];
}
#endif

#ifdef ENABLE_KCLASS_FORMAT
extern const u2 JAVA_LANG_OBJECT_CLASS_ID(){ return 0; }
extern const u2 JAVA_LANG_STRING_CLASS_ID(){ return 9; }
#endif

extern const int getNoMicroKernelClasses(){
	return sizeof(microKernelBinariesSize) / sizeof(microKernelBinariesSize[0]);
}

extern const u1* getMicroKernelBinary(const int i){
	return microkernelBinaries[i];
}

extern const u4 getMicroKernelBinarySize(const int i){
	return microKernelBinariesSize[i];
}

#ifdef ENABLE_KCLASS_FORMAT
static const u2 microkernelExceptions[] =
{	17,//java.lang.ArrayIndexOutOfBoundsException
	INVALID_CLASS_ID,//java.lang.NegativeArraySizeException
	15,//java.lang.NullPointerException
	16,//java.lang.ArithmeticException
	20,//java.lang.ClassCastException
	22,//java.lang.IllegalMonitorStateException
};

extern u2 getMicroKernelExceptionClassId(const int exception){
	return microkernelExceptions[exception];
}
#endif

#endif
