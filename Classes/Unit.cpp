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

Rect Unit::getRect()
{
    auto s = getTexture()->getContentSize();
    return Rect(-s.width / 2, -s.height / 2, s.width, s.height);
}

Vec2 Unit::tilePosition(int x, int y)
{
    Size tileSize = GameManager::getInstance()->tileSize;
    return Vec2(tileSize.width * x + 15 + tileSize.width * 0.5f, tileSize.height * y + tileSize.height * 0.7f);
}

void Unit::alignTile()
{
    Vec2 pos = getPosition();
    Size tileSize = GameManager::getInstance()->tileSize;
    int x = pos.x / tileSize.width;
    int y = pos.y / tileSize.height;
    Vec2 posAligned = tilePosition(x, y);
    //runAction(MoveTo::create(0.1, posAligned)); // positionZ become strange when using action
    float sz = -y - 1.5;
    setPosition(posAligned);
    setPositionZ(sz);
//    CCLOG("zs:%f¥n",sz);
//    CCLOG("zorder:%d¥n",getLocalZOrder());
    
//    auto tiledMap = GameManager::getInstance()->tiledMap;
//    auto layer = tiledMap->getLayer("objects");
//    auto mapSize = tiledMap->getMapSize();
//    if (y > 0 && y < mapSize.height) {
//        Sprite* sp = layer->getTileAt(Vec2(x,mapSize.height - y -2));
//        if (sp != nullptr) {
//            float z = sp->getPositionZ();
//            CCLOG("zt:%f¥n",z);
//        }
//    }

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
    GameManager::getInstance()->isUnitGrabbed = true;
    GameManager::getInstance()->currentUnit = this;
    GameManager::getInstance()->touchLocation = touch->getLocation();
    setScale(1.3f, 1.3f);
    setPosition(this->getPosition() + _fingerAdjust);
    auto parent = getParent();
    setPositionZ(1000);
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
    GameManager::getInstance()->currentUnit = nullptr;
    alignTile();
}

