#ifndef MICROKERNEL_HEARDER 
#define MICROKERNEL_HEARDER 

#include "kvm_type_definitions.h"
#include "micro_kernel.h"

#ifndef NRF51
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#endif

#include "definitions.h"
#include "stack.h"

extern u1 local;
// insert and update here arrays for classes with native methods
// array length ->  at least up to last native method < methods_count
// lock at methods in the *.java or *.class file in increasing order
// if method is non native -> insert NULL, otherwise pointer to nativce C-function
jchar platform_PlatForm_nativeCharIn0(){
#ifndef NRF51
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    opStackPush(toSlot((u4) ch));
    return 1;
#else
    return 0;
#endif
}

jvoid platform_PlatForm_nativeCharOut1(jchar param0){
    PRINTF("%c", param0);
}

jvoid platform_PlatForm_exit2(jint param0){
    exit(param0);
}

jint platform_PlatForm_currentTimeMillis3(){
#ifndef NRF51
    struct timeval timerstart;
    gettimeofday(&timerstart, NULL);
    opStackPush(toSlot((u4)((timerstart.tv_sec * 1000 + timerstart.tv_usec / 1000) & 0x7FFFFFFF)));
    return 1;
#else
    return 1;
#endif
}



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
