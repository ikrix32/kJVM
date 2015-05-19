#ifndef MICROKERNEL_HEARDER 
#define MICROKERNEL_HEARDER 

#include "kvm_type_definitions.h" 


kvm_internal jvoid java_lang_Object_notify0();

kvm_internal jvoid java_lang_Object_notifyAll1();

kvm_internal jvoid java_lang_Object_wait2();

kvm_internal jvoid java_lang_Object_waitTime3(jint param0);

 jchar platform_PlatForm_nativeCharIn0();

 jvoid platform_PlatForm_nativeCharOut1(jchar param0);

 jvoid platform_PlatForm_exit2(jint param0);

 jint platform_PlatForm_currentTimeMillis3();

kvm_internal jint java_lang_Float_floatToIntBits0(jfloat param0);

kvm_internal jfloat java_lang_Float_intBitsToFloat1(jint param0);

kvm_internal jint java_lang_String_nativeStringLength0();

kvm_internal jchar java_lang_String_nativeCharAt1(jint param0);

kvm_internal jvoid java_lang_Thread_start0();

kvm_internal jvoid java_lang_Thread_yield1();

kvm_internal jvoid java_lang_Thread_sleep2(jint param0);

kvm_internal jvoid java_lang_Thread_currentThread3();

kvm_internal jvoid java_lang_Thread_interrupt4();

kvm_internal jboolean java_lang_Thread_interrupted5();

kvm_internal jboolean java_lang_Thread_isInterrupted6();

kvm_internal jvoid java_lang_Thread_join7();

kvm_internal jint java_lang_Runtime_freeMemory3();

kvm_internal jint java_lang_Runtime_totalMemory4();

kvm_internal jvoid java_lang_Runtime_gc5();

 jint tests_NativeMethodsTest_nativeMethod1(jbyte param0);

 jcharArray tests_NativeMethodsTest_nativeMethod2(jchar param0);

 jvoid tests_NativeMethodsTest_nativeMethod3(jint param0);

 jvoid tests_NativeMethodsTest_nativeMethod4(jfloat param0);

 jvoid tests_NativeMethodsTest_nativeMethod5(jbyteArray param0);

 jvoid tests_NativeMethodsTest_nativeMethod6(jcharArray param0);

 jvoid tests_NativeMethodsTest_nativeMethod7(jintArray param0);

 jvoid tests_NativeMethodsTest_nativeMethod8(jfloatArray param0);

 jvoid tests_NativeMethodsTest_nativeMethod9();

 jintArray tests_NativeMethodsTest_stressTest10(jbyteArray param0,jcharArray param1,jintArray param2,jfloatArray param3);
#endif
