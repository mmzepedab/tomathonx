//
//  Enemy.h
//  tomathonx
//
//  Created by Mario Zepeda on 12/6/16.
//
//

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "cocos2d.h"

class Enemy : public cocos2d::Sprite
{
public:
    Enemy();
    ~Enemy();
    static Enemy* create();
    
    void initOptions();
    
    void addEvents();
    void touchEvent(cocos2d::Touch* touch, cocos2d::Vec2 _p);
    
    void createExplotion(cocos2d::Vec2 position);
    
private:
    
};

#endif /* Enemy_h */
