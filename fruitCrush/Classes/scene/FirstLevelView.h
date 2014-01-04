//
//  FirstLevelView.h
//  fruitCrush
//
//  Created by Empire Tsai on 14-1-3.
//
//

#ifndef __fruitCrush__FirstLevelView__
#define __fruitCrush__FirstLevelView__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BaseScrollView.h"

class FirstLevelView : public BaseScrollView {
public:
    virtual bool init();
    CREATE_FUNC(FirstLevelView);
    virtual CCLayer* getContainLayer();
    virtual CCNode* getSpriteByLevel(int level);
    
};

#endif /* defined(__fruitCrush__FirstLevelView__) */
