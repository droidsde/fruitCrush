//
//  GridBll.cpp
//  fruitCrush
//
//  Created by Tsai Empire on 10/29/13.
//
//

#include "GridBll.h"

bool GridBll::init()
{
    /*if (!_layer) {
        return false;
    }*/
    return true;
}

void GridBll::initWithMap(int mapNo)
{
    this->getBox()->initWithMap(mapNo);
}

void GridBll::initWithFruit()
{
    int tileValue;
    for (int tr=0; tr<this->getBox()->getGridTiles()->count(); tr++) {
        CCArray* rowTiles = (CCArray*)this->getBox()->getGridTiles()->objectAtIndex(tr);
        for (int td=0; td<rowTiles->count(); td++) {
            tileValue = (arc4random()%kTotalType+1);
            GridTile *tile =(GridTile*) rowTiles->objectAtIndex(td);
            GridItem *item = GridItem::create();
            tile->setFruitItem(item);
            if (!tile->getCanPutFruit()) {
                tile->getFruitItem()->setItemValue(ktNothing);
                continue;
            }
            
            CCString* name = CCString::createWithFormat("q%d.png",tileValue);
            CCSprite *fruitbg = CCSprite::create(name->getCString());
            
            fruitbg->setPosition(ccp(kHXStart + td * kTileWidth + kTileWidth/2, kHYStart + tr * kTileWidth+kTileWidth/2));
            
            tile->getFruitItem()->setItemValue(tileValue);
            tile->getFruitItem()->setItemBG(fruitbg);
            this->getBox()->getLayer()->addChild(tile->getFruitItem()->getItemBG());
        }
    }
    
    // 待消除的水果
    CCArray* readyForRemove = CCArray::createWithCapacity(kBoxHeight);
    this->getBox()->setReadyForRemoveTiles(readyForRemove);
    
    _checkInit();
}

void GridBll::setLayer(cocos2d::CCLayer *layer)
{
    this->setBox(GridBox::create());
    this->getBox()->setLayer(layer);
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
            CCPoint curPos = tile->getFruitItem()->getItemBG()->getPosition();
            //remove older sprite
            this->getBox()->getLayer()->removeChild(tile->getFruitItem()->getItemBG());
            
            int tileValue = (arc4random()%kTotalType+1);
            CCString* name = CCString::createWithFormat("q%d.png",tileValue);
            CCSprite *fruitbg = CCSprite::create(name->getCString());
            
            // add new sprite
            tile->getFruitItem()->setItemBG(fruitbg);
            tile->getFruitItem()->setItemValue(tileValue);
            tile->getFruitItem()->getItemBG()->setPosition(curPos);
            this->getBox()->getLayer()->addChild(tile->getFruitItem()->getItemBG());
        }
        tiles->removeAllObjects();
    }
    this->getBox()->getReadyForRemoveTiles()->removeAllObjects();
    _checkInit();
}

bool GridBll::matchNormal()
{
    matchWith(kOrientationH);
    matchWith(kOrientationV);
    int removeCount = this->getBox()->getReadyForRemoveTiles()->count();
    
    if (removeCount == 0) return false;
    //CCArray* matchedTTiles = getTMatchTiles(this->getBox()->getReadyForRemoveTiles());
    
    for (int i=0; i<removeCount; i++) {
        CCArray* aMatchedTiles = (CCArray*)this->getBox()->getReadyForRemoveTiles()->objectAtIndex(i);
        for (int j=0; j<aMatchedTiles->count(); j++) {
            GridTile* tile = (GridTile*)aMatchedTiles->objectAtIndex(j);
            CCAction* removeAct = CCSequence::create(CCScaleTo::create(0.2f, 0.0f), CCCallFuncN::create(this, callfuncN_selector(GridBll::_removeItemInTile)), NULL);
            tile->runAction(removeAct);
            
        }
        aMatchedTiles->removeAllObjects();
    }
    this->getBox()->getReadyForRemoveTiles()->removeAllObjects();
    
    
    return true;
}

bool GridBll::matchSuper()
{
    return true;
}

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
            
            if (tile->getFruitItem()->getItemValue() == value && tile->getFruitItem()->getItemValue() != ktNothing) {
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

TypeMatch GridBll::getMatchType(cocos2d::CCArray *matchTiles)
{
    if (matchTiles == NULL) return kMatchNothing;
    int count = matchTiles->count();
    switch (count) {
        case 3:
            return kMatch3;
            break;
        case 4:
            return kMatch4;
            break;
        case 5:
            return kMatch5;
            break;
        
        default:
            break;
    }
    return kMatchNothing;
}

CCArray* GridBll::getTMatchTiles(cocos2d::CCArray *readyForRemoveTiles)
{
    if (NULL == readyForRemoveTiles || readyForRemoveTiles->count() == 0) return NULL;
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

void GridBll::_removeItemInTile(GridTile *tile)
{
    tile->getFruitItem()->setItemValue(ktNothing);
    this->getBox()->getLayer()->removeChild(tile->getFruitItem()->getItemBG());
}

void GridBll::_reFill()
{
    
}

void GridBll::_reFillWithColumn(int columnIndex)
{
    
}