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

const int BIRD_INIT_ROW = 6; //С���ʼ����

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
	void initBall(Sprite *tree);   //��ʼ���򣬲���ʾ������
	void registerTouchBall();  //ע�������������ƶ��Ĵ���

	void checkThreeAndAboveSameBall(BallSprite* sprite);  //ɸѡ��3����������ɫ��ͬ����

	void removeAndMoveBall();  

	void produceNewBallFill();  //��������ȥ���

	double round(double number);

	void checkAbove(Address address, Size ballBoundingBoxSize);//����С���ʹ�ϲ�С������
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

	BallSprite* m_arrBall[9][7]; //����С��Ϊ9��7��

	cocos2d::EventListenerTouchOneByOne* m_listener1;

	vector<vector<BallSprite*>> m_sameBall; //������ͬ����

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
};