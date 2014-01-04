//
//  ActionEffectHub.h
//  fruitCrush
//
//  Created by Tsai Empire on 11/8/13.
//
//

#ifndef __fruitCrush__ActionEffectHub__
#define __fruitCrush__ActionEffectHub__

#include "cocos2d.h"
USING_NS_CC;
class ActionEffectHub : public CCObject
{
public:
    static ActionEffectHub* shareInstance();
    CCAction* playSlink();
    CCFiniteTimeAction* scoreFly();
    CCAction* removeItem( SEL_CallFuncN beforeRmCB,  SEL_CallFuncN afterRmCB);
};

#endif /* defined(__fruitCrush__ActionEffectHub__) */
