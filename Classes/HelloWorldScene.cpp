#include "HelloWorldScene.h"
#include "GameManager.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
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
    _unitList = new Vector<Unit*>();
    _monsterList = new Vector<Monster*>();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);
    
    // info panel
    _upper = new UpperInfoPanel();
    _upper->setPosition(Vec2(visibleSize.width / 2, visibleSize.height));
    addChild(_upper, 1);
    
    // add the tiled map
    _tiledMap = TMXTiledMap::create("map-1.tmx");
    addChild(_tiledMap, 0, kTagTileMap);
    GameManager::getInstance()->tileSize = _tiledMap->getTileSize();
    GameManager::getInstance()->mapSize = _tiledMap->getMapSize();
    GameManager::getInstance()->tiledMap = _tiledMap;
    
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
    u->setPosition(u->tilePosition(2, 3));
    _tiledMap->addChild(u);
    u->alignTile();
    _unitList->pushBack(u);
    
    u = Unit::create("q_02.png");
    u->moveRange = 2;
    u->setPosition(u->tilePosition(4, 1));
    _tiledMap->addChild(u);
    u->alignTile();
    _unitList->pushBack(u);
    
    // monsters
    Monster* m = Monster::create("m_02.png");
    m->setPosition(m->tilePosition(5, 8));
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
    
    // init touch
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
    listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
    listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    _winSize = Director::getInstance()->getWinSize();
    _fringe = _winSize.width / 10.0f;
    
    // frame loop
    if (GameManager::getInstance()->movePattern == mpBring) {
        schedule( CC_SCHEDULE_SELECTOR(HelloWorld::doStep) );
    }
    
    // event listener
    getEventDispatcher()->addCustomEventListener(EVT_UNITGRABBING, [this](EventCustom* evt){
        auto grabDiff = (Vec2*)evt->getUserData();
        auto mapPos = _tiledMap->getPosition();
        _tiledMap->setPosition(mapPos - *grabDiff);
    });
    
    getEventDispatcher()->addCustomEventListener(EVT_UNITGRABBEGAN, [this](EventCustom* evt){
        auto mapGrid = (Vec2*)evt->getUserData();
        this->showMovingGrid(*mapGrid);
        // origin unit
        Sprite* cur = GameManager::getInstance()->currentUnit;
        _originalUnit = Unit::createWithTexture(cur->getTexture());
        _originalUnit->setPosition(cur->getPosition());
        _tiledMap->addChild(_originalUnit);
        _originalUnit->alignTile();
        _originalUnit->setOpacity(0x80);
        // upper info
        _upper->showUnitInfo(cur);
    });
    
    getEventDispatcher()->addCustomEventListener(EVT_UNITGRABEND, [this](EventCustom* evt){
        this->clearMovingGrid();
        _crossMark->hide();
        _targetMark->stopFlash();
        // remove origin unit
        _originalUnit->removeFromParentAndCleanup(true);
    });
    
    _releaseTouchDiff = Vec2(0, 0);
    _frameCnt = 0;
    
    return true;
}

void HelloWorld::showMovingGrid(Vec2 tileGrid)
{
    auto layer = _tiledMap->getLayer("objects");
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
            // object in objects layer
            int gid = layer->getTileGIDAt(Vec2(x, _tiledMap->getMapSize().height - y - 1)); // y coord reverted
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

void HelloWorld::clearMovingGrid()
{
    ssize_t n = _movingGridList->size();
    for (ssize_t i = 0; i < n ; i++) {
        SpriteBase* sp = _movingGridList->at(i);
        sp->removeFromParentAndCleanup(true);
    }
    _movingGridList->clear();
}

void HelloWorld::doStep(float delta)
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
        if (touch.y < _fringe) {
            mapPos = mapPos + slidey;
            unitPos = unitPos - slidey;
        }
        if (touch.y > _winSize.height - _fringe) {
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
    }
    
    _frameCnt++;
}

Vec2 HelloWorld::refrainMapPos(Vec2 pos)
{
    float mapWidth = _tiledMap->getMapSize().width * _tiledMap->getTileSize().width;
    float mapHeight = _tiledMap->getMapSize().height * _tiledMap->getTileSize().height;
    float x = pos.x;
    float y = pos.y;
    if (pos.x > MAP_MARGIN) x = MAP_MARGIN;
    if (pos.x < -mapWidth - MAP_MARGIN + _winSize.width) x = -mapWidth - MAP_MARGIN + _winSize.width;
    if (pos.y > MAP_MARGIN) y = MAP_MARGIN;
    if (pos.y < -mapHeight - MAP_MARGIN + _winSize.height) y = -mapHeight - MAP_MARGIN + _winSize.height;
    return Vec2(x, y);
}

bool HelloWorld::isMapInsideView(Vec2 pos)
{
    float mapWidth = _tiledMap->getMapSize().width * _tiledMap->getTileSize().width;
    float mapHeight = _tiledMap->getMapSize().height * _tiledMap->getTileSize().height;
    bool ret = true;
    if (pos.x > MAP_MARGIN) ret = false;
    if (pos.x < -mapWidth - MAP_MARGIN + _winSize.width) ret = false;
    if (pos.y > MAP_MARGIN) ret = false;
    if (pos.y < -mapHeight - MAP_MARGIN + _winSize.height) ret = false;
    return ret;
}

void HelloWorld::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
    if (GameManager::getInstance()->isUnitGrabbed == false) {
        auto touch = touches[0];
        auto diff = touch->getDelta();
        auto currentPos = _tiledMap->getPosition();
        Vec2 newPos = refrainMapPos(currentPos + diff);
        _tiledMap->setPosition(newPos);
        
        // for slide enertia
        std::queue<Vec2>* touchQueue = GameManager::getInstance()->touchQueue;
        if (touchQueue->size() > 4) {
            touchQueue->pop();
        }
        touchQueue->push(diff);
    }
}

void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{
    GameManager::getInstance()->touchQueue->empty();
    _upper->hideUnitInfo();
}

void HelloWorld::onTouchesEnded(const std::vector<Touch*>& touches, Event  *event)
{
    auto touch = touches[0];
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

void HelloWorld::menuCloseCallback(Ref* pSender)
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
