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
    mapGrid = gridAtMap();
    Vec2 posAligned = tilePosition(mapGrid.x, mapGrid.y);
    //runAction(MoveTo::create(0.1, posAligned)); // positionZ become strange when using action
    float sz = -mapGrid.y - 1.5;
    setPosition(posAligned);
    // both PositionZ & zorder should be set
    setPositionZ(sz + zoffset);
    auto parent = getParent();
    parent->reorderChild(this, 1000-mapGrid.y);
}

Vec2 SpriteBase::gridAtMap()
{
    Vec2 pos = getPosition();
    Size tileSize = GameManager::getInstance()->tileSize;
    int x = (pos.x - 15) / tileSize.width;
    int y = pos.y / tileSize.height;
    return Vec2(x, y);
}
