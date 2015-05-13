//
//  Charactor.cpp
//  DragonTactic
//
//  Created by 丁 中明 on 5/13/15.
//
//

#include "Charactor.h"
#include "GameManager.h"

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

////////////////////////////////////////////////////////////////////////
// ActionLayer
ActionLayer::ActionLayer()
{
    _winSize = Director::getInstance()->getVisibleSize();
    _isShowing = false;
    _mask = LayerColor::create(Color4B(0,0,0,0x80));
    _mask->setContentSize( _winSize );
    this->addChild(_mask);
    setVisible(false);
    _actionLabel = nullptr;
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
            this->showChars();
        });
        auto sequence = Sequence::create(movein, delay, moveout, final, NULL);
        _actionLabel->runAction(sequence);
        
        _isShowing = true;
    }
}

void ActionLayer::hideLayer()
{
    if (_isShowing) {
        if (_left != nullptr) {
            _left->removeFromParentAndCleanup(true);
            _left = nullptr;
        }
        _battleList->clear();
        setVisible(false);
        _isShowing = false;
    }
}

void ActionLayer::addPair(const std::string& left, const std::string& right)
{
    BattlePair* bp = new BattlePair();
    bp->left = left;
    bp->right = right;
    _battleList->push_back(bp);
}

void ActionLayer::showChars()
{
    BattlePair* bp = _battleList->at(0);
    _left = new Charactor(bp->left);
    addChild(_left,2);
    _left->setPosition(Vec2(_winSize.width/2 - 230, _winSize.height - 400));
    auto delay = DelayTime::create(1);
    CallFunc* func = CallFunc::create([&] () {
        _left->fight();
    });
    auto delayHitted = DelayTime::create(1.5);
    CallFunc* func2 = CallFunc::create([&] () {
        this->hideLayer();
        auto evt = EventCustom(EVT_ACTIONFINISHED);
        getEventDispatcher()->dispatchEvent(&evt);
    });
    runAction(Sequence::create(delay, func, delayHitted, func2, NULL));
}



