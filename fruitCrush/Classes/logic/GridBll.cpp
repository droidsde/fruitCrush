//
//  GridBll.cpp
//  fruitCrush
//
//  Created by Tsai Empire on 10/29/13.
//
//

#include "GridBll.h"
#include "SimpleAudioEngine.h"
#include "GameParticle.h"
#include "ActionEffectHub.h"
#include "GameLayer.h"
#include "FruitCrush.h"
using namespace CocosDenshion;

GridBll::~GridBll()
{
    setFirstTransTile(NULL);
    setKeyTile(NULL);
    setBox(NULL);
    setScoreCenter(NULL);
}

bool GridBll::init()
{
    /*if (!_layer) {
        return false;
    }*/

    return true;
}

void GridBll::setLayer(cocos2d::CCLayer *layer)
{
    this->setBox(GridBox::create());
    this->setScoreCenter(ScoreCenter::create());
    this->getBox()->setLayer(layer);
    this->getScoreCenter()->setLayer(layer);
}

void GridBll::initWithMap(int mapNo)
{
    this->getBox()->initWithMap(mapNo);
    this->getScoreCenter()->initWithMap(mapNo);
}

void GridBll::initWithFruit()
{
    //m_FruitBatch = CCSpriteBatchNode::create("candy.png", 100);
    m_FruitBatch = CCSpriteBatchNode::createWithTexture(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("red.png")->getTexture(), 100);
    ;
    this->getBox()->getLayer()->addChild(m_FruitBatch);
    
    int tileValue;
    for (int tr=0; tr<this->getBox()->getGridTiles()->count(); tr++) {
        CCArray* rowTiles = (CCArray*)this->getBox()->getGridTiles()->objectAtIndex(tr);
        for (int td=0; td<rowTiles->count(); td++) {
            tileValue = this->checkDropItem(tr, td);
            if (tileValue<0) // normal item
                tileValue = (arc4random()%(this->getScoreCenter()->getColorNum())+1);
            
            GridTile *tile =(GridTile*) rowTiles->objectAtIndex(td);
            GridItem *item = GridItem::create();
            tile->setFruitItem(item);
            if (!tile->getCanPutFruit()) {
                tile->getFruitItem()->setItemValue(ktNothing);
                continue;
            }
            
            //CCString* name = CCString::createWithFormat("q%d.png",tileValue);
            CCSprite *fruitbg = CCSprite::createWithSpriteFrameName(FruitCrush::shareSingleton()->getItemString(tileValue));
            fruitbg->setScale(kTileScaleFactor);
            fruitbg->setPosition(ccp(kHXStart + td * kTileWidth + kTileWidth/2, kHYStart + tr * kTileWidth+kTileWidth/2));
            
            tile->getFruitItem()->setItemValue(tileValue);
            tile->getFruitItem()->setItemBG(fruitbg);
            if(tileValue<8) tile->getFruitItem()->setItemClass(kcinormal);
            else tile->getFruitItem()->setItemClass(kciDrop);
            
            tile->getFruitItem()->setItemAct(true);
            m_FruitBatch->addChild(fruitbg);
        }
    }
    
    // 待消除的水果
    CCArray* readyForRemove = CCArray::createWithCapacity(kBoxHeight);
    this->getBox()->setReadyForRemoveTiles(readyForRemove);
    
    _checkInit();
}

int GridBll::checkDropItem(int rowIndex, int columnIndex)
{
    if(!this->getScoreCenter()->isDropsMode()) return -1;
    if (!this->getScoreCenter()->getDropBornX() || !this->getScoreCenter()->getDropBornY() || this->getScoreCenter()->getDropBornX()->count()<=0 || this->getScoreCenter()->getDropBornY()->count()<=0) {
        return -1;
    }
    CCString* dropXstr = (CCString*)this->getScoreCenter()->getDropBornX()->objectAtIndex(0);
    CCString* dropYstr = (CCString*)this->getScoreCenter()->getDropBornY()->objectAtIndex(0);
    if (dropXstr && dropYstr) {
        int dropX = dropXstr->intValue();
        int dropY = dropYstr->intValue();
        if(dropX != columnIndex || dropY != rowIndex) return -1;
        
        int curMov = this->getScoreCenter()->getCurrentMoves();
        int levelMov = this->getScoreCenter()->getMoveLimit();
        int curDropType = -1;
        
        
        // things mode
        TypeDropMode tMod = this->getScoreCenter()->getDropMode();
        if (tMod == kDropModSyn) {
            if (dropX == columnIndex && dropY == rowIndex) {
                // check current drop item
                if (this->getScoreCenter()->getDropTypes()) {
                    for (int i=0; i < this->getScoreCenter()->getDropTypes()->count(); i++) {
                        int curTypeNum = ((CCString*)this->getScoreCenter()->getDropNums()->objectAtIndex(i))->intValue();
                        if (curTypeNum > 0) {
                            curDropType = ((CCString*)this->getScoreCenter()->getDropTypes()->objectAtIndex(i))->intValue();
                            curTypeNum--;
                            this->getScoreCenter()->getDropNums()->removeObjectAtIndex(i);
                            this->getScoreCenter()->getDropNums()->insertObject(CCString::createWithFormat("%u", curTypeNum), i);
                            
                            // remove the checked row item
                            this->getScoreCenter()->getDropBornX()->removeObjectAtIndex(0);
                            this->getScoreCenter()->getDropBornY()->removeObjectAtIndex(0);
                            
                            break;
                        }
                    }
                }
                if (curDropType<0) return -1;
                
                return curDropType;
            }
        }
        else if(tMod == kDropModAsyn) {
            if(!this->getScoreCenter()->getBornDropMove()) return -1;
            if (((levelMov-curMov)%(this->getScoreCenter()->getBornDropMove())==0 || curMov==0) && dropX == columnIndex && dropY == rowIndex && this->getScoreCenter()->getIsAsynMark()) {
                // check current drop item
                if (this->getScoreCenter()->getDropTypes()) {
                    for (int i=0; i < this->getScoreCenter()->getDropTypes()->count(); i++) {
                        int curTypeNum = ((CCString*)this->getScoreCenter()->getDropNums()->objectAtIndex(i))->intValue();
                        if (curTypeNum > 0) {
                            // mark the current asyn drop is done
                            this->getScoreCenter()->setIsAsynMark(false);
                            
                            curDropType = ((CCString*)this->getScoreCenter()->getDropTypes()->objectAtIndex(i))->intValue();
                            curTypeNum--;
                            this->getScoreCenter()->getDropNums()->removeObjectAtIndex(i);
                            this->getScoreCenter()->getDropNums()->insertObject(CCString::createWithFormat("%u", curTypeNum), i);
                            
                            // remove the checked row item
                            this->getScoreCenter()->getDropBornX()->removeObjectAtIndex(0);
                            this->getScoreCenter()->getDropBornY()->removeObjectAtIndex(0);
                            
                            break;
                        }
                    }
                }
                if (curDropType<0) return -1;
                return curDropType;
            }
        }
    }
    return -1;
}



