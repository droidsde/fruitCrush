//
//  GridTile.h
//  fruitCrush
//
//  Created by Tsai on 13-10-23.
//
//

#ifndef __fruitCrush__GridTile__
#define __fruitCrush__GridTile__

#include "cocos2d.h"
#include "FruitCrush.h"
#include "GridItem.h"
USING_NS_CC;
class GridTile : public cocos2d::CCNode
{
    //CC_SYNTHESIZE_RETAIN(CCSprite*, _tileSprite, TileSprite);
    CC_SYNTHESIZE_RETAIN(GridItem*, _fruitItem, FruitItem);
    CC_SYNTHESIZE(bool, _canPutFruit, CanPutFruit);
    //CC_SYNTHESIZE(int, _fruitValue, FruitValue);
    //CC_SYNTHESIZE_RETAIN(CCSprite*, _fruitBg, FruitBg);
    CC_SYNTHESIZE(int, _tileType, TileType);
    CC_SYNTHESIZE(int, _xIndex, XIndex);
    CC_SYNTHESIZE(int, _yIndex, YIndex);
    
    CC_SYNTHESIZE_RETAIN(CCDictionary*, _tileSprites, TileSprites);
private:
    

public:
    //GridTile(int rowIndex, int columnIndex);
    //virtual ~GridTile();
    virtual bool init();
    virtual bool initWithType(int type, CCSpriteBatchNode* batchNode, const CCPoint& pos);
    virtual void changeType(int type);
    virtual void removeType(CCSpriteBatchNode* batchNode, TypeTile type);
    
    bool isNearWith(GridTile* tile);
    bool transWith(GridTile* tile);
    void stopItemActionByTag(int tag);
    
    static CCSpriteBatchNode* tileBgBatchSingleton();
    
    CREATE_FUNC(GridTile);
    virtual ~GridTile();
};

#endif /* defined(__fruitCrush__GridTile__) */
