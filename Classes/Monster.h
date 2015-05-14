//
//  Monster.h
//  DragonTactic
//
//  Created by 丁 中明 on 5/8/15.
//
//

#ifndef __DragonTactic__Monster__
#define __DragonTactic__Monster__

#include <stdio.h>
#include "SpriteBase.h"
USING_NS_CC;

class Monster : public SpriteBase
{
private:
    int nFrame;
    bool isFlashing;
    void doStep(float delta);
    bool isTouchBegin;
    
public:
    uint moveRange;
    std::string charName;
    
    Monster(void);
    virtual ~Monster(void);
    bool initWithFile(const std::string& filename) override;
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void startFlash();
    void stopFlash();
    bool getIsFlashing() { return isFlashing; }
    
    static Monster* create(const std::string &fn);
};

#endif /* defined(__DragonTactic__Monster__) */
