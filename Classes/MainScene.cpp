#include "MainScene.h"
#include "GameManager.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    _gameManager = GameManager::getInstance();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // TIP: 2d projection should be used (For TileMap vertexz)
    Director::getInstance()->setProjection(Director::Projection::_2D);
    Director::getInstance()->setDepthTest(true);
    _movingGridList = new Vector<FlashGrid*>();
    _attackGridList = new Vector<FlashGrid*>();
    _unitList = new Vector<Unit*>();
    _monsterList = new Vector<Monster*>();
    _gameManager->isAction = false;
    
    // info panel
    _upper = new UpperInfoPanel();
    _upper->setPosition(Vec2(visibleSize.width / 2, visibleSize.height));
    _upper->setPositionZ(1020);
    addChild(_upper, 2);
    // action layer
    _action = new ActionLayer();
    addChild(_action, 1);
    
    // add bg
    _mapBg = new MapBg();
    addChild(_mapBg, -2);
    _mapBg->setPositionZ(-1020);
    
    // add the tiled map
    //_tiledMap = TMXTiledMap::create("map-1.tmx");
    _tiledMap = TMXTiledMap::create("plane.tmx");
    float mapWidth = _tiledMap->getMapSize().width * _tiledMap->getTileSize().width;
    float mapHeight = _tiledMap->getMapSize().height * _tiledMap->getTileSize().height;
    mapLowBoundary = Vec2(-mapWidth - MAP_MARGIN + _winSize.width + MAP_XADJUST, -mapHeight - MAP_MARGIN + _winSize.height);
    mapHighBoundary = Vec2(MAP_MARGIN + MAP_XADJUST, MAP_MARGIN);
    
    addChild(_tiledMap, 0, kTagTileMap);
    _tiledMap->setPosition(refrainMapPos(Vec2(0, 0)));
    GameManager::getInstance()->tileSize = _tiledMap->getTileSize();
    GameManager::getInstance()->mapSize = _tiledMap->getMapSize();
    GameManager::getInstance()->tiledMap = _tiledMap;
    _mapBg->moveTo(mapBgPos(_tiledMap->getPosition()));
    
    // debug
    auto layer = _tiledMap->getLayer("objects");
    Sprite* sp = layer->getTileAt(Vec2(0,11));
    int a = layer->getTileGIDAt(Vec2(2, 15));
    //int z = sp->getPositionZ();
//    Node* parent = sp->getParent();
//    Unit* sp2 = Unit::createWithTexture(sp->getTexture());
//    parent->addChild(sp2,1000);
//    sp2->setPosition(sp2->tilePosition(1, 0));
    
