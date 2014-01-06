//
//  GridTile.cpp
//  fruitCrush
//
//  Created by Tsai on 13-10-23.
//
//

#include "GridTile.h"
static CCSpriteBatchNode* _bgNode = NULL;

GridTile::~GridTile()
{
    setFruitItem(NULL);
    if(_tileSprites) _tileSprites->removeAllObjects();
    setTileSprites(NULL);
    _bgNode->release();
}

bool GridTile::init()
{
    
    return true;
}

CCSpriteBatchNode* GridTile::tileBgBatchSingleton()
{
    if (_bgNode == NULL) {
        _bgNode = CCSpriteBatchNode::createWithTexture(
                                                       CCTextureCache::sharedTextureCache()->textureForKey("game_grid.png")
                                                       , 81);
        _bgNode->retain();
    }
    return _bgNode;
}

bool GridTile::initWithType(int type, CCSpriteBatchNode* batchNode, const CCPoint& pos)
{
    this->setTileType(type);
    if (type == kttNone) {
        this->setTileSprites(NULL);
        this->setCanPutFruit(false);
        return true;
    }
    
    CCDictionary* tileSprites = CCDictionary::create();
    this->setTileSprites(tileSprites);
    if (type & kttNormal) {
        CCSprite* sprite = CCSprite::createWithTexture(
                                    CCTextureCache::sharedTextureCache()->textureForKey("game_grid.png"),
                                                       CCRect(170, 150, 70, 70));
        sprite->setScale(0.9);
        sprite->setPosition(pos);
        batchNode->addChild(sprite);
        
        tileSprites->setObject(sprite, kttNormal);
        this->setCanPutFruit(true);
        
    }
    if (type & kttIce) {
        CCSprite* sprite = CCSprite::createWithTexture(
                                                       CCTextureCache::sharedTextureCache()->textureForKey("game_grid.png"),
                                                       CCRect(0, 0, 125, 115));
        sprite->setScale(0.54);
        sprite->setPosition(pos);
        batchNode->addChild(sprite);
        tileSprites->setObject(sprite, kttIce);
        this->setCanPutFruit(true);
    }
    if (type & kttIce2) {
        CCSprite* sprite = CCSprite::createWithTexture(
                                                       CCTextureCache::sharedTextureCache()->textureForKey("game_grid.png"),
                                                       CCRect(0, 250, 125, 115));
        sprite->setScale(0.54);
        sprite->setPosition(pos);
        batchNode->addChild(sprite);
        tileSprites->setObject(sprite, kttIce2);
        this->setCanPutFruit(true);
    }
    if (type & kttStone) {
        CCSprite* sprite = CCSprite::createWithTexture(
                                                       CCTextureCache::sharedTextureCache()->textureForKey("game_grid.png"),
                                                       CCRect(0, 240, 125, 115));
        
        /*CCSprite *sprite = CCSprite::createWithSpriteFrameName(FruitCrush::shareSingleton()->getItemString(999));
        sprite->setScale(kTileScaleFactor);
        this->getTileSprites()->setObject(sprite, kttStone);
        this->setCanPutFruit(false);*/
        sprite->setScale(0.54);
        sprite->setPosition(pos);
        batchNode->addChild(sprite);
        tileSprites->setObject(sprite, kttStone);
        this->setCanPutFruit(false);
    }
    if (type & kttDropEnd) {
        
    }
    
    return true;
}