void GridBll::_checkInit()
{
    // 待消除的水果
    //CCArray* readyForRemove = CCArray::createWithCapacity(kBoxHeight);
    //this->getBox()->setReadyForRemoveTiles(readyForRemove);
    
    matchWith(kOrientationH);
    matchWith(kOrientationV);
    if (this->getBox()->getReadyForRemoveTiles()->count() == 0) return;
    int count = this->getBox()->getReadyForRemoveTiles()->count();
    for (int i=0; i < count; i++) {
        CCArray* tiles = (CCArray*)this->getBox()->getReadyForRemoveTiles()->objectAtIndex(i);
        for (int j=0; j<tiles->count(); j++) {
            GridTile* tile = (GridTile*)tiles->objectAtIndex(j);
            
            //have drop item
            if (tile->getFruitItem()->getItemClass() == kciDrop) {
                continue;
            }
            
            CCPoint curPos = tile->getFruitItem()->getItemBG()->getPosition();
            //remove older sprite
            m_FruitBatch->removeChild(tile->getFruitItem()->getItemBG(), true);
            
            int tileValue = (arc4random()%(this->getScoreCenter()->getColorNum())+1);
            //CCString* name = CCString::createWithFormat("q%d.png",tileValue);
            CCSprite *fruitbg = CCSprite::createWithSpriteFrameName(FruitCrush::shareSingleton()->getItemString(tileValue));
            fruitbg->setScale(kTileScaleFactor);
            // add new sprite
            tile->getFruitItem()->setItemBG(fruitbg);
            tile->getFruitItem()->setItemValue(tileValue);
            tile->getFruitItem()->getItemBG()->setPosition(curPos);
            m_FruitBatch->addChild(tile->getFruitItem()->getItemBG());
        }
        tiles->removeAllObjects();
    }
    this->getBox()->getReadyForRemoveTiles()->removeAllObjects();
    _checkInit();
    if (!_haveMoreMatchCheck()) {
        reInitGrid();
    }
}

// check normal match, such as 3、4、5 matchs in horizontal or vertical
bool GridBll::matchNormal()
{
    matchWith(kOrientationH);
    matchWith(kOrientationV);
    int removeCount = this->getBox()->getReadyForRemoveTiles()->count();
    
    if (removeCount == 0 && !this->_matchedDropItem()) return false;
    CCArray* matchedTTiles = getTMatchTiles(this->getBox()->getReadyForRemoveTiles());
    
    for (int i=0; i<removeCount; i++) {
        CCArray* aMatchedTiles = (CCArray*)this->getBox()->getReadyForRemoveTiles()->objectAtIndex(i);
        // match type
        int matchType = this->getMatchType(aMatchedTiles);
        // current match type,for check 4、 5 、T、 L Matchs
        int curMatchType;
        GridTile* curTransTile = NULL;
        GridTile* tile;
        for (int j=0; j<aMatchedTiles->count(); j++) {
            tile = (GridTile*)aMatchedTiles->objectAtIndex(j);
            if (matchedTTiles && matchedTTiles->containsObject(tile) && tile->getFruitItem()->getItemClass() == kciPop) {
                // T或L型，暂且保留
                tile->getFruitItem()->setItemAct(false);
            }
            if (!tile->getFruitItem()->getItemAct()) continue;
            
            CCAction* removeAct = CCSequence::create(CCDelayTime::create(kDelayTime),CCCallFuncN::create(this, callfuncN_selector(GridBll::_beforeItemRemove)),CCScaleTo::create(kDisapearTime, 0.0f), CCCallFuncN::create(this, callfuncN_selector(GridBll::_removeItemInTile)), NULL);
            
            //
            if (tile->getFruitItem()->getItemBG()) {
                //set current match value
                curMatchType = tile->getFruitItem()->getItemValue();
                
                // reset current trans Tile
                if (tile->getTag() == kTagTranTile) {
                    curTransTile = tile;
                    tile->setTag(KTagDefaultTile);
                }
                
                tile->getFruitItem()->getItemBG()->runAction(removeAct);
                tile->getFruitItem()->setItemBG(NULL);
            }
            tile->getFruitItem()->setItemValue(ktNothing);
        }
        
        
        
        // manage score & add super value
        _currentCombo++;
        this->matchedTilescoreMng(_currentCombo, tile, curTransTile, matchType, curMatchType, aMatchedTiles, matchedTTiles);
        
        // remove a pair matched tiles
        aMatchedTiles->removeAllObjects();
        
    }
    this->getBox()->getReadyForRemoveTiles()->removeAllObjects();
    
    // T or L class item Reactive
    if(matchedTTiles)
    {
        for (int i=0; i<matchedTTiles->count(); i++) {
            GridTile* ttile = (GridTile*)matchedTTiles->objectAtIndex(i);
            ttile->getFruitItem()->setItemAct(true);
        }
        matchedTTiles->removeAllObjects();
    }
    
    int columnEmptyCount = this->_reFill();
    this->getBox()->getLayer()->runAction(CCSequence::create(CCDelayTime::create(kMoveTileTime * columnEmptyCount + 0.06f),
                                                             CCCallFunc::create(this, callfunc_selector(GridBll::_matchDone)),
                                        NULL));
    
    return true;
}

