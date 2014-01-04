//
//  GridBll.h
//  fruitCrush
//
//  Created by Tsai Empire on 10/29/13.
//
//

#ifndef __fruitCrush__GridBll__
#define __fruitCrush__GridBll__

#include "cocos2d.h"
#include "GridBox.h"
#include "GridTile.h"
#include "FruitCrush.h"
#include "ScoreCenter.h"
USING_NS_CC;
class GridBll : public cocos2d::CCObject
{
    CC_SYNTHESIZE_RETAIN(ScoreCenter*, _scoreCenter, ScoreCenter);
    CC_SYNTHESIZE_RETAIN(GridBox*, _box, Box);
    CC_SYNTHESIZE_RETAIN(GridTile*, _keyTile, KeyTile);
    CC_SYNTHESIZE_RETAIN(GridTile*, _firstTransTile, FirstTransTile);
    CC_SYNTHESIZE(int, _totalMoves, TotalMoves);
    CC_SYNTHESIZE(int, _totalScore, TotalScore);
private:
    bool _isColumnFirstDown;
    // current combo num
    int _currentCombo;
public:
    CCSpriteBatchNode* m_FruitBatch;
public:
    virtual ~GridBll();
    virtual bool init();
    CREATE_FUNC(GridBll);
    void setLayer(CCLayer* layer);
    
    //Init Map and fill items
    void initWithMap(int mapNo);
    void initWithFruit();
    // check if current xIndex(y=row max) is drop thing item,
    // return itemValue if true ,-1 if false
    int checkDropItem(int rowIndex, int columnIndex);
    
    // match items
    bool matchNormal();
    bool matchSuper(GridTile* keyTile, GridTile* nearTile);
    void matchWith(TypeOrientation orientation);
    
    TypeMatch getMatchType(CCArray* matchTiles);
    TypeMatch getSuperMatchType(GridTile* keyTile, GridTile* nearTile);
    
    // Select T or L matchs from readyForRemoveTiles
    CCArray* getTMatchTiles(CCArray* readyForRemoveTiles);
    
    // manage matched tiles score
    void matchedTilescoreMng(int comboCount, GridTile* lastTile, GridTile* curTransTile, int matchType, int curMatchType, CCArray* matchedTiles, CCArray* matchedTTiles);
    
    // manage extra type matched tiles
    void extraScoreMng(int comboCount, CCArray* matchedTiles);
    
    // manage extra tile type matchs
    void extraTileMatchMng(int comboCount, GridTile* tile);
    
    // manage extra item type matchs
    void extraItemMatchMng(int comboCount, GridTile* tile);
    
    // manage normal item type matchs
    void normalItemMatchMng(int comboCount, GridTile* tile);
    
    // start prompt matchs
    void _startPromptSchedule();
    void _stopPromptSchedule();
    
    // Reinit grid if no more matchs conditions exist
    void reInitGrid();
private:
    // init and remove all matched items
    void _checkInit();
    
    void _beforeItemRemove(CCSprite* sprite);
    void _removeItemInTile(CCSprite* sprite);
    int  _reFill();
    int  _reFillWithColumn(int columnIndex);
    bool _haveMoreMatchCheck();
    void _matchDone();
    void _itemMovedCallback();
    bool _matchedDropItem();
    
    //void m_MatchPrompt();
    void m_ResetPromptTiles();
    
    
};

#endif /* defined(__fruitCrush__GridBll__) */
