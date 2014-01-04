//
//  LevelScene.cpp
//  fruitCrush
//
//  Created by Tsai Empire on 9/29/13.
//
//

#include "LevelScene.h"
#include "StartScene.h"
#include "GameScene.h"
#include "FruitCrush.h"
#include "SimpleAudioEngine.h"
#include "ResCenter.h"
USING_NS_CC;
using namespace CocosDenshion;

LevelScene::~LevelScene()
{
    //CC_SAFE_RELEASE(_levelBG);
    //CC_SAFE_RELEASE(_lifeCount);
    //CC_SAFE_RELEASE(_backSprite);
    //CC_SAFE_RELEASE(_mainMenu);
}

CCScene* LevelScene::scene()
{
    CCScene *scene = NULL;
    do {
        scene = CCScene::create();
        CC_BREAK_IF(!scene);
        
        LevelScene* layer = LevelScene::create();
        CC_BREAK_IF(!layer);
        
        scene->addChild(layer);
    } while (0);
    
    return scene;
}


bool LevelScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    //FruitCrush::shareSingleton()->getWinSize() = CCDirector::sharedDirector()->getWinSize();
    _levelBG = ResCenter::shareInstance()->getSpriteByType(ResCenter::kSpriteLevelScene);
    _levelBG->setAnchorPoint(ccp(0, 0));
    _levelBG->setPosition(ccp(0, 0));
    this->addChild(_levelBG);
    setTouchEnabled(true);
    
    _backSprite = CCSprite::create("Aleft.png");
    //_backSprite->setScale(1);
    _backSprite->setAnchorPoint(ccp(1, 1));
    _backSprite->setPosition(ccp(_backSprite->boundingBox().size.width, FruitCrush::shareSingleton()->getWinSize().height ));
    this->addChild(_backSprite);
    _backSprite->setTag(kTagback);
    
    CCSpriteBatchNode* batch = CCSpriteBatchNode::create("portrait_frame.png");
    this->addChild(batch);
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("candy_crush_intro2.mp3", true);
    
    // init map
    float top = 0;
    float left = 0;
    {
        for (int no = 1; no <= 5; no++) {
            CCSprite* map = addMapBtnWith(no, CCPointZero, batch);
            if( !left ) left = map->boundingBox().size.width;
            if(!top) top = FruitCrush::shareSingleton()->getWinSize().height - map->boundingBox().size.height*1.9f;
            map->setAnchorPoint(ccp(0, 0));
            map->setPosition(ccp(left, top));
            
            left += map->boundingBox().size.width * 2.1f;
            if (no == 5) {
                top -= map->boundingBox().size.height*1.5f;
            }
        }
    }
    
    {
        left = 0;
        for (int no = 6; no <= 10; no++) {
            CCSprite* map = addMapBtnWith(no, CCPointZero, batch);
            if( !left ) left = map->boundingBox().size.width;
            if(!top) top = FruitCrush::shareSingleton()->getWinSize().height - map->boundingBox().size.height*1.9f;
            map->setAnchorPoint(ccp(0, 0));
            map->setPosition(ccp(left, top));
            
            left += map->boundingBox().size.width * 2.1f;
            if (no == 10) {
                top -= map->boundingBox().size.height*1.5f;
            }
        }
    }
    
    {
        left = 0;
        for (int no = 11; no <= 15; no++) {
            CCSprite* map = addMapBtnWith(no, CCPointZero, batch);
            if( !left ) left = map->boundingBox().size.width;
            if(!top) top = FruitCrush::shareSingleton()->getWinSize().height - map->boundingBox().size.height*1.9f;
            map->setAnchorPoint(ccp(0, 0));
            map->setPosition(ccp(left, top));
            
            left += map->boundingBox().size.width * 2.1f;
            if (no == 15) {
                top -= map->boundingBox().size.height*1.5f;
            }
        }
    }
    
    {
        left = 0;
        for (int no = 16; no <= 20; no++) {
            CCSprite* map = addMapBtnWith(no, CCPointZero, batch);
            if( !left ) left = map->boundingBox().size.width;
            if(!top) top = FruitCrush::shareSingleton()->getWinSize().height - map->boundingBox().size.height*1.9f;
            map->setAnchorPoint(ccp(0, 0));
            map->setPosition(ccp(left, top));
            
            left += map->boundingBox().size.width * 2.1f;
            if (no == 20) {
                top -= map->boundingBox().size.height*1.5f;
            }
        }
    }
    
    return true;
}

