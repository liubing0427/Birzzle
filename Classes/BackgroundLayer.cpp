#include "BackgroundLayer.h"
BackgroundLayer::BackgroundLayer(){};
BackgroundLayer::~BackgroundLayer(){};

bool BackgroundLayer::init(){
	if(!Layer::init()){
		return false;
	}
	Size visiableSize = Director::getInstance()->getVisibleSize();
	//create the background image according to the current time;
	Sprite *background = Sprite::create("stage_bg@2x.png");
	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Point::ZERO);
	this->addChild(background);

	return true;
}

float BackgroundLayer::getLandHeight() {
	return 40;
}