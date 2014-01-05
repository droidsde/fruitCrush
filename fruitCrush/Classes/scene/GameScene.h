//
//  GameScene.h
//  fruitCrush
//
//  Created by Tsai Empire on 10/23/13.
//
//

#ifndef __fruitCrush__GameScene__
#define __fruitCrush__GameScene__

#include "cocos2d.h"
#include "GameLayer.h"
class GameScene : public cocos2d::CCScene
{
private:
    GameLayer* _gameLayer;
    //CC_SYNTHESIZE_RETAIN(GameLayer*, _gameLayer, GameLayer);
    
public:
    static GameScene* sceneWithMap(int mapNo);
    GameScene* initWithMap(int mapNo);
    CREATE_FUNC(GameScene);
    virtual ~GameScene();
};

#endif /* defined(__fruitCrush__GameScene__) */
