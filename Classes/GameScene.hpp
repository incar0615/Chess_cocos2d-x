//
//  GameScene.hpp
//  MyProject
//
//  Created by ACE-clidev on 2018. 1. 19..
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "ChessObject.hpp"

class GameScene : public cocos2d::CCLayer
{
    float tileSize;
    float boardOffsetX;
    float boardOffsetY;
       //cocos2d::CCSprite* moveTarget;
    ChessObject* moveTarget;
    
public:
    cocos2d::CCArray* chessMans;
    int* maskBoard;

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    bool isGamePause;
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);
    
    void onEnter();
    void onExit();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch,cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch,cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch,cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent*pEvent);
    void gameEnd();
    void createChessMans();
    bool canMove(int _targetPosX , int _targetPosY);
};
#endif /* GameScene_hpp */
