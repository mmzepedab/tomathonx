//
//  Enemy.cpp
//  tomathonx
//
//  Created by Mario Zepeda on 12/6/16.
//
//

#include "Enemy.h"
#include "HelloWorldScene.h"

#include <sstream>
#include <string>
#include <iostream>

using namespace cocos2d;

Enemy::Enemy() {}

Enemy::~Enemy() {}

Enemy* Enemy::create()
{
    Enemy* pSprite = new Enemy();
    
    int randomValue = cocos2d::random(1,3);
    char enemyFile[100] = {0};
    
    switch (randomValue) {
        case 1:
            sprintf(enemyFile, "bird-1.png");
            break;
        case 2:
            sprintf(enemyFile, "bug-1.png");
            break;
        case 3:
            sprintf(enemyFile, "cat-1.png");
            break;
            
        default:
            break;
    }
    
    if (pSprite->initWithSpriteFrameName(enemyFile))
    {
        pSprite->retain();
        
        //pSprite->autorelease();
        
        pSprite->initOptions();
        
        pSprite->addEvents();
        
        return pSprite;
    }
    
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

void Enemy::initOptions()
{
    // do things here like setTag(), setPosition(), any custom logic.
}

void Enemy::addEvents()
{
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        cocos2d::Vec2 p = touch->getLocation();
        cocos2d::Rect rect = this->getBoundingBox();
        
        if(rect.containsPoint(p))
        {
            return true; // to indicate that we have consumed it.
        }
        
        return false; // we did not consume this event, pass thru.
    };
    
    listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        
        Enemy::touchEvent(touch, touch->getLocation());
        cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
    };
    
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);
}

void Enemy::touchEvent(cocos2d::Touch* touch, cocos2d::Vec2 _point)
{
    CCLOG("touched MySprite");
    
    //removeFromParent();
    //createExplotion(_point);
    
    //HelloWorld* helloWorldScene = (HelloWorld *)this->getParent();
    
    
    HelloWorld *helloWorldScene = (HelloWorld *)this->getParent();

    if(helloWorldScene != NULL){
        helloWorldScene->createExplotion(_point);
    }
    
    this->removeFromParentAndCleanup(false);
    
    //this->removeFromParentAndCleanup(true);
    //removeFromParent();
    
}


void Enemy::createExplotion(cocos2d::Vec2 position){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto s_centre = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    //Add Sprite With Animation Sample - What's added to the screen is the spriteBatch
    SpriteBatchNode* spritebatch = SpriteBatchNode::create("explosion.png");
    auto Sprite1 = Sprite::createWithSpriteFrameName("frame-1.png");
    spritebatch->setPosition(position);
    spritebatch->addChild(Sprite1);
    addChild(spritebatch, 100);
    
    Vector<SpriteFrame*> animFrames(6);
    
    char str[100] = {0};
    for(int i = 1; i < 7; i++)
    {
        sprintf(str, "frame-%d.png", i);
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( str );
        animFrames.pushBack(frame);
    }
    
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    Sprite1->runAction( RepeatForever::create( Animate::create(animation) ) );
    
    //return sequence;
    
}

