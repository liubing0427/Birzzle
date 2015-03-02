#include "GameLayer.h"

GameLayer::GameLayer(){}

GameLayer::~GameLayer(){}

bool GameLayer::init(){
	if (!Layer::init()) {
		return false;
	}

	auto visiableSize = Director::getInstance()->getVisibleSize();
	auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

	Sprite *tree = Sprite::create("stage_tree@2x.png");
	tree->setPosition(Point(visiableSize.width/2 , (visiableSize.height*7)/16));
	this->addChild(tree);

	Sprite *grass = Sprite::create("stage_tree_grass_r@2x.png");
	grass->setAnchorPoint(Point::ZERO);
	grass->setPosition(Point(visiableSize.width/2-grass->getBoundingBox().size.width/2, 0));
	grass->setGlobalZOrder(999);
	this->addChild(grass);

	/*Sprite* edgeSpace=Sprite::create();
	PhysicsBody* boundBody=PhysicsBody::createEdgeBox(tree->getBoundingBox().size*0.9f,PHYSICSBODY_MATERIAL_DEFAULT,3);

	edgeSpace->setPhysicsBody(boundBody);
	edgeSpace->setPosition(Point(visiableSize.width/2 , (visiableSize.height*7)/16));
	this->addChild(edgeSpace);*/

	this->initBall(tree);
	this->registerTouchBall();

	return true;
}


void GameLayer::initBall(Sprite *tree)
{
	//lambda表达式
	auto randProduceBall = [](string ballname){
		string name = BallSprite::createBallByRandom();
		while (name==ballname)
		{
			name = BallSprite::createBallByRandom();
		}
		return name;
	};

	//初始化小球布局算法
	for (auto i=0; i<9; i++) {
		for (auto j=0; j<7; j++) {
			if(i<BIRD_INIT_ROW)
			{
				string ballname = BallSprite::createBallByRandom();
				if (j>=2 && i<2){
					if(ballname==m_arrBall[i][j-1]->getName()&&ballname==m_arrBall[i][j-2]->getName()){
						ballname = randProduceBall(ballname);//重新生成
					}
				} else if (j<2 && i>=2){
					if(ballname==m_arrBall[i-1][j]->getName() && ballname==m_arrBall[i-2][j]->getName()){
						ballname = randProduceBall(ballname);//重新生成
					}
				}else if (j>=2 && i>=2){
					while((ballname==m_arrBall[i][j-1]->getName() && ballname==m_arrBall[i][j-2]->getName()) ||
						(ballname==m_arrBall[i-1][j]->getName() && ballname==m_arrBall[i-2][j]->getName()))
					{
						ballname = randProduceBall(ballname);//重新生成
					}
				}
				BallSprite* temp_ball = BallSprite::createBall(ballname);
				m_arrBall[i][j]=temp_ball;
			}
			else
			{
				m_arrBall[i][j] = nullptr;
			}
		}
	}

	//将小球显示出来
	for (auto i=0; i<BIRD_INIT_ROW; i++) {
		for (auto j=0; j<7; j++) {
			auto ballBoundingBoxSize=m_arrBall[i][j]->getContentSize();
			auto position=Point(j*ballBoundingBoxSize.width*BIRD_WIDTH/120 + X_SKEWING,i*ballBoundingBoxSize.height*BIRD_WIDTH/120 + Y_SKEWING);
			m_arrBall[i][j]->setAnchorPoint(Point::ZERO);
			m_arrBall[i][j]->setPosition(position);
			m_arrBall[i][j]->blink();
			m_arrBall[i][j]->setGlobalZOrder(8-i);
			/*PhysicsBody *body = PhysicsBody::create();
			auto visiableSize = Director::getInstance()->getVisibleSize();
			auto bodyPosition = Point(ballBoundingBoxSize.width*70/240+(visiableSize.width-tree->getBoundingBox().size.width)/2-15*j,ballBoundingBoxSize.height*70/240+((visiableSize.height*7)/16-tree->getBoundingBox().size.height/2)-15*i);
			body->addShape(PhysicsShapeBox::create(Size(ballBoundingBoxSize.width*60/120,ballBoundingBoxSize.height*60/120),PHYSICSBODY_MATERIAL_DEFAULT,bodyPosition));
			body->setDynamic(true);
			body->setLinearDamping(0.0f);
			body->setGravityEnable(false);
			m_arrBall[i][j]->setPhysicsBody(body);*/

			tree->addChild(m_arrBall[i][j],0);
			m_arrBall[i][j]->setAddress(i,j);
		}
	}
}

