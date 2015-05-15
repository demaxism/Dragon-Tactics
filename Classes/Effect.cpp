//
//  Effect.cpp
//  DragonTactic
//
//  Created by 丁 中明 on 5/15/15.
//
//

#include "Effect.h"

EffectHit::EffectHit()
{
    Sprite::init();
    int nframe = 2;
    Vector<SpriteFrame*> animFrames(nframe);
    char str[100] = {0};
    for(int i = 0; i < nframe; i++)
    {
        sprintf(str, "ef_hit1_%02d.png",i);
        auto frame = SpriteFrame::create(str,Rect(0,0,500,500)); //we assume that the sprites' dimentions are 40*40 rectangles.
        animFrames.pushBack(frame);
    }

    auto animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
    _animate = Animate::create(animation);
}

void EffectHit::play()
{
    runAction(_animate);
    CallFunc* hurtFunc = CallFunc::create([&] () {
        this->setVisible(false);
    });
    runAction(Sequence::create(_animate, hurtFunc, NULL));
    BlendFunc bf1;
    bf1.src = GL_ONE_MINUS_DST_COLOR;
    bf1.dst = GL_ONE;
    setBlendFunc(bf1);
}