//
//  StartScene.cpp
//  MyProject
//
//  Created by ACE-clidev on 2018. 1. 19..
//
//

#include "StartScene.hpp"
#include "GameScene.hpp"
using namespace cocos2d;

CCScene* StartScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    StartScene *layer = StartScene::create();
    
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    
    CCMenuItemImage *pStartItem = CCMenuItemImage::create(
                                                          "MyResource/Images/Start.png",
                                                          "MyResource/Images/Start.png",
                                                          this,
                                                          menu_selector(StartScene::menuStartCallback));
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                          "MyResource/Images/Quit.png",
                                                          "MyResource/Images/Quit.png",
                                                          this,
                                                          menu_selector(StartScene::menuCloseCallback));
    

    CCMenu* pMenu = CCMenu::create(pStartItem,pCloseItem, NULL);
    
    pMenu->setPosition(ccp(origin.x + visibleSize.width*0.5f,
                           origin.y + visibleSize.height*0.5f));
    pMenu->CCMenu::alignItemsVertically();
    this->addChild(pMenu, 1);
    
    return true;
}


void StartScene::menuStartCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
   CCDirector::sharedDirector()->replaceScene(GameScene::scene());
#endif
#endif
}

void StartScene::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end(); // 다음 프레임에 디렉터 해제
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

