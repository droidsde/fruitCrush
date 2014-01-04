//
//  GameParticle.h
//  fruitCrush
//
//  Created by Tsai Empire on 11/6/13.
//
//

#ifndef __fruitCrush__GameParticle__
#define __fruitCrush__GameParticle__

#include "cocos2d.h"
USING_NS_CC;

class GameParticle : public cocos2d::CCObject
{
private:
    
public:
    static GameParticle* shareInstance();
    void playSnowDown(CCLayer* layer, CCPoint ppoint);
    void playRowRainbow();
    void playJellyBreak(CCLayer* layer, CCPoint ppoint);
};
#endif /* defined(__fruitCrush__GameParticle__) */
