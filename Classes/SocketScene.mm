//
//  SocketScene.cpp
//  DragonTactic
//
//  Created by 丁 中明 on 5/29/15.
//
//

/* Server side config
 connect to jpnode.demaxism.com, cd /var/www/demo/nodejs
 node ws_server.js 
 */

#include "SocketScene.h"
#include "TitleScene.h"
#import "Gyro.h"

USING_NS_CC;
USING_NS_CC_EXT;

SocketScene::SocketScene()
: _wsiSendText(nullptr)
, _wsiSendBinary(nullptr)
, _wsiError(nullptr)
, _sendTextStatus(nullptr)
, _sendBinaryStatus(nullptr)
, _errorStatus(nullptr)
, _sendTextTimes(0)
, _sendBinaryTimes(0)
{
    auto winSize = Director::getInstance()->getWinSize();
    
    Color3B bgColor = Color3B(0x93, 0x52, 0x18);
    LayerColor* colorBg = LayerColor::create(Color4B(bgColor.r,bgColor.g,bgColor.b,0xff), winSize.width, winSize.height);
    this->addChild(colorBg);
    
    const int MARGIN = 30;
    const int SPACE = 120;
    
    auto label = Label::createWithTTF("WebSocket Test", "fonts/arial.ttf", 28);
    label->setPosition(Vec2(winSize.width / 2, winSize.height - MARGIN));
    label->setOpacity(0x80);
    addChild(label, 0);
    
    auto menuRequest = Menu::create();
    menuRequest->setPosition(Vec2::ZERO);
    addChild(menuRequest);
    
    // Send Text
    auto labelSendText = Label::createWithTTF("Send Text", "fonts/arial.ttf", 40);
    auto itemSendText = MenuItemLabel::create(labelSendText, CC_CALLBACK_1(SocketScene::onMenuSendTextClicked, this));
    itemSendText->setPosition(Vec2(winSize.width / 2, winSize.height - MARGIN - SPACE));
    menuRequest->addChild(itemSendText);
    
    // Send Binary
    auto labelSendBinary = Label::createWithTTF("Send Binary", "fonts/arial.ttf", 40);
    auto itemSendBinary = MenuItemLabel::create(labelSendBinary, CC_CALLBACK_1(SocketScene::onMenuSendBinaryClicked, this));
    itemSendBinary->setPosition(Vec2(winSize.width / 2, winSize.height - MARGIN - 2 * SPACE));
    menuRequest->addChild(itemSendBinary);
    
    // back
    _btnBack = Button::create();
    _btnBack->setTitleText("BACK");
    _btnBack->setTitleFontSize(50);
    _btnBack->setPosition(Vec2(winSize.width - 100, 40));
    _btnBack->setZoomScale(0.3f);
    _btnBack->setPressedActionEnabled(true);
    _btnBack->addClickEventListener([this](Ref* sender) {
        auto scene = TitleScene::createScene();
        Director::getInstance()->replaceScene(scene);
    });
    addChild(_btnBack);
    
    
    // Send Text Status Label
    _sendTextStatus = Label::createWithTTF("Send Text WS is waiting...", "fonts/arial.ttf", 20, cocos2d::Size(400, 100), TextHAlignment::CENTER, TextVAlignment::TOP);
    _sendTextStatus->setAnchorPoint(Vec2(0, 0));
    _sendTextStatus->setPosition(100, 400);
    this->addChild(_sendTextStatus);
    
    // Send Binary Status Label
    _sendBinaryStatus = Label::createWithTTF("Send Binary WS is waiting...", "fonts/arial.ttf", 20);
    _sendBinaryStatus->setAnchorPoint(Vec2(0, 0));
    _sendBinaryStatus->setPosition(Vec2(100, 160));
    this->addChild(_sendBinaryStatus);
    
    // Error Label
    _errorStatus = Label::createWithTTF("Error WS is waiting...", "fonts/arial.ttf", 20);
    _errorStatus->setAnchorPoint(Vec2(0, 0));
    _errorStatus->setPosition(Vec2(100, 100));
    this->addChild(_errorStatus);
    
    
    _wsiSendText = new network::WebSocket();
    _wsiSendBinary = new network::WebSocket();
    _wsiError = new network::WebSocket();
    
    std::vector<std::string>* v = new std::vector<std::string>();
    v->push_back("echo-protocol");
    if (!_wsiSendText->init(*this, "ws://jpnode.demaxism.com:8080", v)) // ws://echo.websocket.org
    {
        CC_SAFE_DELETE(_wsiSendText);
    }
    
    if (!_wsiSendBinary->init(*this, "ws://echo.websocket.org"))
    {
        CC_SAFE_DELETE(_wsiSendBinary);
    }
    
    if (!_wsiError->init(*this, "ws://invalid.url.com"))
    {
        CC_SAFE_DELETE(_wsiError);
    }
    
    _count = 0;
    _gyro = [[Gyro alloc] init];
    int value = [(Gyro*)_gyro showMessage];
    cocos2d::log("test-ding %d", value);
    
    schedule(CC_SCHEDULE_SELECTOR(SocketScene::doStep));
}

