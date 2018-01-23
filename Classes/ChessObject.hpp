//
//  ChessObject.hpp
//  MyProject
//
//  Created by ACE-clidev on 2018. 1. 22..
//
//

#ifndef ChessObject_hpp
#define ChessObject_hpp

#include "cocos2d.h"

enum PIECETYPE { EMPTY = 0,PAWN , ROOK , KNIGHT , BISHOP , QUEEN , KING };

class ChessObject : public cocos2d::CCSprite{
private:
    PIECETYPE pieceType;
    bool isBlack;
   
public:
    CREATE_FUNC(ChessObject)
    cocos2d::CCPoint originPos;
    cocos2d::CCPoint startPos;
    PIECETYPE getPieceType(){return pieceType;}
    bool isFirstMove;    /*static ChessObject* create(const char *pszFileName){
        CCSprite* spr = CCSprite::create(pszFileName);
        return dynamic_cast<ChessObject*>(spr);
    }*/
    static ChessObject* create(const char *fileName){
        ChessObject* cObj = new (std::nothrow) ChessObject;
        if(cObj && fileName &&cObj->initWithFile(fileName)){
            cObj->autorelease();
            cObj->isFirstMove = true;
            return cObj;
        }
        CC_SAFE_DELETE(cObj);
        return nullptr;
    }
    void setPieceType(PIECETYPE _type){pieceType = _type; }
    bool isBlackSide(){return isBlack;}
    void setThisBlackSide(bool _isBlack){isBlack = _isBlack;} // true black false white
};
#endif /* ChessObject_hpp */
