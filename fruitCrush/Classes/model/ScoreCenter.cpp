//
//  ScoreCenter.cpp
//  fruitCrush
//
//  Created by Tsai Empire on 11/12/13.
//
//

#include "ScoreCenter.h"
#include "ResCenter.h"
#include "ActionEffectHub.h"
#include "ResUtil.h"

ScoreCenter::~ScoreCenter()
{
    setLayer(NULL);
    if(_dropType) _dropType->removeAllObjects();
    if(_dropNum) _dropNum->removeAllObjects();
    if(_dropBornX) _dropBornX->removeAllObjects();
    if(_dropBornY) _dropBornY->removeAllObjects();
    setDropTypes(NULL);
    setDropNums(NULL);
    setDropBornX(NULL);
    setDropBornY(NULL);
}

bool ScoreCenter::init()
{
    return true;
}

void ScoreCenter::initWithMap(int mapNo)
{
    CCString* mapNoStr = CCString::createWithFormat("map%02d.plist", mapNo);
    CCDictionary* pdict = CCDictionary::createWithContentsOfFile(mapNoStr->getCString());
    CCString* moveLimit = (CCString*)pdict->objectForKey("moveLimit");
    CCString* timeLimit = (CCString*)pdict->objectForKey("timeLimit");
    CCString* colorNum = (CCString*)pdict->objectForKey("colorNum");
    CCString* gameMode = (CCString*)pdict->objectForKey("gameMode");
    CCString* taskMode = (CCString*)pdict->objectForKey("taskMode");
    CCString* thingMode = (CCString*)pdict->objectForKey("dropMode");
    CCDictionary* scoreTarget = (CCDictionary*)pdict->objectForKey("scoreTarget");
    CCString* star1 = (CCString*)scoreTarget->objectForKey("star1");
    CCString* star2 = (CCString*)scoreTarget->objectForKey("star2");
    CCString* star3 = (CCString*)scoreTarget->objectForKey("star3");
    CCDictionary* taskThings = (CCDictionary*)pdict->objectForKey("taskDrops");
    CCString* bornThingMove = (CCString*)pdict->objectForKey("bornDropMove");
    CCString* thingBornX = (CCString*)pdict->objectForKey("dropBornX");
    CCString* thingBornY = (CCString*)pdict->objectForKey("dropBornY");
    
    if(moveLimit) this->setMoveLimit(moveLimit->intValue());
    if(timeLimit) this->setTimeLimit(timeLimit->intValue());
    if(colorNum) this->setColorNum(colorNum->intValue());
    if(gameMode) this->setGameMode((TypeGameMode)gameMode->intValue());
    if(taskMode) this->setTaskMode((TypeTaskMode)taskMode->intValue());
    if(thingMode) this->setDropMode((TypeDropMode)thingMode->intValue());
    if (taskThings) {
        // task things pair type-number
        CCArray* thingsArr = taskThings->allKeys();
        if (thingsArr->count()>0) {
            CCArray* thingTypes = CCArray::createWithCapacity(thingsArr->count());
            CCArray* thingNums = CCArray::createWithCapacity(thingsArr->count());
            for (int i=0; i < thingsArr->count(); i++) {
                CCString* key = (CCString*)thingsArr->objectAtIndex(i);
                thingTypes->addObject(key);
                thingNums->addObject(taskThings->objectForKey(key->getCString()));
            }
            this->setDropTypes(thingTypes);
            this->setDropNums(thingNums);
        }
    }
    if(bornThingMove) this->setBornDropMove(bornThingMove->intValue());
    if (thingBornX) this->setDropBornX(ResUtil::componentsSeparatedByString(thingBornX, "|"));
    if (thingBornY) this->setDropBornY(ResUtil::componentsSeparatedByString(thingBornY, "|"));
    
    this->setScoreTarget1(star1->intValue());
    this->setScoreTarget2(star2->intValue());
    this->setScoreTarget3(star3->intValue());
    
    this->setIsAsynMark(true);
    
    this->m_initUI();
}

