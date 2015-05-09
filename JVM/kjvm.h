//
//  kJVM
//
//  Created by Florin Cristian on 15/02/15.
//  Copyright (c) 2015 Florin Cristian. All rights reserved.
//

#ifndef __KJVM__
#define __KJVM__
#include "definitions.h"
#include "scheduler.h"

void vm_init(void);
s1 vm_run(const u1 classId);
 
#endif
