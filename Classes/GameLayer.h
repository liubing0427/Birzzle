#pragma once

#include "cocos2d.h"
//#include "UserRecord.h"
#include "OptionLayer.h"
#include "BallSprite.h"
#include "BackgroundLayer.h"
#include "AtlasLoader.h"
#include "SimpleAudioEngine.h"
#include <cstdlib>

using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;

#define min(X,Y) ((X) < (Y) ? (X) : (Y)) 
#define max(X,Y) ((X) > (Y) ? (X) : (Y)) 

const int X_SKEWING = 30;

const int Y_SKEWING = 80;

const int BIRD_WIDTH = 73;

const int BIRD_INIT_ROW = 6; //小鸟初始行数

/**
* Define the game status
* GAME_STATUS_READY game is not start, just ready for payer to start.
* GAME_STATUS_START the game is started, and payer is paying this game.
* GAME_STATUS_OVER the player is lose this game, the game is over.
*/
typedef enum _game_status {
	GAME_STATUS_READY = 1,
	GAME_STATUS_START,
	GAME_STATUS_OVER
} GameStatus;

//class StatusDelegate {
//public:
//	/**
//	* When the game start, this method will be called
//	*/
//	virtual void onGameStart(void) = 0;
//
//	/**
//	* During paying, after the score changed, this method will be called
//	*/
//	virtual void onGamePlaying(int score) = 0;
//
//	/**
//	* When game is over, this method will be called
//	*/
//	virtual void onGameEnd(int curSocre, int bestScore) = 0;
//};

class GameLayer : public Layer{
protected:
	void initBall(Sprite *tree);   //初始化球，并显示到界面
	void registerTouchBall();  //注册球触摸并进行移动的处理

	void checkThreeAndAboveSameBall(BallSprite* sprite);  //筛选出3个及以上颜色相同的球

	void removeAndMoveBall();  

	void produceNewBallFill();  //产生新球去填充

	double round(double number);

	void checkAbove(Address address, Size ballBoundingBoxSize);//消除小球后，使上层小球下落
public:
	GameLayer();

	~GameLayer();

	virtual bool init() override;

	virtual void onExit() override;

	CREATE_FUNC(GameLayer);

	///**
	//* This layer need physical engine work
	//*/
	void setPhyWorld(PhysicsWorld* world){this->world = world;}

private:
    
	PhysicsWorld *world;

	/*GameStatus gameStatus;*/

	BallSprite* m_arrBall[9][7]; //保存小球，为9行7列

	cocos2d::EventListenerTouchOneByOne* m_listener1;

	vector<vector<BallSprite*>> m_sameBall; //保存相同的球

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
};