double GameLayer::round(double number)
{
	return (number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5));
}

void GameLayer::registerTouchBall()
{
	m_listener1 = EventListenerTouchOneByOne::create();  //注册单点触摸
	m_listener1->setSwallowTouches(true);
	m_listener1->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	m_listener1->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
	m_listener1->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
	//将小球增加到监听器
	for (auto i=0; i<BIRD_INIT_ROW; i++) {
		for (auto j=0; j<7; j++) {
			if (i==0&&j==0) {
				_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener1, m_arrBall[i][j]);
			} else {
				_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener1->clone(), m_arrBall[i][j]);
			}
		}
	}
}

bool GameLayer::onTouchBegan(Touch* touch, Event* event)
{
	auto target = static_cast<BallSprite*>(event->getCurrentTarget());
		target->clearFeather();
		//target->getPhysicsBody()->setDynamic(true);
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		if (rect.containsPoint(locationInNode))
		{
			log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
			/*target->setZOrder(1);
			target->setOpacity(180);*/
			return true;
		}
		return false;
}

void GameLayer::onTouchMoved(Touch* touch, Event* event)
{
	auto target = static_cast<BallSprite*>(event->getCurrentTarget());
	auto delta = touch->getDelta();
	auto ballTouchCurrentPosition=target->getPosition();
	auto address=target->getAddress();
	auto ballBoundingBoxSize=target->getContentSize();
	auto x = (ballTouchCurrentPosition.x-X_SKEWING)*120/(BIRD_WIDTH*ballBoundingBoxSize.width);
	auto y = (ballTouchCurrentPosition.y-Y_SKEWING)*120/(BIRD_WIDTH*ballBoundingBoxSize.height);
	//floor向下 ceil向上
	auto column = (int)round(x);
	auto row = (int)round(y);
	column = column<0?0:(column>6?6:column);
	row = row<0?0:(row>8?8:row);
	m_arrBall[address.row][address.column] = nullptr;
	auto position = Point((column)*ballBoundingBoxSize.width*BIRD_WIDTH/120 + X_SKEWING,row*ballBoundingBoxSize.height*BIRD_WIDTH/120 + Y_SKEWING);
	//x->column y->row
	if(delta.x>0 && delta.y>0) //↗
	{
		if(column+1>=7 || row+1>=9 || m_arrBall[row+1][column+1] != nullptr)
		{
			target->setPosition(position);
			return;
		}
	}
	else if(delta.x==0&&delta.y>0) //↑
	{
		if(row+1>=9 || m_arrBall[row+1][column] != nullptr)
		{
			target->setPosition(position);
			return;
		}
	}
	else if(delta.x<0&&delta.y>0) //↖
	{
		if(column-1<0 || row+1>=9 || m_arrBall[row+1][column-1] != nullptr)
		{
			target->setPosition(position);
			return;
		}
	}
	else if(delta.x<0&&delta.y==0) //←
	{
		if(column-1<0 || m_arrBall[row][column-1] != nullptr)
		{
			target->setPosition(position);
			return;
		}
	}
	else if(delta.x<0&&delta.y<0) //↙
	{
		if(column-1<0 || row-1<0 || m_arrBall[row-1][column-1] != nullptr)
		{
			target->setPosition(position);
			return;
		}
	}
	else if(delta.x==0&&delta.y<0) //↓
	{
		if(row-1<0 || m_arrBall[row-1][column] != nullptr)
		{
			target->setPosition(position);
			return;
		}
	}
	else if(delta.x>0&&delta.y<0) //↘
	{
		if(column+1>=7 || row-1<0 || m_arrBall[row-1][column+1] != nullptr)
		{
			target->setPosition(position);
			return;
		}
	}
	else if(delta.x>0&&delta.y==0) //→
	{
		if(column+1>=7 || m_arrBall[row][column+1] != nullptr)
		{
			target->setPosition(position);
			return;
		}
	}
	target->setPosition(target->getPosition() + touch->getDelta());
	//this->setBallExchange(target);
}

