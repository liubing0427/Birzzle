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

const int UP_PIP = 21;

const int DOWN_PIP = 12;

const int PIP_PASS = 30;

const int PIP_NEW = 31;

/**
 * The radius of the bird
 */
const int BIRD_SIZE = 75;

/**
 * The height of the pips
 */
const int PIP_HEIGHT = 320;

/**
 * The width of the pips
 */
const int PIP_WIDTH = 52;

/**
 * Pip shift speed
 */
const float PIP_SHIFT_SPEED = 80.0f;

/**
 * The distance between the down pip and up pip
 */
const int PIP_DISTANCE = 100;

/**
 * The distance between the pips vertical
 */
const int PIP_INTERVAL = 180;

/**
 * The number of pip pairs display in the screen in the same time
 */
const int PIP_COUNT = 2;

/**
 * The distance that the pip will display in the screen, for player to ready
 */
const int WAIT_DISTANCE = 100;

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
	void setBallExchange(BallSprite* target);  //����2����

	void checkThreeAndAboveSameBall();  //ɸѡ��3����������ɫ��ͬ���򣬷ŵ�m_rowThreeAboveSameBall��m_colThreeAboveSameBall

	void removeAndMoveBall();  //�Ƴ�����������������������ƶ���䣬����checkThreeAndAboveSameBall�����ݹ����

	void produceNewBallFill();  //��������ȥ���
public:
	GameLayer();

	~GameLayer();

	virtual bool init() override;

	virtual void onExit() override;

	CREATE_FUNC(GameLayer);

	/**
	* According to current game status, give the order to delegate.
	*/
	//CC_SYNTHESIZE(StatusDelegate*, delegator, Delegator);

	/**
	* @Override 
	* To see @OptionDelegate::onTouch 
	* User have touched the screen
	* This game, user just need only this method to control game
	*/
	//void onTouch();

	///**
	//* This layer need physical engine work
	//*/
	void setPhyWorld(PhysicsWorld* world){this->world = world;}
 //   
 //   void update(float delta);

private:
    
	PhysicsWorld *world;

	/*GameStatus gameStatus;*/

    int score;

	Node *groundNode;

	struct BallSprite* m_arrBall[9][7]; //����С��Ϊ9��7��

	cocos2d::EventListenerTouchOneByOne* m_listener1;

	vector<vector<BallSprite*>> m_rowThreeAboveSameBall; //��������ͬ����
	vector<vector<BallSprite*>> m_colThreeAboveSameBall; //��������ͬ����

	/*bool onContactBegin(EventCustom *event, const PhysicsContact& contact);

	void gameOver();*/
};