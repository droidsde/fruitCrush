//
//  GameLayer.cpp
//  fruitCrush
//
//  Created by Tsai Empire on 10/23/13.
//
//

#include "GameLayer.h"
#include "FruitCrush.h"
#include "SecondLevelView.h"

#include "SimpleAudioEngine.h"
#include "ActionEffectHub.h"
#include "ResCenter.h"
//#include "stdio.h"
using namespace CocosDenshion;

GameLayer::~GameLayer()
{
    setBLL(NULL);
    //CC_SAFE_RELEASE(_sceneBG);
    //CC_SAFE_RELEASE(_backSprite);
    //CC_SAFE_RELEASE(_selectedTile);
}

bool GameLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("candy_crush_soundtrack3.mp3", true);
    
    CCSize winsize = FruitCrush::shareSingleton()->getWinSize();
    _sceneBG = ResCenter::shareInstance()->getSpriteByType(ResCenter::kSpriteGameLayerScene);
    _sceneBG->setAnchorPoint(ccp(0, 0));
    _sceneBG->setPosition(ccp(0, 0));
    this->addChild(_sceneBG);
    this->setTouchEnabled(true);
    
    _backSprite = CCSprite::create("Aleft.png");
    _backSprite->setAnchorPoint(ccp(1, 1));
    _backSprite->setPosition(ccp(_backSprite->boundingBox().size.width, winsize.height ));
    this->addChild(_backSprite);
    _backSprite->setTag(kTagback);
    
    
    
    return true;
}

bool GameLayer::loadMap(int mapNo)
{
    this->setBLL(GridBll::create());
    this->getBLL()->setLayer(this);
    this->getBLL()->initWithMap(mapNo);
    this->getBLL()->initWithFruit();
    
    return true;
}


void GameLayer::onEnter()
{
    CCLayer::onEnter();
    
}

void GameLayer::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
    
}

void GameLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    
}

void GameLayer::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    if (pTouches->count() == 1) {
        CCTouch *pTouch = (CCTouch*)pTouches->anyObject();
        
        /*for (CCSetIterator iterTouch = pTouches->begin(); iterTouch != pTouches->end(); iterTouch++) {
         CCTouch *pcurTouch = (CCTouch*)(*iterTouch);
         }*/
        
        CCPoint touchLocation = pTouch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        CCPoint local = convertToNodeSpace(touchLocation);
        
        // back button
        CCRect rect1 = _backSprite->boundingBox();
        bool flag = rect1.containsPoint(local);
        if (flag) {
            _backSprite->setColor(ccc3(222, 43, 54));
            SimpleAudioEngine::sharedEngine()->playEffect("click.caf");
            CCScene *pScene = SecondLevelView::scene();
            CCDirector::sharedDirector()->replaceScene(CCTransitionSplitRows::create(1, pScene));
        }
    }
}

void GameLayer::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    if (_bll->getBox()->getLock()) return;
    if (pTouches->count() == 1) {
        CCTouch* touch = (CCTouch*)pTouches->anyObject();
        CCPoint touchLocation = touch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        int posx = (touchLocation.x-kHXStart) / kTileWidth;
        int posy = (touchLocation.y-kHYStart) / kTileWidth;
        GridTile* keyTile = this->getBLL()->getKeyTile();
        
        if (keyTile && keyTile->getXIndex() == posx && keyTile->getYIndex() == posy) return;
        GridTile* nearTile = this->getBLL()->getBox()->getTileAt(posx, posy);
        
        if (keyTile && keyTile->isNearWith(nearTile)) {
            // set current transTile tag
            keyTile->setTag(kTagTranTile);
            nearTile->setTag(kTagTranTile);
            
            this->transTile(keyTile, nearTile, callfuncND_selector(GameLayer::matchAfterTrans));
            this->getBLL()->setKeyTile(NULL);
        }
        else {
            this->getBLL()->setKeyTile(nearTile);
        }
    }
    
}

