//
//  Charactor.cpp
//  DragonTactic
//
//  Created by 丁 中明 on 5/13/15.
//
//

#include "Charactor.h"
#include "GameManager.h"
#include "Unit.h"
#include "Monster.h"

Charactor::Charactor()
{
    
}

Charactor::Charactor(const std::string& filename)
{
    Sprite::init();
    _fight = nullptr;
    _name = filename;
    std::string normal = filename + "_n.png";
    _normal = Sprite::create(normal);
    _normal->setPosition(Vec2(-100, _normal->getContentSize().height / 2));
    _normal->setOpacity(0);
    addChild(_normal);
    _normal->runAction(MoveTo::create(0.1, Vec2(0, _normal->getPosition().y)));
    _normal->runAction(FadeTo::create(0.1, 0xff));
}

void Charactor::fight()
{
    auto normalMoveout = MoveTo::create(0.1, Vec2(100, _normal->getPosition().y));
    CallFunc* normalRemove = CallFunc::create([&] () {
        _normal->removeFromParentAndCleanup(true);
        _normal = nullptr;
    });
    auto normalSequence = Sequence::create(normalMoveout, normalRemove, NULL);
    _normal->runAction(normalSequence);
    _normal->runAction(FadeTo::create(0.1, 0));
    
    std::string fight = _name + "_f.png";
    _fight = Sprite::create(fight);
    _fight->setPosition(Vec2(150, _fight->getContentSize().height / 2));
    _fight->setOpacity(0);
    addChild(_fight);
    
    auto delay = DelayTime::create(0.1);
    auto fightMovein = MoveTo::create(0.1, Vec2(250, _fight->getPosition().y));
    auto fightSlowin = MoveTo::create(1, Vec2(280, _fight->getPosition().y));
    auto fightMoveout = MoveTo::create(0.1, Vec2(300, _fight->getPosition().y));
    auto fightMoveSequence = Sequence::create(delay, fightMovein, fightSlowin, fightMoveout, NULL);
    _fight->runAction(fightMoveSequence);
    auto fightFadein = FadeTo::create(0.1, 0xff);
    auto fightFadeout = FadeTo::create(0.1, 0);
    auto fightFadeSequence = Sequence::create(delay, fightFadein, DelayTime::create(1), fightFadeout, NULL);
    _fight->runAction(fightFadeSequence);
}

void Charactor::hurt()
{
    _frame = 0;
    schedule( CC_SCHEDULE_SELECTOR(Charactor::hurtStep) );
}

void Charactor::hurtStep(float delta)
{
    _normal->setPosition(Vec2(5 * cosf(_frame), _normal->getPosition().y));
    if (_frame % 2 > 0) {
        _normal->setColor(Color3B(0xff, 0xff, 0xff));
    }
    else {
        _normal->setColor(Color3B(0xff, 0x00, 0x00));
    }
    if (_frame >= 20) {
        unschedule(CC_SCHEDULE_SELECTOR(Charactor::hurtStep));
        _normal->setColor(Color3B(0xff, 0xff, 0xff));
    }
    _frame++;
}

////////////////////////////////////////////////////////////////////////
// ActionLayer
ActionLayer::ActionLayer()
{
    _winSize = Director::getInstance()->getVisibleSize();
    _isShowing = false;
    _isBgShowing = false;
    _mask = LayerColor::create(Color4B(0,0,0,0x80));
    _mask->setContentSize( _winSize );
    this->addChild(_mask);
    setVisible(false);
    _actionLabel = nullptr;
    _rtex = nullptr;
    _left = nullptr;
    _right = nullptr;
    _eh = nullptr;
    _battleList = new std::vector<BattlePair*>();
}

void ActionLayer::showLayer()
{
    if (!_isShowing) {
        setVisible(true);
        _mask->setOpacity(0x80);
        if (_actionLabel != nullptr)
            _actionLabel->removeFromParentAndCleanup(true);
        _actionLabel = Sprite::create("action.png");
        _actionLabel->setPosition(Vec2(_winSize.width + 300, _winSize.height/2));
        addChild(_actionLabel);
        auto movein = MoveTo::create(0.1, Vec2(_winSize.width/2, _winSize.height/2));
        auto delay = DelayTime::create(0.6);
        auto moveout = MoveTo::create(0.1, Vec2(-300, _winSize.height/2));
        CallFunc* final = CallFunc::create([&] () {
            _actionLabel->removeFromParentAndCleanup(true);
            _actionLabel = nullptr;
            _mask->runAction(FadeTo::create(0.1, 0));
            _iBattle = 0;
            this->nextBattle();
        });
        auto sequence = Sequence::create(movein, delay, moveout, final, NULL);
        _actionLabel->runAction(sequence);
        
        _isShowing = true;
    }
}

void ActionLayer::hideLayer()
{
    if (_isShowing) {
        this->hideChars();
        _battleList->clear();
        this->hideBg();
        _isShowing = false;
    }
}

void ActionLayer::addPair(void* leftSprite, void* rightSprite)
{
    BattlePair* bp = new BattlePair();
    bp->leftSprite = leftSprite;
    bp->rightSprite = rightSprite;
    _battleList->push_back(bp);
}

