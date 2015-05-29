//
//  TitleScene.h
//  DragonTactic
//
//  Created by 丁 中明 on 5/29/15.
//
//

#ifndef __DragonTactic__TitleScene__
#define __DragonTactic__TitleScene__

#include <stdio.h>
#include <cocos/ui/CocosGUI.h>

USING_NS_CC;
using namespace cocos2d::ui;

class TitleScene : public cocos2d::Layer
{
private:
    Button* _btnGame;
    Button* _btnSocketTest;
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(TitleScene);
};

#endif /* defined(__DragonTactic__TitleScene__) */
