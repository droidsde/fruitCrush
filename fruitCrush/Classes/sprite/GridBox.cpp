//
//  GridBox.cpp
//  fruitCrush
//
//  Created by Tsai on 13-10-23.
//
//

#include "GridBox.h"

//CC_SYNTHESIZE(bool, _lock, Lock);

GridBox::~GridBox()
{
    if (this->getGridTiles()) {
        for (int tr=0; tr<this->getGridTiles()->count(); tr++) {
            CCArray* rowTiles = (CCArray*)this->getGridTiles()->objectAtIndex(tr);
            rowTiles->removeAllObjects();
        }
        setGridTiles(NULL);
    }
    
    if (this->getReadyForRemoveTiles()) {
        for (int tr=0; tr<this->getReadyForRemoveTiles()->count(); tr++) {
            CCArray* rowTiles = (CCArray*)this->getReadyForRemoveTiles()->objectAtIndex(tr);
            rowTiles->removeAllObjects();
        }
        setReadyForRemoveTiles(NULL);
    }
    
    if (this->getPromptTiles()) {
        for (int tr=0; tr<this->getPromptTiles()->count(); tr++) {
            CCArray* rowTiles = (CCArray*)this->getPromptTiles()->objectAtIndex(tr);
            rowTiles->removeAllObjects();
        }
        setPromptTiles(NULL);
    }
    
    
    setLayer(NULL);
    
}

bool GridBox::init()
{
    return true;
}

bool GridBox::initWithMap(int mapNo)
{
    CCString* mapNoStr = CCString::createWithFormat("map%02d.plist", mapNo);
    CCDictionary* pdictRoot = CCDictionary::createWithContentsOfFile(mapNoStr->getCString());
    CCDictionary* pdict = (CCDictionary*)pdictRoot->objectForKey("tileMap");
    int width;
    int height = pdict->count();
    // init gridTiles
    CCArray *gridTiles = CCArray::createWithCapacity(height);
    this->m_tileBgBatch = CCSpriteBatchNode::createWithTexture(
                                                               CCTextureCache::sharedTextureCache()->textureForKey("game_grid.png")
                                                               , 81);
    
    CCString *trNo;
    CCString *tdNo;
    for (int tr=0; tr<height; tr++) {
        trNo = CCString::createWithFormat("tr%u", tr);
        CCDictionary *trData = (CCDictionary*)pdict->objectForKey(trNo->getCString());
        width = trData->count();
        CCArray* rowTiles = CCArray::createWithCapacity(width);
        
        for (int td=0; td<width; td++) {
            tdNo = CCString::createWithFormat("td%u", td);
            CCString* str = (CCString*)trData->objectForKey(tdNo->getCString());
            CCPoint curPoint = ccp(kHXStart + td * kTileWidth + kTileWidth/2, kHYStart + tr * kTileWidth+kTileWidth/2);
            
            printf("ceshi%s+%u+%u\n",str->getCString(),tr, td);
            
            GridTile *tile = GridTile::create();
            tile->setXIndex(td);
            tile->setYIndex(tr);
            tile->setPosition(curPoint);
            tile->initWithType((TypeTile)(atoi(str->getCString())), this->m_tileBgBatch, curPoint);
            rowTiles->addObject(tile);
        }
        gridTiles->addObject(rowTiles);
    }
    this->setGridTiles(gridTiles);

    
    this->getLayer()->addChild(m_tileBgBatch);
    //this->_initUI();

    return true;
}

void GridBox::_initUI()
{
    for (int tr=0; tr<this->getGridTiles()->count(); tr++) {
        CCArray* rowTiles = (CCArray*)this->getGridTiles()->objectAtIndex(tr);
        for (int td=0; td<rowTiles->count(); td++) {
            GridTile *tile =(GridTile*) rowTiles->objectAtIndex(td);
            CCPoint curPoint = ccp(kHXStart + td * kTileWidth + kTileWidth/2, kHYStart + tr * kTileWidth+kTileWidth/2);
            tile->setPosition(curPoint);
            
            // tileSprites
            CCDictionary* tileSprites = tile->getTileSprites();
            if(!tileSprites) continue;
            CCArray* allKeys = tileSprites->allKeys();
            for (int i=0; i<tileSprites->count(); i++) {
                CCSprite* tilebg = (CCSprite*)tileSprites->objectForKey((int)allKeys->objectAtIndex(i));
                if(!tilebg) continue;
                tilebg->setPosition(curPoint);
                this->m_tileBgBatch->addChild(tilebg);
            }
            //CCSprite *tilebg = tile->getTileSprite();
            
            //tilebg->setPosition(curPoint);
            //this->getLayer()->addChild(tilebg);
            //this->m_tileBgBatch->addChild(tilebg);
        }
    }
}

void GridBox::resetTilesTag()
{
    for (int tr=0; tr<this->getGridTiles()->count(); tr++) {
        CCArray* rowTiles = (CCArray*)this->getGridTiles()->objectAtIndex(tr);
        for (int td=0; td<rowTiles->count(); td++) {
            GridTile *tile =(GridTile*) rowTiles->objectAtIndex(td);
            tile->setTag(KTagDefaultTile);
        }
    }
}

GridTile* GridBox::getTileAt(int posx, int posy)
{
    if (posx < 0 || posy < 0 || posx >= kBoxWidth || posy >= kBoxHeight) {
        return NULL;
    }
    return (GridTile*) ((CCArray*)this->getGridTiles()->objectAtIndex(posy))->objectAtIndex(posx);
}

void GridBox::changeTileTypeAt(TypeTile type, int row, int column)
{
    GridTile *tile = (GridTile*) ((CCArray*)this->getGridTiles()->objectAtIndex(column))->objectAtIndex(row);
    tile->changeType(type);
}


