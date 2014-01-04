//
//  ResUtil.h
//  fruitCrush
//
//  Created by Empire Tsai on 13-12-25.
//
//

#ifndef __fruitCrush__ResUtil__
#define __fruitCrush__ResUtil__

#include "cocos2d.h"
#include <string>
#include <stdarg.h>
USING_NS_CC;

class ResUtil
{
public:
    static CCArray* componentsSeparatedByString(CCString* str, const char *delimiter);
};

#endif /* defined(__fruitCrush__ResUtil__) */
