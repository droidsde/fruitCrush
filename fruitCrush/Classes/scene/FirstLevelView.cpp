//
//  FirstLevelView.cpp
//  fruitCrush
//
//  Created by Empire Tsai on 14-1-3.
//
//

#include "FirstLevelView.h"
#include "FruitCrush.h"
#include "SimpleAudioEngine.h"
#include "ResCenter.h"

using namespace CocosDenshion;

bool FirstLevelView::init(){
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
        
        // 创建一个 CCScrollView, 内容大小和当前的界面一样
        CCScrollView* scrollView = CCScrollView::create(this->getContentSize());
        scrollView->setContainer(getContainLayer());
        scrollView->setTouchEnabled(false);
        
        
        setScrollView(scrollView);
        this->addChild(scrollView);
        
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("candy_crush_intro2.mp3", true);
        
        CCLog("level view ...");
        setTouchEnabled(true);
        
        setCurPageBall();
        
        bRef = true;
    }while (0);
    return bRef;
}

CCLayer* FirstLevelView::getContainLayer(){
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

CCNode* FirstLevelView::getSpriteByLevel(int level){
    
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