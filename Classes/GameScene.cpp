//
//  GameScene.cpp
//  MyProject
//
//  Created by ACE-clidev on 2018. 1. 19..
//
//

#include "GameScene.hpp"
using namespace cocos2d;

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();
    
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    isGamePause = false;
    
    maskBoard = new int[64];
    for(int i=0; i<64; i++) maskBoard[i] = PIECETYPE::EMPTY;
    
    chessMans = CCArray::createWithCapacity(32); // -> retain cnt = 1
    chessMans->retain(); // -> retain cnt = 2
    //this->addChild(chessMans); // CCARRAY는 노드가 아닌 오브젝트를 상속해서 addChild가 불가능
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                          "CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(GameScene::menuCloseCallback));
    
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    
    pMenu->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width*0.5f,
                           origin.y + visibleSize.height - pCloseItem->getContentSize().height * 0.5f));
    
    this->addChild(pMenu, 1);
    
    if(visibleSize.width > visibleSize.height)
        tileSize = visibleSize.height * 0.1f;
    else tileSize = visibleSize.width * 0.1f;
    
    boardOffsetX = visibleSize.width * 0.5f - tileSize * 4.0f;
    boardOffsetY = visibleSize.height * 0.5f - tileSize * 4.0f;
    
    CCSprite* GameBoard = CCSprite::create("MyResource/Images/Chess_Board.png");
    GameBoard->setPosition(ccp(visibleSize.width * 0.5f,visibleSize.height * 0.5f));
    GameBoard->setScaleX(tileSize * 8.0f/GameBoard->getContentSize().width);
    GameBoard->setScaleY(tileSize * 8.0f/GameBoard->getContentSize().height);
    
    this->addChild(GameBoard,2);
    createChessMans();
    
    return true;
}

