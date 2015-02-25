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
#include "classloader.h"

#ifdef NRF51
#include <stdbool.h>

#include "nrf_gpio.h"
#include "boards.h"

#include "retarget.h"

void setPin(int pin,bool on){
    if(on){
        NRF_GPIO->OUTSET = 1 << pin;
    }else{
        NRF_GPIO->OUTCLR = 1<< pin;
    }
}
#endif



static const u1 helloWorldBin[] =
#include "HelloWorld.h"

static const u1 arithmeticBin[] =
#include "Arithmetic.h"

static const u1 ICMPBin[] =
#include "ICMP.h"

static const u1 fibonacciBin[] =
#include "Fibonacci.h"

static const u1 selfInstanceBin[] =
#include "SelfInstance.h"

static const u1 staticTestBin[] =
#include "StaticTest.h"

static const u1 stringAndHeapTestBin[] =
#include "StringAndHeapTest.h"

static const u1 switchTestBin[] =
#include "Switch.h"

static const u1 arraySizeTestBin[] =
#include "ArraySize.h"

static const u1 inheritanceABin[]=
#include "InheritanceA.h"

static const u1 inheritanceBBin[]=
#include "InheritanceB.h"

static const u1 inheritanceBin[]=
#include "Inheritance.h"

static const u1 floatTestBin[]=
#include "FloatTest.h"

static const u1 quickSortBin[]=
#include "QuickSort.h"

static const u1 erathostenesBin[]=
#include "Erathostenes.h"

static const u1 divizionbyZeroBin[]=
#include "DivByZero.h"

/*static const u1 thread1Bin[]=
#include "ThreadTest$1.h"

static const u1 threadBin[]=
#include "ThreadTest.h"
 */

static const u1* testBinaries[] =
{
    helloWorldBin,
    arithmeticBin,
    ICMPBin,
    fibonacciBin,
    selfInstanceBin,
    staticTestBin,
    stringAndHeapTestBin,
    switchTestBin,
    arraySizeTestBin,
    inheritanceABin,
    inheritanceBBin,
    inheritanceBin,
    floatTestBin,
    quickSortBin,
    erathostenesBin,
    divizionbyZeroBin,
    //thread1Bin,
    //threadBin,
};

static const u4 testBinariesSize[] =
{
    sizeof(helloWorldBin),
    sizeof(arithmeticBin),
    sizeof(ICMPBin),
    sizeof(fibonacciBin),
    sizeof(selfInstanceBin),
    sizeof(staticTestBin),
    sizeof(stringAndHeapTestBin),
    sizeof(switchTestBin),
    sizeof(arraySizeTestBin),
    sizeof(inheritanceABin),
    sizeof(inheritanceBBin),
    sizeof(inheritanceBin),
    sizeof(floatTestBin),
    sizeof(quickSortBin),
    sizeof(erathostenesBin),
    sizeof(divizionbyZeroBin),
    //sizeof(thread1Bin),
    //sizeof(threadBin),
};

static const char* testNames[] =
{
    "HelloWorld",
    "Arithmetic",
    "ICMP",
    "Fibonacci",
    "SelfInstance",
    "StaticTest",
    "StringAndHeap",
    "SwitchTest",
    "ArraySizeTest",
    "InheritanceA",
    "InheritanceB",
    "InheritanceTest",
    "FloatTest",
    "QuickSort",
    "Erathostenes",
    "DivByZero",
    //"Thread1",
    //"Thread",
};

#ifdef TINYBAJOS
void main() __attribute__ ((noreturn));
void main()
{
#else
    int main(int argc, char** argv)
    {
#endif

#ifdef NRF51
        LEDS_CONFIGURE(0xFFFFFFFF);
        setPin(8,true);
#endif

#ifdef BLE_CONSOLE
        init_ble_console();
        while(!is_ble_console_connected())
        {
            //power_manage();
        }
        nrf_delay_ms(1000);
        PRINTF("BLE console connected\n");
        nrf_delay_ms(1000);
#endif

        vm_init();

        const int noTests = sizeof(testBinariesSize) / sizeof(testBinariesSize[0]);

        for (int i = 0; i < noTests; i++)
        {
            printf("\n========== Start Test %s (id: %d) =========\n",testNames[i],i);
            const u1 classId = classLoader_loadClass(testBinaries[i], testBinariesSize[i]);

            classLoader_clinitClass(classId);

            vm_run(classId);//run main on last loaded class
            printf("\n========== End Test %s =========\n",testNames[i]);
        }

#ifdef NRF51
#ifdef BLE_CONSOLE
        nrf_delay_ms(10000);
        //while(true){}
#endif
        setPin(8,false);
#endif
        //ERROREXIT(0, "\nno main found %d", numClasses);
#ifndef TINYBAJOS
        return 1;
#endif
    }