//    auto spriteLayer = _tiledMap->getLayer("sprites");
//    sp = spriteLayer->getTileAt(Vec2(0, 0));
//    Unit* player = Unit::createWithTexture(sp->getTexture());
//    parent->addChild(player);
//    player->setPosition(player->tilePosition(5, 5));
    
    // units
    Unit* u = Unit::create("q_01.png");
    u->moveRange = 3;
    u->charName = "ani01";
    u->setMapGrid(2, 3);
    _tiledMap->addChild(u);
    u->alignTile();
    _unitList->pushBack(u);
    
    u = Unit::create("q_02.png");
    u->moveRange = 3;
    u->attackRange = 2;
    u->charName = "ani02";
    u->setMapGrid(4, 6);
    _tiledMap->addChild(u);
    u->alignTile();
    _unitList->pushBack(u);
    
    // monsters
    Monster* m = Monster::create("m_02.png");
    m->charName = "m01";
    m->setPosition(m->tilePosition(4, 12));
    _tiledMap->addChild(m);
    m->alignTile();
    _monsterList->pushBack(m);
    
    m = Monster::create("m_02.png");
    m->charName = "m01";
    m->setPosition(m->tilePosition(1, 5));
    _tiledMap->addChild(m);
    m->alignTile();
    _monsterList->pushBack(m);
    
    m = Monster::create("m_03.png");
    m->charName = "m01";
    m->setPosition(m->tilePosition(8, 12));
    _tiledMap->addChild(m);
    m->alignTile();
    _monsterList->pushBack(m);
    
    // battle UI
    _crossMark = CrossMark::create("cross_mark.png");
    _crossMark->hide();
    _tiledMap->addChild(_crossMark, 1010);
    _crossMark->setPositionZ(1010);
    
    _targetMark = TargetMark::create("target_mark.png");
    _tiledMap->addChild(_targetMark, 1005);
    _targetMark->setPositionZ(1005);
    _targetMark->stopFlash();
    
    _attackTarget = AttackTarget::create("attack_target.png");
    _tiledMap->addChild(_attackTarget, 1006);
    _attackTarget->setPositionZ(1006);
    _attackTarget->stopLock();
    
    // init touch
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesMoved = CC_CALLBACK_2(MainScene::onTouchesMoved, this);
    listener->onTouchesBegan = CC_CALLBACK_2(MainScene::onTouchesBegan, this);
    listener->onTouchesEnded = CC_CALLBACK_2(MainScene::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    _winSize = Director::getInstance()->getWinSize();
    _fringe = _winSize.width / 10.0f;
    
    // frame loop
    if (GameManager::getInstance()->movePattern == mpBring) {
        schedule( CC_SCHEDULE_SELECTOR(MainScene::doStep) );
    }
    
    // event listener
    getEventDispatcher()->addCustomEventListener(EVT_UNITCHECK, [this](EventCustom* evt){
        this->clearActionUI();
        Unit* cur = (Unit*)GameManager::getInstance()->currentUnit;
        _upper->showEnemyInfo(cur->attackTarget);
        _attackTarget->lockTarget(cur, cur->attackTarget);
    });
                                                 
    getEventDispatcher()->addCustomEventListener(EVT_UNITGRABBING, [this](EventCustom* evt){
        auto grabDiff = (Vec2*)evt->getUserData();
        auto mapPos = _tiledMap->getPosition();
        _tiledMap->setPosition(mapPos - *grabDiff);
    });
    
    getEventDispatcher()->addCustomEventListener(EVT_UNITGRABBEGAN, [this](EventCustom* evt){
        this->clearActionUI();
        Unit* cur = (Unit*)GameManager::getInstance()->currentUnit;

        this->showMovingGrid(cur->originMapGrid);
        // origin unit
        _originalUnit = Unit::createWithTexture(cur->getTexture());
        //_originalUnit->setMapGrid(cur->originMapGrid.x, cur->originMapGrid.y); // origin pos
        _originalUnit->setPosition(cur->getPosition()); // recent pos
        _tiledMap->addChild(_originalUnit);
        _originalUnit->alignTile();
        _originalUnit->setOpacity(0x80);
        // upper info
        _upper->showUnitInfo(cur);
        _upper->hideLow();
    });
    
    getEventDispatcher()->addCustomEventListener(EVT_UNITGRABEND, [this](EventCustom* evt){
        this->clearMovingGrid();
        _crossMark->hide();
        _targetMark->stopFlash();
        _upper->showLow();
        // remove origin unit
        _originalUnit->removeFromParentAndCleanup(true);
        bool isMoved = (bool*)evt->getUserData();
        if (isMoved) {
            this->showAttackGrid(((Unit*)(GameManager::getInstance()->currentUnit))->mapGrid);
        }
    });
    
    getEventDispatcher()->addCustomEventListener(EVT_ENEMYTOUCHED, [this](EventCustom* evt){
        Unit* cur = (Unit*)(GameManager::getInstance()->currentUnit);
        Monster* m = (Monster*)evt->getUserData();
        if (m->getIsFlashing()) {
            this->clearAttackGrid();
            cur->attackTarget = m;
            m->stopFlash();
            _upper->showEnemyInfo(cur->attackTarget);
            _attackTarget->lockTarget(cur, cur->attackTarget);
            cur->actionFinish();
            // commit battle pair
            _action->addPair(cur, cur->attackTarget);
            // if all unit action finished
            int nFinished = 0;
            for (int i = 0; i < _unitList->size(); i++) {
                Unit* u = _unitList->at(i);
                if (u->getIsActionFinished())
                    nFinished++;
            }
            if (nFinished == _unitList->size()){
                CallFunc* func = CallFunc::create([&] () {
                    this->onActionDecided();
                });
                runAction(Sequence::create(DelayTime::create(0.6), func, NULL));
            }
        }
    });
    
    getEventDispatcher()->addCustomEventListener(EVT_ACTIONDECIDED, [this](EventCustom* evt){
        this->onActionDecided();
    });
    
    getEventDispatcher()->addCustomEventListener(EVT_ACTIONFINISHED, [this](EventCustom* evt){
        this->turnControlMode();
        _gameManager->isAction = false;
        this->clearActionUI();
        _upper->showLow();
    });
    
    getEventDispatcher()->addCustomEventListener(EVT_BATTLESTART, [this](EventCustom* evt){
        this->clearActionUI();
        this->highlightBattleUnit();
        this->focusBattle();
    });
    
    _releaseTouchDiff = Vec2(0, 0);
    _frameCnt = 0;
    
    return true;
}

