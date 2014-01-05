//
//  SecondLevelView.cpp
//  fruitCrush
//
//  Created by Empire Tsai on 14-1-3.
//
//

#include "SecondLevelView.h"
#include "StartScene.h"
#include "GameScene.h"
#include "FruitCrush.h"
#include "SimpleAudioEngine.h"
#include "ResCenter.h"
#include "FirstLevelView.h"

using namespace CocosDenshion;


CCScene* SecondLevelView::scene(){
    CCScene* scene = CCScene::create();
    CCLayer* layer = SecondLevelView::create();
    scene->addChild(layer);
    return scene;
}

CCScene* SecondLevelView::sceneWithTheme(int themeNo)
{
    CCScene* scene = CCScene::create();
    SecondLevelView* layer = SecondLevelView::create();
    scene->addChild(layer);
    layer->loadWiththeme(themeNo);
    return scene;
}

bool SecondLevelView::init(){
    bool bRef = false;
    do{
        CC_BREAK_IF(!CCLayer::init());
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCPoint pointCenter = ccp(winSize.width / 2, winSize.height / 2);
        
        // 显示背景图片
        CCSprite* background = ResCenter::shareInstance()->getSpriteByType(ResCenter::kSpriteLevelScene);//CCSprite::create("portrait_frame.png");
        background->setPosition(pointCenter);
        background->setScale(1.2f);
        this->addChild(background);
        
        // 添加一个半透明的灰显层
        CCLayerColor* backLayerColor = CCLayerColor::create(ccc4(25, 25, 25, 125));
        this->addChild(backLayerColor);
        
        // back sprite
        CCSprite* _backSprite = CCSprite::create("Aleft.png");
        //_backSprite->setScale(1);
        _backSprite->setAnchorPoint(ccp(1, 1));
        _backSprite->setPosition(ccp(_backSprite->boundingBox().size.width, FruitCrush::shareSingleton()->getWinSize().height ));
        //this->addChild(_backSprite);
        //_backSprite->setTag(kTagback);
        // 触摸处理
        LsTouch* touch = LsTouch::create();
        touch->setDisplay(_backSprite);
        this->addLsTouch(touch, kTagback);
        this->addChild(touch);
        
        
        bRef = true;
    }while (0);
    return bRef;
}

void SecondLevelView::loadWiththeme(int themeNo)
{
    // 创建一个 CCScrollView, 内容大小和当前的界面一样
    CCScrollView* scrollView = CCScrollView::create(this->getContentSize());
    scrollView->setContainer(getContainLayerWithTheme(themeNo));
    scrollView->setTouchEnabled(false);
    
    setScrollView(scrollView);
    this->addChild(scrollView);
    
    //SimpleAudioEngine::sharedEngine()->playBackgroundMusic("candy_crush_intro2.mp3", true);
    
    CCLog("level view ...");
    setTouchEnabled(true);
    
    setCurPageBall();
}

void SecondLevelView::touchEventAction(LsTouch *touch, int type){
    // type 事件类型，0：touchbegin 1：touchend 触发 2：touchend 未触发
    if (touch)
        CCLog("touch event action id: %d  type: %d", touch->getEventId(), type);
    const int selectTag = 10001;
    if (type == 0 && touch){
        getScrollView()->getContainer()->removeChildByTag(selectTag);
        // 添加选撞状态的精灵，背景
        CCSprite* sprite = CCSprite::create("sel.png");
        sprite->setScaleX(2);
        sprite->setScaleY(4);
        sprite->setPosition(touch->getPosition());
        getScrollView()->getContainer()->addChild(sprite, 1, selectTag);
        
    } else {
        getScrollView()->getContainer()->removeChildByTag(selectTag);
    }
    if (type == 1 && touch){
        // 收到 type 为 1 表示触发关卡选择
        //CCString* str = CCString::createWithFormat("您选择的关卡为 %d .", touch->getEventId() + 1);
        //CCMessageBox("关卡", str->getCString());
        SimpleAudioEngine::sharedEngine()->playEffect("button_press.wav");
        if (touch->getEventId() == kTagback) {
            CCDirector::sharedDirector()->replaceScene(CCTransitionSplitRows::create(1, FirstLevelView::scene()));
            return;
        }
        
        CCDirector::sharedDirector()->replaceScene(CCTransitionSplitRows::create(1, GameScene::sceneWithMap(touch->getEventId() + 1)));
    }
}

