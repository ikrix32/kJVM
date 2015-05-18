#ifndef MICROKERNEL_HEARDER 
#define MICROKERNEL_HEARDER 

#include "kvm_type_definitions.h"
#include "micro_kernel.h" 


jvoid tests_NativeMethodsTest_nativeMethod3(jint param0);
jvoid tests_NativeMethodsTest_nativeMethod4(jfloat param0);
jvoid tests_NativeMethodsTest_nativeMethod5(jbyteArray param0);
jvoid tests_NativeMethodsTest_nativeMethod6(jcharArray param0);
jvoid tests_NativeMethodsTest_nativeMethod7(jintArray param0);
jvoid tests_NativeMethodsTest_nativeMethod8(jfloatArray param0);
jvoid tests_NativeMethodsTest_nativeMethod9();

jint tests_NativeMethodsTest_nativeMethod1(jbyte param0){
    printf("nativeMethod(byte:%d)\n",param0);
    return 10;
}

jcharArray tests_NativeMethodsTest_nativeMethod2(jchar param0){
    printf("nativeMethod(char:%c)\n",param0);
    jcharArray text;
    text.length = 3;
    text.values = malloc(text.length * sizeof(jchar));
    text.values[0] = 'S';
    text.values[1] = 'T';
    text.values[2] = 'F';

    return text;
}

jvoid tests_NativeMethodsTest_nativeMethod3(jint param0){
    printf("nativeMethod(int:%d)\n",param0);
}

jvoid tests_NativeMethodsTest_nativeMethod4(jfloat param0){
    printf("nativeMethod(float:%f)\n",param0);
}

jvoid tests_NativeMethodsTest_nativeMethod5(jbyteArray param0){
    printf("nativeMethod(byte[%d]:",param0.length);
    for(int i = 0; i < param0.length;i++){
        printf("%d,",param0.values[i]);
        param0.values[i]++;
    }
    printf(")\n");
}

jvoid tests_NativeMethodsTest_nativeMethod6(jcharArray param0){
    printf("nativeMethod(char[%d]:",param0.length);
    for(int i = 0; i < param0.length;i++){
        printf("%c,",param0.values[i]);
    }
    printf(")\n");
}

jvoid tests_NativeMethodsTest_nativeMethod7(jintArray param0){
    printf("nativeMethod(int[%d]:",param0.length);
    for(int i = 0; i < param0.length;i++){
        printf("%d,",param0.values[i]);
    }
    printf(")\n");
}

jvoid tests_NativeMethodsTest_nativeMethod8(jfloatArray param0){
    printf("nativeMethod(float[%d]:",param0.length);
    for(int i = 0; i < param0.length;i++){
        printf("%f,",param0.values[i]);
    }
    printf(")\n");
}

jvoid tests_NativeMethodsTest_nativeMethod9(){
    printf("Void method\n");
}

#endif
