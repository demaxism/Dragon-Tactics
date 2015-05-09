//
//  Monster.cpp
//  DragonTactic
//
//  Created by 丁 中明 on 5/8/15.
//
//

#include "Monster.h"

Monster::Monster(void)
{
    offset = Vec2(0, 15);
    zoffset = 0;
    moveRange = 1;
}

Monster::~Monster(void)
{
}

Monster* Monster::create(const std::string &fn)
{
    Monster* monster = new (std::nothrow) Monster();
    monster->initWithFile(fn);
    monster->autorelease();
    
    return monster;
}

bool Monster::initWithFile(const std::string& filename)
{
    if( Sprite::initWithFile(filename) )
    {
        
    }
    
    return true;
}

void Monster::onEnter()
{
    Sprite::onEnter();
}

void Monster::onExit()
{
    //    auto director = Director::getInstance();
    //    director->getTouchDispatcher()->removeDelegate(this);
    Sprite::onExit();
}