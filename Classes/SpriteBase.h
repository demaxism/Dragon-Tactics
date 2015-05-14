//
//  SpriteBase.h
//  DragonTactic
//
//  Created by 丁 中明 on 5/4/15.
//
//

#ifndef __DragonTactic__SpriteBase__
#define __DragonTactic__SpriteBase__

#include <stdio.h>
USING_NS_CC;

class SpriteBase : public Sprite
{
public:
    float zoffset;
    Vec2 offset;
    Vec2 tilePosition(int x, int y);
    Vec2 mapGrid;
    Vec2 originMapGrid; // the origin map grid in a turn, doesnt change in turn. change after action decide
    void alignTile();
    void flyToGrid(Vec2 pos);
    Vec2 gridAtMap();
    void setMapGrid(int x, int y); // put to a map position

    Rect getRect();
    bool containsTouchLocation(Touch* touch);
};

#endif /* defined(__DragonTactic__SpriteBase__) */
