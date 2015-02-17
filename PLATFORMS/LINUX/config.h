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
//#undef DEBUG

#ifdef NRF51
#define exit(x)
#endif

#define BOOTSTRAP_BINARIES

#endif
