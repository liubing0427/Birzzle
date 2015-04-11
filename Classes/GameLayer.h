#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"
//#include "UserRecord.h"
#include "OptionLayer.h"
#include "BallSprite.h"
#include "BackgroundLayer.h"
#include "AtlasLoader.h"
#include "SimpleAudioEngine.h"
#include <algorithm>
#include <cstdlib>

using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;

#define min(X,Y) ((X) < (Y) ? (X) : (Y)) 
#define max(X,Y) ((X) > (Y) ? (X) : (Y)) 

const int X_SKEWING = 25;

const int Y_SKEWING = 80;

const int BIRD_WIDTH = 73;

const int WIDTH = 100;

const int GAME_ROW = 9;
const int BIRD_INIT_ROW = 3; //小鸟初始行数

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

class StatusDelegate {
public:
	/**
	* When the game start, this method will be called
	*/
	virtual bool isGameStart(void) = 0;

	/**
	* During paying, after the score changed, this method will be called
	*/
	virtual void onGamePlaying(int score) = 0;

	/**
	* When game is over, this method will be called
	*/
	virtual void onGameEnd(int curSocre, int popBirds) = 0;

	virtual void onGamePause() = 0;
};

class GameLayer : public Layer, public OptionDelegate{
protected:
	void initBall();   //初始化球，并显示到界面
	void registerTouchBall();  //注册球触摸并进行移动的处理

	void checkThreeAndAboveSameBall(BallSprite* sprite);  //筛选出3个及以上颜色相同的球

	void removeAndMoveBall();  

	void produceNewBall();  //产生新球

	void fallDown(float dt);

	double round(double number);

	void gameOver();

	void menuPauseCallback(Object *sender);

public:
	GameLayer();

	~GameLayer();

	virtual bool init() override;

	virtual void onExit() override;

	CREATE_FUNC(GameLayer);

	/**
	* @Override 
	* To see @OptionDelegate::onTouch 
	* User have touched the screen
	* This game, user just need only this method to control game
	*/
	void onTouch();

	///**
	//* This layer need physical engine work
	//*/
	void setPhyWorld(PhysicsWorld* world){this->world = world;}

	void removeFromArray(int row, int column);

	BallSprite* getBall(int row, int column);

	void swithBall(int row, int column, int destrow, int destcolumn);

	static GameLayer* getInstance();

	CC_SYNTHESIZE(StatusDelegate*, delegator, Delegator);

	void startOrStopSchedule(bool isStart);

private:
    
	PhysicsWorld *world;

	Sprite *tree;

	GameStatus gameStatus;

	BallSprite* m_arrBall[9][7]; //保存小球，为9行7列

	BallSprite* m_newBall[7];

	cocos2d::EventListenerTouchOneByOne* m_listener1;

	vector<vector<BallSprite*>> m_sameBall; //保存相同的球

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

	static GameLayer* shared;

	int score;

	int popBirds;

	Size visiableSize;

	Point visibleOrigin;
};
#endif