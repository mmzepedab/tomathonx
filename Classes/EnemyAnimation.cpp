//
//  EnemyAnimation.cpp
//  tomathonx
//
//  Created by Mario Zepeda on 12/10/16.
//
//

#include "EnemyAnimation.hpp"

using namespace cocos2d;

EnemyAnimation::EnemyAnimation() {}

EnemyAnimation::~EnemyAnimation() {}

EnemyAnimation* EnemyAnimation::create()
{
    EnemyAnimation* pSprite = new EnemyAnimation();
    
    int randomValue = cocos2d::random(1,3);
    char enemyFile[100] = {0};
    
    CCLOG("Creating object");
    
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
    
    if (pSprite->init());
    {
        pSprite->retain();
        
        //pSprite->autorelease();
        
        //pSprite->setContentSize(cocos2d::Size::Size(1, 1));
        
        pSprite->initOptions();
        
        //pSprite->addEvents();
        
        //pSprite->createSpriteAnimation();
        //Sprite *Sprite1 = Sprite::createWithSpriteFrameName("cat-1.png");
        //pSprite->setContentSize(Sprite1->getContentSize());
        
        return pSprite;
    }
    
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

void EnemyAnimation::initOptions()
{
    // do things here like setTag(), setPosition(), any custom logic.
    CCLOG("Creating Sprite Animation");
    
    //Add Sprite With Animation Sample - What's added to the screen is the spriteBatch
    SpriteBatchNode* spritebatch = SpriteBatchNode::create("cat.png");
    Sprite *Sprite1 = Sprite::createWithSpriteFrameName("cat-1.png");
    //Vec2 _origin;
    //Size _visibleSize;
    //Vec2 _s_centre;
    //_origin = Director::getInstance()->getVisibleOrigin();
    //_visibleSize = Director::getInstance()->getVisibleSize();
    //_s_centre = Vec2(_visibleSize.width/2 + _origin.x, _visibleSize.height/2 + _origin.y);
    //spritebatch->setPosition(_s_centre);
    
    Sprite1->setAnchorPoint(Vec2(0, 0));
    spritebatch->addChild(Sprite1);
    addChild(spritebatch);
    
    this->setContentSize(Sprite1->getContentSize());
    
    Vector<SpriteFrame*> animFrames(6);
    
    char str[100] = {0};
    for(int i = 1; i < 7; i++)
    {
        sprintf(str, "cat-%d.png", i);
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( str );
        animFrames.pushBack(frame);
    }
    
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    Sprite1->runAction( RepeatForever::create( Animate::create(animation) ) );

}

//SpriteBatchNode *EnemyAnimation::createSpriteAnimation(){
    
    
    //auto sequence = Sequence::create(Animate::create(animation), RemoveSelf::create() , NULL);
    //Sprite1->runAction(sequence);
    
    //-- Add Sprite With Animation Sample - What's added to the screen is the spriteBatch
    
    
    //return spritebatch;

//}
