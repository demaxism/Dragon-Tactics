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
    return Vec2(tileSize.width * x + tileSize.width * 0.5f + offset.x, tileSize.height * y + tileSize.height * 0.5f + offset.y);
}

void SpriteBase::alignTile()
{
    mapGrid = gridAtMap();
    Vec2 posAligned = tilePosition(mapGrid.x, mapGrid.y);
    float sz = -mapGrid.y - 1.5;
    setPosition(posAligned);
    // both PositionZ & zorder should be set
    setPositionZ(sz + zoffset);
    auto parent = getParent();
    parent->reorderChild(this, 1000-mapGrid.y+zoffset*10);
}

void SpriteBase::flyToGrid(Vec2 grid)
{
    float sz = -grid.y - 1.5;
    //setPositionZ(sz + zoffset);
    auto parent = getParent();
    parent->reorderChild(this, 1000-mapGrid.y);
    // both PositionZ & zorder should be set
    Vec2 posAligned = tilePosition(mapGrid.x, mapGrid.y);
    Vec3 posXYZ = Vec3(posAligned.x, posAligned.y, sz + zoffset); // should set z coordinate
    runAction(MoveTo::create(0.2, posXYZ)); // positionZ become strange when using action
}

Vec2 SpriteBase::gridAtMap()
{
    Vec2 pos = getPosition();
    Size tileSize = GameManager::getInstance()->tileSize;
    int x = (pos.x - 15) / tileSize.width;
    int y = pos.y / tileSize.height;
    return Vec2(x, y);
}

void SpriteBase::setMapGrid(int x, int y)
{
    originMapGrid = Vec2(x, y);
    setPosition(tilePosition(x, y));
}

Rect SpriteBase::getRect()
{
    auto s = getTexture()->getContentSize();
    return Rect(-s.width / 2, -s.height / 2, s.width, s.height);
}

bool SpriteBase::containsTouchLocation(Touch* touch)
{
    return getRect().containsPoint(convertTouchToNodeSpaceAR(touch));
}
