//
//  LoadingLayer.cpp
//  fruitCrush
//
//  Created by Tsai on 13-11-5.
//
//

#include "LoadingLayer.h"
#include "SimpleAudioEngine.h"
#include "StartScene.h"
#include "FruitCrush.h"
using namespace CocosDenshion;
CCScene* LoadingLayer::scene()
{
    CCScene *scene = NULL;
    do {
        scene = CCScene::create();
        CC_BREAK_IF(!scene);
        
        LoadingLayer* layer = LoadingLayer::create();
        CC_BREAK_IF(!layer);
        
        scene->addChild(layer);
    } while (0);
    
    return scene;
}

bool LoadingLayer::init()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite* _bgSprite = CCSprite::create("map_scene_coral_002-i5-hd.png");
    _bgSprite->setAnchorPoint(ccp(0, 0));
    _bgSprite->setPosition(ccp(0, 0));
    this->addChild(_bgSprite);
    setTouchEnabled(false);
    
    CCMenuItemFont* loadingItem = CCMenuItemFont::create("加载中..",this, NULL);
    loadingItem->setTag(1);
    loadingItem->setPosition(ccp(size.width/2, size.height/3));
    this->addChild(loadingItem);
    
    
    
    
    bool isret = false;
    
    
    do {
        _totalPicNum = 6;
        SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("candy_crush_loop5.mp3");
        SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("candy_crush_intro2.mp3");
        SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("candy_crush_soundtrack3.mp3");
        
        SimpleAudioEngine::sharedEngine()->preloadEffect("candy_land3.wav");
        SimpleAudioEngine::sharedEngine()->preloadEffect("button_press.wav");
        SimpleAudioEngine::sharedEngine()->preloadEffect("combo_sound1.wav");
        
        CCTextureCache::sharedTextureCache()->addImageAsync("map_scene_coral_002-i5-hd.png", this, callfuncO_selector(LoadingLayer::loadingCB));
        CCTextureCache::sharedTextureCache()->addImageAsync("button_help_begin_001-hd.png", this, callfuncO_selector(LoadingLayer::loadingCB));
        CCTextureCache::sharedTextureCache()->addImageAsync("titleLogo.png", this, callfuncO_selector(LoadingLayer::loadingCB));
        CCTextureCache::sharedTextureCache()->addImageAsync("map_logo_ocean_001-i5-hd.png", this, callfuncO_selector(LoadingLayer::loadingCB));
        CCTextureCache::sharedTextureCache()->addImageAsync("portrait_frame.png", this, callfuncO_selector(LoadingLayer::loadingCB));
        CCTextureCache::sharedTextureCache()->addImageAsync("Aleft.png", this, callfuncO_selector(LoadingLayer::loadingCB));
        //CCTextureCache::sharedTextureCache()->addImageAsync("candy.png", this, callfuncO_selector(LoadingLayer::loadingCB));
        CCTextureCache::sharedTextureCache()->addImageAsync("game_grid.png", this, callfuncO_selector(LoadingLayer::loadingCB));
        
        CCSpriteFrameCache* candyFrame = CCSpriteFrameCache::sharedSpriteFrameCache();
        candyFrame->addSpriteFramesWithFile("candy.plist");
        
        //CCTextureAtlas
        
        isret = true;
    } while (0);
    
    return isret;
}

void LoadingLayer::loadingCB(cocos2d::CCObject *ped)
{
    _picNum++;
    if (_picNum==_totalPicNum) {
        CCScene *pScene = StartScene::scene();
        CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1, pScene));
    }
}