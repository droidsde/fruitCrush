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

using namespace CocosDenshion;

SecondLevelView::SecondLevelView():
m_nCurPage(0),
m_nPageCount(0),
m_pScrollView(0)
{
    
}

SecondLevelView::~SecondLevelView(){
    
}

CCScene* SecondLevelView::scene(){
    CCScene* scene = CCScene::create();
    CCLayer* layer = SecondLevelView::create();
    scene->addChild(layer);
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
        CCDirector::sharedDirector()->replaceScene(CCTransitionSplitRows::create(1, GameScene::sceneWithMap(touch->getEventId() + 1)));
    }
}

void SecondLevelView::registerWithTouchDispatcher(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool SecondLevelView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCLog("touch begin.");
    m_touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    m_touchOffset = getScrollView()->getContentOffset();
    // 发送触摸消息，并会在 touEventAction 自动相应， 如果触摸到元素
    sendTouchMessage(pTouch, 0);
    
    return true;
}

void SecondLevelView::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint movePoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    float distance = movePoint.x - m_touchPoint.x;
    // 设定当前偏移位置
    CCPoint adjustPoint = ccp(m_touchOffset.x + distance, 0);
    getScrollView()->setContentOffset(adjustPoint, false);
}

void SecondLevelView::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint endPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    float distance = endPoint.x - m_touchPoint.x;
    float distanceY = endPoint.y - m_touchPoint.y;
    if (fabs(distance) < 3 && fabs(distanceY) < 3){
        // 小于三，不做拖动操作，也排除了（抖动误操作）,第二个参数，事件类型 1： touch end， 由 touchEventAction 自动相应
        sendTouchMessage(pTouch, 1);
        
    }else if (fabs(distance) > 50){
        // 大于 50，执行拖动效果
        adjustScrollView(distance);
        setCurPageBall();
    }else{
        // 回退为拖动之前的位置
        adjustScrollView(0);
    }
    
    sendTouchMessage(pTouch, 2);
}

void SecondLevelView::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    adjustScrollView(0);
}

void SecondLevelView::adjustScrollView(float offset){
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    // 我们根据 offset 的实际情况来判断移动效果
    if (offset < 0)
        m_nCurPage ++;
    else if (offset > 0)
        m_nCurPage --;
    
    if (m_nCurPage < 0)
        m_nCurPage = 0;
    else if (m_nCurPage > m_nPageCount - 1)
        m_nCurPage = m_nPageCount - 1;
    
    CCPoint adjustPoint = ccp(-winSize.width * m_nCurPage , 0);
    getScrollView()->setContentOffsetInDuration(adjustPoint, 0.2f);
    
    CCLog("current page index: %d", m_nCurPage);
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


void SecondLevelView::setCurPageBall()
{
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    const int tagNodeBall = 1002;
	float fs = 0.2f;
	float scale = 0.7f;
	// 根据分屏数量显示小圆球标示
	CCNode* layer = this->getChildByTag(tagNodeBall);
	if (layer)
	{
		CCLayer* lyBall = dynamic_cast<CCLayer*>(layer);
		for (int i = 0; i < m_nPageCount; i++)
		{
			CCNode* n = lyBall->getChildByTag(i);
			if (n)
			{
				CCSprite* sprite = dynamic_cast<CCSprite*>(n);
				if (m_nCurPage == i)
				{
					sprite->runAction(
                                      CCScaleTo::create(fs,
                                                        1.0f));
				}
				else
				{
					sprite->runAction(
                                      CCScaleTo::create(0.5,
                                                        scale));
				}
			}
		}
	}
	else
	{
		CCLayer* lyBall = CCLayer::create();
		this->addChild(lyBall, 0, tagNodeBall);
        
		int ball_width = 30;			// 圆球间距
		for (int i = 0; i < m_nPageCount; i++)
		{
			CCSprite* sprite = CCSprite::create("ball.png");
			sprite->setPosition(ccp(ball_width / 2 + (i - 1) * ball_width, ball_width / 2));
			sprite->setScale(scale);
			lyBall->addChild(sprite, 0, i);
			if (i == m_nCurPage)
			{
				sprite->runAction(CCScaleTo::create(fs, 1.0f));
			}
		}
		lyBall->setPosition(
                            size.width / 2 - ball_width * (m_nPageCount - 1) / 2,
                            size.height / 2 + 516 / 2 + 5);
	}
}