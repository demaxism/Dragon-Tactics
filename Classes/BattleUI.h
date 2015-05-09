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

USING_NS_CC;

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

class UpperInfoPanel : public Sprite
{
private:
    Sprite* _panel;
    Sprite* _icon;
    Sprite* _lastUnit;
    Vec2 _hidePos;
    Vec2 _showPos;
    bool _isShowing;
public:
    UpperInfoPanel(void);
    bool initWithFile(const std::string& filename) override;
    void showUnitInfo(Sprite* unit);
    void hideUnitInfo();
    static UpperInfoPanel* create(const std::string &fn);
};

#endif /* defined(__DragonTactic__BattleUI__) */