// match special with two power item
bool GridBll::matchSuper(GridTile *keyTile, GridTile *nearTile)
{
    TypeMatch type = this->getSuperMatchType(keyTile, nearTile);
    if (type != kMatchNothing) {
        // manage score & add super value
        _currentCombo++;
    }
    
    switch (type) {
        case kMatchHV:
        {
            // key score
            this->getScoreCenter()->showPromptScore(kScoreSuperHVFactor, keyTile->getPosition());
            int curScore = this->getScoreCenter()->getCurrentScore()+kScoreSuperHVFactor;
            this->getScoreCenter()->setCurrentScore(curScore);
            
            // clear all the H and V tiles
            int keyHIndex = keyTile->getXIndex();
            int nearVIndex = nearTile->getYIndex();
            
            for (int i=0; i<kBoxWidth; i++) {
                GridTile* vTile = (GridTile*)((CCArray*)this->getBox()->getGridTiles()->objectAtIndex(nearVIndex))->objectAtIndex(i);
                GridTile* hTile = (GridTile*)((CCArray*)this->getBox()->getGridTiles()->objectAtIndex(i))->objectAtIndex(keyHIndex);
                
                // filter the drop item and remove item bg
                if (vTile->getFruitItem()->getItemBG() && vTile->getFruitItem()->getItemClass() != kciDrop) {
                    // clear v tile
                    CCAction* vRmAct = CCSequence::create(CCDelayTime::create(kDelayTime),CCCallFuncN::create(this, callfuncN_selector(GridBll::_beforeItemRemove)),CCScaleTo::create(kDisapearTime, 0.0f), CCCallFuncN::create(this, callfuncN_selector(GridBll::_removeItemInTile)), NULL);
                    vTile->getFruitItem()->getItemBG()->runAction(vRmAct);
                    vTile->getFruitItem()->setItemBG(NULL);
                    vTile->getFruitItem()->setItemValue(ktNothing);
                    
                    // set every score
                    this->normalItemMatchMng(this->_currentCombo, vTile);
                    this->extraItemMatchMng(this->_currentCombo, vTile);
                    this->extraTileMatchMng(this->_currentCombo, vTile);
                    
                }
                if (hTile->getFruitItem()->getItemBG() && hTile->getFruitItem()->getItemClass() != kciDrop) {
                    // clear h tile
                    CCAction* hRmAct = CCSequence::create(CCDelayTime::create(kDelayTime),CCCallFuncN::create(this, callfuncN_selector(GridBll::_beforeItemRemove)),CCScaleTo::create(kDisapearTime, 0.0f), CCCallFuncN::create(this, callfuncN_selector(GridBll::_removeItemInTile)), NULL);
                    hTile->getFruitItem()->getItemBG()->runAction(hRmAct);
                    hTile->getFruitItem()->setItemBG(NULL);
                    hTile->getFruitItem()->setItemValue(ktNothing);
                    
                    //set every score
                    this->normalItemMatchMng(this->_currentCombo, hTile);
                    this->extraItemMatchMng(this->_currentCombo, hTile);
                    this->extraTileMatchMng(this->_currentCombo, hTile);
                }
            }
            
            break;
        }
        case kMatchTAndHV:
        {
            // score
            this->getScoreCenter()->showPromptScore(kScoreSuperHVAndTFactor, keyTile->getPosition());
            int curScore = this->getScoreCenter()->getCurrentScore()+kScoreSuperHVAndTFactor;
            this->getScoreCenter()->setCurrentScore(curScore);
            
            break;
        }
        case kMatchPowerAndHV:
        {
            // score
            this->getScoreCenter()->showPromptScore(kScoreSuperPowerAndHV, keyTile->getPosition());
            int curScore = this->getScoreCenter()->getCurrentScore()+kScoreSuperPowerAndHV;
            this->getScoreCenter()->setCurrentScore(curScore);
            
            break;
        }
        case kMatchPowerAndT:
        {
            // score
            this->getScoreCenter()->showPromptScore(kScoreSuperPowerAndT, keyTile->getPosition());
            int curScore = this->getScoreCenter()->getCurrentScore()+kScoreSuperPowerAndT;
            this->getScoreCenter()->setCurrentScore(curScore);
            
            break;
        }
        case kMatchTT:
        {
            // score
            this->getScoreCenter()->showPromptScore(kScoreSuperTTFactor, keyTile->getPosition());
            int curScore = this->getScoreCenter()->getCurrentScore()+kScoreSuperTTFactor;
            this->getScoreCenter()->setCurrentScore(curScore);
            
            break;
        }
        case kMatchPowerAndPower:
        {
            // score
            this->getScoreCenter()->showPromptScore(kScoreSuperPowerAndPower, keyTile->getPosition());
            int curScore = this->getScoreCenter()->getCurrentScore()+kScoreSuperPowerAndPower;
            this->getScoreCenter()->setCurrentScore(curScore);
            
            break;
        }
        case KMatchPowerAndNormal:
        {
            // score
            this->getScoreCenter()->showPromptScore(kScoreSuperpowerAndNormalFactor, keyTile->getPosition());
            int curScore = this->getScoreCenter()->getCurrentScore()+kScoreSuperpowerAndNormalFactor;
            this->getScoreCenter()->setCurrentScore(curScore);
            
            GridTile* norTile = (keyTile->getFruitItem()->getItemClass() == kcinormal) ? keyTile : nearTile;
            int tileValue = norTile->getFruitItem()->getItemValue();
            for (int i=0; i<this->getBox()->getGridTiles()->count(); i++) {
                CCArray* rowArr = (CCArray*)this->getBox()->getGridTiles()->objectAtIndex(i);
                for (int j=0; j<rowArr->count(); j++) {
                    GridTile* theTile = (GridTile*)rowArr->objectAtIndex(j);
                    if (theTile->getFruitItem() && theTile->getFruitItem()->getItemValue() == tileValue) {
                        // clear v tile
                        CCAction* RmAct = CCSequence::create(CCDelayTime::create(kDelayTime),CCCallFuncN::create(this, callfuncN_selector(GridBll::_beforeItemRemove)),CCScaleTo::create(kDisapearTime, 0.0f), CCCallFuncN::create(this, callfuncN_selector(GridBll::_removeItemInTile)), NULL);
                        theTile->getFruitItem()->getItemBG()->runAction(RmAct);
                        theTile->getFruitItem()->setItemBG(NULL);
                        theTile->getFruitItem()->setItemValue(ktNothing);
                        
                        // set every score
                        this->normalItemMatchMng(this->_currentCombo, theTile);
                        this->extraItemMatchMng(this->_currentCombo, theTile);
                        this->extraTileMatchMng(this->_currentCombo, theTile);
                    }
                }
            }
            
            break;
        }
        case kMatchNothing:
        {
            return false;
            break;
        }
        default:
        {
            return false;
            break;
        }
    }
    
    if (type != kMatchNothing) {
        // clean two super tiles
        CCAction* keyRmAct = CCSequence::create(CCDelayTime::create(kDelayTime),CCCallFuncN::create(this, callfuncN_selector(GridBll::_beforeItemRemove)),CCScaleTo::create(kDisapearTime, 0.0f), CCCallFuncN::create(this, callfuncN_selector(GridBll::_removeItemInTile)), NULL);
        CCAction* nearRmAct = CCSequence::create(CCDelayTime::create(kDelayTime),CCCallFuncN::create(this, callfuncN_selector(GridBll::_beforeItemRemove)),CCScaleTo::create(kDisapearTime, 0.0f), CCCallFuncN::create(this, callfuncN_selector(GridBll::_removeItemInTile)), NULL);
        
        //CCAction* keyRmAct = ActionEffectHub::shareInstance()->removeItem(callfuncN_selector(GridBll::_beforeItemRemove), callfuncN_selector(GridBll::_removeItemInTile));
        //CCAction* nearRmAct = ActionEffectHub::shareInstance()->removeItem(callfuncN_selector(GridBll::_beforeItemRemove), callfuncN_selector(GridBll::_removeItemInTile));
        //
        if (keyTile->getFruitItem()->getItemBG()) {
            keyTile->getFruitItem()->getItemBG()->runAction(keyRmAct);
            keyTile->getFruitItem()->setItemBG(NULL);
        }
        if (nearTile->getFruitItem()->getItemBG()) {
            nearTile->getFruitItem()->getItemBG()->runAction(nearRmAct);
            nearTile->getFruitItem()->setItemBG(NULL);
        }
        keyTile->getFruitItem()->setItemValue(ktNothing);
        nearTile->getFruitItem()->setItemValue(ktNothing);
    }
    
    
    int columnEmptyCount = this->_reFill();
    this->getBox()->getLayer()->runAction(CCSequence::create(CCDelayTime::create(kMoveTileTime * columnEmptyCount + 0.06f),
                                                             CCCallFunc::create(this, callfunc_selector(GridBll::_matchDone)),
                                                             NULL));
    return true;
}

