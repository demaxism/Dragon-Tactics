#ifndef __MainScene_SCENE_H__
#define __MainScene_SCENE_H__

#include "cocos2d.h"
#include "Unit.h"
#include "Monster.h"
#include "BattleUI.h"
#include "GameManager.h"
#include "Charactor.h"

using namespace cocos2d;

enum
{
    kTagTileMap = 1,
};

class MainScene : public cocos2d::Layer
{
private:
    TMXTiledMap* _tiledMap;
    Sprite* _mapBg;
    Size _winSize;
    float _fringe;
    Vec2 _releaseTouchDiff;
    int _frameCnt;
    GameManager* _gameManager;
    Vector<FlashGrid*>* _movingGridList;
    Vector<FlashGrid*>* _attackGridList;
    Vector<Unit*>* _unitList;
    Vector<Monster*>* _monsterList;
    CrossMark* _crossMark;
    TargetMark* _targetMark;
    AttackTarget* _attackTarget;
    Unit* _originalUnit;
    UpperInfoPanel* _upper;
    ActionLayer* _action;
    Vec2 mapLowBoundary;
    Vec2 mapHighBoundary;
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
private:
    void onTouchesMoved(const std::vector<Touch*>& touches, Event *event) override;
    void onTouchesBegan(const std::vector<Touch*>& touches, Event *event) override;
    void onTouchesEnded(const std::vector<Touch*>& touches, Event *event) override;
    void doStep(float delta);
    Vec2 refrainMapPos(Vec2 pos);
    Vec2 mapBgPos(Vec2 mapPos);
    bool isMapInsideView(Vec2 pos);
    void showMovingGrid(Vec2 pos);
    void showAttackGrid(Vec2 pos);
    void turnActionMode();
    void turnControlMode();
    void highlightBattleUnit();
    void clearMovingGrid();
    void clearAttackGrid();
    void clearActionUI();
    void focusBattle(); // focus the battle pair in view center during action
    void onActionDecided();
};

#endif // __MainScene_SCENE_H__
