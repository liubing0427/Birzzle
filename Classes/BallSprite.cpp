#include "BallSprite.h"

BallSprite::BallSprite(){
}

BallSprite::~BallSprite(){
}

BallSprite* BallSprite::createBall(string birdName){
	BallSprite* ball = new BallSprite();
	ball->birdName = birdName;
	if (ball && ball->init())
	{
		ball->autorelease();
		return ball;
	}
	CC_SAFE_DELETE(ball);
	return nullptr;
}

bool BallSprite::init(){

	string name = String::createWithFormat("%s%s",birdName.c_str(), "_normal_00.png")->getCString();
	SpriteFrame *spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
	if(spriteFrame&&this->initWithSpriteFrame(spriteFrame))
	{
		this->setScale(0.83f);
		this->blinkAction = Repeat::create(Sequence::create(
			DelayTime::create(10.0f), 
			CallFunc::create([&](){
				Animation *animation = Animation::create();
				string blinkname = String::createWithFormat("%s%s",this->birdName.c_str(), "_normal_02.png")->getCString();
				SpriteFrame *blinkFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(blinkname);
				string name = String::createWithFormat("%s%s",this->birdName.c_str(), "_normal_00.png")->getCString();
				SpriteFrame *spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
				animation->addSpriteFrame(blinkFrame);
				animation->addSpriteFrame(spriteFrame);
				animation->setDelayPerUnit(0.1f);
				Animate* animate = Animate::create(animation);
				this->runAction(animate);
			}),
			NULL), -1);

		this->featherName = String::createWithFormat("%s%s",birdName.c_str(), "_feather.png")->getCString();
		return true;
	}
	return false;
}

bool BallSprite::changeState(State state) {
	this->state = state;
	return true;
}

string BallSprite::getName(){
	return this->birdName;
}

string BallSprite::createBallByRandom(){
	auto birdName = "";
	int type = abs(rand()%8);
	switch (type){
	case 0:
		birdName = "box00";
		break;
	case 1:
		birdName = "box01";
		break;
	case 2:
		birdName = "box02";
		break;
	case 3:
		birdName = "box03";
		break;
	case 4:
		birdName = "box04";
		break;
	case 5:
		birdName = "box05";
		break;
	case 6:
		birdName = "box06";
		break;
	case 7:
		birdName = "box07";
		break;
	default:
		birdName = "box00";
		break;
	}
	return birdName;
}

void BallSprite::blink()
{
	if(changeState(ACTION_STATE_BLINK)) {
		this->runAction(blinkAction);
	}
}

void BallSprite::MoveToAction(ActionInterval* action, bool isYChange)
{
	if(changeState(ACTION_STATE_FEATHER)){
		auto all = Sequence::create(
			action,
			ScaleTo::create(0.1f, this->getScaleX()*1.1f, this->getScaleY()*0.8f),
			ScaleTo::create(0.1f, this->getScaleX(), this->getScaleY()),
			CallFunc::create([&](){
				if (isYChange)
				{
					for (auto i=0;i<4;i++)
					{
						auto feather = Sprite::createWithSpriteFrameName(this->featherName);
						int j=1;
						int iRandPos = 30 + rand() % 20; 
						if(i%2==0)
						{
							j=-1;
						}
						feather->setPosition(Point(this->boundingBox().size.width/2 + j*iRandPos,0));
						feather->setGlobalZOrder(20);
						this->addChild(feather); 
						iRandPos = rand() % 40; 
						Point position = feather->getPosition();
						MoveTo *moveTo = MoveTo::create(3.0f, Point(position.x + j*iRandPos, - 50 - iRandPos)); 
						FadeOut *fadeout = FadeOut::create(5.0f);
						RotateBy *rotaBy1 = RotateBy::create(1, 30);  
						RotateBy *rotaBy2 = RotateBy::create(1, -30);  
						FiniteTimeAction *seq2 = Sequence::create(rotaBy1, rotaBy2, NULL);
						Repeat *baidong = Repeat::create(seq2, 6);
						auto featherAction = Spawn::create(moveTo, baidong, fadeout, NULL);

						auto all = Sequence::create(
							featherAction,
							CallFunc::create(CC_CALLBACK_0(BallSprite::removeFeather,this, feather)), NULL);
						feather->runAction(all); 
					}
				}
			}), NULL);
		this->runAction(all);
	}
}

void BallSprite::removeFeather(Node* sender)
{
	auto feather = (Sprite*)sender;
	this->removeChild(feather,true);
}

void BallSprite::clearFeather()
{
	this->removeAllChildren();
}

Address BallSprite::getAddress()
{
	return this->address;
}

void BallSprite::setAddress(int row, int column)
{
	this->address.row = row;
	this->address.column = column;
}