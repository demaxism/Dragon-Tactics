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
USING_NS_CC;

struct BattlePair {
    std::string left;
    std::string right;
};

class Charactor : public Sprite
{
private:
    std::string _name;
    Sprite* _normal;
    Sprite* _fight;
public:
    Charactor(void);
    Charactor(const std::string& filename);
    void fight();
};

class ActionLayer : public Sprite
{
private:
    bool _isShowing;
    LayerColor* _mask;
    Sprite* _actionLabel;
    Size _winSize;
    Charactor* _left;
    std::vector<BattlePair*>* _battleList;
public:
    ActionLayer(void);
    void showLayer();
    void hideLayer();
    void addPair(const std::string& left, const std::string& right);
    void showChars();
};

#endif /* defined(__DragonTactic__Charactor__) */
