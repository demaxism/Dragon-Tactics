//
//  Unit.h
//  DragonTactic
//
//  Created by 丁 中明 on 4/24/15.
//
//

#ifndef __DragonTactic__Unit__
#define __DragonTactic__Unit__

USING_NS_CC;

typedef enum tagUnitState
{
    kUnitStateGrabbed,
    kUnitStateUngrabbed
} UnitState;

class Unit : public Sprite
{
    UnitState _state;
public:
    Unit(void);
    virtual ~Unit(void);
    bool initWithFile(const std::string& filename) override;
    
    Rect getRect();
    virtual void onEnter() override;
    virtual void onExit() override;
    bool containsTouchLocation(Touch* touch);
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    
    static Unit* create(const std::string &fn);
};

#endif /* defined(__DragonTactic__Unit__) */