SocketScene::~SocketScene()
{
    if (_wsiSendText)
        _wsiSendText->close();
    
    if (_wsiSendBinary)
        _wsiSendBinary->close();
    
    if (_wsiError)
        _wsiError->close();
}

void SocketScene::doStep(float delta) {
    _count++;
    if (_count % 10 == 0) {
        int value = [(Gyro*)_gyro showMessage];
        cocos2d::log("debug-001: %d :count %d", value, _count);
        float yaw = [(Gyro*)_gyro attitudeYaw];
        cocos2d::log("debug-gyro: %f ", yaw);
    }
}

// Delegate methods
void SocketScene::onOpen(network::WebSocket* ws)
{
    log("Websocket (%p) opened", ws);
    if (ws == _wsiSendText)
    {
        _sendTextStatus->setString("Send Text WS was opened.");
    }
    else if (ws == _wsiSendBinary)
    {
        _sendBinaryStatus->setString("Send Binary WS was opened.");
    }
    else if (ws == _wsiError)
    {
        CCASSERT(0, "error test will never go here.");
    }
}

void SocketScene::onMessage(network::WebSocket* ws, const network::WebSocket::Data& data)
{
    if (!data.isBinary)
    {
        _sendTextTimes++;
        char times[100] = {0};
        sprintf(times, "%d", _sendTextTimes);
        std::string textStr = std::string("response text msg: ")+data.bytes+", "+times;
        log("%s", textStr.c_str());
        
        _sendTextStatus->setString(textStr.c_str());
    }
    else
    {
        _sendBinaryTimes++;
        char times[100] = {0};
        sprintf(times, "%d", _sendBinaryTimes);
        
        std::string binaryStr = "response bin msg: ";
        
        for (int i = 0; i < data.len; ++i) {
            if (data.bytes[i] != '\0')
            {
                binaryStr += data.bytes[i];
            }
            else
            {
                binaryStr += "\'\\0\'";
            }
        }
        
        binaryStr += std::string(", ")+times;
        log("%s", binaryStr.c_str());
        _sendBinaryStatus->setString(binaryStr.c_str());
    }
}

void SocketScene::onClose(network::WebSocket* ws)
{
    log("websocket instance (%p) closed.", ws);
    if (ws == _wsiSendText)
    {
        _wsiSendText = nullptr;
    }
    else if (ws == _wsiSendBinary)
    {
        _wsiSendBinary = nullptr;
    }
    else if (ws == _wsiError)
    {
        _wsiError = nullptr;
    }
    // Delete websocket instance.
    CC_SAFE_DELETE(ws);
}

void SocketScene::onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error)
{
    log("Error was fired, error code: %d", error);
    if (ws == _wsiError)
    {
        char buf[100] = {0};
        sprintf(buf, "an error was fired, code: %d", error);
        _errorStatus->setString(buf);
    }
}

// Menu Callbacks
void SocketScene::onMenuSendTextClicked(cocos2d::Ref *sender)
{
    if (! _wsiSendText)
    {
        return;
    }
    
    if (_wsiSendText->getReadyState() == network::WebSocket::State::OPEN)
    {
        _sendTextStatus->setString("Send Text WS is waiting...");
        _wsiSendText->send("Hello WebSocket, I'm a text message.");
    }
    else
    {
        std::string warningStr = "send text websocket instance wasn't ready...";
        log("%s", warningStr.c_str());
        _sendTextStatus->setString(warningStr.c_str());
    }
}

void SocketScene::onMenuSendBinaryClicked(cocos2d::Ref *sender)
{
    if (! _wsiSendBinary) {
        return;
    }
    
    if (_wsiSendBinary->getReadyState() == network::WebSocket::State::OPEN)
    {
        _sendBinaryStatus->setString("Send Binary WS is waiting...");
        char buf[] = "Hello WebSocket,\0 I'm\0 a\0 binary\0 message\0.";
        _wsiSendBinary->send((unsigned char*)buf, sizeof(buf));
    }
    else
    {
        std::string warningStr = "send binary websocket instance wasn't ready...";
        log("%s", warningStr.c_str());
        _sendBinaryStatus->setString(warningStr.c_str());
    }
}