void ScoreCenter::m_initUI()
{
    CCLabelTTF* sLb = CCLabelTTF::create("scores:", "Helvetica", 18, CCSizeMake(100, 20), kCCTextAlignmentCenter);
    sLb->setPosition(ccp(kTileWidth, FruitCrush::shareSingleton()->getWinSize().height - kTileWidth*3));
    this->getLayer()->addChild(sLb);
    _scoreItem =  ResCenter::shareInstance()->getLableAtlasByType(ResCenter::klableScore);
    _scoreItem->setString("0");
    _scoreItem->setPosition(ccp(kTileWidth*3, FruitCrush::shareSingleton()->getWinSize().height - kTileWidth*3));
    //_scoreItem->setScale(0.5);
    this->getLayer()->addChild(_scoreItem, 10, 1);
    
    setCurrentScore(0);
    
    //this->getLayer()->addChild(_scoreItem, 10, 3);
    
    switch (this->getGameMode()) {
        case kModMove: {
            CCLabelTTF* moveLb = CCLabelTTF::create("moves:", "Helvetica", 18, CCSizeMake(100, 20), kCCTextAlignmentCenter);
            moveLb->setPosition(ccp(kTileWidth, FruitCrush::shareSingleton()->getWinSize().height - kTileWidth*2));
            this->getLayer()->addChild(moveLb);
            
            _moveNumItem =  ResCenter::shareInstance()->getLableAtlasByType(ResCenter::klableScore);
            _moveNumItem->setString("0");
            _moveNumItem->setPosition(ccp(kTileWidth*3, FruitCrush::shareSingleton()->getWinSize().height - kTileWidth*2));
            //_moveNumItem->setScale(0.5);
            //_moveNumItem->setColor(ccc3(0, 0, 0));
            this->getLayer()->addChild(_moveNumItem, 10, 1);
            
            setCurrentMoves(this->getMoveLimit());
            
            break;
        }
            
        case kModTime:{
            CCLabelTTF* timeLb = CCLabelTTF::create("times:", "Helvetica", 18, CCSizeMake(100, 20), kCCTextAlignmentCenter);
            //timeLb->setAnchorPoint(CCPointZero);
            timeLb->setPosition(ccp(kTileWidth, FruitCrush::shareSingleton()->getWinSize().height - kTileWidth*2));
            this->getLayer()->addChild(timeLb);
            
            _timeItem =  ResCenter::shareInstance()->getLableAtlasByType(ResCenter::klableScore);
            _timeItem->setString("0");
            _timeItem->setPosition(ccp(kTileWidth*3, FruitCrush::shareSingleton()->getWinSize().height - kTileWidth*2));
            //_timeItem->setScale(0.5);
            
            this->getLayer()->addChild(_timeItem, 10, 2);
            setCurrentTimes(0);
            break;
        }
            
        default:
            break;
    }
}

void ScoreCenter::setCurrentMoves(int var)
{
    CCString* curMove = CCString::createWithFormat("%u", var);
    _moveNumItem->setString(curMove->getCString());

    //_moveNumItem->setScale(0.5);
    _curMoves = var;
}

int ScoreCenter::getCurrentMoves()
{
    return _curMoves;
}

void ScoreCenter::setCurrentTimes(int var)
{
    CCString* curTime = CCString::createWithFormat("%u", var);
    _timeItem->setString(curTime->getCString());
       //_timeItem->setColor(ccc3(0, 0, 0));
    //_timeItem->setScale(0.5);
    _curTimes = var;
}

int ScoreCenter::getCurrentTimes()
{
    return _curTimes;
}

void ScoreCenter::setCurrentScore(int var)
{
    _curScore = var;
    
    CCAction* dispSore = CCSequence::create(CCDelayTime::create(kScorePromptDuration), CCCallFuncN::create(this, callfuncN_selector(ScoreCenter::_delayDisplayScore)));
    _scoreItem->runAction(dispSore);

    //_scoreItem->setColor(ccc3(0, 0, 0));
    //_scoreItem->setScale(0.5);
    
}

void ScoreCenter::_delayDisplayScore(cocos2d::CCLabelAtlas *label)
{
    CCString* curScore = CCString::createWithFormat("%u", _curScore);
    label->setString(curScore->getCString());
}

int ScoreCenter::getCurrentScore()
{
    return _curScore;
}

int ScoreCenter::getScoreLevel()
{
    return 0;
}

bool ScoreCenter::isLimitCatch()
{
    return false;
}


void ScoreCenter::showPromptScore(int score, CCPoint ppoint)
{
    CCString* curScore = CCString::createWithFormat("%u", score);
    CCLabelAtlas* label = ResCenter::shareInstance()->getLableAtlasByType(ResCenter::klableScore);
    label->setPosition(ppoint);
    //label->setScale(0.5);
    label->setString(curScore->getCString());
    
    CCFiniteTimeAction* sf = ActionEffectHub::shareInstance()->scoreFly();
    CCFiniteTimeAction* cb = CCCallFuncN::create(this, callfuncN_selector(ScoreCenter::_removeScorePrompt));

    CCSequence* cbSeq = CCSequence::create(sf, cb, NULL);
    label->runAction(cbSeq);
    this->getLayer()->addChild(label);
}

void ScoreCenter::_removeScorePrompt(CCLabelAtlas* label)
{
    this->getLayer()->removeChild(label);
}

void ScoreCenter::addMoveBy(int num)
{
    // add move number
    if (this->getGameMode() == kModMove) {
        int curMove = this->getCurrentMoves()+num;
        this->setCurrentMoves(curMove);
    }
}

void ScoreCenter::subMoveBy(int num)
{
    // sub move number
    if (this->getGameMode() == kModMove) {
        int curMove = this->getCurrentMoves()-num;
        this->setCurrentMoves(curMove);
    }
}

void ScoreCenter::addTimeBy(int num)
{
    // add time number
    if (this->getGameMode() == kModTime) {
        int curTime = this->getCurrentTimes()+num;
        this->setCurrentTimes(curTime);
    }
}

void ScoreCenter::subTimeBy(int num)
{
    // sub time number
    if (this->getGameMode() == kModTime) {
        int curTime = this->getCurrentTimes()-num;
        this->setCurrentTimes(curTime);
    }
}

bool ScoreCenter::isDropsMode()
{
    if (this->getTaskMode() == kTaskModDrop || this->getTaskMode() == kTaskModJellyAndDrop) {
        return true;
    }
    return false;
}