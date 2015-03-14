#include "StatusLayer.h"
#include "GameLayer.h"

StatusLayer::StatusLayer(){};

StatusLayer::~StatusLayer(){};


bool StatusLayer::init(){
	if(!Layer::init()){
		return false;
	}
	// init numbers
	this->currentScore = 0;
	this->level = 1;
	this->popBirds = 0;
	this->isStart = false;
	Number::getInstance()->loadNumber(STAGE_GAMEOVER_NUM1.c_str(), "stage_gameover_num1_%d@2x.png");
	Number::getInstance()->loadNumber(STAGE_GAMEOVER_NUM2.c_str(), "stage_gameover_num2_%d@2x.png");
	Number::getInstance()->loadNumber(STAGE_MENUBAR_LEVEL_NUM.c_str(), "stage_menubar_level_%d@2x.png");
	Number::getInstance()->loadNumber(STAGE_MENUBAR_SCORE.c_str(), "stage_menubar_score_%d@2x.png");
	Number::getInstance()->loadNumber(STAGE_LEVEL.c_str(), "stage_level_%d@2x.png");
	this->visibleSize = Director::getInstance()->getVisibleSize();
	this->originPoint = Director::getInstance()->getVisibleOrigin();
	this->showReadyStatus();
	this->setGlobalZOrder(1000);
	return true;
}

void StatusLayer::showReadyStatus() {

	levelSprite = Sprite::create("stage_menubar_level@2x.png");
	levelSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width*5/6,this->originPoint.y + this->visibleSize.height *29/30));
	this->addChild(levelSprite);

	levelNumSprite = (Sprite *)Number::getInstance()->convert(STAGE_MENUBAR_LEVEL_NUM.c_str(), this->level, Gravity::GRAVITY_LEFT);
	levelNumSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width*47/50,this->originPoint.y + this->visibleSize.height *959/1000));
	this->addChild(levelNumSprite);

	scopeSprite = (Sprite *)Number::getInstance()->convert(STAGE_MENUBAR_SCORE.c_str(), 0, Gravity::GRAVITY_LEFT);
	scopeSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width*47/50,this->originPoint.y + this->visibleSize.height *23/25));
	this->addChild(scopeSprite);

	stageSprite = Sprite::create("stage_level_bg@2x.png");
	stageSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width / 2, this->originPoint.y + this->visibleSize.height *1/2));
	this->addChild(stageSprite);

	readySprite = Sprite::createWithSpriteFrameName("stage_level_ready.png");
	readySprite->setPosition(Point(this->originPoint.x + this->visibleSize.width* 4/9, this->originPoint.y + this->visibleSize.height * 1/2));
	this->addChild(readySprite);

	downNumSprite = (Sprite *)Number::getInstance()->convert(STAGE_LEVEL.c_str(), 3, Gravity::GRAVITY_LEFT);
	downNumSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width*2/3,this->originPoint.y + this->visibleSize.height *8/17));
	this->addChild(downNumSprite);

	this->runAction(Sequence::create(
		DelayTime::create(1.0f),
		CallFunc::create([&](){
			this->removeChild(downNumSprite);
			downNumSprite = (Sprite *)Number::getInstance()->convert(STAGE_LEVEL.c_str(), 2, Gravity::GRAVITY_LEFT);
			downNumSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width*2/3,this->originPoint.y + this->visibleSize.height *8/17));
			this->addChild(downNumSprite);
		}),
		DelayTime::create(1.0f), 
		CallFunc::create([&](){
			this->removeChild(downNumSprite);
			downNumSprite = (Sprite *)Number::getInstance()->convert(STAGE_LEVEL.c_str(), 1, Gravity::GRAVITY_LEFT);
			downNumSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width*2/3,this->originPoint.y + this->visibleSize.height *8/17));
			this->addChild(downNumSprite);
		}),
		DelayTime::create(1.0f), 
		CallFunc::create([&](){
			this->removeChild(readySprite);
			this->removeChild(downNumSprite);
			startSprite = Sprite::createWithSpriteFrameName("stage_level_start.png");
			startSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width / 2, this->originPoint.y + this->visibleSize.height * 1/2));
			this->addChild(startSprite);
		}), 
		DelayTime::create(0.5f), 
		CallFunc::create([&](){
			this->removeChild(startSprite);
			this->removeChild(stageSprite);
			this->isStart = true;
			gettimeofday(&startTimeval, NULL);
			GameLayer::getInstance()->onTouch();
		}),
		nullptr
	));
}

