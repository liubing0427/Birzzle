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
		this->setScale(0.80f);
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
			nullptr), -1);

		this->featherName = String::createWithFormat("%s%s",birdName.c_str(), "_feather.png")->getCString();
		this->isVisited = false;
		this->skillState = SKILL_STATE_NORMAL;
		return true;
	}
	return false;
}

bool BallSprite::changeActionState(ActionState state) {
	this->actionState = state;
	return true;
}

ActionState BallSprite::getActionState(){
	return this->actionState;
}

bool BallSprite::changeSkillState(SkillState state) {
	this->skillState = state;
	return true;
}

SkillState BallSprite::getSkillState(){
	return this->skillState;
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

void BallSprite::blink(){
	if(changeActionState(ACTION_STATE_BLINK)) {
		blinkAction->setTag(ACTION_STATE_BLINK);
		this->runAction(blinkAction);
	}
}

void BallSprite::MoveToAction(ActionInterval* action, const std::function<void(Node*)> &func, bool isYChange){
	if(changeActionState(ACTION_STATE_MOVE)){
		auto all = Sequence::create(
			action,
			ScaleTo::create(0.1f, this->getScaleX()*1.1f, this->getScaleY()*0.8f),
			ScaleTo::create(0.1f, this->getScaleX(), this->getScaleY()),
			CallFunc::create([&,isYChange](){
				if (isYChange)
				{
					this->feather();
				}
			}),
			CallFuncN::create(func), 
			nullptr);
		all->setTag(ACTION_STATE_MOVE); 
		this->runAction(all);
	}
}

void BallSprite::feather(){
	if(changeActionState(ACTION_STATE_FEATHER)){
		for (auto i=0;i<4;i++)
		{
			auto feather = Sprite::createWithSpriteFrameName(this->featherName);
			int j=1;
			int iRandPos = 30 + rand() % 20; 
			if(i%2==0)
			{
				j=-1;
			}
			feather->setPosition(Point(this->getPosition().x+this->boundingBox().size.width/2 + j*iRandPos,this->getPosition().y));
			feather->setGlobalZOrder(20);
			this->getParent()->addChild(feather); 
			feather->setTag(ACTION_STATE_FEATHER);
			iRandPos = rand() % 40; 
			Point position = feather->getPosition();
			MoveTo *moveTo = MoveTo::create(3.0f, Point(position.x + j*iRandPos, position.y- 50 - iRandPos)); 
			FadeOut *fadeout = FadeOut::create(5.0f);
			RotateBy *rotaBy1 = RotateBy::create(1, 30);  
			RotateBy *rotaBy2 = RotateBy::create(1, -30);  
			FiniteTimeAction *seq2 = Sequence::create(rotaBy1, rotaBy2, nullptr);
			Repeat *baidong = Repeat::create(seq2, 6);
			auto featherAction = Spawn::create(moveTo, baidong, fadeout, nullptr);

			auto all = Sequence::create(
				featherAction,
				CallFuncN::create([&](Node* sender){
					auto sp = (Sprite*)sender;
					this->getParent()->removeChild(sp,true);
			}), nullptr);
			feather->runAction(all); 
		}
	}
}

void BallSprite::clearFeather(){
	this->getParent()->removeChildByTag(ACTION_STATE_FEATHER);
}

void BallSprite::star(){
	if(changeActionState(ACTIOn_STATE_STAR)){
		string name = String::createWithFormat("%s%d%s", "bomb_star_", (rand() % 3)+1,"@2x.png")->getCString();
		auto star = Sprite::create(name);

		int iRandPos = -(rand() % 51) + 25;
		star->setPosition(Point(this->getPosition().x+iRandPos,this->getPosition().y));
		star->setGlobalZOrder(21);
		this->getParent()->addChild(star); 
		star->setTag(ACTIOn_STATE_STAR);
		auto all = Sequence::create(
			DelayTime::create(0.2f),
			CallFuncN::create([&](Node* sender){
				auto sp = (Sprite*)sender;
				this->getParent()->removeChild(sp,true);
		}), nullptr);
		star->runAction(all); 
	}
}

void BallSprite::bone(){
	SpriteFrame *spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("bone.png");
	this->setSpriteFrame(spriteFrame);
}

Address BallSprite::getAddress(){
	return this->address;
}

void BallSprite::setAddress(int row, int column){
	this->address.row = row;
	this->address.column = column;
}

bool BallSprite::getVisited(){
	return this->isVisited;
}

void BallSprite::setVisited(bool isVisited){
	this->isVisited = isVisited;
}

void BallSprite::remove(const std::function<void(Node*)> &func, const std::function<void(Node*)> &checkFunc){
	if(this->actionState == ACTION_STATE_SHAKE || this->skillState != SKILL_STATE_NORMAL){
		this->stopActionByTag(ACTION_STATE_SHAKE);
		this->stopActionByTag(ACTION_STATE_SKILL);
	}

	if(changeActionState(ACTION_STATE_SHAKE)){
		Sequence* action;
		if(this->isLast)
		{
			action = Sequence::create(Shake::create(SHAKE_TIME, 3), 
				CallFuncN::create(func),
				DelayTime::create(0.1f),
				CallFuncN::create(checkFunc),
				nullptr);
		}
		else
		{
			action = Sequence::create(Shake::create(SHAKE_TIME, 3), 
				CallFuncN::create(func), nullptr);
		}
		action->setTag(ACTION_STATE_SHAKE);
		this->runAction(action);
	}
}

void BallSprite::changeTo(SkillState skillState){
	if(this->actionState == ACTION_STATE_SHAKE || this->skillState != SKILL_STATE_NORMAL){
		this->stopActionByTag(ACTION_STATE_SHAKE);
		this->stopActionByTag(ACTION_STATE_SKILL);
	}
	if(changeActionState(ACTION_STATE_SHAKE)){
		this->removeAllChildren();
		auto action = Sequence::create(
			Shake::create(SHAKE_TIME, 3),
			CallFuncN::create(CC_CALLBACK_0(BallSprite::changeToCallBack, this, skillState)), 
			nullptr);
		action->setTag(ACTION_STATE_SHAKE);
		this->runAction(action);
	}
}

void BallSprite::changeToCallBack(SkillState skillState){
	if(changeActionState(ACTION_STATE_BLINK) && changeSkillState(skillState)){
		Vector<FiniteTimeAction*> list;
		for (auto i=0;i<12;i++){
			list.pushBack(CallFunc::create([&,i,skillState](){
				char str[50] = {0};
				string name;
				switch (skillState){
				case SKILL_STATE_BOMB:
					name = "itemBomb";
					break;
				case SKILL_STATE_FIRE:
					name = "itemFirebird";
					break;
				case SKILL_STATE_LIGHTNING:
					name = "itemLightning";
					break;
				case SKILL_STATE_BLACKHOLE:
					name = "itemBlackhole";
					break;
				default:
					name = "itemBomb";
					break;
				}
				if(i<10){
					sprintf(str, "%s_00%d.png", name.c_str(), i);
				}
				else{
					sprintf(str, "%s_0%d.png", name.c_str(), i);
				}
				auto skill = Sprite::createWithSpriteFrameName(str);
				skill->setScale(1.1f);
				skill->setPosition(this->boundingBox().size.width/2+10, this->boundingBox().size.height/2+10);
				skill->setGlobalZOrder(21);
				this->addChild(skill);
			}));
			list.pushBack(DelayTime::create(0.2f));
			list.pushBack(CallFunc::create([&](){
				this->removeAllChildren();
			}));
		}
		auto action = RepeatForever::create(Sequence::create(list));
		action->setTag(ACTION_STATE_SKILL);
		this->runAction(action);
	}
}