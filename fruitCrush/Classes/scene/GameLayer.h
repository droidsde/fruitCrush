//
//  GameLayer.h
//  fruitCrush
//
//  Created by Tsai Empire on 10/23/13.
//
//

#ifndef __fruitCrush__GameLayer__
#define __fruitCrush__GameLayer__

#include "cocos2d.h"
#include "GridBox.h"
#include "GridBll.h"
USING_NS_CC;
class GameLayer : public cocos2d::CCLayer
{
    CC_SYNTHESIZE_RETAIN(GridBll*, _bll, BLL);
private:
    int _mapNo;
    //CCDictionary *_mapTiles;
    
    CCSprite *_sceneBG;
    CCSprite *_backSprite;
    //CCSpriteBatchNode *_batchNode;
    
    GridTile *_selectedTile;
    
    
public:
    virtual ~GameLayer();
    virtual bool init();
    bool loadMap(int mapNo);
    
    void mapCompleted();
    void transTile(GridTile* tileA, GridTile* tileB, SEL_CallFuncND sel);
    void matchAfterTrans(GridTile* sender, GridTile* data);
    void recoverTrans(GridTile* sender, GridTile* data);
    
    void promptMatch();

    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    CREATE_FUNC(GameLayer);
};

#endif /* defined(__fruitCrush__GameLayer__) */
