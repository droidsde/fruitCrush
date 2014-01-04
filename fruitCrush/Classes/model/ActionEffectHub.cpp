//
//  ActionEffectHub.cpp
//  fruitCrush
//
//  Created by Tsai Empire on 11/8/13.
//
//

#include "ActionEffectHub.h"
#include "FruitCrush.h"
static ActionEffectHub* _instance=NULL;
ActionEffectHub* ActionEffectHub::shareInstance()
{
    if (_instance == NULL) {
        _instance = new ActionEffectHub();
    }
    return _instance;
}

CCAction* ActionEffectHub::playSlink()
{
    CCScaleBy* popScale1 = CCScaleBy::create(0.5f, 1.1f);
    CCScaleBy* popScale2 = CCScaleBy::create(0.5f, 1.0f/1.1f);
    
    CCEaseSineInOut* easePop1 = CCEaseSineInOut::create(popScale1);
    CCEaseSineInOut* easePop2 = CCEaseSineInOut::create(popScale2);
    CCSequence* scaleSeq = CCSequence::create(easePop1, easePop2, NULL);
    
    CCRepeat* repeatS = CCRepeat::create(scaleSeq, 3);
    CCRepeatForever* repeatScale = CCRepeatForever::create(CCSequence::create(CCDelayTime::create(1.0f),repeatS,NULL));
    repeatScale->setTag(kPromptMatchTag);
    return repeatScale;
}

CCFiniteTimeAction* ActionEffectHub::scoreFly()
{
    CCMoveTo* movTosc = CCMoveTo::create(kScorePromptDuration, ccp(kTileWidth*3, FruitCrush::shareSingleton()->getWinSize().height - kTileWidth*3));
    CCScaleTo* sca = CCScaleTo::create(kScorePromptDuration, 0.5f);
    CCSpawn* scoreFly = CCSpawn::create(movTosc, sca, NULL);
    return scoreFly;
}

CCAction* ActionEffectHub::removeItem(SEL_CallFuncN beforeRmCB, SEL_CallFuncN afterRmCB)
{
    CCAction* removeAct = CCSequence::create(CCDelayTime::create(0.1f), CCCallFuncN::create(this,beforeRmCB),CCScaleTo::create(kDisapearTime, 0.0f), CCCallFuncN::create(this,afterRmCB), NULL);
    return removeAct;
}