void MainScene::onActionDecided()
{
    // enable all units
    for (int i = 0; i < _unitList->size(); i++) {
        Unit* u = _unitList->at(i);
        u->actionEnable();
    }
    this->clearActionUI();
    _action->showLayer();
    _upper->hideUnitInfo();
    _upper->hideLow();
    this->turnActionMode();
    _gameManager->isAction = true;
}

void MainScene::clearActionUI()
{
    this->clearAttackGrid();
    _attackTarget->stopLock();
}

void MainScene::showMovingGrid(Vec2 tileGrid)
{
    auto layer = _tiledMap->getLayer("objects");
    auto ground = _tiledMap->getLayer("ground");
    Unit* unit = (Unit*)(GameManager::getInstance()->currentUnit);
    int range = unit->moveRange;
    int xmin = MAX(0, tileGrid.x - range);
    int xmax = MIN(_tiledMap->getMapSize().width - 1, tileGrid.x + range);
    int ymin = MAX(0, tileGrid.y - range);
    int ymax = MIN(_tiledMap->getMapSize().height - 1, tileGrid.y + range);
    for (int x = xmin; x <= xmax; x++) {
        uint xmove = abs(x - (int)tileGrid.x);
        for (int y = ymin; y <= ymax; y++) {
            uint ymove = abs(y - (int)tileGrid.y);
            if (xmove + ymove > range) continue;
            int yIdx = _tiledMap->getMapSize().height - y - 1;// y coord reverted
            int gg = ground->getTileGIDAt(Vec2(x, yIdx));
            if (gg == 0) continue; // no object at ground
            // object in objects layer
            int gid = layer->getTileGIDAt(Vec2(x, yIdx)); // y coord reverted
            if (gid == 0) { // no object in grid
                bool hasOtherUnit = false;
                // check unit on this grid
                for (int i = 0; i < _unitList->size(); i++) {
                    Unit* u = _unitList->at(i);
                    if (u->mapGrid.x == x && u->mapGrid.y == y && u != GameManager::getInstance()->currentUnit)
                        hasOtherUnit = true;
                }
                for (int i = 0; i < _monsterList->size(); i++) {
                    Monster* m = _monsterList->at(i);
                    if (m->mapGrid.x == x && m->mapGrid.y == y)
                        hasOtherUnit = true;
                }
                if (!hasOtherUnit) {
                    FlashGrid* grid = FlashGrid::create("grid-light.png");
                    grid->setPosition(grid->tilePosition(x, y));
                    _tiledMap->addChild(grid);
                    grid->alignTile();
                    grid->startFlash();
                    _movingGridList->pushBack(grid);
                }
            }
        }
    }
}

