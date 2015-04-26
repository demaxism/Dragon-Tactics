//
//  GameManager.cpp
//  DragonTactic
//
//  Created by 丁 中明 on 4/26/15.
//
//

#include "GameManager.h"

using namespace cocos2d;

static GameManager *_sharedGameManager = NULL;

GameManager* GameManager::getInstance()
{
    if (! _sharedGameManager)
    {
        _sharedGameManager = new GameManager();
        _sharedGameManager->init();
    }
    
    return _sharedGameManager;
}

GameManager::~GameManager(void)
{
}

bool GameManager::init(void)
{
    grabDiff = Vec2(0, 0);
    isUnitGrabbed = false;
    return true;
}