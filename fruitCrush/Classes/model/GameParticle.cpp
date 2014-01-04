//
//  GameParticle.cpp
//  fruitCrush
//
//  Created by Tsai Empire on 11/6/13.
//
//

#include "GameParticle.h"
#include "ResCenter.h"

static GameParticle* _instance=NULL;
GameParticle* GameParticle::shareInstance()
{
    if (_instance == NULL) {
        _instance = new GameParticle();
    }
    return _instance;
}

void GameParticle::playRowRainbow()
{
    
}

void GameParticle::playSnowDown(CCLayer* layer, CCPoint ppoint)
{
    CCParticleSystem* m_snow =  CCParticleFlower::createWithTotalParticles(30);
    layer->addChild(m_snow, 10);
    m_snow->setTexture( CCTextureCache::sharedTextureCache()->addImage("stars.png") );
    m_snow->setDuration(0.3f);
    
    
    // angle
    m_snow->setAngle(90);
    m_snow->setAngleVar(150);
    
    // speed of particles
    m_snow->setSpeed(120);
    m_snow->setSpeedVar(20);
    
    // radial
    //m_snow->setRadialAccel(-120);
    //m_snow->setRadialAccelVar(0);
    
    // tagential
    m_snow->setTangentialAccel(10);
    m_snow->setTangentialAccelVar(0);
    
    // emitter position
    m_snow->setPosition( ppoint );
    m_snow->setPosVar(CCPointZero);
    
    m_snow->setLife(3);
    m_snow->setLifeVar(1);
    m_snow->setGravity(ccp(0,-100));
    
    
    // spin of particles
    m_snow->setStartSize(10);
    m_snow->setStartSpin(0);
    m_snow->setStartSizeVar(2);
    m_snow->setEndSpin(0);
    m_snow->setEndSpinVar(0);
    

    //m_snow->setStartSize(4);
    //    m_snow->setStartSizeVar(2);
    //    m_snow->setEndSize(kCCParticleStartSizeEqualToEndSize);
    
    // color of particles
    ccColor4F startColor = {0.5f, 0.5f, 0.5f, 1.0f};
    m_snow->setStartColor(startColor);
    
    ccColor4F startColorVar = {0.5f, 0.5f, 0.5f, 1.0f};
    m_snow->setStartColorVar(startColorVar);
    
    ccColor4F endColor = {0.1f, 0.1f, 0.1f, 0.2f};
    m_snow->setEndColor(endColor);
    
    ccColor4F endColorVar = {0.1f, 0.1f, 0.1f, 0.2f};
    m_snow->setEndColorVar(endColorVar);
    
    m_snow->setEmissionRate(m_snow->getTotalParticles()/m_snow->getLife());
    
    
    
    m_snow->setPosition(ppoint);
    //m_snow->autorelease();
    m_snow->setAutoRemoveOnFinish(true);
}

void GameParticle::playJellyBreak(CCLayer* layer, CCPoint ppoint)
{
    CCParticleSystem* jellyBreak = CCParticleSystemQuad::create(p_jellyBreak);
    CCParticleBatchNode *batch = CCParticleBatchNode::createWithTexture(jellyBreak->getTexture());
    jellyBreak->setPosition(ppoint);
    batch->addChild(jellyBreak);
    layer->addChild(batch, 10);
    jellyBreak->setAutoRemoveOnFinish(true);
}