CCSprite* LevelScene::addMapBtnWith(int no, cocos2d::CCPoint pos, CCSpriteBatchNode* batch)
{
    CCSprite *map = NULL;
    CCSprite *lockSprite = NULL;
    CCLabelAtlas *mapNo = CCLabelAtlas::create("0123456789", "fonts_score_002-hd.png", 38, 44, '0');

    if (FruitCrush::shareSingleton()->canPlayMap(no)) {
        map = CCSprite::createWithTexture(batch->getTexture());
        map->setTag(no);
        //map->setScale(0.6);
        CCString* num = CCString::createWithFormat("%u", no);
        mapNo->setString(num->getCString());
        //mapNo->setColor(ccc3(242, 247, 240));
        mapNo->setAnchorPoint(ccp(0.5f,0.5f));
        mapNo->setPosition(ccp(map->boundingBox().size.width/2, map->boundingBox().size.height/2));
        map->addChild(mapNo);
        //this->addChild(mapNo);
    }
    else
    {
        
    }
    
    map->setPosition(pos);
    map->setScale(0.8);
    this->addChild(map);
    return map;
    
}

void LevelScene::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    

    
}

void LevelScene::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCTouch *pTouch = (CCTouch*)pTouches->anyObject();
    CCPoint touchLocation = pTouch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    CCPoint local = convertToNodeSpace(touchLocation);
    
    /*for (CCSetIterator iterTouch = pTouches->begin(); iterTouch != pTouches->end(); iterTouch++) {
        CCTouch *pcurTouch = (CCTouch*)(*iterTouch);
        CCPoint pcurPoint = pcurTouch->getLocationInView();
        pcurPoint = CCDirector::sharedDirector()->convertToGL(pcurPoint);
        CCPoint curLoc = convertToNodeSpace(pcurPoint);
        
    }*/
    
    CCArray* childArr = getChildren();
    for (int i=0; i<getChildrenCount(); i++) {
        CCSprite *sprite = (CCSprite*)childArr->objectAtIndex(i);
        if (sprite->getTag() <= 0 ) continue;
        CCRect spriteRect = sprite->boundingBox();
        
        // back
        if (sprite->getTag() == kTagback && spriteRect.containsPoint(local)) {
            sprite->setColor(ccc3(222, 43, 54));
            SimpleAudioEngine::sharedEngine()->playEffect("button_press.wav");
            CCScene *pScene = StartScene::scene();
            CCDirector::sharedDirector()->replaceScene(CCTransitionSplitRows::create(1, pScene));
            return;
        }
        
        // map no
        if (spriteRect.containsPoint(local)) {
            sprite->setColor(ccc3(222, 43, 54));
            SimpleAudioEngine::sharedEngine()->playEffect("button_press.wav");
            CCDirector::sharedDirector()->replaceScene(CCTransitionSplitRows::create(1, GameScene::sceneWithMap(sprite->getTag())));
            return;
        }
        
    }
    
    
    // back button
    /*CCRect rect1 = _backSprite->boundingBox();
    bool flag = rect1.containsPoint(local);
    if (flag) {
        CCScene *pScene = StartScene::scene();
        CCDirector::sharedDirector()->replaceScene(CCTransitionSplitRows::create(1, pScene));
    }*/
}

void LevelScene::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    
}


