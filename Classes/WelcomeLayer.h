#pragma once

#include "AtlasLoader.h"
#include "SimpleAudioEngine.h"
#include "CCMenuItem.h"
#include "GameScene.h"
#include "time.h"
#include "cocos2d.h"
//#include "BirdSprite.h"

using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;

const int START_BUTTON_TAG = 100;

class WelcomeLayer : public Layer{
public:
	WelcomeLayer(void);
	~WelcomeLayer(void);
	virtual bool init();
	// implement the "static create()" method manually
	CREATE_FUNC(WelcomeLayer);
    
private:
	/**
     * The start button has been pressed will call this function
     */
	void menuStartCallback(Object *sender);
};