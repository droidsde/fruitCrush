//
//  ResCenter.h
//  fruitCrush
//
//  Created by Tsai on 13-11-17.
//
//

#ifndef __fruitCrush__ResCenter__
#define __fruitCrush__ResCenter__

#include "cocos2d.h"
USING_NS_CC;

// particle
static const char p_jellyBreak[]         = "jellyBreak.plist";

class ResCenter : public cocos2d::CCObject
{
    
public:
    enum typeSprite{
        kSpriteStartBG = 1,
        kSpriteLevelScene,
        kSpriteGameLayerScene,
        kSpritePlay,
        kSpriteOption,
        kSpriteScoreFont
    };
    
    enum typeLableAtlas{
        klableScore = 1
    };
private:
    //CCSprite* _startBG;
    //CCSprite* _levelSelectSceneBG;
    //CCSprite* _gameSceneBG;
    CCSprite* _play;
    CCSprite* _option;
    CCLabelAtlas* _scoreFont;
public:
    static ResCenter* shareInstance();
    CCSprite* getSpriteByType(typeSprite);
    CCLabelAtlas* getLableAtlasByType(typeLableAtlas);
    
    
};

#endif /* defined(__fruitCrush__ResCenter__) */
