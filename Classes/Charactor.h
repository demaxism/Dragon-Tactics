//
//  Charactor.h
//  DragonTactic
//
//  Created by 丁 中明 on 5/13/15.
//
//

#ifndef __DragonTactic__Charactor__
#define __DragonTactic__Charactor__

#include <stdio.h>
#include <vector>
#include "Effect.h"
USING_NS_CC;

struct BattlePair {
    void* leftSprite;
    void* rightSprite;
};

class Charactor : public Sprite
{
private:
    std::string _name;
    Sprite* _normal;
    Sprite* _fight;
    int _frame;
    Vec2 _pos;
public:
    Charactor(void);
    Charactor(const std::string& filename);
    void fight();
    void hurt();
    void hurtStep(float);
};

class ActionLayer : public Sprite
{
private:
    bool _isShowing;
    bool _isBgShowing;
    LayerColor* _mask;
    Sprite* _actionLabel;
    Size _winSize;
    Charactor* _left;
    Charactor* _right;
    std::vector<BattlePair*>* _battleList;
    RenderTexture* _rtex;
    Sprite* bg;
    Sprite* mask;
    EffectHit* _eh;
    Sprite* _flash;
    int _cntMaskLoop;
    int _iBattle; // battle index
public:
    ActionLayer(void);
    void showLayer();
    void hideLayer();
    void addPair(void* leftSprite, void* rightSprite);
    void showBg();
    void hideBg();
    void showChars();
    void hideChars();
    void nextBattle();
};

#endif /* defined(__DragonTactic__Charactor__) */
