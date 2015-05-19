#ifndef MICROKERNEL_HEARDER 
#define MICROKERNEL_HEARDER 

#include "kvm_type_definitions.h" 


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
