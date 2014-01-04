//
//  StartScene.h
//  fruitCrush
//
//  Created by Tsai Empire on 9/29/13.
//
//

#ifndef __fruitCrush__StartScene__
#define __fruitCrush__StartScene__

#include "cocos2d.h"
USING_NS_CC;

class StartScene : public CCLayer
{
private:
    enum TagForStartButtons
    {
        kTagPlay = 50,
        kTagOption,
        kTagSocial
    };
    CCSprite* startBG;
    CCSprite* logo;
    CCMenu* mainMenu;
    void menubuttonPressedCB(CCMenuItem* menuItem);
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    
    void update(float dt);
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(StartScene);
    virtual ~StartScene();
};

#endif /* defined(__fruitCrush__StartScene__) */