// check matchs with special Orientation
void GridBll::matchWith(TypeOrientation orient)
{
    int widthSize = (orient == kOrientationH) ? kBoxWidth : kBoxHeight;
    int heightSize = (orient == kOrientationV) ? kBoxHeight : kBoxWidth;
    GridTile* firstOne;
    GridTile* secondOne;
    int matchCount;
    int value;
    
    for (int i=0; i<widthSize; i++) {
        matchCount = 0;
        value = ktNothing;
        firstOne = NULL;
        secondOne = NULL;
        CCArray* matchedTiles;
        matchedTiles = CCArray::createWithCapacity(heightSize);
        for (int j=0; j<heightSize; j++) {
            GridTile* tile = this->getBox()->getTileAt((orient==kOrientationH?i:j), (orient==kOrientationH?j:i));
            
            // item is not null and is not drop item
            if (tile->getFruitItem()->getItemValue() == value && tile->getFruitItem()->getItemValue() != ktNothing && tile->getFruitItem()->getItemClass() != kciDrop) {
                matchCount++;
                if (matchCount >3) {
                    matchedTiles->addObject(tile);
                }
                else if (matchCount == 3)
                {
                    matchedTiles->addObject(firstOne);
                    matchedTiles->addObject(secondOne);
                    matchedTiles->addObject(tile);
                    firstOne = NULL;
                    secondOne = NULL;
                }
                else if (matchCount == 2)
                {
                    secondOne = tile;
                }
                else
                {
                    //..
                }
            }
            else
            {
                if (matchedTiles->count() >= 3) {
                    this->getBox()->getReadyForRemoveTiles()->addObject(matchedTiles);
                    matchedTiles = NULL;
                    matchedTiles = CCArray::createWithCapacity(heightSize);
                }
                
                firstOne = tile;
                secondOne = NULL;
                value = tile->getFruitItem()->getItemValue();
                matchCount = 1;
            }
        }
        if (matchedTiles->count() >= 3) {
            this->getBox()->getReadyForRemoveTiles()->addObject(matchedTiles);
        }
    }
}

// check a match tile array's type
TypeMatch GridBll::getMatchType(cocos2d::CCArray *matchTiles)
{
    if (matchTiles == NULL) return kMatchNothing;
    int count = matchTiles->count();
    switch (count) {
        case 3:
        {
            return kMatch3;
            break;
        }
        case 4:
        {
            if (((GridTile*)matchTiles->objectAtIndex(0))->getXIndex() == ((GridTile*)matchTiles->objectAtIndex(1))->getXIndex()) {
                return kMatch4V;
            }
            else
                return kMatch4H;
            break;
        }
        case 5:
            return kMatch5;
            break;
        
        default:
            break;
    }
    return kMatchNothing;
}

// check the super tile's match type
TypeMatch GridBll::getSuperMatchType(GridTile *keyTile, GridTile *nearTile)
{
    ClassItemSprite keyClass = keyTile->getFruitItem()->getItemClass();
    ClassItemSprite nearClass = nearTile->getFruitItem()->getItemClass();
    if ((keyClass == kciHor || keyClass == kciVec ) && (nearClass == kciHor || nearClass == kciVec)) {
        return kMatchHV;
    }
    else if (((keyClass == kciHor || keyClass == kciVec) && (nearClass == kciPop)) || ((nearClass == kciHor || nearClass == kciVec) && (keyClass == kciPop)))
    {
        return kMatchTAndHV;
    }
    else if (((keyClass == kciHor || keyClass == kciVec) && (nearClass == kciBomb)) || ((nearClass == kciHor || nearClass == kciVec) && (keyClass == kciBomb)))
    {
        return kMatchPowerAndHV;
    }
    else if ((keyClass == kcinormal && nearClass == kciBomb) || (nearClass == kcinormal && keyClass == kciBomb))
    {
        return KMatchPowerAndNormal;
    }
    else if ((keyClass == kciPop && nearClass == kciBomb) || (nearClass == kciPop && keyClass == kciBomb))
    {
        return kMatchPowerAndT;
    }
    else if (keyClass == kciPop && nearClass == kciPop)
    {
        return kMatchTT;
    }
    else if (keyClass == kciBomb && nearClass == kciBomb)
    {
        return kMatchPowerAndPower;
    }
    return kMatchNothing;
}

// Get the detail match type for score mng
/*TypeMatch GridBll::getDetailMatchType(cocos2d::CCArray *matchTiles)
{
    if (matchTiles == NULL) return kMatchNothing;
    int count = matchTiles->count();
    switch (count) {
        case 3:
        {
            for (int i=0; i<count; i++) {
                
            }
            break;
        }
        case 4:
        {
            if (((GridTile*)matchTiles->objectAtIndex(0))->getXIndex() == ((GridTile*)matchTiles->objectAtIndex(1))->getXIndex()) {
                return kMatch4V;
            }
            else
                return kMatch4H;
            break;
        }
        case 5:
            return kMatch5;
            break;
            
        default:
            break;
    }
    return kMatchNothing;
}*/

/*
 * search T or L type match key tile from readyForRemoveTiles array, 
 * one T or L type matchs only have one key tile, return all key tiles in a array
 */
CCArray* GridBll::getTMatchTiles(cocos2d::CCArray *readyForRemoveTiles)
{
    if (NULL == readyForRemoveTiles || readyForRemoveTiles->count() == 0 || readyForRemoveTiles->count() == 1) return NULL;
    CCArray* tTiles = CCArray::createWithCapacity(kBoxWidth);
    for (int i = 0; i<readyForRemoveTiles->count(); i++) {
        CCArray* matchTiles = (CCArray*)readyForRemoveTiles->objectAtIndex(i);
        for (int j = 0; j < matchTiles->count(); j++) {
            GridTile* curtile = (GridTile*)matchTiles->objectAtIndex(j);
            for (int k=i+1; k<readyForRemoveTiles->count(); k++) {
                CCArray* otherMatchTiles = (CCArray*)readyForRemoveTiles->objectAtIndex(k);
                if (otherMatchTiles->containsObject(curtile)) {
                    tTiles->addObject(curtile);
                }
            }
        }
    }
    return tTiles;
}


