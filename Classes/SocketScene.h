//
//  SocketScene.h
//  DragonTactic
//
//  Created by 丁 中明 on 5/29/15.
//
//

#ifndef __DragonTactic__SocketScene__
#define __DragonTactic__SocketScene__

#include <stdio.h>
#include "extensions/cocos-ext.h"
#include "network/WebSocket.h"

USING_NS_CC;
using namespace cocos2d::ui;

class SocketScene : public cocos2d::Layer, public cocos2d::network::WebSocket::Delegate
{

public:
    SocketScene();
    virtual ~SocketScene();
    
    virtual void onOpen(cocos2d::network::WebSocket* ws);
    virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data);
    virtual void onClose(cocos2d::network::WebSocket* ws);
    virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error);
    
    // Menu Callbacks
    void onMenuSendTextClicked(cocos2d::Ref *sender);
    void onMenuSendBinaryClicked(cocos2d::Ref *sender);
    
private:
    cocos2d::network::WebSocket* _wsiSendText;
    cocos2d::network::WebSocket* _wsiSendBinary;
    cocos2d::network::WebSocket* _wsiError;
    
    cocos2d::Label* _sendTextStatus;
    cocos2d::Label* _sendBinaryStatus;
    cocos2d::Label* _errorStatus;
    
    int _sendTextTimes;
    int _sendBinaryTimes;
    Button* _btnBack;
};


#endif /* defined(__DragonTactic__SocketScene__) */
