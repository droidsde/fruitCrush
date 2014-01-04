//
//  BaseScrollView.h
//  fruitCrush
//
//  Created by Empire Tsai on 14-1-3.
//
//

#ifndef __fruitCrush__BaseScrollView__
#define __fruitCrush__BaseScrollView__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "LsTouch.h"

USING_NS_CC;
USING_NS_CC_EXT;

class BaseScrollView : public CCLayer, public LsTouchEvent{
public:
    BaseScrollView();
    virtual ~BaseScrollView();
    
    static CCScene* scene();
    virtual bool init();
    //CREATE_FUNC(BaseScrollView);
    
    virtual void touchEventAction(LsTouch* touch, int type);
    
    virtual void registerWithTouchDispatcher(void);
    
	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent);
    
    void adjustScrollView(float offset);
    virtual CCLayer* getContainLayer()=0;
    virtual CCNode* getSpriteByLevel(int level)=0;
    
    void setCurPageBall();
    
    
protected:
    CCPoint m_touchPoint;
    CCPoint m_touchOffset;
    
    int m_nCurPage;
    int m_nPageCount;
    
    CC_SYNTHESIZE_RETAIN(CCScrollView*, m_pScrollView, ScrollView);
};

#endif /* defined(__fruitCrush__BaseScrollView__) */
