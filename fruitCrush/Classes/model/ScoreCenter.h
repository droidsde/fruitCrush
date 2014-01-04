//
//  ScoreCenter.h
//  fruitCrush
//
//  Created by Tsai Empire on 11/12/13.
//
//

#ifndef __fruitCrush__ScoreCenter__
#define __fruitCrush__ScoreCenter__

#include "cocos2d.h"
#include "FruitCrush.h"
USING_NS_CC;
/*
#define kScore3Factor 60
#define kScore4Factor 120
#define kScoreLFactor 180
#define kScore5Factor 220
*/
typedef enum {
    kScoreNormalFactor = 60,
    
    kScore3Factor = 60,
    kScore4Factor = 120,
    kScoreLFactor = 180,
    kScore5Factor = 220,
    
    kScoreItemHVFactor = 1000,
    kScoreItemTFactor = 1500,
    kScoreItemPFactor = 3000,
    
    kScoreSuperHVFactor = 3000,
    kScoreSuperHVAndTFactor = 4000,
    kScoreSuperTTFactor = 5000,
    kScoreSuperpowerAndNormalFactor = 5000,
    kScoreSuperPowerAndHV = 6000,
    kScoreSuperPowerAndT = 7000,
    kScoreSuperPowerAndPower = 8000,
    
    kScoreTileIceFactor = 1060,
    kScoreTileIce2Factor = 3060,
    kScoreTileIceStone = 4060
}ScoreFactor;

class ScoreCenter : public cocos2d::CCObject
{

    
public:
    CC_SYNTHESIZE(TypeGameMode, _gameMode, GameMode);
    CC_SYNTHESIZE(int, _colorNum, ColorNum);
    CC_SYNTHESIZE(int, _moveLimit, MoveLimit);
    CC_SYNTHESIZE(int, _timeLimit, TimeLimit);
    CC_SYNTHESIZE(int, _scoreTarget1, ScoreTarget1);
    CC_SYNTHESIZE(int, _scoreTarget2, ScoreTarget2);
    CC_SYNTHESIZE(int, _scoreTarget3, ScoreTarget3);
    CC_PROPERTY(int, _curScore, CurrentScore);
    CC_PROPERTY(int, _curMoves, CurrentMoves);
    CC_PROPERTY(int, _curTimes, CurrentTimes);
    CC_SYNTHESIZE_RETAIN(CCLayer*, _layer, Layer);
    CC_SYNTHESIZE_RETAIN(CCArray*, _dropType, DropTypes);
    CC_SYNTHESIZE_RETAIN(CCArray*, _dropNum, DropNums);
    CC_SYNTHESIZE_RETAIN(CCArray*, _dropBornX, DropBornX);
    CC_SYNTHESIZE_RETAIN(CCArray*, _dropBornY, DropBornY);
    CC_SYNTHESIZE(TypeTaskMode, _taskMode, TaskMode);
    CC_SYNTHESIZE(TypeDropMode, _dropMode, DropMode);
    // Asyn drop only
    CC_SYNTHESIZE(int, _bornDropMove, BornDropMove);
    CC_SYNTHESIZE(bool, _isAsynMark, IsAsynMark);
    
public:
    virtual ~ScoreCenter();
    virtual bool init();
    CREATE_FUNC(ScoreCenter);
    
    void initWithMap(int mapNo);
    int getScoreLevel();
    bool isLimitCatch();
    
    // show current grain point on game panel
    void showPromptScore(int score, CCPoint ppoint);
    //void setCurMove();
    //void setCurTime();
    //void setCurScore();
    void addMoveBy(int num);
    void subMoveBy(int num);
    void addTimeBy(int num);
    void subTimeBy(int num);
    
    bool isDropsMode();
    
private:
    CCLabelAtlas* _moveNumItem;
    CCLabelAtlas* _scoreItem;
    CCLabelAtlas* _timeItem;
    
private:
    void m_initUI();
    void _removeScorePrompt(CCLabelAtlas* label);
    void _delayDisplayScore(CCLabelAtlas* label);
};

#endif /* defined(__fruitCrush__ScoreCenter__) */