void GridTile::changeType(int type)
{
    if (this->getTileType() == type) return;
    if (type == kttNone) {
        if(this->getTileSprites()) this->getTileSprites()->removeAllObjects();
        this->setTileSprites(NULL);
        this->setCanPutFruit(false);
        return;
    }
    if (!this->getTileSprites())
    {
        CCDictionary* tileSprites = CCDictionary::create();
        this->setTileSprites(tileSprites);
    }
    // kttNormal
    if ((this->getTileType() ^ type) & kttNormal) {
        // if not exist kttNormal sprite
        if(this->getTileSprites() && !this->getTileSprites()->objectForKey(kttNormal))
        {
            CCSprite* sprite = CCSprite::createWithTexture(
                                                           CCTextureCache::sharedTextureCache()->textureForKey("game_grid.png"),
                                                           CCRect(170, 150, 70, 70));
            sprite->setScale(0.9);
            this->getTileSprites()->setObject(sprite, kttNormal);
            this->setCanPutFruit(true);
        }
    }
    // kttIce
    if ((this->getTileType() ^ type) & kttIce) {
        if(this->getTileSprites() && !this->getTileSprites()->objectForKey(kttIce))
        {
            CCSprite* sprite = CCSprite::createWithTexture(
                                                           CCTextureCache::sharedTextureCache()->textureForKey("game_grid.png"),
                                                           CCRect(0, 0, 125, 115));
            sprite->setScale(0.54);
            this->getTileSprites()->setObject(sprite, kttIce);
            this->setCanPutFruit(true);
        }
    }
    // kttIce2
    if ((this->getTileType() ^ type) & kttIce2) {
        if(this->getTileSprites() && !this->getTileSprites()->objectForKey(kttIce2))
        {
            CCSprite* sprite = CCSprite::createWithTexture(
                                                           CCTextureCache::sharedTextureCache()->textureForKey("game_grid.png"),
                                                           CCRect(0, 250, 125, 115));
            sprite->setScale(0.54);
            this->getTileSprites()->setObject(sprite, kttIce2);
            this->setCanPutFruit(true);
        }
    }
    // kttStone
    if ((this->getTileType() ^ type) & kttStone) {
        if(this->getTileSprites() && !this->getTileSprites()->objectForKey(kttStone))
        {
            //CCSprite* sprite = CCSprite::createWithTexture(
             //                                              CCTextureCache::sharedTextureCache()->textureForKey("game_grid.png"),
             //                                              CCRect(0, 240, 125, 115));
            CCSprite *sprite = CCSprite::createWithSpriteFrameName(FruitCrush::shareSingleton()->getItemString(999));
            sprite->setScale(kTileScaleFactor);
            this->getTileSprites()->setObject(sprite, kttStone);
            this->setCanPutFruit(false);
        }
    }
    
    this->setTileType(type);
    
    // change type sprites
    /*switch (type) {
        case kttNone:
        {
            //            CCSprite* sprite = CCSprite::create();
            //sprite->setOpacity(127);
            this->setTileSprite(NULL);
            this->setCanPutFruit(false);
            break;
        }
        case kttNormal:
        {
            CCSprite* sprite = //CCSprite::create("game_grid.png", CCRect(0, 0, 100, 100));
            CCSprite::createWithTexture(
                                        CCTextureCache::sharedTextureCache()->textureForKey("game_grid.png"),
                                        CCRect(170, 150, 70, 70));
            //sprite->setOpacity(127);
            CCPoint pos = this->getTileSprite()->getPosition();
            sprite->setPosition(pos);
            sprite->setScale(0.9);
            this->setTileSprite(sprite);
            this->setCanPutFruit(true);
            break;
        }
        case kttIce:
        {
            CCSprite* sprite = //CCSprite::create("tileIce.png");
            CCSprite::createWithTexture(
                                        CCTextureCache::sharedTextureCache()->textureForKey("game_grid.png"),
                                        CCRect(0, 0, 125, 115));
            //sprite->setOpacity(127);
            CCPoint pos = this->getTileSprite()->getPosition();
            sprite->setPosition(pos);
            sprite->setScale(0.54);
            this->setTileSprite(sprite);
            this->setCanPutFruit(true);
            break;
        }
        case kttIce2:
        {
            CCSprite* sprite = //CCSprite::create("tileIce.png");
            CCSprite::createWithTexture(
                                        CCTextureCache::sharedTextureCache()->textureForKey("game_grid.png"),
                                        CCRect(0, 250, 125, 115));
            //sprite->setOpacity(127);
            CCPoint pos = this->getTileSprite()->getPosition();
            sprite->setPosition(pos);
            
            sprite->setScale(0.54);
            this->setTileSprite(sprite);
            this->setCanPutFruit(false);
            break;
        }
        case kttStone:
        {
            CCSprite* sprite = //CCSprite::create("tileIce.png");
            CCSprite::createWithTexture(
                                        CCTextureCache::sharedTextureCache()->textureForKey("game_grid.png"),
                                        CCRect(0, 240, 125, 115));
            //sprite->setOpacity(127);
            CCPoint pos = this->getTileSprite()->getPosition();
            sprite->setPosition(pos);
            
            sprite->setScale(0.54);
            this->setTileSprite(sprite);
            this->setCanPutFruit(false);
            break;
        }
        default:
            break;
    }*/
}