void StatusLayer::showOverStatus(int curScore, int popBirds) {
	this->currentScore = curScore;
	this->popBirds = popBirds;
	this->blinkFullScreen();
}

bool StatusLayer::isGameStart(){
	return this->isStart;
}

void StatusLayer::onGamePlaying(int score){
	this->removeChild(scopeSprite);
	scopeSprite = (Sprite *)Number::getInstance()->convert(STAGE_MENUBAR_SCORE.c_str(), score, Gravity::GRAVITY_LEFT);
	scopeSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width*47/50,this->originPoint.y + this->visibleSize.height *23/25));
	this->addChild(scopeSprite);
}

void StatusLayer::onGameEnd(int curScore, int popBirds){
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	time_t time_sec = nowTimeval.tv_sec - startTimeval.tv_sec;
	this->playTime = gmtime(&time_sec);

	this->showOverStatus(curScore, popBirds);
}

void StatusLayer::onGamePause(){
	grayLayer = LayerColor::create(Color4B(30,30,30,120), this->visibleSize.width, this->visibleSize.height);
	grayLayer->setGlobalZOrder(99);
	this->addChild(grayLayer);
	auto listener = EventListenerTouchOneByOne::create();//创建一个触摸监听(单点触摸）
	listener->onTouchBegan = [&](Touch* touch, Event* event){return true;};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,grayLayer);
	pauseSprite = Sprite::create("stage_pause_bg_nosave@2x.png");
	pauseSprite->setAnchorPoint(Point::ZERO);
	pauseSprite->setPosition(Point(this->originPoint.x, this->originPoint.y - pauseSprite->getContentSize().height));
	pauseSprite->setGlobalZOrder(100);
	this->addChild(pauseSprite);

	// Start next action
	auto pauseMoveTo = MoveTo::create(0.8f ,Point(this->originPoint.x,this->originPoint.y));
	// add variable motion for the action
	EaseExponentialOut* sineIn = EaseExponentialOut::create(pauseMoveTo);
	CallFunc *actionDone = CallFunc::create(bind(&StatusLayer::fadeInResumeBtn, this));
	auto sequence = Sequence::createWithTwoActions(sineIn, actionDone);
	pauseSprite->stopAllActions();
	//SimpleAudioEngine::getInstance()->playEffect("sfx_swooshing.ogg");
	pauseSprite->runAction(sequence);
}

void StatusLayer::fadeInResumeBtn(){
	//create the quit menu
	Sprite* resumeBtn = Sprite::create("stage_pause.png");
	resumeBtn->setGlobalZOrder(101);
	Sprite* resumeBtnActive = Sprite::create("stage_pause_resume@2x.png");
	resumeBtnActive->setGlobalZOrder(101);
	auto  resumeMenuItem = MenuItemSprite::create(resumeBtn,resumeBtnActive,nullptr,CC_CALLBACK_1(StatusLayer::menuResumeCallback,this));
	//create the restart menu;
	Sprite* restartBtn= Sprite::create("stage_pause.png");
	restartBtn->setGlobalZOrder(101);
	Sprite* restartBtnActive = Sprite::create("stage_pause_restart@2x.png");
	restartBtnActive->setGlobalZOrder(101);
	auto  restartMenuItem = MenuItemSprite::create(restartBtn,restartBtnActive,nullptr,CC_CALLBACK_1(StatusLayer::menuRestartCallback,this));
	//create the quit menu;
	Sprite* quitBtn = Sprite::create("stage_pause.png");
	quitBtn->setGlobalZOrder(101);
	Sprite* quitBtnActive = Sprite::create("stage_pause_quit_nosave@2x.png");
	quitBtnActive->setGlobalZOrder(101);
	auto  quitMenuItem = MenuItemSprite::create(quitBtn,quitBtnActive,nullptr,CC_CALLBACK_1(StatusLayer::menuQuitCallback,this));

	auto menu = Menu::create(resumeMenuItem, restartMenuItem, quitMenuItem, NULL);
	menu->alignItemsVerticallyWithPadding(40.0f);
	menu->setPosition(Point((pauseSprite->getContentSize().width*13)/22 , (pauseSprite->getContentSize().height * 9) / 24));
	menu->setGlobalZOrder(101);
	pauseSprite->addChild(menu);
}

