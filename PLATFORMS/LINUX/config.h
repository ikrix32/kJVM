//
//  config.h
//  kJVM
//
//  Created by Florin Cristian on 15/02/15.
//  Copyright (c) 2015 Florin Cristian. All rights reserved.
//

#ifndef kJVM_config_h
#define kJVM_config_h

#define LINUX 1 //krix
#define __i386__ 1
#undef DEBUG

#define DEBUG_VM
//#define DEBUG_CLASS_LOADING

#define USE_MICROKERNEL

#define ENABLE_KCLASS_FORMAT
//#define ENABLE_KMETHOD
#define DEBUG_KCLASS//native methods don't work without this

#define ENABLE_TESTS

#ifdef NRF51
//#define BLE_CONSOLE

//#define TINYBAJOS_MULTITASKING

#define exit(...)
#ifdef BLE_CONSOLE
#define printf(...) ble_print(__VA_ARGS__)
#endif
#endif

#endif//kJVM_config_h
