//
//  native_dispach.h
//  kJVM
//
//  Created by Florin Cristian on 16/03/15.
//  Copyright (c) 2015 Florin Cristian. All rights reserved.
//

#ifndef kJVM_nativedispach_h
#define kJVM_nativedispach_h

#include "definitions.h"

u1 sizeOfType(char type,u1 isPointer);
u1 nativeDispath(const u1 isInvokeStatic,const u2 classId,const u2 methodId);
#endif