void GameScene::createChessMans(){
    for(int y=0; y<8; y++){
        // 위 아래 각 두번째 줄에 폰 생성
        if(y==1 || y==6)
        {
            for(int x=0; x<8;x++){
                ChessObject* chessMan = ChessObject::create("MyResource/Images/Pawn.png");
                maskBoard[y*8 +x] = PIECETYPE::PAWN;
                chessMan->setPieceType(PIECETYPE::PAWN);
                chessMan->setScaleX(tileSize/chessMan->getContentSize().width);
                chessMan->setScaleY(tileSize/chessMan->getContentSize().height);
                chessMan->setPosition(ccp(boardOffsetX + ((x+0.5f) * tileSize) , boardOffsetY + ((y+0.5f) * tileSize)));
                chessMan->startPos = chessMan->getPosition();

                if(y<2) {chessMan->setColor(ccc3(0,0,0)); chessMan->setThisBlackSide(true);}
                else chessMan->setThisBlackSide(false);
                
                this->addChild(chessMan,3);
                chessMans->addObject(chessMan);
            }
        }
       /// 각 X 위치에 맞는 말 생성
        else if(y==0 || y==7){
            for(int x=0; x<8;x++){
                ChessObject* chessMan;
                if(x==0 || x==7){
                    chessMan = (ChessObject*)ChessObject::create("MyResource/Images/Rook.png");
                    chessMan->setPieceType(PIECETYPE::ROOK);
                    maskBoard[y*8 +x] = PIECETYPE::ROOK;
                }
                else if(x==1 || x==6){
                    chessMan = (ChessObject*)ChessObject::create("MyResource/Images/Knight.png");
                    chessMan->setPieceType(PIECETYPE::KNIGHT);
                    maskBoard[y*8 +x] = PIECETYPE::KNIGHT;
                }
                else if(x==2 || x==5){
                    chessMan = (ChessObject*)ChessObject::create("MyResource/Images/Bishop.png");
                    chessMan->setPieceType(PIECETYPE::BISHOP);
                    maskBoard[y*8 +x] = PIECETYPE::BISHOP;
                }
                else if(x==3){
                    chessMan =(ChessObject*)ChessObject::create("MyResource/Images/Queen.png");
                    chessMan->setPieceType(PIECETYPE::QUEEN);
                    maskBoard[y*8 +x] = PIECETYPE::QUEEN;
                }
                else {
                    chessMan = (ChessObject*)ChessObject::create("MyResource/Images/King.png"); /*x == 4*/
                    chessMan->setPieceType(PIECETYPE::KING);
                    maskBoard[y*8 +x] = PIECETYPE::KING;
                }
                if(y<2) {chessMan->setColor(ccc3(0,0,0)); chessMan->setThisBlackSide(true);}
                else chessMan->setThisBlackSide(false);
                
               
                chessMan->setScaleX(tileSize/chessMan->getContentSize().width);
                chessMan->setScaleY(tileSize/chessMan->getContentSize().height);
                chessMan->setPosition(ccp(boardOffsetX + ((x+0.5f) * tileSize) , boardOffsetY + ((y+0.5f) * tileSize)));
                chessMan->startPos = chessMan->getPosition();
                this->addChild(chessMan,3);
                
                chessMans->addObject(chessMan);
            }
        }
    }
}
bool GameScene::ccTouchBegan(cocos2d::CCTouch *pTouch,cocos2d::CCEvent* event){
    CCPoint touchPoint = pTouch->getLocation();
    
    CCObject * obj;
    CCARRAY_FOREACH(chessMans, obj){
        ChessObject* chessObj = (ChessObject*)obj;
        if(chessObj->boundingBox().containsPoint(touchPoint)){
            moveTarget = chessObj;
            chessObj->originPos =  ccp((int) ((touchPoint.x - boardOffsetX)/tileSize),
                                       (int) ((touchPoint.y - boardOffsetY)/tileSize));
        }
    }
    return true;
}
void GameScene::ccTouchMoved(cocos2d::CCTouch *pTouch,cocos2d::CCEvent* event){
    CCPoint touchPoint = pTouch->getLocation();
    
    if(!moveTarget)return;
    moveTarget->setPosition(touchPoint);
}
void GameScene::ccTouchEnded(cocos2d::CCTouch *pTouch,cocos2d::CCEvent* event){
    if(!moveTarget) return;
    
    CCPoint touchPoint = pTouch->getLocation();
    
    // 보드밖으로 나가거나 이동이 부적절하면 제자리로
    if(!canMove(touchPoint.x, touchPoint.y)){
        moveTarget->setPosition(ccp(boardOffsetX + moveTarget->originPos.x * tileSize + tileSize * 0.5f ,
                                    boardOffsetY + moveTarget->originPos.y * tileSize + tileSize * 0.5f));
        moveTarget = NULL;
        return;
    }
    moveTarget->setPosition(ccp(boardOffsetX + (int) ((touchPoint.x - boardOffsetX)/tileSize) * tileSize + tileSize * 0.5f,
                                boardOffsetY + (int) ((touchPoint.y - boardOffsetY)/tileSize) * tileSize + tileSize * 0.5f));
    // 이동 후 마스크 보드 갱신
    CCObject* obj;
    CCARRAY_FOREACH(chessMans, obj){
        ChessObject* chessObj = (ChessObject*)obj;
        if(chessObj == moveTarget) continue;
        
        //제거
        if(chessObj->boundingBox().containsPoint(touchPoint)){
            chessObj->setPosition(ccp(-500, -500));
            if(chessObj->getPieceType() == PIECETYPE::KING) gameEnd(); // 킹이 죽으면 게임종료
        }
    }

    int originX = moveTarget->originPos.x;
    int originY = moveTarget->originPos.y;
    int destX = (int) ((touchPoint.x - boardOffsetX)/tileSize);
    int destY = (int) ((touchPoint.y - boardOffsetY)/tileSize);
    
    maskBoard[originX + originY *8] = PIECETYPE::EMPTY;
    maskBoard[destX + destY *8] = moveTarget->getPieceType();
    
    moveTarget = NULL;
    // 보드 출력
    for(int y=7; y>-1;y--){
        CCLOG("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d" , maskBoard[y*8 + 0] ,maskBoard[y*8 + 1],maskBoard[y*8 + 2],maskBoard[y*8 + 3],maskBoard[y*8 + 4],maskBoard[y*8 + 5],maskBoard[y*8 + 6],maskBoard[y*8 + 7]);
    }CCLOG(" ");
}
void GameScene::gameEnd(){
    CCObject* obj;
    CCARRAY_FOREACH(chessMans, obj){
        ChessObject* cObj = (ChessObject*)obj;
        cObj->setPosition(cObj->startPos);
    }
    //asdasd
    // 마스크 보드 초기화
    for(int y=0; y<8; y++){
        // 위 아래 각 두번째 줄에 폰 생성
        if(y==1 || y==6) for(int x=0; x<8;x++)maskBoard[y*8 +x] = PIECETYPE::PAWN;
        /// 각 X 위치에 맞는 말 생성
        else if(y==0 || y==7){
            for(int x=0; x<8;x++){
                if(x==0 || x==7) maskBoard[y*8 +x] = PIECETYPE::ROOK;
                else if(x==1 || x==6) maskBoard[y*8 +x] = PIECETYPE::KNIGHT;
                else if(x==2 || x==5) maskBoard[y*8 +x] = PIECETYPE::BISHOP;
                else if(x==3) maskBoard[y*8 +x] = PIECETYPE::QUEEN;
                else maskBoard[y*8 +x] = PIECETYPE::KING;
            }
        }
        else for(int x=0; x<8;x++){maskBoard[y*8 +x] = PIECETYPE::EMPTY;}
    }
}
bool GameScene::canMove(int _targetPosX , int _targetPosY){
    
    if(_targetPosX < boardOffsetX || _targetPosX > boardOffsetX + tileSize * 8.0f ||
       _targetPosY < boardOffsetY || _targetPosY > boardOffsetY + tileSize * 8.0f ){
        return false;
    }
    int originX = moveTarget->originPos.x;
    int originY = moveTarget->originPos.y;
    int destX = (int) ((_targetPosX - boardOffsetX)/tileSize);
    int destY = (int) ((_targetPosY - boardOffsetY)/tileSize);
    
    // 이동 지점에 말이 있다면 targetObj에 저장
    ChessObject* targetObj = nullptr;
    CCObject* obj;
    CCARRAY_FOREACH(chessMans, obj){
        ChessObject* chessObj = (ChessObject*)obj;
        if(chessObj == moveTarget) continue;
        if(chessObj->boundingBox().containsPoint(ccp(_targetPosX , _targetPosY))){
            targetObj = chessObj;
            break;
        }
    }
    if(targetObj!=nullptr)
     if(moveTarget->isBlackSide()== targetObj->isBlackSide())return false; // 같은 진영이면 이동 불가
    
    switch (moveTarget->getPieceType()) {
        case PIECETYPE::PAWN :
            // 이동장소가 비어있을경우 전진가능 (첫 이동이면 2칸까지 이동 가능)
            if(maskBoard[destY * 8 + destX] == PIECETYPE::EMPTY){
                
                int limitY = moveTarget->isFirstMove ? 2 : 1;
                int direction = moveTarget->isBlackSide() ? 1 : -1;
                // X좌표가 , 방향 , 이동거리가 적합한지 체크
                if( originX ==  destX && (destY - originY) * direction > 0 && abs(destY - originY) > 0 && abs(destY - originY) <= limitY && (destY - originY)){
                    moveTarget->isFirstMove = false;
                    return true;
                }
                else return false;
            }
            // 이동장소가 비어있지 않은경우
            else{
                if( abs(originX - destX) == 1 &&  (moveTarget->isBlackSide() ? (destY - originY) == 1 : (destY - originY) == -1) ){moveTarget->isFirstMove = false; return true;}
                else return false;
            }
            break;
            
        case PIECETYPE::ROOK :
            // 직진이 아니면 이동 X
            if(destX - originX != 0  && destY - originY !=0)return false;
            else{
                bool isMoveDirX = destX == originX ? false : true; //  X축 이동인지 Y축 이동인지
                int rptCnt = isMoveDirX ? abs(destX - originX) : abs(destY - originY); // 보드체크할 횟수
                bool moveDirPlus = isMoveDirX ? (destX - originX) >0 : (destY - originY) >0; // 양의 방향인지 음의 방향인지
                int tempX = originX, tempY = originY;
                
                for(int i=0; i<rptCnt-1; i++){
                    if(isMoveDirX) moveDirPlus? tempX++ : tempX--;
                    else moveDirPlus ? tempY++ : tempY--;
                    
                    if(maskBoard[tempY*8 + tempX] != PIECETYPE::EMPTY) return false;
                }
                return true;
            }
            break;
            
        case PIECETYPE::KNIGHT :
            if(abs(destX - originX)>2 ||abs(destY - originY)>2 ) return false;
            else if(abs(destX - originX) + abs(destY - originY) == 3){
                return true;
            }
            return false;
            break;
            
        case PIECETYPE::BISHOP :
            // 대각선이 아니면 이동 X
            if(abs(destX - originX) != abs(destY - originY)) return false;
            else{
                bool isMoveDirXPlus = destX - originX > 0 ? true : false;
                bool isMoveDirYPlus = destY - originY > 0 ? true : false;
                int rptCnt = abs(destX - originX);
                
                int tempX = originX, tempY = originY;
                
                for(int i=0; i<rptCnt-1; i++){
                    isMoveDirXPlus ? tempX++ : tempX--;
                    isMoveDirYPlus ? tempY++ : tempY--;
                    if(maskBoard[tempY*8 + tempX] != PIECETYPE::EMPTY) return false;
                }
                return true;
            }
            break;
            
        case PIECETYPE::QUEEN :
            // 대각선 이동
            if(abs(destX - originX) == abs(destY - originY)){
                bool isMoveDirXPlus = destX - originX > 0 ? true : false;
                bool isMoveDirYPlus = destY - originY > 0 ? true : false;
                int rptCnt = abs(destX - originX);
                
                int tempX = originX, tempY = originY;
                
                for(int i=0; i<rptCnt-1; i++){
                    isMoveDirXPlus ? tempX++ : tempX--;
                    isMoveDirYPlus ? tempY++ : tempY--;
                    if(maskBoard[tempY*8 + tempX] != PIECETYPE::EMPTY) return false;
                }
                return true;
            }
            // 직선 이동
            else if(destX - originX == 0  || destY - originY ==0){
                bool isMoveDirX = destX == originX ? false : true; //  X축 이동인지 Y축 이동인지
                int rptCnt = isMoveDirX ? abs(destX - originX) : abs(destY - originY); // 보드체크할 횟수
                bool moveDirPlus = isMoveDirX ? (destX - originX) >0 : (destY - originY) >0; // 양의 방향인지 음의 방향인지
                int tempX = originX, tempY = originY;
                
                for(int i=0; i<rptCnt-1; i++){
                    if(isMoveDirX) moveDirPlus? tempX++ : tempX--;
                    else moveDirPlus ? tempY++ : tempY--;
                    
                    if(maskBoard[tempY*8 + tempX] != PIECETYPE::EMPTY) return false;
                }
                return true;
            }
            return false;
            break;
            
        case PIECETYPE::KING :
            if(abs(destX - originX)>1 ||abs(destY - originY)>1 ) return false;
            return true;
            break;
            
        case PIECETYPE::EMPTY:
            return false;
            break;
    }
    return false;
}
void GameScene::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent*pEvent){
    moveTarget->setPosition(ccp(boardOffsetX + moveTarget->originPos.x * tileSize + tileSize * 0.5f ,
                                boardOffsetY + moveTarget->originPos.y * tileSize + tileSize * 0.5f));
}
void GameScene::onEnter(){
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    CCLayer::onEnter();
}
void GameScene::onExit(){
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    chessMans->release();
    delete[] maskBoard;
    CCLayer::onExit();
}
void GameScene::menuCloseCallback(CCObject* pSender){
    CCDirector::sharedDirector()->end(); // 다음 프레임에 디렉터 해제
    
    exit(0);
}
