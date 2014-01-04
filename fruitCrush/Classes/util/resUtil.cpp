//
//  ResUtil.cpp
//  fruitCrush
//
//  Created by Empire Tsai on 13-12-25.
//
//

#include "ResUtil.h"

CCArray* ResUtil::componentsSeparatedByString(CCString* cstr, const char *delimiter)
{
    CCArray* result = CCArray::create();
    
    int cutAt;
    std::string str=cstr->getCString();
    
    while( (cutAt = str.find_first_of(delimiter)) != str.npos )
    {
        if(cutAt > 0)
        {
            result->addObject(CCString::create(str.substr(0, cutAt)));
        }
        str = str.substr(cutAt + 1);
    }
    
    if(str.length() > 0)
    {
        result->addObject(CCString::create(str));
    }
    
    return result;
}