#ifndef __STATUSLAYER_H__
#define __STATUSLAYER_H__

#include "cocos2d.h"
#include "AtlasLoader.h"
#include "GameLayer.h"
#include "GameScene.h"
#include "WelcomeScene.h"
#include "Number.h"
#include "SimpleAudioEngine.h"
#include <cstdlib>

using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;

const string STAGE_GAMEOVER_NUM1 = "stage_gameover_num1@2x";
const string STAGE_GAMEOVER_NUM2 = "stage_gameover_num2@2x";
const string STAGE_MENUBAR_LEVEL_NUM = "stage_menubar_level_num@2x";
const string STAGE_MENUBAR_SCORE = "stage_menubar_score@2x";
const string STAGE_LEVEL = "stage_level@2x";
const int CURRENT_SCORE_SPRITE_TAG = 10001;

class GameLayer;

class StatusLayer:public Layer,public StatusDelegate{
public:
	StatusLayer(void);

	~StatusLayer(void);

	virtual bool init();

	CREATE_FUNC(StatusLayer);

	bool isGameStart();

	void onGamePlaying(int score);

	void onGameEnd(int curScore, int popBirds);

	void onGamePause();

private:
	void showReadyStatus();

	void showOverStatus(int curScore, int popBirds);

	void blinkFullScreen();

	void fadeInRestartBtn();

	void fadeInResumeBtn();

	Sprite* blink;

	void menuResumeCallback(Object *sender);

	void menuQuitCallback(Object *sender);

	void menuRestartCallback(Object *sender);

	Sprite* scorepanelSprite;
	Sprite* pauseSprite;

	Sprite* levelSprite;

	Sprite* levelNumSprite;

	Sprite* scopeSprite;

	Sprite* stageSprite;

	Sprite* readySprite;

	Sprite* downNumSprite;

	Sprite* startSprite;

	int currentScore;

	int tmpScore;

	int level;

	int popBirds;

	tm* playTime;

	Point originPoint;

	Size visibleSize;

	bool isStart;

	struct timeval startTimeval;

	LayerColor* grayLayer;
};
#endif
