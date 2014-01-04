//
//  GridBox.h
//  fruitCrush
//
//  Created by Tsai on 13-10-23.
//
//

#ifndef __fruitCrush__GridBox__
#define __fruitCrush__GridBox__

#include "cocos2d.h"
#include "GridTile.h"
USING_NS_CC;
class GridBox : public cocos2d::CCNode
{
    CC_SYNTHESIZE(bool, _lock, Lock);
    CC_SYNTHESIZE_RETAIN(CCLayer*, _layer, Layer);
    CC_SYNTHESIZE_RETAIN(CCArray*, _gridTiles, GridTiles);
    CC_SYNTHESIZE_RETAIN(CCArray*, _readyForRemoveTiles, ReadyForRemoveTiles);
    //CC_SYNTHESIZE_RETAIN(GridTile*, _firstMatchedTile, FirstMatchedTile);
    //CC_SYNTHESIZE_RETAIN(GridTile*, _secondMatchedTile, SecondMatchedTile);
    CC_SYNTHESIZE_RETAIN(CCArray*, _promptTiles, PromptTiles);
private:
    CCSize _boxSize;
    //CCArray* _readyForRemoveTiles;
    GridTile* _outTile;
public:
    CCSpriteBatchNode* m_tileBgBatch;
    

public:
    //GridBox(int mapNo);
    //virtual ~GridBox();
    virtual bool init();
    bool initWithMap(int mapNo);
    
    GridTile* getTileAt(int row, int column);
    void changeTileTypeAt(TypeTile type, int row, int column);
    
    // Reset all tiles tag for the trans mark
    void resetTilesTag();
    
    virtual ~GridBox();

    CREATE_FUNC(GridBox);
    
private:
    void _addTileWithType(TypeTile type, int row, int column);
    void _initUI();

};

#endif /* defined(__fruitCrush__GridBox__) */
