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

#define BOOTSTRAP_BINARIES

//#define USE_STACK_MACROS

#ifdef NRF51
#define BLE_CONSOLE

#define exit(...)
#ifdef BLE_CONSOLE
#define printf(...) ble_print(__VA_ARGS__)
#endif
#endif

#endif//kJVM_config_h
