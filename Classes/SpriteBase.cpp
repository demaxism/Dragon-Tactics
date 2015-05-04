//
//  SpriteBase.cpp
//  DragonTactic
//
//  Created by 丁 中明 on 5/4/15.
//
//

#include "SpriteBase.h"
#include "GameManager.h"

Vec2 SpriteBase::tilePosition(int x, int y)
{
    Size tileSize = GameManager::getInstance()->tileSize;
    return Vec2(tileSize.width * x + 15 + tileSize.width * 0.5f + offset.x, tileSize.height * y + tileSize.height * 0.5f + offset.y);
}

void SpriteBase::alignTile()
{
    Vec2 pos = getPosition();
    Size tileSize = GameManager::getInstance()->tileSize;
    int x = pos.x / tileSize.width;
    int y = pos.y / tileSize.height;
    Vec2 posAligned = tilePosition(x, y);
    //runAction(MoveTo::create(0.1, posAligned)); // positionZ become strange when using action
    float sz = -y - 1.5;
    setPosition(posAligned);
    setPositionZ(sz + zoffset);
    auto parent = getParent();
    parent->reorderChild(this, 1000-y);
}
