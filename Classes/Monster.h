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
    
public:
    uint moveRange;
    
    Monster(void);
    virtual ~Monster(void);
    bool initWithFile(const std::string& filename) override;

    virtual void onEnter() override;
    virtual void onExit() override;
    
    static Monster* create(const std::string &fn);
};

#endif /* defined(__DragonTactic__Monster__) */