void GameLayer::transTile(GridTile *tileA, GridTile *tileB, SEL_CallFuncND sel)
{
    if (!tileA->getCanPutFruit() || !tileB->getCanPutFruit()) return;
    
    // clear the prompt tiles actions
    if(this->getBLL()->getBox()->getPromptTiles())
    {
        int count = this->getBLL()->getBox()->getPromptTiles()->count();
        for (int i=0; i<count; i++) {
            CCArray* aComb = (CCArray*)this->getBLL()->getBox()->getPromptTiles()->objectAtIndex(i);
            GridTile* aTile = (GridTile*)aComb->objectAtIndex(0);
            GridTile* bTile = (GridTile*)aComb->objectAtIndex(1);
            GridTile* cTile = (GridTile*)aComb->objectAtIndex(2);
            aTile->stopItemActionByTag(kPromptMatchTag);
            bTile->stopItemActionByTag(kPromptMatchTag);
            cTile->stopItemActionByTag(kPromptMatchTag);
            aTile->getFruitItem()->getItemBG()->setScale(kTileScaleFactor);
            bTile->getFruitItem()->getItemBG()->setScale(kTileScaleFactor);
            cTile->getFruitItem()->getItemBG()->setScale(kTileScaleFactor);
        }
    }
    // unschedule prompt time
    this->unschedule(schedule_selector(GameLayer::promptMatch));
    
    this->getBLL()->getBox()->setLock(true);
    CCFiniteTimeAction* actionA = CCSequence::create(CCMoveTo::create(kMoveTileTime, tileB->getFruitItem()->getItemBG()->getPosition()),
                                                     CCCallFuncND::create(this, sel, tileA),
                                                     NULL);
    CCFiniteTimeAction* actionB = CCSequence::create(CCMoveTo::create(kMoveTileTime, tileA->getFruitItem()->getItemBG()->getPosition()),
                                                     CCCallFuncND::create(this, sel, tileB),
                                                     NULL);
    tileA->getFruitItem()->getItemBG()->runAction(actionA);
    tileB->getFruitItem()->getItemBG()->runAction(actionB);
    
    tileA->transWith(tileB);
}

void GameLayer::matchAfterTrans(GridTile *sender, GridTile *data)
{
    // firstTransTile is the same as keyTile and the data is the same as nearTile
    if (this->getBLL()->getFirstTransTile() == NULL) {
        this->getBLL()->setFirstTransTile(data);
        return;
    }
    
    if (this->getBLL()->matchSuper(this->getBLL()->getFirstTransTile(), data)) {// check super match
        if (this->getBLL()->getScoreCenter()->getGameMode() == kModMove) {
            // sub move number
            this->getBLL()->getScoreCenter()->subMoveBy(1);
            
            // reset drop item current asyn mark
            if (this->getBLL()->getScoreCenter()->getDropMode() && this->getBLL()->getScoreCenter()->getDropMode() == kDropModAsyn) {
                this->getBLL()->getScoreCenter()->setIsAsynMark(true);
            }
        }
        
    }
    else if (this->getBLL()->matchNormal()) {// check normal match
        if (this->getBLL()->getScoreCenter()->getGameMode() == kModMove) {
            // sub move number
            this->getBLL()->getScoreCenter()->subMoveBy(1);
            
            // reset drop item current asyn mark
            if (this->getBLL()->getScoreCenter()->getDropMode() && this->getBLL()->getScoreCenter()->getDropMode() == kDropModAsyn) {
                this->getBLL()->getScoreCenter()->setIsAsynMark(true);
            }
        }
    }
    else
    {
        this->transTile(data, this->getBLL()->getFirstTransTile(), callfuncND_selector(GameLayer::recoverTrans));
        
    }
    this->getBLL()->setFirstTransTile(NULL);
    
    
}

void GameLayer::recoverTrans(GridTile *sender, GridTile *data)
{
    // reset current transTile tag
    data->setTag(KTagDefaultTile);
    
    if (this->getBLL()->getFirstTransTile() == NULL) {
        this->getBLL()->setFirstTransTile(data);
        return;
    }
    this->getBLL()->setFirstTransTile(NULL);
    this->getBLL()->getBox()->setLock(false);
    
    // start prompt
    this->schedule(schedule_selector(GameLayer::promptMatch), 5, 0, 0);
}

void GameLayer::promptMatch()
{
    if(! this->getBLL()->getBox()->getPromptTiles() || this->getBLL()->getBox()->getPromptTiles()->count()==0) return;
    int count = this->getBLL()->getBox()->getPromptTiles()->count();
    int randomSeed = (arc4random()%count);
    CCArray* aComb = (CCArray*)this->getBLL()->getBox()->getPromptTiles()->objectAtIndex(randomSeed);
    GridTile* aTile = (GridTile*)aComb->objectAtIndex(0);
    GridTile* bTile = (GridTile*)aComb->objectAtIndex(1);
    GridTile* cTile = (GridTile*)aComb->objectAtIndex(2);
    
    //aTile->runAction(CCScaleTo::create(1, 0.1f));
    aTile->getFruitItem()->getItemBG()->runAction(ActionEffectHub::shareInstance()->playSlink());
    bTile->getFruitItem()->getItemBG()->runAction(ActionEffectHub::shareInstance()->playSlink());
    cTile->getFruitItem()->getItemBG()->runAction(ActionEffectHub::shareInstance()->playSlink());
    this->unschedule(schedule_selector(GameLayer::promptMatch));
}
