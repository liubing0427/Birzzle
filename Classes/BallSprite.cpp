#include "BallSprite.h"
#include "GameLayer.h"

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
		//this->setScale(0.80f);
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
		CCParticleSystemQuad *_emitter = CCParticleSystemQuad::create("Explosion.plist");
		this->getParent()->addChild(_emitter, 10);
		_emitter->setAutoRemoveOnFinish(true);
		_emitter->setEmissionRate(100.0f);
		_emitter->setPosition(Point(this->getPosition().x + this->boundingBox().size.width/2,this->getPosition().y+this->boundingBox().size.height/2));
	}
}

void BallSprite::bone(){
	SpriteFrame *spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("bone.png");
	this->setSpriteFrame(spriteFrame);
}

void BallSprite::burn(){
	Animation *animation = Animation::create();
	string burnname = String::createWithFormat("%s%s",this->birdName.c_str(), "_burn.png")->getCString();
	SpriteFrame *burnFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(burnname);
	string cloud = String::createWithFormat("%s%d%s","cloud_burn_0",rand()%4,".png")->getCString();
	SpriteFrame *cloudFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(cloud);
	animation->addSpriteFrame(burnFrame);
	animation->addSpriteFrame(cloudFrame);
	animation->setDelayPerUnit(0.05f);
	Animate* animate = Animate::create(animation);
	this->runAction(animate);
}