void GameLayer::onTouchEnded(Touch* touch, Event* event)
{
	auto target = static_cast<BallSprite*>(event->getCurrentTarget());
	//target->getPhysicsBody()->setDynamic(false);
	log("sprite onTouchesEnded.. ");
	/*target->setZOrder(0);
	target->setOpacity(255);*/

	auto ballTouchCurrentPosition=target->getPosition();
	auto address=target->getAddress();
	auto ballBoundingBoxSize=target->getContentSize();
	auto column = (int)round((ballTouchCurrentPosition.x-X_SKEWING)*120/(BIRD_WIDTH*ballBoundingBoxSize.width));
	auto row = (int)round((ballTouchCurrentPosition.y-Y_SKEWING)*120/(BIRD_WIDTH*ballBoundingBoxSize.height));
	target->setPosition(Point(column*ballBoundingBoxSize.width*BIRD_WIDTH/120 + X_SKEWING,row*ballBoundingBoxSize.height*BIRD_WIDTH/120 + Y_SKEWING));
	if(row==address.row&&column==address.column)
	{
		m_arrBall[address.row][address.column] = target;
		return;
	}
	for (auto i=row;i>=-1;i--)
	{
		if(i==-1 || m_arrBall[i][column]!=nullptr)
		{
			float speed = (row-(i+1))/4;
			target->setGlobalZOrder(8-(i+1));
			target->setAddress(i+1, column);
			m_arrBall[i+1][column]=target;
			target->MoveToAction(MoveTo::create(speed, Point(column*ballBoundingBoxSize.width*BIRD_WIDTH/120 + X_SKEWING,(i+1)*ballBoundingBoxSize.height*BIRD_WIDTH/120 + Y_SKEWING)), 
			[&](Node* ball)
			{
				auto bird = (BallSprite*)ball;
				checkThreeAndAboveSameBall(bird);
			}
			,row!=(i+1));

			auto j=1;
			while(address.row + j<9&&m_arrBall[address.row + j][address.column]!=nullptr&&m_arrBall[address.row + j][address.column]->getActionState()!=ACTION_STATE_SHAKE)
			{
				auto p = Point(address.column*ballBoundingBoxSize.width*BIRD_WIDTH/120 + X_SKEWING,(address.row + j-1)*ballBoundingBoxSize.height*BIRD_WIDTH/120 + Y_SKEWING);

				m_arrBall[address.row + j][address.column]->MoveToAction(MoveTo::create(1/4.0f, p), 
					[&](Node* ball){
						auto bird = (BallSprite*)ball;
						checkThreeAndAboveSameBall(bird);
				}
				, true);

				m_arrBall[address.row + j][address.column]->setAddress(address.row + j-1, address.column);
				if(m_arrBall[address.row + j][address.column]->getSkillState()!=SKILL_STATE_NORMAL)
				{
					m_arrBall[address.row + j][address.column]->setGlobalZOrder(19);
				}
				else
				{
					m_arrBall[address.row + j][address.column]->setGlobalZOrder(8-(address.row + j-1));
				}
				m_arrBall[address.row + j-1][address.column] = m_arrBall[address.row + j][address.column];
				m_arrBall[address.row + j][address.column] = nullptr;
				j++;
			}

			break;
		}
	}
}

void GameLayer::checkAbove(Address address, Size ballBoundingBoxSize){

}