CCLayer* SecondLevelView::getContainLayerWithTheme(int themeNo)
{
    // 48 个关卡
    int levelCount = 48;
    int widthCount = 4;
    int heightCount = 4;
    
    int themeFactor = themeNo * levelCount;
    m_nPageCount = (levelCount - 1) / (widthCount * heightCount) + 1;
    
    CCLog("关卡数：%d, 页数：%d", levelCount, m_nPageCount);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint pointCenter = ccp(winSize.width / 2, winSize.height / 2);
    
    CCLayer* layer = CCLayer::create();
    layer->setPosition(CCPointZero);
    
    int frameWith = 0;
    
    // 添加 frame
    for (int i = 0; i < m_nPageCount; i++) {
        CCSprite* frame = CCSprite::create("frame.png");
        frame->setPosition(ccpAdd(pointCenter, ccp(winSize.width * i, 0)));
        layer->addChild(frame, 0, 0);
        frameWith = frame->boundingBox().size.width;
    }
    
    // 添加关卡项
    for (int i = 0; i < levelCount; i ++) {
        CCNode* level = getSpriteByLevel(i+themeFactor);
        // 设置位置
        int curPageIndex = i / (widthCount * heightCount);
        float xOffset = i % 4 * 100 + 70 + (winSize.width-frameWith)/2;
        float yOffset = winSize.height - ((i % (widthCount * heightCount)) / widthCount * 120 + 150);
        
        level->setPosition(ccp(xOffset + winSize.width * curPageIndex, yOffset));
        layer->addChild(level ,2, i);
    }
    
    layer->setContentSize(CCSizeMake(winSize.width * m_nPageCount, winSize.height));
    
    return layer;
}

CCLayer* SecondLevelView::getContainLayer(){
    // 48 个关卡
    int levelCount = 48;
    int widthCount = 4;
    int heightCount = 4;
    m_nPageCount = (levelCount - 1) / (widthCount * heightCount) + 1;
    
    CCLog("关卡数：%d, 页数：%d", levelCount, m_nPageCount);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint pointCenter = ccp(winSize.width / 2, winSize.height / 2);
    
    CCLayer* layer = CCLayer::create();
    layer->setPosition(CCPointZero);
    
    int frameWith = 0;
    
    // 添加 frame
    for (int i = 0; i < m_nPageCount; i++) {
        CCSprite* frame = CCSprite::create("frame.png");
        frame->setPosition(ccpAdd(pointCenter, ccp(winSize.width * i, 0)));
        layer->addChild(frame, 0, 0);
        frameWith = frame->boundingBox().size.width;
    }
    
    // 添加关卡项
    for (int i = 0; i < levelCount; i ++) {
        CCNode* level = getSpriteByLevel(i);
        // 设置位置
        int curPageIndex = i / (widthCount * heightCount);
        float xOffset = i % 4 * 100 + 70 + (winSize.width-frameWith)/2;
        float yOffset = winSize.height - ((i % (widthCount * heightCount)) / widthCount * 120 + 150);
        
        level->setPosition(ccp(xOffset + winSize.width * curPageIndex, yOffset));
        layer->addChild(level ,2, i);
    }
    
    layer->setContentSize(CCSizeMake(winSize.width * m_nPageCount, winSize.height));
    
    return layer;
}

CCNode* SecondLevelView::getSpriteByLevel(int level){
    
    CCSprite* sprite = CCSprite::create("level.png");
    // 添加关卡标示
    CCString* str = CCString::createWithFormat("%d", level + 1);
    CCLabelBMFont* label = CCLabelBMFont::create(str->getCString(),"prim30.fnt");
    label->setPosition(ccp(40, 45));
    sprite->addChild(label);
    // 这里可以再为 关卡精灵做些装饰，就像 lable 一样
    
    // 触摸处理
    LsTouch* touch = LsTouch::create();
    touch->setDisplay(sprite);
    this->addLsTouch(touch, level);
    
    return touch;
}