void StatusLayer::blinkFullScreen(){    
	// create the score panel
	scorepanelSprite = Sprite::create("stage_gameover_bg@2x.png");
	scorepanelSprite->setPosition(Point(this->originPoint.x + this->visibleSize.width / 2, this->originPoint.y - scorepanelSprite->getContentSize().height));
	this->addChild(scorepanelSprite);

	auto lastLevelSprite = (Sprite *)Number::getInstance()->convert(STAGE_GAMEOVER_NUM1.c_str(), this->level, Gravity::GRAVITY_LEFT);
	lastLevelSprite->setPosition(scorepanelSprite->getContentSize().width*9/10, scorepanelSprite->getContentSize().height*10/13);
	scorepanelSprite->addChild(lastLevelSprite);

	TTFConfig ttfConfig = TTFConfig("fonts/SAF.ttf", 35, GlyphCollection::DYNAMIC, nullptr, false);
	Label* playTimeSprite = Label::createWithTTF(ttfConfig, String::createWithFormat("%d:%d",this->playTime->tm_hour*60 + this->playTime->tm_min, this->playTime->tm_sec)->getCString(), TextHAlignment::RIGHT);
	playTimeSprite->setAnchorPoint(Point(1,1));
	playTimeSprite->setColor(Color3B::WHITE);
	playTimeSprite->setPosition(scorepanelSprite->getContentSize().width*23/25, scorepanelSprite->getContentSize().height*16/22);
	scorepanelSprite->addChild(playTimeSprite);

	auto popBirdsSprite = (Sprite *)Number::getInstance()->convert(STAGE_GAMEOVER_NUM1.c_str(), this->popBirds, Gravity::GRAVITY_LEFT);
	popBirdsSprite->setPosition(scorepanelSprite->getContentSize().width*9/10, scorepanelSprite->getContentSize().height*6/10);
	scorepanelSprite->addChild(popBirdsSprite);

	auto totalScoreSprite = (Sprite *)Number::getInstance()->convert(STAGE_GAMEOVER_NUM2.c_str(), this->currentScore, Gravity::GRAVITY_LEFT);
	totalScoreSprite->setPosition(scorepanelSprite->getContentSize().width*9/10, scorepanelSprite->getContentSize().height*5/10);
	scorepanelSprite->addChild(totalScoreSprite);

	// Start next action
	auto scorePanelMoveTo = MoveTo::create(0.8f ,Point(this->originPoint.x + this->visibleSize.width / 2,this->originPoint.y + this->visibleSize.height/2 - 10.0f));
	// add variable motion for the action
	EaseExponentialOut* sineIn = EaseExponentialOut::create(scorePanelMoveTo);
	CallFunc *actionDone = CallFunc::create(bind(&StatusLayer::fadeInRestartBtn, this));
	auto sequence = Sequence::createWithTwoActions(sineIn, actionDone);
	scorepanelSprite->stopAllActions();
	//SimpleAudioEngine::getInstance()->playEffect("sfx_swooshing.ogg");
	scorepanelSprite->runAction(sequence);
}

void StatusLayer::fadeInRestartBtn(){
	//create the quit menu
	Sprite* quitBtn = Sprite::create("stage_gameover_up_button@2x.png");
	Sprite* quitBtnActive = Sprite::create("stage_gameover_quit@2x.png");
	auto  quitMenuItem = MenuItemSprite::create(quitBtn,quitBtnActive,nullptr,CC_CALLBACK_1(StatusLayer::menuQuitCallback,this));
	//create the restart menu;
	Sprite* restartBtn= Sprite::create("stage_gameover_up_button@2x.png");
	Sprite* restartBtnActive = Sprite::create("stage_gameover_restart@2x.png");
	auto  restartMenuItem = MenuItemSprite::create(restartBtn,restartBtnActive,nullptr,CC_CALLBACK_1(StatusLayer::menuRestartCallback,this));

	auto menu = Menu::create(quitMenuItem, restartMenuItem, NULL);
	menu->alignItemsHorizontallyWithPadding(20.0f);
	menu->setPosition(Point((scorepanelSprite->getContentSize().width*10)/20 , (scorepanelSprite->getContentSize().height * 3) / 23));
	scorepanelSprite->addChild(menu, 100);
}

void StatusLayer::menuResumeCallback(Object *sender){
	this->removeChild(grayLayer);
	this->removeChild(pauseSprite);
	GameLayer::getInstance()->resume();
}

void StatusLayer::menuQuitCallback(Object *sender){
	auto scene = WelcomeScene::create();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}

void StatusLayer::menuRestartCallback(Object* pSender){
	//SimpleAudioEngine::getInstance()->playEffect("sfx_swooshing.ogg");
	auto scene = GameScene::create();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}