void GridTile::removeType(CCSpriteBatchNode* batchNode, TypeTile type)
{
    if ( !(this->getTileType() & type) || !this->getTileSprites() ) return;
    
    this->setTileType((TypeTile)(this->getTileType() ^ type));
    
    switch (type) {
        case kttNormal:
        {
            // if not exist kttNormal sprite
            if(this->getTileSprites() && this->getTileSprites()->objectForKey(kttNormal))
            {
                CCSprite* sprite = (CCSprite*)this->getTileSprites()->objectForKey(kttNormal);
                batchNode->cocos2d::CCNode::removeChild(sprite);
                this->getTileSprites()->removeObjectForKey(kttNormal);
                this->setCanPutFruit(false);
            }
            break;
        }
        case kttIce:
        {
            if(this->getTileSprites() && this->getTileSprites()->objectForKey(kttIce))
            {
                CCSprite* sprite = (CCSprite*)this->getTileSprites()->objectForKey(kttIce);
                batchNode->cocos2d::CCNode::removeChild(sprite);
                this->getTileSprites()->removeObjectForKey(kttIce);
                this->setCanPutFruit(true);
            }
            break;
        }
        case kttIce2:
        {
            if(this->getTileSprites() && this->getTileSprites()->objectForKey(kttIce2))
            {
                CCSprite* sprite = (CCSprite*)this->getTileSprites()->objectForKey(kttIce2);
                batchNode->cocos2d::CCNode::removeChild(sprite);
                this->getTileSprites()->removeObjectForKey(kttIce2);
                this->setCanPutFruit(true);
            }
            break;
        }
        case kttStone:
        {
            if(this->getTileSprites() && this->getTileSprites()->objectForKey(kttStone))
            {
                CCSprite* sprite = (CCSprite*)this->getTileSprites()->objectForKey(kttStone);
                batchNode->cocos2d::CCNode::removeChild(sprite);
                this->getTileSprites()->removeObjectForKey(kttStone);
                this->setCanPutFruit(true);
            }
            break;
        }
        default:
            break;
    }
}

bool GridTile::isNearWith(GridTile *tile)
{
    if(!tile) return false;
    return
    (_xIndex == tile->getXIndex() && abs(_yIndex - tile->getYIndex())==1)||
	(_yIndex == tile->getYIndex() && abs(_xIndex - tile->getXIndex())==1);
}

bool GridTile::transWith(GridTile *tile)
{
    if (!this->getFruitItem()->getItemBG() || !tile->getFruitItem()->getItemBG()) return false;
    CCSprite* tempSprite = this->getFruitItem()->getItemBG();
    int tmpvalue = this->getFruitItem()->getItemValue();
    ClassItemSprite tmpClass = this->getFruitItem()->getItemClass();
    this->getFruitItem()->setItemBG(tile->getFruitItem()->getItemBG());
    this->getFruitItem()->setItemValue(tile->getFruitItem()->getItemValue());
    this->getFruitItem()->setItemClass(tile->getFruitItem()->getItemClass());
    tile->getFruitItem()->setItemBG(tempSprite);
    tile->getFruitItem()->setItemValue(tmpvalue);
    tile->getFruitItem()->setItemClass(tmpClass);
    return true;
}

void GridTile::stopItemActionByTag(int tag)
{
    this->getFruitItem()->getItemBG()->stopActionByTag(tag);
    this->getFruitItem()->getItemBG()->setScale(1.0f);
}