void GridBll::matchedTilescoreMng(int comboCount, GridTile* tile, GridTile* curTransTile, int matchType, int curMatchType, CCArray* matchedTiles, CCArray* matchedTTiles)
{
    if (curTransTile) {
        tile = curTransTile;
    }
    // T tiles
    if (matchedTTiles && matchedTTiles->containsObject(tile) && tile->getFruitItem()->getItemAct()) {
        char type[3];
        sprintf(type, "%u%u", curMatchType,3);
        int itemStr = atoi(type);
        tile->getFruitItem()->setItemValue(curMatchType);
        tile->getFruitItem()->setItemClass(kciPop);
        CCSprite* newItem = CCSprite::createWithSpriteFrameName(FruitCrush::shareSingleton()->getItemString(itemStr));
        newItem->setScale(kTileScaleFactor);
        newItem->setPosition(ccp(kHXStart + tile->getXIndex() * kTileWidth + kTileWidth/2, kHYStart + tile->getYIndex() * kTileWidth + kTileWidth/2));
        m_FruitBatch->addChild(newItem);
        
        tile->getFruitItem()->setItemBG(newItem);
    }
    else
    {
        switch (matchType) {
            case kMatch3:
            {
                /*if (curTransTile) {
                 tile = curTransTile;
                 }*/
                int addScore = kScore3Factor*comboCount;
                this->getScoreCenter()->showPromptScore(addScore, tile->getPosition());
                int curScore = this->getScoreCenter()->getCurrentScore()+addScore;
                this->getScoreCenter()->setCurrentScore(curScore);
                
                this->extraScoreMng(comboCount, matchedTiles);
                
                break;
            }
            case kMatch4H:
            {
                /*if (curTransTile) {
                 tile = curTransTile;
                 }*/
                int addScore = kScore4Factor*comboCount;
                this->getScoreCenter()->showPromptScore(addScore, tile->getPosition());
                int curScore = this->getScoreCenter()->getCurrentScore()+addScore;
                this->getScoreCenter()->setCurrentScore(curScore);
                
                this->extraScoreMng(comboCount, matchedTiles);
                
                char type[3];
                sprintf(type, "%u%u", curMatchType,1);
                int itemStr = atoi(type);
                tile->getFruitItem()->setItemValue(curMatchType);
                tile->getFruitItem()->setItemClass(kciHor);
                CCSprite* newItem = CCSprite::createWithSpriteFrameName(FruitCrush::shareSingleton()->getItemString(itemStr));
                newItem->setScale(kTileScaleFactor);
                newItem->setPosition(ccp(kHXStart + tile->getXIndex() * kTileWidth + kTileWidth/2, kHYStart + tile->getYIndex() * kTileWidth + kTileWidth/2));
                m_FruitBatch->addChild(newItem);
                
                tile->getFruitItem()->setItemBG(newItem);
                break;
            }
            case kMatch4V:
            {
                /*if (curTransTile) {
                 tile = curTransTile;
                 }*/
                int addScore = kScore4Factor*comboCount;
                this->getScoreCenter()->showPromptScore(addScore, tile->getPosition());
                int curScore = this->getScoreCenter()->getCurrentScore()+addScore;
                this->getScoreCenter()->setCurrentScore(curScore);
                
                this->extraScoreMng(comboCount, matchedTiles);
                
                char type[3];
                sprintf(type, "%u%u", curMatchType,2);
                int itemStr = atoi(type);
                tile->getFruitItem()->setItemValue(curMatchType);
                tile->getFruitItem()->setItemClass(kciVec);
                CCSprite* newItem = CCSprite::createWithSpriteFrameName(FruitCrush::shareSingleton()->getItemString(itemStr));
                newItem->setScale(kTileScaleFactor);
                newItem->setPosition(ccp(kHXStart + tile->getXIndex() * kTileWidth + kTileWidth/2, kHYStart + tile->getYIndex() * kTileWidth + kTileWidth/2));
                m_FruitBatch->addChild(newItem);
                
                tile->getFruitItem()->setItemBG(newItem);
                break;
            }
            case kMatch5:
            {
                /*if (curTransTile) {
                 tile = curTransTile;
                 }*/
                int addScore = kScore5Factor*comboCount;
                this->getScoreCenter()->showPromptScore(addScore, tile->getPosition());
                int curScore = this->getScoreCenter()->getCurrentScore()+addScore;
                this->getScoreCenter()->setCurrentScore(curScore);
                
                this->extraScoreMng(comboCount, matchedTiles);
                
                tile->getFruitItem()->setItemValue(ktSpacial);
                tile->getFruitItem()->setItemClass(kciBomb);
                CCSprite* newItem = CCSprite::createWithSpriteFrameName(FruitCrush::shareSingleton()->getItemString(7));
                newItem->setScale(kTileScaleFactor);
                newItem->setPosition(ccp(kHXStart + tile->getXIndex() * kTileWidth + kTileWidth/2, kHYStart + tile->getYIndex() * kTileWidth + kTileWidth/2));
                m_FruitBatch->addChild(newItem);
                
                tile->getFruitItem()->setItemBG(newItem);
                break;
            }
                
            default:
                break;
        }
    }
    
    
}

