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
    void alignTile();
};

#endif /* defined(__DragonTactic__SpriteBase__) */
