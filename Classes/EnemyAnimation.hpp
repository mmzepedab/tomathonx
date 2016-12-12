//
//  EnemyAnimation.hpp
//  tomathonx
//
//  Created by Mario Zepeda on 12/10/16.
//
//

#ifndef EnemyAnimation_hpp
#define EnemyAnimation_hpp

#include "cocos2d.h"

class EnemyAnimation : public cocos2d::Sprite
{
public:
    EnemyAnimation();
    ~EnemyAnimation();
    static EnemyAnimation* create();
    
    void initOptions();
    
    void addEvents();
    void touchEvent(cocos2d::Touch* touch, cocos2d::Vec2 _p);
    
    void createExplotion(cocos2d::Vec2 position);
    
    cocos2d::SpriteBatchNode *createSpriteAnimation();
    
private:
    
};


#endif /* EnemyAnimation_hpp */
