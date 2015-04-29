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
    
    // sprite
    Unit* u = Unit::create("q_01.png");
    u->setPosition(u->tilePosition(0, 0));
    _tiledMap->addChild(u, 1);
    //u->reorder();
    
    u = Unit::create("q_02.png");
    u->setPosition(u->tilePosition(0, 1));
    _tiledMap->addChild(u, 1);
    //u->reorder();
    
    // init touch
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    _winSize = Director::getInstance()->getWinSize();
    _fringe = _winSize.width / 7.0f;
    
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
}

void HelloWorld::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
    if (GameManager::getInstance()->isUnitGrabbed == false) {
    auto touch = touches[0];
    
    auto diff = touch->getDelta();
    auto currentPos = _tiledMap->getPosition();
    _tiledMap->setPosition(currentPos + diff);
    }
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
