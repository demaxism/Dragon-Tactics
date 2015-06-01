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
    Size size = Director::getInstance()->getVisibleSize();
    Sprite::init();
    _icon = nullptr;
    _lastUnit = nullptr;
    _enemy = nullptr;
    _lastEnemy = nullptr;
    _hidePos = Vec2(0, 55);
    _showPos = Vec2(0, -30);
    _panel = Sprite::create("up_panel.png");
    _panel->setPosition(_hidePos);
    addChild(_panel);
    
    _lowHidePos = Vec2(0, -size.height - 52);
    _lowShowPos = Vec2(0, -size.height + 50);
    _lowPanel = Sprite::create();
    _lowPanel->setPosition(_lowShowPos);
    addChild(_lowPanel);
    _isShowing = false;
    _isLowShowing = true;
    
    _btnDecide = Button::create("btn_decide.png");
    _btnDecide->setPosition(Vec2(-size.width/2+80, -size.height+50));
    _btnDecide->addTouchEventListener(CC_CALLBACK_2(UpperInfoPanel::touchEvent, this));
    _btnDecide->setZoomScale(0.4f);
    _btnDecide->setPressedActionEnabled(true);
    addChild(_btnDecide, 2);
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
        if (_icon != nullptr) {
            _icon->removeFromParentAndCleanup(true);
            _icon = nullptr;
        }
        if (_enemy != nullptr) {
            _enemy->removeFromParentAndCleanup(true);
            _enemy = nullptr;
        }
        _lastUnit = nullptr;
    }
}

void UpperInfoPanel::showLow()
{
    if (!_isLowShowing) {
        _lowPanel->stopAllActions();
        _btnDecide->stopAllActions();
        _lowPanel->runAction(MoveTo::create(0.2, _lowShowPos));
        _btnDecide->setVisible(true);
        _btnDecide->runAction(ScaleTo::create(0.2, 1));
        _isLowShowing = true;
    }
}

void UpperInfoPanel::hideLow()
{
    if (_isLowShowing) {
        _lowPanel->stopAllActions();
        _btnDecide->stopAllActions();
        
        _btnDecide->runAction(ScaleTo::create(0.1, 0));
        
        CallFunc* func = CallFunc::create([&] () {
            _btnDecide->setVisible(false);
            _lowPanel->runAction(MoveTo::create(0.2, _lowHidePos));
        });
        _lowPanel->runAction(Sequence::create(DelayTime::create(0.1), func, NULL));
        _isLowShowing = false;
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

MapBg::MapBg()
{
    _bgType = 2;
    Sprite::init();
    _winSize = Director::getInstance()->getVisibleSize();
    
    if (_bgType == 0) { // color
        bgColor = Color3B(0x11, 0x6b, 0xb7);
        _colorBg = LayerColor::create(Color4B(bgColor.r,bgColor.g,bgColor.b,0xff), _winSize.width, _winSize.height);
        this->addChild(_colorBg);
    }
    else if (_bgType == 1) { // depth
        _bg = Sprite::create("map_bg01.jpg");
        addChild(_bg);
        _bg->setColor(Color3B::GRAY);
        _bg->setScale(2.4);
    }
    else if (_bgType == 2) { // sea
        _sea = Sprite::create("sea-0.png");
        addChild(_sea);
    }
}

void MapBg::moveTo(Vec2 moveRate)
{
    if (_bgType == 1) {
        Size size = Director::getInstance()->getVisibleSize();
        Vec2 pos = Vec2(moveRate.x * 200 + size.width /2, moveRate.y * 100 + size.height /2);
        _bg->setPosition(pos);
    }
}

void MapBg::updateSeaPos(Vec2 mapPos) {
    if (_bgType == 2) {
        float xOffset = (int)mapPos.x % 32 + _winSize.width / 2;
        float yOffset = (int)mapPos.y % 32 + _winSize.height / 2;
        _sea->setPosition(Vec2(xOffset, yOffset));
    }
}

void MapBg::turnOff()
{
    if (_bgType == 0)
        _colorBg->setColor(Color3B::BLACK);
    else if (_bgType == 1)
        _bg->setColor(Color3B::BLACK);
    
}

void MapBg::turnOn()
{
    if (_bgType == 0)
        _colorBg->setColor(bgColor);
    else if (_bgType == 1)
        _bg->setColor(Color3B::GRAY);
}