void GameLayer::checkThreeAndAboveSameBall(BallSprite* sprite)
{
	queue<BallSprite*> Q;
	vector<BallSprite*> sameBall;
	sprite->setVisited(true);
	Q.push(sprite);
	sameBall.push_back(sprite);
	while(!Q.empty()) 
	{
		int i,j;
		auto ball=Q.front();         //取队首顶点
		auto address = ball->getAddress();
		auto name = ball->getName();
		Q.pop();
		if(address.row<8 && m_arrBall[address.row+1][address.column]&&m_arrBall[address.row+1][address.column]->getVisited()==false
			&&m_arrBall[address.row+1][address.column]->getName() == name)//↑
		{
			m_arrBall[address.row+1][address.column]->setVisited(true);
			Q.push(m_arrBall[address.row+1][address.column]);
			sameBall.push_back(m_arrBall[address.row+1][address.column]);
		}
		if(address.column<6 && m_arrBall[address.row][address.column+1]&&m_arrBall[address.row][address.column+1]->getVisited()==false
			&&m_arrBall[address.row][address.column+1]->getName() == name)//→
		{
			m_arrBall[address.row][address.column+1]->setVisited(true);
			Q.push(m_arrBall[address.row][address.column+1]);
			sameBall.push_back(m_arrBall[address.row][address.column+1]);
		}
		if(address.row>0 && m_arrBall[address.row-1][address.column]&&m_arrBall[address.row-1][address.column]->getVisited()==false
			&&m_arrBall[address.row-1][address.column]->getName() == name)//↓
		{
			m_arrBall[address.row-1][address.column]->setVisited(true);
			Q.push(m_arrBall[address.row-1][address.column]);
			sameBall.push_back(m_arrBall[address.row-1][address.column]);
		}
		if(address.column>0 && m_arrBall[address.row][address.column-1]&&m_arrBall[address.row][address.column-1]->getVisited()==false
			&&m_arrBall[address.row][address.column-1]->getName() == name)//←
		{
			m_arrBall[address.row][address.column-1]->setVisited(true);
			Q.push(m_arrBall[address.row][address.column-1]);
			sameBall.push_back(m_arrBall[address.row][address.column-1]);
		}
	}
	for (auto& bird : sameBall)
	{
		bird->setVisited(false);
	}
	if(sameBall.size()<3)
	{
		sameBall.clear();
	}
	else
	{
		auto removeBirdFunc = [&](Node* ball){
			auto bd = (BallSprite*)ball;
			auto arr=bd->getAddress();
			switch (bd->getSkillState())
			{
			case SKILL_STATE_NORMAL:
				break;
			case SKILL_STATE_BOMB:
				if(arr.column>0&&m_arrBall[arr.row][arr.column-1]!=nullptr){
					m_arrBall[arr.row][arr.column+1]->setVisible(false);
					m_arrBall[arr.row][arr.column-1]->feather();
					m_arrBall[arr.row][arr.column-1]=nullptr;
				}
				if(arr.column<6&&m_arrBall[arr.row][arr.column+1]!=nullptr){
					m_arrBall[arr.row][arr.column+1]->setVisible(false);
					m_arrBall[arr.row][arr.column+1]->feather();
					m_arrBall[arr.row][arr.column+1]=nullptr;
				}
				break;
			case SKILL_STATE_FIRE:
				break;
			case SKILL_STATE_LIGHTNING:
				break;
			case SKILL_STATE_BLACKHOLE:
				break;
			default:
				break;
			}
			bd->setVisible(false);
			bd->feather();
			m_arrBall[arr.row][arr.column]=nullptr;
		};
		auto checkFunc = [&](Node* ball){
			auto bd = (BallSprite*)ball;
			auto ballBoundingBoxSize=bd->getContentSize();
			for (auto i=8; i>0; i--) {
				for (auto j=0; j<7; j++) {
					if(m_arrBall[i-1][j]==nullptr){
						auto k = 0;
						while (m_arrBall[i+k][j]!=nullptr)
						{
							auto p = Point(j*ballBoundingBoxSize.width*BIRD_WIDTH/120 + X_SKEWING,(i+k-1)*ballBoundingBoxSize.height*BIRD_WIDTH/120 + Y_SKEWING);

							m_arrBall[i+k][j]->setPosition(p);
							m_arrBall[i+k][j]->setAddress(i+k-1, j);
							if(m_arrBall[i+k][j]->getSkillState()!=SKILL_STATE_NORMAL)
							{
								m_arrBall[i+k][j]->setGlobalZOrder(19);
							}
							else
							{
								m_arrBall[i+k][j]->setGlobalZOrder(8-(i+k));
							}
							m_arrBall[i+k-1][j] = m_arrBall[i+k][j];
							k++;
						}
						m_arrBall[i+k-1][j] = nullptr;
					}
				}
			}
		};
		auto count = sameBall.size();
		auto k=0;
		for(auto& bird : sameBall){
			k++;
			if(k==1&&count > 3){
				bird->isLast = false;
				switch (count){
				case 4:
					if(bird->getSkillState() == SKILL_STATE_NORMAL){
						bird->changeTo(SKILL_STATE_BOMB);
					}
					break;
				case 5:
					if(bird->getSkillState() == SKILL_STATE_NORMAL){
						bird->changeTo(SKILL_STATE_FIRE);
					}
					break;
				case 6:
					if(bird->getSkillState() == SKILL_STATE_NORMAL){
						bird->changeTo(SKILL_STATE_LIGHTNING);
					}
					break;
				case 7:
				default:
					if(bird->getSkillState() == SKILL_STATE_NORMAL){
						bird->changeTo(SKILL_STATE_BLACKHOLE);
					}
					break;
				}
			}
			else{
				bird->isLast = false;
				if(k==count)
				{
					bird->isLast = true;
				}
				bird->remove(removeBirdFunc, checkFunc);
			}
		}

	}
}

void GameLayer::produceNewBallFill()
{
	
}

void GameLayer::onExit()
{
	_eventDispatcher->removeEventListener(m_listener1);

	Layer::onExit();
}
