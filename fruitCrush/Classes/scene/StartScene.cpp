//
//  StartScene.cpp
//  fruitCrush
//
//  Created by Tsai Empire on 9/29/13.
//
//

#include "StartScene.h"
#include "LevelScene.h"
#include "SecondLevelView.h"
#include "SimpleAudioEngine.h"
#include "ResCenter.h"
USING_NS_CC;
using namespace CocosDenshion;
StartScene::~StartScene()
{
    //CC_SAFE_RELEASE(startBG);
    //CC_SAFE_RELEASE(logo);
    //CC_SAFE_RELEASE(mainMenu);
}

CCScene* StartScene::scene()
{
    CCScene *scene = NULL;
    do {
        scene = CCScene::create();
        CC_BREAK_IF(!scene);
        
        StartScene* layer = StartScene::create();
        CC_BREAK_IF(!layer);
        
        scene->addChild(layer);
    } while (0);
    
    return scene;
}

bool StartScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    startBG = ResCenter::shareInstance()->getSpriteByType(ResCenter::kSpriteStartBG);
    startBG->setAnchorPoint(ccp(0, 0));
    startBG->setPosition(ccp(0, 0));
    this->addChild(startBG);
    setTouchEnabled(true);
    
    // game logo
    //logo = CCSprite::create("titleLogo.png");
    logo = CCSprite::createWithSpriteFrameName("red.png");
    logo->setPosition(ccp(size.width/2, size.height + logo->getContentSize().height));
    this->addChild(logo);
    CCMoveBy* showDown = CCMoveBy::create(0.8, ccp(0,-size.height/2));
    CCEaseBounceInOut* sineInOut = CCEaseBounceInOut::create(showDown);
    logo->runAction(sineInOut);
    
    // play background music
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("candy_crush_loop5.mp3", true);
    
    
    // main menu
    CCMenuItemImage* playMenuItem = CCMenuItemImage::create("button_help_begin_001-hd.png", "button_help_begin_001-hd.png", this, menu_selector(StartScene::menubuttonPressedCB));
    playMenuItem->setTag(kTagPlay);
    playMenuItem->setPosition(ccp(size.width/2, size.height/2));
    CCMenuItemFont* connectMenuItem = CCMenuItemFont::create("connect", this, menu_selector(StartScene::menubuttonPressedCB));
    connectMenuItem->setTag(kTagSocial);
    connectMenuItem->setPosition(ccp(size.width/2, size.height/3));
    
    mainMenu = CCMenu::create(playMenuItem, connectMenuItem, NULL);
    mainMenu->setPosition(CCPointZero);
    this->addChild(mainMenu);
    
    CCScaleBy* popScale1 = CCScaleBy::create(1, 0.9);
    CCScaleBy* popScale2 = CCScaleBy::create(1, 1/0.9);
    CCEaseSineInOut* easePop1 = CCEaseSineInOut::create(popScale1);
    CCEaseSineInOut* easePop2 = CCEaseSineInOut::create(popScale2);
    CCSequence* scaleSeq = CCSequence::create(easePop1, easePop2, NULL);
    CCRepeatForever* popRepeat = CCRepeatForever::create(scaleSeq);
    
    //playMenuItem->setColor(ccc3(160,32,240));
    connectMenuItem->setColor(ccc3(160,32,240));
    //popRepeat->retain();
    playMenuItem->runAction(popRepeat);
    connectMenuItem->runAction((CCAction*)popRepeat->copy());
    //popRepeat->release();
    
    
    return true;
}

void StartScene::update(float dt)
{
    
}

void StartScene::menubuttonPressedCB(cocos2d::CCMenuItem *menuItem)
{
    switch (menuItem->getTag()) {
        case kTagPlay:
        {
            SimpleAudioEngine::sharedEngine()->playEffect("button_press.wav");
            //CCScene *pScene = LevelScene::scene();
            CCScene *pScene = SecondLevelView::scene();
            CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1, pScene));
            break;
        }
        case kTagOption:
            
            break;
        case kTagSocial:
            
            break;
        default:
            break;
    }
}

void StartScene::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    
}

void StartScene::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    
}

void StartScene::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    
}