void BallSprite::fire(){
	if(changeActionState(ACTION_STATE_FIRE)){
		this->removeAllChildren();
		Vector<FiniteTimeAction*> list;
		for (auto i=0;i<12;i++){
			list.pushBack(CallFunc::create([&,i](){
				char str[50] = {0};
				if(i<10){
					sprintf(str, "firebird_00%d.png", i);
				}
				else{
					sprintf(str, "firebird_0%d.png", i);
				}
				auto fire = Sprite::createWithSpriteFrameName(str);
				fire->setPosition(this->boundingBox().size.width/2, this->boundingBox().size.height/2);
				fire->setGlobalZOrder(21);
				this->addChild(fire);
			}));
			list.pushBack(DelayTime::create(0.1f));
		}
		auto action = RepeatForever::create(Sequence::create(list));
		action->setTag(ACTION_STATE_FIRE);
		this->runAction(action);
	}
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

void BallSprite::remove(bool isshake, SkillState skill){
	if(this->actionState == ACTION_STATE_SHAKE || this->skillState != SKILL_STATE_NORMAL){
		this->stopActionByTag(ACTION_STATE_SHAKE);
		this->stopActionByTag(ACTION_STATE_SKILL);
	}

	if(changeActionState(ACTION_STATE_SHAKE)){
		Vector<FiniteTimeAction*> list;
		if(isshake){
			list.pushBack(Shake::create(SHAKE_TIME, 3));
		}
		double delay = 0;
		auto speed = 0.1f;
		auto count = 0;
		switch (this->skillState){
		case SKILL_STATE_NORMAL:
			if(this->isLast && skill!=SKILL_STATE_NORMAL){
				this->changeTo(skill);
			}
			else{
				list.pushBack(CallFunc::create([&](){
					this->feather();
					GameLayer::getInstance()->removeFromArray(this->address.row, this->address.column);
				}));
			}
			break;
		case SKILL_STATE_BOMB:
			list.pushBack(CallFunc::create([&](){
				this->feather();
				this->star();
				if(this->address.column>0&&this->address.row>0&&GameLayer::getInstance()->getBall(this->address.row-1,this->address.column-1)!=nullptr){//¨L
					GameLayer::getInstance()->getBall(this->address.row-1,this->address.column-1)->remove(false, SKILL_STATE_NORMAL);
				}
				if(this->address.column>0&&GameLayer::getInstance()->getBall(this->address.row,this->address.column-1)!=nullptr){//¡û
					GameLayer::getInstance()->getBall(this->address.row,this->address.column-1)->remove(false, SKILL_STATE_NORMAL);
				}
				if(this->address.column<6&&this->address.row>0&&GameLayer::getInstance()->getBall(this->address.row-1,this->address.column+1)!=nullptr){//¨K
					GameLayer::getInstance()->getBall(this->address.row-1,this->address.column+1)->remove(false, SKILL_STATE_NORMAL);
				}
				if(this->address.column<6&&this->address.row<GAME_ROW-1&&GameLayer::getInstance()->getBall(this->address.row+1,this->address.column+1)!=nullptr){//¨J
					GameLayer::getInstance()->getBall(this->address.row+1,this->address.column+1)->remove(false, SKILL_STATE_NORMAL);
				}
				if(this->address.column<6&&GameLayer::getInstance()->getBall(this->address.row,this->address.column+1)!=nullptr){//¡ú
					GameLayer::getInstance()->getBall(this->address.row,this->address.column+1)->remove(false, SKILL_STATE_NORMAL);
				}
				if(this->address.column>0&&this->address.row<GAME_ROW-1&&GameLayer::getInstance()->getBall(this->address.row+1,this->address.column-1)!=nullptr){//¨I
					GameLayer::getInstance()->getBall(this->address.row+1,this->address.column-1)->remove(false, SKILL_STATE_NORMAL);
				}
				GameLayer::getInstance()->removeFromArray(this->address.row, this->address.column);
			}));
			break;
		case SKILL_STATE_FIRE:
			list.pushBack(CallFunc::create([&](){//×Å»ð
				this->fire();
				count = this->address.row;
				while (count>0)
				{
					count--;
					GameLayer::getInstance()->removeFromArray(count, this->address.column);
				}
			}));
			speed += this->address.row/4.0f;
			list.pushBack(MoveTo::create(this->address.row/4.0f,Point(this->address.column*BIRD_WIDTH + X_SKEWING, 0 + Y_SKEWING)));//ÏÂÂä
			list.pushBack(CallFunc::create([&](){//Ã°ÑÌ
				if(this->address.column>0&&GameLayer::getInstance()->getBall(0, this->address.column-1)!=nullptr){//¡û
					GameLayer::getInstance()->getBall(0, this->address.column-1)->burn();
				}
				if(this->address.column>0&&GameLayer::getInstance()->getBall(1, this->address.column-1)!=nullptr){//¨I
					GameLayer::getInstance()->getBall(1, this->address.column-1)->burn();
				}
				if(this->address.column<6&&GameLayer::getInstance()->getBall(0, this->address.column+1)!=nullptr){//¡ú
					GameLayer::getInstance()->getBall(0, this->address.column+1)->burn();
				}
				if(this->address.column<6&&GameLayer::getInstance()->getBall(1, this->address.column+1)!=nullptr){//¨J
					GameLayer::getInstance()->getBall(1, this->address.column+1)->burn();
				}
			}));
			speed += 0.1f;
			list.pushBack(DelayTime::create(0.1f));
			list.pushBack(CallFunc::create([&](){//É¾³ý
				if(this->address.column>0&&GameLayer::getInstance()->getBall(0, this->address.column-1)!=nullptr){//¡û
					GameLayer::getInstance()->removeFromArray(0, this->address.column-1);
				}
				if(this->address.column>0&&GameLayer::getInstance()->getBall(1, this->address.column-1)!=nullptr){//¨I
					GameLayer::getInstance()->removeFromArray(1, this->address.column-1);
				}
				if(this->address.column<6&&GameLayer::getInstance()->getBall(0, this->address.column+1)!=nullptr){//¡ú
					GameLayer::getInstance()->removeFromArray(0, this->address.column+1);
				}
				if(this->address.column<6&&GameLayer::getInstance()->getBall(1, this->address.column+1)!=nullptr){//¨J
					GameLayer::getInstance()->removeFromArray(1, this->address.column+1);
				}
				GameLayer::getInstance()->removeFromArray(this->address.row, this->address.column);
			}));
			break;
		case SKILL_STATE_LIGHTNING:
			list.pushBack(CallFunc::create([&](){
				auto vertical = Sprite::create("lightning_col@2x.png");
				auto horizontal = Sprite::create("lightning_row@2x.png");
				vertical->setAnchorPoint(Point::ZERO);
				vertical->setPosition(this->address.column*BIRD_WIDTH + X_SKEWING,0*BIRD_WIDTH + Y_SKEWING);
				horizontal->setAnchorPoint(Point::ZERO);
				horizontal->setPosition(0*BIRD_WIDTH + X_SKEWING,this->address.row*BIRD_WIDTH + Y_SKEWING);
				vertical->setTag(998);
				horizontal->setTag(999);
				this->getParent()->addChild(vertical);
				this->getParent()->addChild(horizontal);

				for (auto i=0;i<7;i++){
					if(GameLayer::getInstance()->getBall(i, this->address.column)!=nullptr){
						GameLayer::getInstance()->getBall(i, this->address.column)->bone();
					}
				}
				for (auto j=0;j<GAME_ROW;j++){
					if(GameLayer::getInstance()->getBall(this->address.row, j)!=nullptr){
						GameLayer::getInstance()->getBall(this->address.row, j)->bone();
					}
				}
			}));
			list.pushBack(DelayTime::create(0.05f));
			list.pushBack(CallFunc::create([&](){
				for (auto i=0;i<7;i++){
					if(GameLayer::getInstance()->getBall(i, this->address.column)!=nullptr){
						GameLayer::getInstance()->removeFromArray(i, this->address.column);
					}
				}
				for (auto j=0;j<GAME_ROW;j++){
					if(GameLayer::getInstance()->getBall(this->address.row, j)!=nullptr){
						GameLayer::getInstance()->removeFromArray(this->address.row, j);
					}
				}
				this->getParent()->removeChildByTag(998);
				this->getParent()->removeChildByTag(999);
				this->feather();
				GameLayer::getInstance()->removeFromArray(this->address.row, this->address.column);
			}));
			break;
		case SKILL_STATE_BLACKHOLE:
			break;
		default:
			this->feather();
			GameLayer::getInstance()->removeFromArray(this->address.row, this->address.column);
			break;
		}
		if(this->isLast){
			list.pushBack(DelayTime::create(speed));
			list.pushBack(CallFunc::create([&](){
				for (auto i=GAME_ROW-1; i>0; i--) {
					for (auto j=0; j<7; j++) {
						if(GameLayer::getInstance()->getBall(i-1, j)==nullptr){
							auto k = 0;
							while (i+k<9&&GameLayer::getInstance()->getBall(i+k, j)!=nullptr)
							{
								GameLayer::getInstance()->swithBall(i+k,j, i+k-1,j);
								k++;
							}
						}
					}
				}
			}));
		}
		Sequence* action = Sequence::create(list);
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
				//skill->setScale(1.1f);
				skill->setPosition(this->boundingBox().size.width/2, this->boundingBox().size.height/2);
				skill->setGlobalZOrder(21);
				this->setGlobalZOrder(19);
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