//
//  SecondLevelView.h
//  fruitCrush
//
//  Created by Empire Tsai on 14-1-3.
//
//

#ifndef __fruitCrush__SecondLevelView__
#define __fruitCrush__SecondLevelView__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BaseScrollView.h"

USING_NS_CC;
USING_NS_CC_EXT;

class SecondLevelView : public BaseScrollView {
public:
    static CCScene* scene();
    static CCScene* sceneWithTheme(int themeNo);
    virtual bool init();
    CREATE_FUNC(SecondLevelView);
    
    virtual void touchEventAction(LsTouch* touch, int type);
    
    CCLayer* getContainLayer();
    
    CCNode* getSpriteByLevel(int level);
    
    void loadWiththeme(int themeNo);
    CCLayer* getContainLayerWithTheme(int themeNo);
    
};

#endif /* defined(__fruitCrush__SecondLevelView__) */