void MainScene::showAttackGrid(Vec2 tileGrid)
{
    Unit* unit = (Unit*)(GameManager::getInstance()->currentUnit);
    int range = unit->attackRange;
    int xmin = MAX(0, tileGrid.x - range);
    int xmax = MIN(_tiledMap->getMapSize().width - 1, tileGrid.x + range);
    int ymin = MAX(0, tileGrid.y - range);
    int ymax = MIN(_tiledMap->getMapSize().height - 1, tileGrid.y + range);
    for (int x = xmin; x <= xmax; x++) {
        uint xmove = abs(x - (int)tileGrid.x);
        for (int y = ymin; y <= ymax; y++) {
            uint ymove = abs(y - (int)tileGrid.y);
            if (xmove + ymove > range) continue;
            
            if (x == tileGrid.x && y == tileGrid.y) continue;
            FlashGrid* grid = FlashGrid::create("grid-attack.png");
            grid->setPosition(grid->tilePosition(x, y));
            _tiledMap->addChild(grid);
            grid->alignTile();
            grid->startFlash();
            _attackGridList->pushBack(grid);
            for (int i = 0; i < _monsterList->size(); i++) {
                Monster* m = _monsterList->at(i);
                if (m->mapGrid.x == x && m->mapGrid.y == y)
                    m->startFlash();
            }
        }
    }
}

void MainScene::highlightBattleUnit()
{
    Unit* cur = (Unit*)(_gameManager->currentUnit);
    for (int i = 0; i < _unitList->size(); i++) {
        Unit* u = _unitList->at(i);
        if (u == cur)
            u->setColor(Color3B(0xff, 0xff, 0xff));
        else
            u->setColor(Color3B(0x80, 0x80, 0x80));
    }
    for (int i = 0; i < _monsterList->size(); i++) {
        Monster* m = _monsterList->at(i);
        if (m == cur->attackTarget)
            m->setColor(Color3B(0xff, 0xff, 0xff));
        else
            m->setColor(Color3B(0x80, 0x80, 0x80));
    }
}

void MainScene::turnActionMode()
{
    auto ground = _tiledMap->getLayer("ground");
    auto fringe = _tiledMap->getLayer("fringe");
    auto objects = _tiledMap->getLayer("objects");
    for (int x = 0; x < _tiledMap->getMapSize().width; x++) {
        for (int y = 0; y < _tiledMap->getMapSize().height; y++) {
            Color3B dark = Color3B(0x60, 0x60, 0x60);
            Sprite* g = ground->getTileAt(Vec2(x, y));
            if (g != nullptr)
                g->setColor(dark);
            
            Sprite* o = objects->getTileAt(Vec2(x, y));
            if (o != nullptr)
                o->setColor(dark);
            
            Sprite* f = fringe->getTileAt(Vec2(x, y));
            if (f != nullptr)
                f->setColor(dark);
        }
    }

    for (int i = 0; i < _unitList->size(); i++) {
        Unit* u = _unitList->at(i);
        u->setColor(Color3B(0x80, 0x80, 0x80));
    }
    for (int i = 0; i < _monsterList->size(); i++) {
        Monster* m = _monsterList->at(i);
        m->setColor(Color3B(0x80, 0x80, 0x80));
    }
    _mapBg->turnOff();
}

void MainScene::turnControlMode()
{
    auto ground = _tiledMap->getLayer("ground");
    auto objects = _tiledMap->getLayer("objects");
    auto fringe = _tiledMap->getLayer("fringe");
    for (int x = 0; x < _tiledMap->getMapSize().width; x++) {
        for (int y = 0; y < _tiledMap->getMapSize().height; y++) {
            Color3B white = Color3B(0xff, 0xff, 0xff);
            Sprite* g = ground->getTileAt(Vec2(x, y));
            if (g != nullptr)
                g->setColor(white);
            
            Sprite* o = objects->getTileAt(Vec2(x, y));
            if (o != nullptr)
                o->setColor(white);
            
            Sprite* f = fringe->getTileAt(Vec2(x, y));
            if (f != nullptr)
                f->setColor(white);
        }
    }
    for (int i = 0; i < _unitList->size(); i++) {
        Unit* u = _unitList->at(i);
        u->setColor(Color3B(0xff, 0xff, 0xff));
    }
    for (int i = 0; i < _monsterList->size(); i++) {
        Monster* m = _monsterList->at(i);
        m->setColor(Color3B(0xff, 0xff, 0xff));
    }
    _mapBg->turnOn();
}

void MainScene::clearMovingGrid()
{
    ssize_t n = _movingGridList->size();
    for (ssize_t i = 0; i < n ; i++) {
        SpriteBase* sp = _movingGridList->at(i);
        sp->removeFromParentAndCleanup(true);
    }
    _movingGridList->clear();
}

