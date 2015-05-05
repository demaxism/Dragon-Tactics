#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Unit.h"
#include "BattleUI.h"
#include "GameManager.h"

using namespace cocos2d;

enum
{
    kTagTileMap = 1,
};

class HelloWorld : public cocos2d::Layer
{
private:
    TMXTiledMap* _tiledMap;
    Size _winSize;
    float _fringe;
    Vec2 _releaseTouchDiff;
    int _frameCnt;
    GameManager* _gameManager;
    Vector<FlashGrid*>* _movingGridList;
    CrossMark* _crossMark;
    TargetMark* _targetMark;
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
    void onTouchesMoved(const std::vector<Touch*>& touches, Event *event) override;
    void onTouchesBegan(const std::vector<Touch*>& touches, Event *event) override;
    void onTouchesEnded(const std::vector<Touch*>& touches, Event *event) override;
    void doStep(float delta);
    Vec2 refrainMapPos(Vec2 pos);
    bool isMapInsideView(Vec2 pos);
    void showMovingGrid(Vec2 pos);
    void clearMovingGrid();
};

#endif // __HELLOWORLD_SCENE_H__
