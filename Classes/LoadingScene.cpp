#include "LoadingScene.h"
LoadingScene::LoadingScene(){}

LoadingScene::~LoadingScene(){}

bool LoadingScene::init() {
	if(Scene::init()){
		return true;
	} else {
		return false;
	}
}

void LoadingScene::onEnter(){
	// add background to current scene
	Sprite *background = AtlasLoader::getInstance()->getSpriteByName("Default-568h@2x.png");
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	background->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);
	this->addChild(background);
	this->loading();
}

void LoadingScene::loading(){
	AtlasLoader::getInstance()->loadAtlas("Character_100.plist");
	AtlasLoader::getInstance()->loadAtlas("Item_100.plist");
	AtlasLoader::getInstance()->loadAtlas("GameSheet_100.plist");

	// After loading the texture , preload all the sound
	/*SimpleAudioEngine::getInstance()->preloadEffect("sfx_die.ogg");
	SimpleAudioEngine::getInstance()->preloadEffect("sfx_hit.ogg");
	SimpleAudioEngine::getInstance()->preloadEffect("sfx_point.ogg");
	SimpleAudioEngine::getInstance()->preloadEffect("sfx_swooshing.ogg");
	SimpleAudioEngine::getInstance()->preloadEffect("sfx_wing.ogg");*/

	// After load all the things, change the scene to new one
	//auto scene = HelloWorld::createScene();
	auto scene = WelcomeScene::create();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}