void MainScene::clearAttackGrid()
{
    ssize_t n = _attackGridList->size();
    for (ssize_t i = 0; i < n ; i++) {
        SpriteBase* sp = _attackGridList->at(i);
        sp->removeFromParentAndCleanup(true);
    }
    _attackGridList->clear();
    for (int i = 0; i < _monsterList->size(); i++) {
        Monster* m = _monsterList->at(i);
        m->stopFlash();
    }
}

void MainScene::focusBattle()
{
    Unit* cur = (Unit*)(GameManager::getInstance()->currentUnit);
    Vec2 battlePos = (cur->getPosition() + cur->attackTarget->getPosition()) / 2;
    Vec2 absScreenPos = _tiledMap->getPosition() + battlePos; // absolute pos in view
    Vec2 distToViewCenter = Vec2(_winSize.width / 2 - absScreenPos.x, _winSize.height / 2 - absScreenPos.y);
    Vec2 mapPosTo = _tiledMap->getPosition() + distToViewCenter;
    mapPosTo += Vec2(10, -180); // small adjust
    Vec2 refrained = refrainMapPos(mapPosTo);
    _tiledMap->runAction(MoveTo::create(0.2, refrained));
    _mapBg->moveTo(mapBgPos(refrained));
    // attack target
    _attackTarget->lockTarget(cur, cur->attackTarget);
}

void MainScene::doStep(float delta)
{
    // mpBring
    if (GameManager::getInstance()->isUnitGrabbed) {
        Vec2 touch = GameManager::getInstance()->touchLocation;
        auto mapPos = _tiledMap->getPosition();
        
        Sprite* unit = GameManager::getInstance()->currentUnit;
        auto unitPos = unit->getPosition();
        Vec2 slidex = Vec2(FRINGE_MOVE_SPEED, 0);
        Vec2 slidey = Vec2(0, FRINGE_MOVE_SPEED);
        
        if (touch.x < _fringe) {
            mapPos = mapPos + slidex;
            unitPos = unitPos - slidex;
        }
        if (touch.x > _winSize.width - _fringe) {
            mapPos = mapPos - slidex;
            unitPos = unitPos + slidex;
        }
        if (touch.y < _fringe + 30) {
            mapPos = mapPos + slidey;
            unitPos = unitPos - slidey;
        }
        if (touch.y > _winSize.height - _fringe - 60) {
            mapPos = mapPos - slidey;
            unitPos = unitPos + slidey;
        }
        
        if (isMapInsideView(mapPos)) {
            _tiledMap->setPosition(mapPos);
            unit->setPosition(unitPos);
            
            // check moving range
            bool isMovingGrid = false;
            ssize_t n = _movingGridList->size();
            Unit* cur = (Unit*)(GameManager::getInstance()->currentUnit);
            Vec2 tileGrid = cur->gridAtMap();
            for (ssize_t i = 0; i < n ; i++) {
                FlashGrid* sp = _movingGridList->at(i);
                if (sp->mapGrid.x == tileGrid.x && sp->mapGrid.y == tileGrid.y) {
                   isMovingGrid = true;
                    sp->doFlash = false;
                }
                else {
                    sp->doFlash = true;
                }
            }
            if (!isMovingGrid) {
                _crossMark->showForCurrentUnit(cur);
                _targetMark->stopFlash();
            }
            else {
                _crossMark->hide();
                _targetMark->startFlash();
                _targetMark->setPosition(_targetMark->tilePosition(tileGrid.x, tileGrid.y));
            }
        }
    }
    
    // inertia move
    if (_releaseTouchDiff.length() > 10.0f && GameManager::getInstance()->isUnitGrabbed == false) {
        if (_frameCnt % 2 == 0)
            _releaseTouchDiff = _releaseTouchDiff * 0.9f;
        
        int x, y;
        if (_releaseTouchDiff.x > 0)
            x = floorf(_releaseTouchDiff.x);
        else
            x = ceilf(_releaseTouchDiff.x);
        if (_releaseTouchDiff.y > 0)
            y = floorf(_releaseTouchDiff.y);
        else
            y = ceilf(_releaseTouchDiff.y);
        _releaseTouchDiff = Vec2(x, y);
        auto currentPos = _tiledMap->getPosition();
        _tiledMap->setPosition(currentPos + _releaseTouchDiff);
        
        Vec2 newPos = refrainMapPos(currentPos + _releaseTouchDiff);
        _tiledMap->setPosition(newPos);
        _mapBg->moveTo(mapBgPos(newPos));
    }
    
    _frameCnt++;
}

