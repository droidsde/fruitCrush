//
//  ResCenter.cpp
//  fruitCrush
//
//  Created by Tsai on 13-11-17.
//
//

#include "ResCenter.h"
static ResCenter* _instance=NULL;
ResCenter* ResCenter::shareInstance()
{
    if (_instance == NULL) {
        _instance = new ResCenter();
    }
    return _instance;
}

CCSprite* ResCenter::getSpriteByType(typeSprite type)
{
    switch (type) {
        case kSpriteStartBG:
        {
            //if (!_startBG || (_startBG && !_startBG->retainCount())) {
            CCSprite* startBG = CCSprite::create("map_logo_ocean_001-i5-hd.png");
            //}
            return startBG;
            break;
        }
        case kSpriteLevelScene:
        {
            //if ((_levelSelectSceneBG && !_levelSelectSceneBG->retainCount()) || !_levelSelectSceneBG) {
            CCSprite* levelSelectSceneBG = CCSprite::create("map_scene_coral_002-i5-hd.png");
            //}
            return levelSelectSceneBG;
            break;
        }
        case kSpriteGameLayerScene:
        {
            //if (!_gameSceneBG || (_gameSceneBG && !_gameSceneBG->retainCount())) {
            CCSprite* gameSceneBG = CCSprite::create("map_logo_ocean_001-i5-hd.png");
            //}
            return gameSceneBG;
            break;
        }
        default:
            break;
    }
    return NULL;
}

CCLabelAtlas* ResCenter::getLableAtlasByType(ResCenter::typeLableAtlas type)
{
    switch (type) {
        case klableScore:
        {
            CCLabelAtlas* lable = CCLabelAtlas::create("0123456789", "fonts_roulette_001-hd.png", 24, 32, '0');
            lable->setAnchorPoint(ccp(0.5, 0.5));
            return lable;
            break;
        }
            
            
        default:
            break;
    }
}