void GridBll::extraScoreMng(int comboCount, CCArray* matchedTiles)
{
    for (int i=0; i<matchedTiles->count(); i++) {
        GridTile* tile = (GridTile*)matchedTiles->objectAtIndex(i);
        this->extraItemMatchMng(comboCount, tile);
        
        // item class extra
        /*int addScore = 0;
        switch (tile->getFruitItem()->getItemClass()) {
            case kciHor:
            {
                addScore = kScoreItemHVFactor*comboCount;
                break;
            }
            case kciVec:
            {
                addScore = kScoreItemHVFactor*comboCount;
                break;
            }
            case kciPop:
            {
                addScore = kScoreItemTFactor*comboCount;
                break;
            }
            default:
                break;
        }
        if (addScore > 0) {
            this->getScoreCenter()->showPromptScore(addScore, tile->getPosition());
            int curScore = this->getScoreCenter()->getCurrentScore()+addScore;
            this->getScoreCenter()->setCurrentScore(curScore);
        }*/
        
        
        
        
        // tile type extra
        /*addScore = 0;
        switch (tile->getType()) {
            case kttIce:
            {
                addScore = kScoreTileIceFactor*comboCount;
                this->getBox()->m_tileBgBatch->removeChild(tile->getTileSprite(), true);
                tile->changeType(kttNormal);
                this->getBox()->m_tileBgBatch->addChild(tile->getTileSprite());
                
                GameParticle* jellyBreak = GameParticle::shareInstance();
                jellyBreak->playJellyBreak(this->getBox()->getLayer(), tile->getPosition());
                
                break;
            }
            case kttIce2:
            {
                addScore = kScoreTileIce2Factor*comboCount;
                this->getBox()->m_tileBgBatch->removeChild(tile->getTileSprite(), true);
                tile->changeType(kttIce);
                this->getBox()->m_tileBgBatch->addChild(tile->getTileSprite());
                
                break;
            }
            case kttStone:
            {
                addScore = kScoreTileIceStone*comboCount;
                this->getBox()->m_tileBgBatch->removeChild(tile->getTileSprite(), true);
                tile->changeType(kttNormal);
                this->getBox()->m_tileBgBatch->addChild(tile->getTileSprite());
                
                break;
            }
            default:
                break;
        }
        if (addScore > 0) {
            this->getScoreCenter()->showPromptScore(addScore, tile->getPosition());
            int curScore = this->getScoreCenter()->getCurrentScore()+addScore;
            this->getScoreCenter()->setCurrentScore(curScore);
        }*/
        
        this->extraTileMatchMng(comboCount, tile);
    }
}

void GridBll::normalItemMatchMng(int comboCount, GridTile *tile)
{
    int addScore = 0;
    if (tile->getFruitItem()->getItemClass() == kcinormal) {
        addScore = kScoreNormalFactor*comboCount;
        this->getScoreCenter()->showPromptScore(addScore, tile->getPosition());
        int curScore = this->getScoreCenter()->getCurrentScore()+addScore;
        this->getScoreCenter()->setCurrentScore(curScore);
    }
}

void GridBll::extraItemMatchMng(int comboCount, GridTile *tile)
{
    // item class extra
    int addScore = 0;
    switch (tile->getFruitItem()->getItemClass()) {
        case kciHor:
        {
            addScore = kScoreItemHVFactor*comboCount;
            break;
        }
        case kciVec:
        {
            addScore = kScoreItemHVFactor*comboCount;
            break;
        }
        case kciPop:
        {
            addScore = kScoreItemTFactor*comboCount;
            break;
        }
        default:
            break;
    }
    if (addScore > 0) {
        this->getScoreCenter()->showPromptScore(addScore, tile->getPosition());
        int curScore = this->getScoreCenter()->getCurrentScore()+addScore;
        this->getScoreCenter()->setCurrentScore(curScore);
    }
}

void GridBll::extraTileMatchMng(int comboCount, GridTile* tile)
{
    // tile type extra
    int addScore = 0;
    if (tile->getTileType() & kttStone) {
        addScore = kScoreTileIceStone*comboCount;
        tile->removeType(this->getBox()->m_tileBgBatch, kttStone);
    }
    else if(tile->getTileType() & kttIce2)
    {
        addScore = kScoreTileIce2Factor*comboCount;
        tile->removeType(this->getBox()->m_tileBgBatch, kttIce2);
        
        GameParticle* jellyBreak = GameParticle::shareInstance();
        jellyBreak->playJellyBreak(this->getBox()->getLayer(), tile->getPosition());
    }
    else if(tile->getTileType() & kttIce)
    {
        addScore = kScoreTileIceFactor*comboCount;
        tile->removeType(this->getBox()->m_tileBgBatch, kttIce);
        
        GameParticle* jellyBreak = GameParticle::shareInstance();
        jellyBreak->playJellyBreak(this->getBox()->getLayer(), tile->getPosition());
    }
    
    /*switch (tile->getTileType()) {
        case kttIce:
        {
            addScore = kScoreTileIceFactor*comboCount;
            tile->removeType(this->getBox()->m_tileBgBatch, kttIce);
            
            GameParticle* jellyBreak = GameParticle::shareInstance();
            jellyBreak->playJellyBreak(this->getBox()->getLayer(), tile->getPosition());
            
            break;
        }
        case kttIce2:
        {
            addScore = kScoreTileIce2Factor*comboCount;
            tile->removeType(this->getBox()->m_tileBgBatch, kttIce2);
            
            GameParticle* jellyBreak = GameParticle::shareInstance();
            jellyBreak->playJellyBreak(this->getBox()->getLayer(), tile->getPosition());
            break;
        }
        case kttStone:
        {
            addScore = kScoreTileIceStone*comboCount;
            tile->removeType(this->getBox()->m_tileBgBatch, kttStone);
            
            break;
        }
        default:
            break;
    }*/
    if (addScore > 0) {
        this->getScoreCenter()->showPromptScore(addScore, tile->getPosition());
        int curScore = this->getScoreCenter()->getCurrentScore()+addScore;
        this->getScoreCenter()->setCurrentScore(curScore);
    }
}

void GridBll::_beforeItemRemove(CCSprite* sprite)
{
    GameParticle* particleSnow = GameParticle::shareInstance();
    particleSnow->playSnowDown(this->getBox()->getLayer(), sprite->getPosition());
}

void GridBll::_removeItemInTile(CCSprite *sprite)
{
    //tile->getFruitItem()->setItemValue(ktNothing);
    m_FruitBatch->removeChild(sprite, true);
    SimpleAudioEngine::sharedEngine()->playEffect("combo_sound1.wav", false);
}

// refill all the blank tile
int GridBll::_reFill()
{
    int maxColumnEmptyCount = 0;
    for (int i=0; i<kBoxWidth; i++) {
        int count = _reFillWithColumn(i);
        if (count>maxColumnEmptyCount) {
            maxColumnEmptyCount += count;
        }
    }
    
    return maxColumnEmptyCount;
}

