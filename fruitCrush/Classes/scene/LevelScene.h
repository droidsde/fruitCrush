//
//  LevelScene.h
//  fruitCrush
//
//  Created by Tsai Empire on 9/29/13.
//
//

#ifndef __fruitCrush__LevelScene__
#define __fruitCrush__LevelScene__

#include "cocos2d.h"

USING_NS_CC;

class LevelScene : public CCLayer
{
private:
    
    CCSprite* _levelBG;
    CCSprite* _lifeCount;
    CCSprite* _backSprite;
    CCMenu* _mainMenu;
    //CCSize _winSize;
    //void buttonPressedCB(CCMenuItem* menuItem);
    
    //CC_SYNTHESIZE_RETAIN(CCScrollView*, m_pScrollView, ScrollView);
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    
    CCSprite* addMapBtnWith(int mapNo, CCPoint pos, CCSpriteBatchNode* batch);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(LevelScene);
    //virtual void touchEventAction(LsTouch* touch, int type);
    
    virtual ~LevelScene();
};

#endif /* defined(__fruitCrush__LevelScene__) */
