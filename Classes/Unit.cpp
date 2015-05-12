//
//  Unit.cpp
//  DragonTactic
//
//  Created by 丁 中明 on 4/24/15.
//
//

#include "Unit.h"
#include "GameManager.h"

Unit::Unit(void)
{
    offset = Vec2(0, 15);
    zoffset = 0;
    moveRange = 1;
    attackRange = 1;
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

Unit* Unit::createWithTexture(Texture2D *texture)
{
    Unit* unit = new (std::nothrow) Unit();
    unit->initWithTexture(texture);
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

bool Unit::initWithTexture(Texture2D *texture)
{
    if (Sprite::initWithTexture(texture)) {
        _state = kUnitStateUngrabbed;
    }
    return true;
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

bool Unit::onTouchBegan(Touch* touch, Event* event)
{
    CCLOG("Unit::onTouchBegan id = %d, x = %f, y = %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
    
    if (_state != kUnitStateUngrabbed) return false;
    if ( !containsTouchLocation(touch) ) return false;
    
    _state = kUnitStateGrabbed;
    GameManager::getInstance()->isUnitGrabbed = true;
    GameManager::getInstance()->currentUnit = this;
    GameManager::getInstance()->touchLocation = touch->getLocation();
    setScale(1.3f, 1.3f);
    setPosition(this->getPosition() + _fingerAdjust);
    auto parent = getParent();
    parent->reorderChild(this, 1000);
    setPositionZ(1000);
    
    auto evt = EventCustom(EVT_UNITGRABBEGAN);
    evt.setUserData(&mapGrid);
    getEventDispatcher()->dispatchEvent(&evt);
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
    GameManager::getInstance()->touchLocation = touch->getLocation();
    auto diff = touch->getDelta();
    GameManager::getInstance()->grabDiff = diff;
    auto currentPos = this->getPosition();
    switch (GameManager::getInstance()->movePattern) {
        case mpGrab:
            setPosition(currentPos + diff);
            break;
        case mpGrabx2:
        {
            setPosition(currentPos + diff * 2.0f);
            // issue event
            auto evt = EventCustom(EVT_UNITGRABBING);
            evt.setUserData(&diff);
            getEventDispatcher()->dispatchEvent(&evt);
            break;
        }
        case mpBring:
        {
            setPosition(currentPos + diff);
        }
            break;
        default:
            break;
    }
}

void Unit::onTouchEnded(Touch* touch, Event* event)
{
    CCASSERT(_state == kUnitStateGrabbed, "Unit - Unexpected state!");
    setScale(1.0f, 1.0f);
    setPosition(this->getPosition() - _fingerAdjust);
    _state = kUnitStateUngrabbed;
    GameManager::getInstance()->isUnitGrabbed = false;

    // back to initial pos
    bool isMoved = false;
    if (!GameManager::getInstance()->isMovable) {
        flyToGrid(mapGrid);
    }
    else {
        alignTile();
        isMoved = true;
    }
    auto evt = EventCustom(EVT_UNITGRABEND);
    evt.setUserData(&isMoved);
    getEventDispatcher()->dispatchEvent(&evt);
}


