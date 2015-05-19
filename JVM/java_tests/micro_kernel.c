#ifndef MICROKERNEL_HEARDER 
#define MICROKERNEL_HEARDER 

#include "kvm_type_definitions.h"
#include "micro_kernel.h" 


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

jintArray tests_NativeMethodsTest_stressTest10(jbyteArray param0,jcharArray param1,jintArray param2,jfloatArray param3)
{
    printf("STRESS TEST Start\n");
    jintArray result;
    result.length = param0.length + param1.length + param2.length + param3.length;
    result.values = malloc(result.length * sizeof(jint));
    int n = 0;
    printf("\nbytes:");
    for(int i = 0; i < param0.length;i++){
        printf("%d,",param0.values[i]);
        result.values[n] = param0.values[i];
        n++;
        param0.values[i]++;
    }
    printf("\nchars:");
    for(int i = 0; i < param1.length;i++){
        printf("%c,",param1.values[i]);
        result.values[n] = param1.values[i];
        n++;
        param1.values[i]++;
    }
    printf("\nints:");
    for(int i = 0; i < param2.length;i++){
        printf("%d,",param2.values[i]);
        result.values[n] = param2.values[i];
        n++;
        param2.values[i]++;
    }
    printf("\nfloats:");
    for(int i = 0; i < param3.length;i++){
        printf("%f,",param3.values[i]);
        result.values[n] = param3.values[i];
        n++;
        param3.values[i]++;
    }

    printf("\nSTRESS TEST End\n");
    return result;
}

#endif
