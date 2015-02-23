//
//  kJVM Class Loader
//
//  Created by Florin Cristian on 15/02/15.
//  Copyright (c) 2015 Florin Cristian. All rights reserved.
//

#ifndef __KJVM_CLASSLOADER__
#define __KJVM_CLASSLOADER__
void classLoader_init(void);
u1 classLoader_loadClass(const u1* bin,const u4 binSize);
void classLoader_clinitClass(const u1 classId);
//todo - unload all and unload to bootstrap
#endif
