//
//  Unit.cpp
//  DragonTactic
//
//  Created by 丁 中明 on 4/24/15.
//
//

#include "Unit.h"

Unit::Unit(void)
{
}

Unit::~Unit(void)
{
}

Unit* Unit::create(const std::string &fn)
{
    Unit* unit = new (std::nothrow) Unit();
    unit->initWithFile(fn);
    unit->autorelease();
    
    return unit;
}

bool Unit::initWithFile(const std::string& filename)
{
    if( Sprite::initWithFile(filename) )
    {
        _state = kUnitStateUngrabbed;
    }
    
    return true;
}

Rect Unit::getRect()
{
    auto s = getTexture()->getContentSize();
    return Rect(-s.width / 2, -s.height / 2, s.width, s.height);
}

void Unit::onEnter()
{
    Sprite::onEnter();
    
    // Register Touch Event
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(Unit::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(Unit::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(Unit::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Unit::onExit()
{
    //    auto director = Director::getInstance();
    //    director->getTouchDispatcher()->removeDelegate(this);
    Sprite::onExit();
}

bool Unit::containsTouchLocation(Touch* touch)
{
    return getRect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

bool Unit::onTouchBegan(Touch* touch, Event* event)
{
    CCLOG("Unit::onTouchBegan id = %d, x = %f, y = %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
    
    if (_state != kUnitStateUngrabbed) return false;
    if ( !containsTouchLocation(touch) ) return false;
    
    _state = kUnitStateGrabbed;
    CCLOG("return true");
    return true;
}

void Unit::onTouchMoved(Touch* touch, Event* event)
{
    // If it weren't for the TouchDispatcher, you would need to keep a reference
    // to the touch from touchBegan and check that the current touch is the same
    // as that one.
    // Actually, it would be even more complicated since in the Cocos dispatcher
    // you get Sets instead of 1 UITouch, so you'd need to loop through the set
    // in each touchXXX method.
    
    //CCLOG("Unit::onTouchMoved id = %d, x = %f, y = %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
    
    CCASSERT(_state == kUnitStateGrabbed, "Paddle - Unexpected state!");
    
    //auto touchPoint = touch->getLocation();
    
    auto diff = touch->getDelta();
    auto currentPos = this->getPosition();
    setPosition(currentPos + diff);
}

void Unit::onTouchEnded(Touch* touch, Event* event)
{
    CCASSERT(_state == kUnitStateGrabbed, "Unit - Unexpected state!");
    
    _state = kUnitStateUngrabbed;
}
