//
//  BattleUI.cpp
//  DragonTactic
//
//  Created by 丁 中明 on 5/3/15.
//
//

#include "BattleUI.h"

FlashGrid* FlashGrid::create(const std::string &fn)
{
    FlashGrid* unit = new (std::nothrow) FlashGrid();
    unit->initWithFile(fn);
    unit->autorelease();
    
    return unit;
}

bool FlashGrid::initWithFile(const std::string& filename)
{
    if( Sprite::initWithFile(filename) )
    {
        
    }
    
    return true;
}

FlashGrid::FlashGrid(void)
{
    offset = Vec2(0, 0);
    zoffset = -0.4;
}

FlashGrid::~FlashGrid(void)
{
}

void FlashGrid::startFlash()
{
//    BlendFunc light;
//    light.dst = GL_ONE;
//    light.src = GL_ZERO;
//    setBlendFunc(light);
    nFrame = 0;
    schedule( CC_SCHEDULE_SELECTOR(FlashGrid::doStep) );
}

void FlashGrid::doStep(float delta)
{
    setOpacity(0x80 + 0x50 * cosf(nFrame * 0.3f));
    nFrame++;
}