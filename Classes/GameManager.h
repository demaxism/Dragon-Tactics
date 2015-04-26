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

#include "cocos2d.h"
using namespace cocos2d;

class GameManager : public cocos2d::Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static GameManager* getInstance(void);
    
    Vec2 grabDiff;
    bool isUnitGrabbed;
    
public:
    virtual ~GameManager();
    bool init(void);
};

#endif /* defined(__DragonTactic__GameManager__) */
