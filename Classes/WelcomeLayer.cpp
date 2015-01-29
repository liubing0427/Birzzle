#include "WelcomeLayer.h"

WelcomeLayer::WelcomeLayer(){};

WelcomeLayer::~WelcomeLayer(){};

bool WelcomeLayer::init(){
	if(!Layer::init()){
		return false;
	}
	//get the origin point of the X-Y axis, and the visiable size of the screen
	Size visiableSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	//get the current time, the background image will selected by current time day or night: bg_day or bg_night
	Sprite *sky = Sprite::create("main_bg_c@2x.png");
	sky->setAnchorPoint(Point(0,1));
	sky->setPosition(Point(0,visiableSize.height));
	this->addChild(sky);

	Sprite *tree1 = Sprite::create("main_trees_a@2x.png");
	tree1->setAnchorPoint(Point(0,1));
	tree1->setPosition(Point(0,visiableSize.height));
	this->addChild(tree1);
	
	Sprite *tree2 = Sprite::create("main_trees_b@2x.png");
	tree2->setAnchorPoint(Point(1,1));
	tree2->setPosition(Point(visiableSize.width,visiableSize.height));
	this->addChild(tree2);

	Sprite *middle = Sprite::create("main_bg_b@2x.png");
	middle->setAnchorPoint(Point::ZERO);
	middle->setPosition(Point(0,visiableSize.height*3/32));//93
	this->addChild(middle);

	Sprite *menuback  = Sprite::create("main_menu_bg@2x.png");
	menuback->setAnchorPoint(Point::ZERO);
	menuback->setPosition(Point((visiableSize.width*3)/8 , visiableSize.height/12));
	menuback->setRotation(-8);
	this->addChild(menuback);

	Sprite *classic = Sprite::create("main_menu_classic@2x.png");
	classic->setRotation(-8);
	auto menuItem  = MenuItemSprite::create(classic,NULL,CC_CALLBACK_1(WelcomeLayer::menuStartCallback, this));
	menuItem->setAnchorPoint(Point::ZERO);
	menuItem->setPosition(Point((visiableSize.width*27)/64 , (visiableSize.height * 12) / 25));

	auto menu = Menu::create(menuItem,NULL);
	menu->setAnchorPoint(Point::ZERO);
	menu->setPosition(Point(origin.x ,origin.y));
	this->addChild(menu,1);

	Sprite *land = Sprite::create("main_bg_a@2x.png");
	land->setAnchorPoint(Point::ZERO);
	land->setPosition(Point::ZERO);
	this->addChild(land);

	//add the word game-title to the current scene
	Sprite *title  = Sprite::create("main_title@2x.png");
	title->setPosition(Point(origin.x + visiableSize.width/2 , (visiableSize.height * 6) / 7));
	this->addChild(title);

	////add the copyright-text to the current scne
	//Sprite *copyright = Sprite::createWithSpriteFrame(AtlasLoader::getInstance()->getSpriteFrameByName("brand_copyright"));
	//copyright->setPosition(Point(origin.x + visiableSize.width/2, origin.y + visiableSize.height/6));
	//this->addChild(copyright, 10);

	return true;
}

void WelcomeLayer::menuStartCallback(Object *sender){
	//SimpleAudioEngine::getInstance()->playEffect("sfx_swooshing.ogg");
	//this->removeChildByTag(BIRD_SPRITE_TAG);
	auto scene = GameScene::create();
	TransitionScene *transition = TransitionFade::create(1, scene);
	Director::getInstance()->replaceScene(transition);
}
