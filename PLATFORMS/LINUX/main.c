#include <stdio.h>
//#include <stdlib.h>
/*#ifndef NRF51
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif*/
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

#ifdef ENABLE_TESTS
#include "KVMTestPackage.h"
#endif

extern const char* getClassName(const u2 classId){
#ifdef USE_MICROKERNEL
    extern const int getNoMicroKernelClasses();
    const int  noMicroClasses = getNoMicroKernelClasses();
    if(classId < noMicroClasses)
    {
#ifdef DEBUG_KCLASS
        extern const char* getMicroKernelClassName(const u2 classId);
        return getMicroKernelClassName(classId);
#else
        return "kernelclass";
#endif
    }
#endif
    extern const char* testNames[];
    return testNames[classId - noMicroClasses];
}

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

#ifdef ENABLE_TESTS
        const int noTests = sizeof(testBinariesSize) / sizeof(testBinariesSize[0]);

        for (int i = 0; i < noTests; i++)
        {
            printf("\n========== Start %s (id: %d) =========\n",testNames[i],i);
            const u1 classId = classLoader_loadClass(testBinaries[i], testBinariesSize[i]);

            classLoader_clinitClass(classId);

            if(vm_run(classId) == 0)//run main on last loaded class
                unloadLastClass();
            printf("\n========== End %s =========\n",testNames[i]);
            printf("Heap free space %d\n",getHeapFreeSpace());
        }
#endif
				
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

