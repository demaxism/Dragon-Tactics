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