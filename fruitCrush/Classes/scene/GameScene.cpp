//
//  GameScene.cpp
//  fruitCrush
//
//  Created by Tsai Empire on 10/23/13.
//
//

#include "GameScene.h"
GameScene::~GameScene()
{
    //CC_SAFE_RELEASE(_gameLayer);
}
GameScene* GameScene::sceneWithMap(int mapNo)
{
    return GameScene::create()->initWithMap(mapNo);
}

GameScene* GameScene::initWithMap(int mapNo)
{
    if (this->init()) {
        _gameLayer = GameLayer::create();
        
        this->addChild(_gameLayer);
        _gameLayer->loadMap(mapNo);
    }
    return this;
}
