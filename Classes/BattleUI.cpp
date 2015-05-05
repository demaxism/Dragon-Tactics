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
    doFlash = true;
    schedule( CC_SCHEDULE_SELECTOR(FlashGrid::doStep) );
}

void FlashGrid::doStep(float delta)
{
    if (doFlash)
        setOpacity(0x80 + 0x50 * cosf(nFrame * 0.2f));
    else
        setOpacity(0xFF);
    nFrame++;
}

////////////////////////
// CrossMark

CrossMark* CrossMark::create(const std::string &fn)
{
    CrossMark* unit = new (std::nothrow) CrossMark();
    unit->initWithFile(fn);
    unit->autorelease();
    
    return unit;
}

bool CrossMark::initWithFile(const std::string& filename)
{
    if( Sprite::initWithFile(filename) )
    {
        
    }
    
    return true;
}

void CrossMark::showForCurrentUnit(SpriteBase* unit)
{
    setVisible(true);
    setPosition(unit->getPosition() + Vec2(0, 90));
}

void CrossMark::hide()
{
    setVisible(false);
}

////////////////////////
// TargetMark

TargetMark* TargetMark::create(const std::string &fn)
{
    TargetMark* unit = new (std::nothrow) TargetMark();
    unit->initWithFile(fn);
    unit->autorelease();
    
    return unit;
}

bool TargetMark::initWithFile(const std::string& filename)
{
    if( Sprite::initWithFile(filename) )
    {
        
    }
    
    return true;
}

void TargetMark::startFlash()
{
    if (!isFlashing) {
        isFlashing = true;
        setVisible(true);
        nFrame = 0;
        schedule( CC_SCHEDULE_SELECTOR(TargetMark::doStep) );
    }
}

void TargetMark::doStep(float delta)
{
    float scale = 1.2 + cosf(nFrame * 0.2) * 0.2;
    setScale(scale);
    nFrame++;
}

void TargetMark::stopFlash()
{
    if (isFlashing) {
        setVisible(false);
        unschedule(CC_SCHEDULE_SELECTOR(TargetMark::doStep));
        isFlashing = false;
    }
}
