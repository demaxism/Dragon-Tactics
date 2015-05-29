//
//  BattleUI.h
//  DragonTactic
//
//  Created by 丁 中明 on 5/3/15.
//
//

#ifndef __DragonTactic__BattleUI__
#define __DragonTactic__BattleUI__

#include <stdio.h>
#include "SpriteBase.h"
#include <cocos/ui/CocosGUI.h>

USING_NS_CC;
using namespace cocos2d::ui;

class FlashGrid : public SpriteBase
{
public:
    FlashGrid(void);
    virtual ~FlashGrid(void);
    void startFlash();
    bool initWithFile(const std::string& filename) override;
    static FlashGrid* create(const std::string &fn);
    bool doFlash;
    
private:
    int nFrame;
    void doStep(float delta);
};

class CrossMark : public SpriteBase
{
public:
    bool initWithFile(const std::string& filename) override;
    static CrossMark* create(const std::string &fn);
    void showForCurrentUnit(SpriteBase* unit);
    void hide();
};

class TargetMark : public SpriteBase
{
public:
    TargetMark(void);
    bool initWithFile(const std::string& filename) override;
    static TargetMark* create(const std::string &fn);
    void startFlash();
    void stopFlash();
    bool isFlashing;
private:
    int nFrame;
    void doStep(float delta);
};

class AttackTarget : public SpriteBase
{
public:
    AttackTarget(void);
    bool initWithFile(const std::string& filename) override;
    static AttackTarget* create(const std::string &fn);
    void lockTarget(Sprite* from, Sprite* target);
    void stopLock();
private:
    Sprite* _from;
    Sprite* _target;
    bool _isLocking;
    int nFrame;
    void doStep(float delta);
};

class UpperInfoPanel : public Sprite
{
private:
    Sprite* _panel;
    Sprite* _lowPanel;
    Sprite* _icon;
    Sprite* _enemy;
    Sprite* _lastUnit;
    Sprite* _lastEnemy;
    Button* _btnDecide;
    Vec2 _hidePos;
    Vec2 _showPos;
    Vec2 _lowHidePos;
    Vec2 _lowShowPos;
    bool _isShowing;
    bool _isLowShowing;
public:
    UpperInfoPanel(void);
    bool initWithFile(const std::string& filename) override;
    void showUnitInfo(Sprite* unit);
    void hideUnitInfo();
    void showLow();
    void hideLow();
    void showEnemyInfo(Sprite* enemy);
    static UpperInfoPanel* create(const std::string &fn);
    void touchEvent(Ref *pSender, Widget::TouchEventType type);
};

class MapBg : public Sprite
{
public:
    MapBg(void);
    void moveTo(Vec2);
    void turnOff();
    void turnOn();
    Color3B bgColor;
private:
    Sprite* _bg;
    LayerColor* _colorBg;
};

#endif /* defined(__DragonTactic__BattleUI__) */
