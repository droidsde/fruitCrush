//
//  GridItem.h
//  fruitCrush
//
//  Created by Tsai on 13-10-31.
//
//

#ifndef __fruitCrush__GridItem__
#define __fruitCrush__GridItem__

#include "cocos2d.h"
#include "FruitCrush.h"
USING_NS_CC;
class GridItem : public CCNode
{
    CC_SYNTHESIZE_RETAIN(CCSprite*, _itemBG, ItemBG);
    CC_SYNTHESIZE(ClassItemSprite, _itemClass, ItemClass);
    CC_SYNTHESIZE(int, _itemValue, ItemValue);
    CC_SYNTHESIZE(bool, _itemAct, ItemAct); // 当前Item是否可活动
public:
    virtual bool init();
    CREATE_FUNC(GridItem);
    virtual ~GridItem();
};
#endif /* defined(__fruitCrush__GridItem__) */
