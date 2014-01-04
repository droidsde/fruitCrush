//
//  LoadingLayer.h
//  fruitCrush
//
//  Created by Tsai on 13-11-5.
//
//

#ifndef __fruitCrush__LoadingLayer__
#define __fruitCrush__LoadingLayer__

#include "cocos2d.h"
USING_NS_CC;
class LoadingLayer : public CCLayer
{
private:
    int _picNum;
    int _soundNum;
    int _totalPicNum;
    int _totalSoundNum;
public:
    virtual bool init();
    CREATE_FUNC(LoadingLayer);
    static CCScene* scene();
    
    void loadingCB(CCObject* ped);
};

#endif /* defined(__fruitCrush__LoadingLayer__) */
