//
//  TitleScene.cpp
//  DragonTactic
//
//  Created by 丁 中明 on 5/29/15.
//
//

#include "TitleScene.h"
#include "MainScene.h"
#include "SocketScene.h"
USING_NS_CC;

Scene* TitleScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TitleScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool TitleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    // TIP: 2d projection should be used (For TileMap vertexz)
    Director::getInstance()->setProjection(Director::Projection::_2D);
    Director::getInstance()->setDepthTest(true);
    
    _btnGame = Button::create();
    _btnGame->setTitleText("PLAY GAME");
    _btnGame->setTitleFontSize(50);
    _btnGame->setNormalizedPosition(Vec2(0.5f, 0.8f));
    _btnGame->setZoomScale(0.3f);
    _btnGame->setPressedActionEnabled(true);
    _btnGame->addClickEventListener([this](Ref* sender) {
        auto scene = MainScene::createScene();
        Director::getInstance()->replaceScene(scene);
    });
    addChild(_btnGame);
    
    _btnSocketTest = Button::create();
    _btnSocketTest->setTitleText("Socket Test");
    _btnSocketTest->setTitleFontSize(50);
    _btnSocketTest->setNormalizedPosition(Vec2(0.5f, 0.5f));
    _btnSocketTest->setZoomScale(0.3f);
    _btnSocketTest->setPressedActionEnabled(true);
    _btnSocketTest->addClickEventListener([this](Ref* sender) {
        auto scene = Scene::create();
        auto layer = new (std::nothrow) SocketScene();
        scene->addChild(layer);
        Director::getInstance()->replaceScene(scene);
    });
    addChild(_btnSocketTest);
    
    return true;
}