int GridBll::_reFillWithColumn(int columnIndex)
{
    int columnEmptyCount = 0;
    //int canotPutCount = 0;
    //int totalMoveCount = 0;
    _isColumnFirstDown = true;
    
    // 没匹配项向下填补已匹配的空白
    for (int k = 0; k < kBoxWidth; k++) {
        // every blank tile check once
        columnEmptyCount = 0;
        bool isfillComp = false;
        
        for (int i=0; i<kBoxHeight; i++) {
            GridTile* curtile = getBox()->getTileAt(columnIndex, i);
            if (curtile->getFruitItem()->getItemValue() == ktNothing && curtile->getCanPutFruit()) {
                columnEmptyCount++;
                
                //totalMoveCount++;
            }
            else if(columnEmptyCount == 0  )
            {
                continue;
            }
            else if(!curtile->getCanPutFruit())
            {
                columnEmptyCount++;
                //totalMoveCount++;
            }
            else
            {
                //columnEmptyCount += canotPutCount;
                
                GridTile *forMoveTile = getBox()->getTileAt(columnIndex, i-columnEmptyCount);
                if (!forMoveTile->getCanPutFruit()) {
                    
                    continue;
                }
                
                //int realMoveCount = totalMoveCount-canotPutCount;
                CCMoveBy* fillAction = CCMoveBy::create(kMoveTileTime * columnEmptyCount, ccp(0, -kTileWidth*columnEmptyCount));
                
                CCFiniteTimeAction* actionSeq = CCSequence::create(fillAction, CCCallFunc::create(this, callfunc_selector(GridBll::_itemMovedCallback)), NULL);
                
                curtile->getFruitItem()->getItemBG()->runAction(actionSeq);
                forMoveTile->getFruitItem()->setItemBG(curtile->getFruitItem()->getItemBG());
                forMoveTile->getFruitItem()->setItemValue(curtile->getFruitItem()->getItemValue());
                forMoveTile->getFruitItem()->setItemClass(curtile->getFruitItem()->getItemClass());
                
                
                curtile->getFruitItem()->setItemValue(ktNothing);
                break;
            }
            if (i == kBoxHeight) isfillComp = true;
        }
        // all nonblank tile is moved, break than get more item to fill the leaved blank tile
        if (isfillComp) break;
    }
    
    
    // 补全上方的空白格
    for (int i = 0; i<columnEmptyCount; i++) {
        GridTile* emptyTile = getBox()->getTileAt(columnIndex, kBoxHeight-columnEmptyCount+i);
        if (!emptyTile->getCanPutFruit()) {
            _isColumnFirstDown = true;
            continue;
        }
        
        
        int tileValue = this->checkDropItem(kBoxHeight-columnEmptyCount+i, columnIndex);
        if (tileValue < 0) {
            tileValue = (arc4random()%(this->getScoreCenter()->getColorNum())+1);
        }
        
        //CCString* str = CCString::createWithFormat("q%d.png",tileValue);
        CCSprite* newItem = CCSprite::createWithSpriteFrameName(FruitCrush::shareSingleton()->getItemString(tileValue));
        newItem->setScale(kTileScaleFactor);
        newItem->setPosition(ccp(kHXStart + columnIndex * kTileWidth + kTileWidth/2, kHYStart + (kBoxHeight + i) * kTileWidth + kTileWidth/2));
        m_FruitBatch->addChild(newItem);
        CCMoveBy* fillAction = CCMoveBy::create(kMoveTileTime * columnEmptyCount, ccp(0, -kTileWidth*columnEmptyCount));
        
        CCFiniteTimeAction* actionSeq = CCSequence::create(fillAction, CCCallFunc::create(this, callfunc_selector(GridBll::_itemMovedCallback)), NULL);
        newItem->runAction(actionSeq);
        
        emptyTile->getFruitItem()->setItemBG(newItem);
        emptyTile->getFruitItem()->setItemValue(tileValue);
        //emptyTile->getFruitItem()->setItemClass(kcinormal);
        if(tileValue<8) emptyTile->getFruitItem()->setItemClass(kcinormal);
        else emptyTile->getFruitItem()->setItemClass(kciDrop);
    }
    
    return columnEmptyCount;
}


void GridBll::_itemMovedCallback()
{
    //sound
    if (_isColumnFirstDown) {
        SimpleAudioEngine::sharedEngine()->playEffect("candy_land3.wav", false);
    }
    _isColumnFirstDown = false;
    
}

bool GridBll::_matchedDropItem()
{
    bool isExistDropItem = false;
    for (int tr=0; tr<this->getBox()->getGridTiles()->count(); tr++) {
        CCArray* rowTiles = (CCArray*)this->getBox()->getGridTiles()->objectAtIndex(tr);
        for (int td=0; td<rowTiles->count(); td++) {
            GridTile* tile = (GridTile*)rowTiles->objectAtIndex(td);
            if ( (tile->getTileType() & kttDropEnd) && tile->getFruitItem() && tile->getFruitItem()->getItemClass() == kciDrop) {
                CCAction* removeAct = CCSequence::create(CCDelayTime::create(kDelayTime),CCCallFuncN::create(this, callfuncN_selector(GridBll::_beforeItemRemove)),CCScaleTo::create(kDisapearTime, 0.0f), CCCallFuncN::create(this, callfuncN_selector(GridBll::_removeItemInTile)), NULL);
                
                //
                if (tile->getFruitItem()->getItemBG()) {
                    tile->getFruitItem()->getItemBG()->runAction(removeAct);
                    tile->getFruitItem()->setItemBG(NULL);
                }
                tile->getFruitItem()->setItemValue(ktNothing);
                isExistDropItem = true;
            }
            
        }
    }
    return isExistDropItem;
}

void GridBll::_matchDone()
{
    // Reset tiles trans tag
    this->getBox()->resetTilesTag();
    if(matchNormal()) return;
    if (_haveMoreMatchCheck()) {
        this->getBox()->setLock(false);
    }
}

