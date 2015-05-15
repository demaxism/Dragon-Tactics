//
//  Effect.h
//  DragonTactic
//
//  Created by 丁 中明 on 5/15/15.
//
//

#ifndef __DragonTactic__Effect__
#define __DragonTactic__Effect__

#include <stdio.h>
#include "SpriteBase.h"
USING_NS_CC;

class EffectHit : public Sprite
{
    Animate* _animate;
public:
    EffectHit();
    void play();
};

#endif /* defined(__DragonTactic__Effect__) */
