//
//  BattleUI.cpp
//  DragonTactic
//
//  Created by 丁 中明 on 5/3/15.
//
//

#include "BattleUI.h"
#include "GameManager.h"

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
TargetMark::TargetMark()
{
    isFlashing = true;
}

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
        GameManager::getInstance()->isMovable = true;
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
        GameManager::getInstance()->isMovable = false;
        setVisible(false);
        unschedule(CC_SCHEDULE_SELECTOR(TargetMark::doStep));
        isFlashing = false;
    }
}

////////////////////////
// Attack Target
AttackTarget::AttackTarget()
{
    _isLocking = true;
}

AttackTarget* AttackTarget::create(const std::string &fn)
{
    AttackTarget* unit = new (std::nothrow) AttackTarget();
    unit->initWithFile(fn);
    unit->autorelease();
    
    return unit;
}

bool AttackTarget::initWithFile(const std::string& filename)
{
    if( Sprite::initWithFile(filename) )
    {
        
    }
    
    return true;
}

void AttackTarget::lockTarget(Sprite* from, Sprite* target)
{
    if (!_isLocking) {
        _isLocking = true;
        setVisible(true);
        _from = from;
        _target = target;
        nFrame = 0;
        schedule( CC_SCHEDULE_SELECTOR(AttackTarget::doStep) );
    }
}

void AttackTarget::stopLock()
{
    if (_isLocking) {
        setVisible(false);
        unschedule(CC_SCHEDULE_SELECTOR(AttackTarget::doStep));
        _isLocking = false;
    }
}

void AttackTarget::doStep(float delta)
{
    setRotation(nFrame * 2);
    if (nFrame % 50 == 0) {
        setPosition(_from->getPosition());
        setOpacity(0x60);
        runAction(MoveTo::create(0.2, _target->getPosition()));
        runAction(FadeTo::create(0.2, 0xff));
    }
    nFrame++;
}

////////////////////////
// UpperInfoPanel
UpperInfoPanel::UpperInfoPanel()
{
    Sprite::init();
    _icon = nullptr;
    _lastUnit = nullptr;
    _enemy = nullptr;
    _lastEnemy = nullptr;
    _hidePos = Vec2(0, 25);
    _showPos = Vec2(0, -30);
    _panel = Sprite::create("up_panel.png");
    _panel->setPosition(_hidePos);
    addChild(_panel);
    _isShowing = false;
    
    Size size = Director::getInstance()->getVisibleSize();
    _btnDecide = Button::create("btn_decide.png");
    _btnDecide->setPosition(Vec2(size.width -90, -40));
    _btnDecide->addTouchEventListener(CC_CALLBACK_2(UpperInfoPanel::touchEvent, this));
    _btnDecide->setZoomScale(0.4f);
    _btnDecide->setPressedActionEnabled(true);
    _panel->addChild(_btnDecide, 2);
    _btnDecide->setVisible(false);
}

void UpperInfoPanel::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
            break;
            
        case Widget::TouchEventType::MOVED:
            break;
            
        case Widget::TouchEventType::ENDED:
        {
            auto evt = EventCustom(EVT_ACTIONDECIDED);
            getEventDispatcher()->dispatchEvent(&evt);
        }
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}

UpperInfoPanel* UpperInfoPanel::create(const std::string &fn)
{
    UpperInfoPanel* unit = new (std::nothrow) UpperInfoPanel();
    unit->initWithFile(fn);
    unit->autorelease();
    
    return unit;
}

bool UpperInfoPanel::initWithFile(const std::string& filename)
{
    if( Sprite::initWithFile(filename) )
    {
        
    }
    
    return true;
}

void UpperInfoPanel::showUnitInfo(Sprite* unit)
{
    if (!_isShowing) {
        _panel->runAction(MoveTo::create(0.2, _showPos));
        _btnDecide->setVisible(true);
        _isShowing = true;
    }
    if (_lastUnit != unit) {
        if (_enemy != nullptr) {
            _enemy->removeFromParentAndCleanup(true);
            _enemy = nullptr;
        }
        if (_icon != nullptr)
            _icon->removeFromParentAndCleanup(true);
        _icon = Sprite::createWithTexture(unit->getTexture());
        _icon->setPosition(Vec2(80, 55));
        _icon->setScale(0.7);
        _panel->addChild(_icon);
        _lastUnit = unit;
    }
}

void UpperInfoPanel::hideUnitInfo()
{
    if (_isShowing) {
        _panel->runAction(MoveTo::create(0.2, _hidePos));
        _isShowing = false;
        _icon->removeFromParentAndCleanup(true);
        _icon = nullptr;
        _enemy->removeFromParentAndCleanup(true);
        _enemy = nullptr;
        _lastUnit = nullptr;
        _btnDecide->setVisible(false);
    }
}

void UpperInfoPanel::showEnemyInfo(Sprite* enemy)
{
    if (_enemy != nullptr)
        _enemy->removeFromParentAndCleanup(true);
    _enemy = Sprite::createWithTexture(enemy->getTexture());
    _enemy->setPosition(Vec2(560, 55));
    _enemy->setScale(0.7);
    _panel->addChild(_enemy);
}