bool GridBll::_haveMoreMatchCheck()
{
    // reset combo number
    _currentCombo = 0;
    
    //_stopPromptSchedule();
    this->m_ResetPromptTiles();
    CCArray* _promptTiles = this->getBox()->getPromptTiles();
    
    bool isMatched = false;
    for (int y=0; y<kBoxHeight; y++)
    {
		for (int x=0; x<kBoxWidth; x++)
        {
            GridTile* aTile = this->getBox()->getTileAt(x, y);
            
            // filter can not put fruit tile and drop item tile
            if(!aTile->getCanPutFruit() || (aTile->getFruitItem() && aTile->getFruitItem()->getItemClass() == kciDrop)) continue;
			//v 1 2
            do {
                if (aTile->getYIndex()-1 >= 0) {
                    GridTile* bTile = this->getBox()->getTileAt(x, y-1);
                    CC_BREAK_IF(!bTile->getCanPutFruit());
                    
                    if (bTile && aTile->getFruitItem()->getItemValue() == bTile->getFruitItem()->getItemValue()) {
                        GridTile* tmpTile = this->getBox()->getTileAt(x, y+1);
                        if (tmpTile && tmpTile->getCanPutFruit()) {
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x, y+2);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x-1, y+1);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x+1, y+1);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                        }
                        tmpTile = this->getBox()->getTileAt(x, y-2);
                        if (tmpTile && tmpTile->getCanPutFruit())
                        {
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x,y-3);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x-1, y-2);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x+1, y-2);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                        }
                    }
                }
            } while (0);
            
            //v 1 3
            do {                
                if (aTile->getYIndex()-2 >= 0) {
                    GridTile *bTile = this->getBox()->getTileAt(x, y-2);
                    CC_BREAK_IF(!bTile->getCanPutFruit());
                    if (bTile && aTile->getFruitItem()->getItemValue() == bTile->getFruitItem()->getItemValue())
                    {
                        GridTile* tmpTile = this->getBox()->getTileAt(x, y-1);
                        if (tmpTile && tmpTile->getCanPutFruit())
                        {
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x, y+1);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x,y-3);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x-1, y-1);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x+1, y-1);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                        }
                    }
                }
            } while (0);
			
			
			// h 1 2
            do {
                if (aTile->getXIndex()+1 < kBoxWidth) {
                    GridTile* bTile = this->getBox()->getTileAt(x+1, y);
                    CC_BREAK_IF(!bTile->getCanPutFruit());
                    if (bTile && aTile->getFruitItem()->getItemValue() == bTile->getFruitItem()->getItemValue()) {
                        GridTile* tmpTile = this->getBox()->getTileAt(x-1, y);
                        if (tmpTile && tmpTile->getCanPutFruit())
                        {
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x-2, y);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x-1, y-1);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x-1, y+1);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                        }
                        
                        tmpTile = this->getBox()->getTileAt(x+2, y);
                        if (tmpTile && tmpTile->getCanPutFruit())
                        {
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x+3, y);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x+2, y-1);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x+2, y+1);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                        }
                    }
                }
            } while (0);
			
			//h 1 3
            do {
                if (aTile->getXIndex()+2 < kBoxWidth) {
                    GridTile* bTile = this->getBox()->getTileAt(x+2, y);
                    CC_BREAK_IF(!bTile->getCanPutFruit());
                    if (bTile && aTile->getFruitItem()->getItemValue() == bTile->getFruitItem()->getItemValue())
                    {
                        GridTile* tmpTile = this->getBox()->getTileAt(x+1, y);
                        if (tmpTile && tmpTile->getCanPutFruit())
                        {
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x+3, y);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x-1, y);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x+1, y-1);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                            {
                                GridTile* cTile = this->getBox()->getTileAt(x+1, y+1);
                                if (cTile && cTile->getFruitItem()->getItemValue() == aTile->getFruitItem()->getItemValue() && cTile->getCanPutFruit())
                                {
                                    isMatched = true;
                                    CCArray* aComb = CCArray::createWithCapacity(3);
                                    aComb->addObject(aTile);
                                    aComb->addObject(bTile);
                                    aComb->addObject(cTile);
                                    _promptTiles->addObject(aComb);
                                }
                            }
                        }
                    }
                }
            } while (0);
			
		}
	}
    
    if(isMatched) _startPromptSchedule();
    
    return isMatched;
}

void GridBll::m_ResetPromptTiles()
{
    if (!this->getBox()->getPromptTiles()) {
        CCArray* _promptTiles = CCArray::createWithCapacity(kBoxWidth);
        this->getBox()->setPromptTiles(_promptTiles);
        return;
    }
    for (int i=0; i < this->getBox()->getPromptTiles()->count(); i++) {
        CCArray* aComb = (CCArray*)this->getBox()->getPromptTiles()->objectAtIndex(i);
        aComb->removeAllObjects();
    }
    this->getBox()->getPromptTiles()->removeAllObjects();
    
}

void GridBll::_startPromptSchedule()
{
    this->getBox()->getLayer()->schedule(schedule_selector(GameLayer::promptMatch), 5, 0, 0);
    
}

void GridBll::_stopPromptSchedule()
{
    this->getBox()->unschedule(schedule_selector(GameLayer::promptMatch));
}

/*void GridBll::m_MatchPrompt()
{
    
    if(! this->getBox()->getPromptTiles() || this->getBox()->getPromptTiles()->count()==0) return;
    int count = this->getBox()->getPromptTiles()->count();
    int randomSeed = (arc4random()%count+1);
    CCArray* aComb = (CCArray*)this->getBox()->getPromptTiles()->objectAtIndex(randomSeed);
    if (aComb->count() <= 1) return;
    
    GridTile* aTile = (GridTile*)aComb->objectAtIndex(0);
    GridTile* bTile = (GridTile*)aComb->objectAtIndex(1);
    
    aTile->runAction(ActionEffectHub::shareInstance()->playSlink());
    bTile->runAction(ActionEffectHub::shareInstance()->playSlink());
    this->getBox()->getLayer()->unschedule(schedule_selector(GridBll::m_MatchPrompt));
}*/

// ReInit grid item if no matched condition exists
void GridBll::reInitGrid()
{
    int tileValue;
    // remove all item
    for (int tr=0; tr<this->getBox()->getGridTiles()->count(); tr++) {
        CCArray* rowTiles = (CCArray*)this->getBox()->getGridTiles()->objectAtIndex(tr);
        for (int td=0; td<rowTiles->count(); td++) {
            GridTile *tile =(GridTile*) rowTiles->objectAtIndex(td);
            
            // check drop item
            if (tile->getFruitItem()->getItemClass() == kciDrop) {
                continue;
            }
            
            tile->getFruitItem()->setItemBG(NULL);
            tile->getFruitItem()->setItemClass(kcinormal);
            tile->getFruitItem()->setItemValue(ktNothing);
            tile->setFruitItem(NULL);
        }
    }
    
    // reFill items
    for (int tr=0; tr<this->getBox()->getGridTiles()->count(); tr++) {
        CCArray* rowTiles = (CCArray*)this->getBox()->getGridTiles()->objectAtIndex(tr);
        for (int td=0; td<rowTiles->count(); td++) {
            GridTile *tile =(GridTile*) rowTiles->objectAtIndex(td);
            
            // check drop item
            if (tile->getFruitItem() && tile->getFruitItem()->getItemClass() == kciDrop) {
                continue;
            }
            
            GridItem *item = GridItem::create();
            tile->setFruitItem(item);
            if (!tile->getCanPutFruit()) {
                tile->getFruitItem()->setItemValue(ktNothing);
                continue;
            }
            
            tileValue = (arc4random()%(this->getScoreCenter()->getColorNum())+1);
            //CCString* name = CCString::createWithFormat("q%d.png",tileValue);
            CCSprite *fruitbg = CCSprite::createWithSpriteFrameName(FruitCrush::shareSingleton()->getItemString(tileValue));
            fruitbg->setScale(kTileScaleFactor);
            fruitbg->setPosition(ccp(kHXStart + td * kTileWidth + kTileWidth/2, kHYStart + tr * kTileWidth+kTileWidth/2));
            
            tile->getFruitItem()->setItemValue(tileValue);
            tile->getFruitItem()->setItemBG(fruitbg);
            m_FruitBatch->addChild(tile->getFruitItem()->getItemBG());
        }
    }
    
    // 待消除的水果
    CCArray* readyForRemove = CCArray::createWithCapacity(kBoxHeight);
    this->getBox()->setReadyForRemoveTiles(readyForRemove);
    
    _checkInit();
}