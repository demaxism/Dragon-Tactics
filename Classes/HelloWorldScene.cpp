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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // TIP: 2d projection should be used (For TileMap vertexz)
    Director::getInstance()->setProjection(Director::Projection::_2D);
    Director::getInstance()->setDepthTest(true);

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
    
    // add the tiled map
    _tiledMap = TMXTiledMap::create("field2.tmx");
    addChild(_tiledMap, 0, kTagTileMap);
    GameManager::getInstance()->tileSize = _tiledMap->getTileSize();
    GameManager::getInstance()->mapSize = _tiledMap->getMapSize();
    GameManager::getInstance()->tiledMap = _tiledMap;
    
    // debug
    auto layer = _tiledMap->getLayer("objects");
    Sprite* sp = layer->getTileAt(Vec2(0,11));
    int z = sp->getPositionZ();
//    Node* parent = sp->getParent();
//    Unit* sp2 = Unit::createWithTexture(sp->getTexture());
//    parent->addChild(sp2,1000);
//    sp2->setPosition(sp2->tilePosition(1, 0));
    
//    auto spriteLayer = _tiledMap->getLayer("sprites");
//    sp = spriteLayer->getTileAt(Vec2(0, 0));
//    Unit* player = Unit::createWithTexture(sp->getTexture());
//    parent->addChild(player);
//    player->setPosition(player->tilePosition(5, 5));
    
    // sprite
    Unit* u = Unit::create("q_01.png");
    u->setPosition(u->tilePosition(0, 3));
    _tiledMap->addChild(u, 1000);
    u->alignTile();
    
    u = Unit::create("q_02.png");
    u->setPosition(u->tilePosition(4, 1));
    _tiledMap->addChild(u, 1000);
    u->alignTile();
    
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
    
    _releaseTouchDiff = Vec2(0, 0);
    _frameCnt = 0;
    
    return true;
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
        _tiledMap->setPosition(mapPos);
        unit->setPosition(unitPos);
    }
    
    // inertia move
    if (_releaseTouchDiff.length() > 10.0f) {
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
    }
    
    _frameCnt++;
}

void HelloWorld::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
    if (GameManager::getInstance()->isUnitGrabbed == false) {
        auto touch = touches[0];
        auto diff = touch->getDelta();
        auto currentPos = _tiledMap->getPosition();
        _tiledMap->setPosition(currentPos + diff);
        
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
    
    CCLOG("touch queue: %lu , vx:%f, vy:%f¥n",n, _releaseTouchDiff.x, _releaseTouchDiff.y);
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