void ActionLayer::showBg()
{
    if (_isBgShowing) return;
    
    if (_rtex != nullptr) {
        _rtex->removeFromParentAndCleanup(true);
    }
    bg = Sprite::create("bg_02.jpg");
    BlendFunc bf1;
    bf1.src = GL_DST_ALPHA;
    bf1.dst = GL_ZERO;
    bg->setBlendFunc(bf1);
    bg->retain();
    mask = Sprite::create("bg_mask.png");
    BlendFunc bf2;
    bf2.src = GL_ONE;
    bf2.dst = GL_ZERO;
    mask->setBlendFunc(bf2);
    mask->retain();
    
    int nFrame = 20;
    _cntMaskLoop = nFrame;
    _rtex = RenderTexture::create(_winSize.width, 500);
    addChild(_rtex, -1);
    _rtex->setPosition(Vec2(_winSize.width/2, _winSize.height - 250));
    
    CallFunc* stepFunc = CallFunc::create([&] () {
        if (_cntMaskLoop > 0) {
            _rtex->beginWithClear(0, 0, 0, 0);
            mask->setPosition(Vec2(_winSize.width/2, 250 + _cntMaskLoop * 25));
            mask->visit();
            bg->setPosition(Vec2(_winSize.width/2, 250));
            bg->visit();
            _rtex->end();
        }
        _cntMaskLoop--;
    });
    Sequence* sequence = Sequence::create(stepFunc, DelayTime::create(0.02), NULL);
    runAction(Repeat::create(sequence, nFrame)); // repeat nFrame times
    _isBgShowing = true;
    
    // for effect flash
    _flash = Sprite::create("bg_mask.png");
    addChild(_flash, 2);
    _flash->setPosition(_rtex->getPosition());
    BlendFunc bf3;
    bf3.src = GL_SRC_ALPHA;
    bf3.dst = GL_ONE;
    setBlendFunc(bf3);
    _flash->setVisible(false);
}

void ActionLayer::hideBg()
{
    if (!_isBgShowing) return;
    
    if (_flash != nullptr) {
        _flash->removeFromParentAndCleanup(true);
        _flash = nullptr;
    }
    
    int nFrame = 10;
    _cntMaskLoop = nFrame;

    CallFunc* stepFunc = CallFunc::create([&] () {
        if (_cntMaskLoop > 0) {
            _rtex->beginWithClear(0, 0, 0, 0);
            mask->setPosition(Vec2(_winSize.width/2, 750 - _cntMaskLoop * (50)));
            mask->visit();
            bg->setPosition(Vec2(_winSize.width/2, 250));
            bg->visit();
            _rtex->end();
        }
        else if (_cntMaskLoop == 0) {
            mask->release();
            bg->release();
            if (_rtex != nullptr) {
                _rtex->removeFromParentAndCleanup(true);
                _rtex = nullptr;
            }
            setVisible(false);
        }
        _cntMaskLoop--;
    });
    Sequence* sequence = Sequence::create(stepFunc, DelayTime::create(0.02), NULL);
    runAction(Repeat::create(sequence, nFrame+1)); // repeat nFrame times
    _isBgShowing = false;
}

void ActionLayer::showChars()
{
    float ypos = _winSize.height - 430;
    float xd = 210;
    BattlePair* bp = _battleList->at(_iBattle);
    Unit* left = (Unit*)bp->leftSprite;
    _left = new Charactor(left->charName);
    addChild(_left,2);
    _left->setPosition(Vec2(_winSize.width/2 - xd, ypos));
    auto delay = DelayTime::create(1);
    CallFunc* func = CallFunc::create([&] () {
        _left->fight();
    });
    auto delayHitted = DelayTime::create(1.6);
    CallFunc* func2 = CallFunc::create([&] () {
        this->nextBattle();
    });
    runAction(Sequence::create(delay, func, delayHitted, func2, NULL));
    
    Monster* right = (Monster*)bp->rightSprite;
    _right = new Charactor(right->charName);
    addChild(_right, 1);
    _right->setPosition(Vec2(_winSize.width/2 + xd, ypos));
    _right->setScaleX(-1);
    CallFunc* hurtFunc = CallFunc::create([&] () {
        _right->hurt();
        // effect
        _eh = new EffectHit();
        _eh->setPosition(_right->getPosition() + Vec2(-40, 150));
        addChild(_eh, 3);
        _eh->play();
        _flash->setVisible(true);
        _flash->setOpacity(0xff);
        _flash->runAction(FadeTo::create(0.4, 0));
    });
    runAction(Sequence::create(delay = DelayTime::create(1.2), hurtFunc, NULL));

    // send battle start event to main, for focus battle
    GameManager::getInstance()->currentUnit = left;
    auto evt = EventCustom(EVT_BATTLESTART);
    getEventDispatcher()->dispatchEvent(&evt);
}

void ActionLayer::hideChars()
{
    if (_left != nullptr) {
        _left->removeFromParentAndCleanup(true);
        _left = nullptr;
    }
    if (_right != nullptr) {
        _right->removeFromParentAndCleanup(true);
        _right = nullptr;
    }
    if (_eh != nullptr) {
        _eh->removeFromParentAndCleanup(true);
        _eh = nullptr;
    }
}

void ActionLayer::nextBattle()
{
    if (_iBattle < _battleList->size()) {
        this->showBg();
        this->hideChars();
        this->showChars();
    }
    else {
        this->hideLayer();
        auto evt = EventCustom(EVT_ACTIONFINISHED);
        getEventDispatcher()->dispatchEvent(&evt);
    }
    _iBattle++;
}


