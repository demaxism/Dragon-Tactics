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
    
private:
    int nFrame;
    void doStep(float delta);
};

#endif /* defined(__DragonTactic__BattleUI__) */
