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
	Sprite *sky = AtlasLoader::getInstance()->getSpriteByName("main_bg_c_r@2x.png");
	sky->setAnchorPoint(Point(0,1));
	sky->setPosition(Point(0,visiableSize.height));
	this->addChild(sky);

	Sprite *tree1 = AtlasLoader::getInstance()->getSpriteByName("main_trees_a@2x.png");
	tree1->setAnchorPoint(Point(0,1));
	tree1->setPosition(Point(0,visiableSize.height));
	this->addChild(tree1);
	
	Sprite *tree2 = AtlasLoader::getInstance()->getSpriteByName("main_trees_b@2x.png");
	tree2->setAnchorPoint(Point(1,1));
	tree2->setPosition(Point(visiableSize.width,visiableSize.height));
	this->addChild(tree2);

	Sprite *middle = AtlasLoader::getInstance()->getSpriteByName("main_bg_b@2x.png");
	middle->setAnchorPoint(Point::ZERO);
	middle->setPosition(Point(0,visiableSize.height*5/64));
	this->addChild(middle);

	//add the word game-title to the current scene
	Sprite *title  = AtlasLoader::getInstance()->getSpriteByName("main_title@2x.png");
	title->setPosition(Point(visiableSize.width/2 , (visiableSize.height * 6) / 7));
	this->addChild(title);

	Sprite *white  = AtlasLoader::getInstance()->getSpriteByName("main_cha_white@2x.png");
	white->setPosition(Point(visiableSize.width/4 , (visiableSize.height * 12) / 21));
	this->addChild(white);

	Sprite *blue  = AtlasLoader::getInstance()->getSpriteByName("main_cha_blue@2x.png");
	blue->setPosition(Point(visiableSize.width/5 , (visiableSize.height * 9) / 21));
	this->addChild(blue);

	Sprite *orange  = AtlasLoader::getInstance()->getSpriteByName("main_cha_orange@2x.png");
	orange->setPosition(Point(visiableSize.width/7 , (visiableSize.height * 7) / 23));
	this->addChild(orange);
	
	Sprite *pink  = AtlasLoader::getInstance()->getSpriteByName("main_cha_pink@2x.png");
	pink->setPosition(Point(visiableSize.width*5/16 , (visiableSize.height * 10) / 29));
	this->addChild(pink);

	Sprite *green  = AtlasLoader::getInstance()->getSpriteByName("main_cha_green@2x.png");
	green->setPosition(Point(visiableSize.width*7/22 , (visiableSize.height * 8) / 31));
	this->addChild(green);

	Sprite *red  = AtlasLoader::getInstance()->getSpriteByName("main_cha_red@2x.png");
	red->setPosition(Point(visiableSize.width*1/5 , (visiableSize.height * 6) / 31));
	this->addChild(red);

	Sprite *purple  = AtlasLoader::getInstance()->getSpriteByName("main_cha_purple@2x.png");
	purple->setPosition(Point(visiableSize.width*6/16 , (visiableSize.height * 6) / 32));
	this->addChild(purple);

	Sprite *menuback  = AtlasLoader::getInstance()->getSpriteByName("main_menu_bg@2x.png");
	menuback->setAnchorPoint(Point::ZERO);
	menuback->setPosition(Point((visiableSize.width*3)/8 , visiableSize.height/13));
	menuback->setRotation(-6);
	this->addChild(menuback);

	Sprite *classic = AtlasLoader::getInstance()->getSpriteByName("main_menu_classic@2x.png");
	classic->setRotation(-6);
	auto menuItem  = MenuItemSprite::create(classic,NULL,CC_CALLBACK_1(WelcomeLayer::menuStartCallback, this));
	menuItem->setAnchorPoint(Point::ZERO);
	menuItem->setPosition(Point((visiableSize.width*27)/64 , (visiableSize.height * 11) / 27));

	auto menu = Menu::create(menuItem,NULL);
	menu->setAnchorPoint(Point::ZERO);
	menu->setPosition(Point(origin.x ,origin.y));
	this->addChild(menu,1);

	Sprite *land = AtlasLoader::getInstance()->getSpriteByName("main_bg_a@2x.png");
	land->setAnchorPoint(Point::ZERO);
	land->setPosition(Point::ZERO);
	this->addChild(land);

	Sprite *yellow  = AtlasLoader::getInstance()->getSpriteByName("main_cha_yellow@2x.png");
	yellow->setPosition(Point(visiableSize.width*16/18 , (visiableSize.height*1) /15));
	this->addChild(yellow);

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