Vec2 MainScene::refrainMapPos(Vec2 pos)
{
    float mapWidth = _tiledMap->getMapSize().width * _tiledMap->getTileSize().width;
    float mapHeight = _tiledMap->getMapSize().height * _tiledMap->getTileSize().height;
    float x = pos.x;
    float y = pos.y;
    
    if (pos.x > MAP_MARGIN + MAP_XADJUST) x = MAP_MARGIN + MAP_XADJUST;
    if (pos.x < -mapWidth - MAP_MARGIN + _winSize.width + MAP_XADJUST) x = -mapWidth - MAP_MARGIN + _winSize.width + MAP_XADJUST;
    if (pos.y > MAP_MARGIN) y = MAP_MARGIN;
    if (pos.y < -mapHeight - MAP_MARGIN + _winSize.height) y = -mapHeight - MAP_MARGIN + _winSize.height;
    return Vec2(x, y);
}

Vec2 MainScene::mapBgPos(Vec2 mapPos)
{
    float xOffset = mapPos.x - mapLowBoundary.x;
    float xOffsetRate = xOffset / (mapHighBoundary.x - mapLowBoundary.x);
    float yOffset = mapPos.y - (mapLowBoundary.y);
    float yOffsetRate = yOffset / (mapHighBoundary.y - mapLowBoundary.y);
    
    return Vec2(xOffsetRate, yOffsetRate);
}

bool MainScene::isMapInsideView(Vec2 pos)
{
    float mapWidth = _tiledMap->getMapSize().width * _tiledMap->getTileSize().width;
    float mapHeight = _tiledMap->getMapSize().height * _tiledMap->getTileSize().height;
    bool ret = true;
    if (pos.x > mapHighBoundary.x) ret = false;
    if (pos.x < mapLowBoundary.x) ret = false;
    if (pos.y > mapHighBoundary.y) ret = false;
    if (pos.y < mapLowBoundary.y) ret = false;
    return ret;
}

void MainScene::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
    if (_gameManager->isAction) return; // return if during action animation
    if (GameManager::getInstance()->isUnitGrabbed == false) {
        auto touch = touches[0];
        auto diff = touch->getDelta();
        auto currentPos = _tiledMap->getPosition();
        Vec2 newPos = refrainMapPos(currentPos + diff);
        _tiledMap->setPosition(newPos);
        _mapBg->moveTo(mapBgPos(newPos));
        
        // for slide enertia
        std::queue<Vec2>* touchQueue = GameManager::getInstance()->touchQueue;
        if (touchQueue->size() > 4) {
            touchQueue->pop();
        }
        touchQueue->push(diff);
    }
}

void MainScene::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{
    if (_gameManager->isAction) return; // return if during action animation
    GameManager::getInstance()->touchQueue->empty();
}

void MainScene::onTouchesEnded(const std::vector<Touch*>& touches, Event  *event)
{
    if (_gameManager->isAction) return; // return if during action animation
    
    std::queue<Vec2>* touchQueue = GameManager::getInstance()->touchQueue;
    
    Vec2 diffTotal = Vec2(0, 0);
    long n = touchQueue->size();
    for (int i = 0; i < n; i++) {
        Vec2 diff = touchQueue->front();
        diffTotal = diffTotal + diff;
        touchQueue->pop();
    }
    _releaseTouchDiff = diffTotal / n;
    
    float length = _releaseTouchDiff.length();
    
    if (length > SWIPE_INERTIA_MAX) {
        _releaseTouchDiff = _releaseTouchDiff * (SWIPE_INERTIA_MAX / length);
    }
    
    CCLOG("map x: %f¥n",_tiledMap->getPosition().x);
}

void MainScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
