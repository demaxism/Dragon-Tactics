//
//  Unit.h
//  DragonTactic
//
//  Created by 丁 中明 on 4/24/15.
//
//

#ifndef __DragonTactic__Unit__
#define __DragonTactic__Unit__

#include "SpriteBase.h"
USING_NS_CC;

typedef enum tagUnitState
{
    kUnitStateGrabbed,
    kUnitStateUngrabbed
} UnitState;

class Unit : public SpriteBase
{
    UnitState _state;
    Vec2 _fingerAdjust = Vec2(0, 15.0f); // make the sprite move upward when grabbed
    Vec2 _touchLocation = Vec2(0, 0);
    Sprite* _checkMark;
    bool _isActionFinished;

public:
    uint moveRange;
    uint attackRange;
    SpriteBase* attackTarget;
    std::string charName;
    
    Unit(void);
    virtual ~Unit(void);
    bool initWithFile(const std::string& filename) override;
    bool initWithTexture(Texture2D *texture) override;
    
    virtual void onEnter() override;
    virtual void onExit() override;
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void actionFinish();
    void actionEnable();
    bool getIsActionFinished() { return _isActionFinished; }
    
    static Unit* create(const std::string &fn);
    static Unit* createWithTexture(Texture2D *texture);
};

#endif /* defined(__DragonTactic__Unit__) */
