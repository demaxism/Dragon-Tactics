//
//  GameManager.h
//  DragonTactic
//
//  Created by 丁 中明 on 4/26/15.
//
//

#ifndef __DragonTactic__GameManager__
#define __DragonTactic__GameManager__

#define EVT_UNITGRABBING "evtUnitGrabbing"
#define FRINGE_MOVE_SPEED 6.0f

#include "cocos2d.h"
using namespace cocos2d;

typedef enum tagMovePattern
{
    mpGrab,
    mpGrabx2,
    mpBring
} MovePattern;

class GameManager : public cocos2d::Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static GameManager* getInstance(void);
    
    Vec2 grabDiff;
    Vec2 touchLocation;
    bool isUnitGrabbed;
    MovePattern movePattern;
    Sprite* currentUnit;
    
public:
    virtual ~GameManager();
    bool init(void);
};

#endif /* defined(__DragonTactic__GameManager__) */
