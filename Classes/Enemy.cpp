//
//  Enemy.cpp
//  tomathonx
//
//  Created by Mario Zepeda on 12/6/16.
//
//

#include "Enemy.h"

using namespace cocos2d;

Enemy::Enemy() {}

Enemy::~Enemy() {}

Enemy* Enemy::create()
{
    Enemy* pSprite = new Enemy();
    
    if (pSprite->initWithSpriteFrameName("frame-1.png"))
    {
        pSprite->autorelease();
        
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
        cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
        Enemy::touchEvent(touch, touch->getLocation());
    };
    
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);
}

void Enemy::touchEvent(cocos2d::Touch* touch, cocos2d::Vec2 _point)
{
    CCLOG("touched MySprite");
    
    removeFromParent();
    //createExplotion(_point);
    
}


void Enemy::createExplotion(cocos2d::Vec2 position){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto s_centre = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    SpriteBatchNode* spritebatch = SpriteBatchNode::create("bird.png");
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    //cache->addSpriteFramesWithFile("explosion.plist");
    Sprite* Sprite1 = Sprite::createWithSpriteFrameName("frame-1.png");
    Sprite1->setPosition(position);
    Sprite1->setScale(0.3);
    spritebatch->addChild(Sprite1);
    addChild(spritebatch);
    Vector<SpriteFrame*> animFrames(6);
    char str[100] = {0};
    for(int i = 1; i < 6; i++)
    {
        sprintf(str, "frame-%d.png", i);
        SpriteFrame* frame = cache->getSpriteFrameByName( str );
        //animFrames->addObject(frame);
        animFrames.pushBack(frame);
    }
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
    //auto spri = Sprite::createWithSpriteFrame(animFrames.front());
    //Sprite1->runAction( RepeatForever::create( Animate::create(animation) ) );
    auto sequence = Sequence::create(Animate::create(animation), RemoveSelf::create() , NULL);
    Sprite1->runAction(sequence);
    
    //return sequence;
    
}

