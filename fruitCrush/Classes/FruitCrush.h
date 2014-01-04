//
//  FruitCrush.h
//  fruitCrush
//
//  Created by Tsai Empire on 10/22/13.
//
//

#ifndef __fruitCrush__FruitCrush__
#define __fruitCrush__FruitCrush__

#include "cocos2d.h"

USING_NS_CC;

#define kHXStart                400
#define kVXStart                30
#define kHYStart                30
#define kVYStart                30

#define kBoxWidth               9
#define kBoxHeight              9
#define kTileWidth              65.0f
#define kMoveTileTime           0.2f
#define kDisapearTime           0.1f
#define kDelayTime              0.01f

//#define kTotalType              4
#define kTileScaleFactor        0.65
#define kScorePromptDuration    0.7f

typedef enum {
    kTagTranTile = 10000,
    KTagDefaultTile
}TypeTransTile;

typedef enum  {
    ktNothing   = -1,
    ktOrange    = 1000,
    ktApple,
    ktBanana,
    ktPear,
    ktLemon,
    ktMango,
    ktStrawberry,
    ktPeach,
    ktWatermelon,
    ktPlum,
    ktPineapple,
    ktGrape,
    ktApricot,
    ktBlackberry,
    ktCarambola,
    ktCoconut,
    ktCherry,
    ktGooseberry,
    ktPersimmon,
    ktMulberry,
    ktPomegranate,
    ktFig,
    
    ktSpacial
}TypeFruit;

typedef  enum {
    kcinormal   = 1100,
    kciHor,
    kciVec,
    kciPop,
    kciBomb,
    
    kciDrop
}ClassItemSprite;

typedef enum  {
    kttNone     = 0,
    kttNormal   = 1 << 1,
    kttIce      = 1 << 2,
    kttIce2     = 1 << 3,
    kttStone    = 1 << 4,
    kttDropEnd  = 1 << 5
    
}TypeTile;

typedef enum {
    ktfsNormal  = 1200,
    ktfsLevel1,
    ktfsLevel2
}TypeFruitStatus;


typedef enum 
{
    kTagback    = 1300,
    kTagOption,
    kTagSocial
}TagForGameSceneButtons;

typedef enum {
    kOrientationH   = 1400,
    kOrientationV
}TypeOrientation;

typedef enum {
    kMatchNothing   = -1,
    kMatch3         = 1500,
    kMatch3WithH,
    kMacth3WithV,
    kMatch3WithHV,
    kMatch3WithMHV,
    kMatch3WithT,
    
    kMatch4H,
    kMatch4HWithH,
    kMatch4HWithV,
    kMatch4HWithHV,
    kMatch4HWith3HV,
    kMatch4HWithT,
    kMatch4HWithMT,
    
    kMatch4V,
    kMatch4VWithH,
    kMatch4VWithV,
    kMatch4VWithHV,
    kMatch4VWith3HV,
    kMatch4VWithT,
    kMatch4VWithMT,
    
    kMatch5,
    kMatchT,
    kMatchTT,
    kMatchHV,
    kMatchTAndHV,
    KMatchPowerAndNormal,
    kMatchPowerAndT,
    kMatchPowerAndHV,
    kMatchPowerAndPower
}TypeMatch;

typedef enum {
    kPromptMatchTag = 1600
}TypeActionTag;

typedef enum{
    kModMove        = 1,
    kModTime        = 2
}TypeGameMode;

typedef enum{
    kTaskModJelly   = 1,
    kTaskModDrop,
    kTaskModJellyAndDrop
}TypeTaskMode;

typedef enum{
    kDropModSyn     =1,
    kDropModAsyn
} TypeDropMode;

typedef enum {
    kDropTypeCherry = 8,
    kDropTypeHazelnut
} TypeDropsClass;

class FruitCrush : public cocos2d::CCObject
{
public:
    
    static FruitCrush* shareSingleton(void);
    
    bool canPlayMap(int mapNo);
    void setMapCompleted(int mapNo, bool completed);
    CCSize getWinSize();
    const char* getItemString(int value) const;
    
};

#endif /* defined(__fruitCrush__FruitCrush__) */
