//
//  Monster.cpp
//  DragonTactic
//
//  Created by 丁 中明 on 5/8/15.
//
//

#include "Monster.h"
#include "GameManager.h"

Monster::Monster(void)
{
    offset = Vec2(0, 15);
    zoffset = 0;
    moveRange = 1;
    isFlashing = false;
    isTouchBegin = false;
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
    // Register Touch Event
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(Monster::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(Monster::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Monster::onExit()
{
    //    auto director = Director::getInstance();
    //    director->getTouchDispatcher()->removeDelegate(this);
    Sprite::onExit();
}

void Monster::startFlash()
{
    if (!isFlashing) {
        isFlashing = true;
        nFrame = 0;
        schedule( CC_SCHEDULE_SELECTOR(Monster::doStep) );
    }
}

void Monster::doStep(float delta)
{
    int c = cosf(nFrame * 0.2) * 0x7F + 0x7F;
    setColor(Color3B(0xFF, c, c));
    nFrame++;
}

void Monster::stopFlash()
{
    if (isFlashing) {
        unschedule(CC_SCHEDULE_SELECTOR(Monster::doStep));
        setColor(Color3B(0xFF, 0xFF, 0xFF));
        isFlashing = false;
    }
}

bool Monster::onTouchBegan(Touch* touch, Event* event)
{
    if ( !containsTouchLocation(touch) ) return false;
    isTouchBegin = true;
    return true;
}

void Monster::onTouchEnded(Touch* touch, Event* event)
{
    if ( containsTouchLocation(touch) && isTouchBegin) {
        auto evt = EventCustom(EVT_ENEMYTOUCHED);
        evt.setUserData(this);
        getEventDispatcher()->dispatchEvent(&evt);
    }
    isTouchBegin = false;
}