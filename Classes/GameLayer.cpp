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
	tree->setScale(0.8f);
	tree->setPosition(Point(visiableSize.width/2 , (visiableSize.height*7)/16));
	this->addChild(tree);

	Sprite *grass = Sprite::create("stage_tree_grass_r@2x.png");
	grass->setScale(0.8f);
	grass->setAnchorPoint(Point::ZERO);
	grass->setPosition(Point(visiableSize.width/2-grass->getBoundingBox().size.width/2, 0));
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
			if(i<3)
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
				m_arrBall[i][j]=temp_ball;}
			else
			{
				m_arrBall[i][j] = NULL;
			}
		}
	}

	//将小球显示出来
	for (auto i=2; i>=0; i--) {
		for (auto j=0; j<7; j++) {
			auto ballBoundingBoxSize=m_arrBall[i][j]->getContentSize();
			auto position=Point(j*ballBoundingBoxSize.width*75/120 + 20,i*ballBoundingBoxSize.height*75/120 + 30);
			m_arrBall[i][j]->setAnchorPoint(Point::ZERO);
			m_arrBall[i][j]->setPosition(position);
			m_arrBall[i][j]->blink();
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

void GameLayer::registerTouchBall()
{
	auto round = [](double number)
	{
		return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
	};

	m_listener1 = EventListenerTouchOneByOne::create();  //注册单点触摸
	m_listener1->setSwallowTouches(true);

	m_listener1->onTouchBegan = [](Touch* touch, Event* event){
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
	};

	m_listener1->onTouchMoved = [=](Touch* touch, Event* event){
		auto target = static_cast<BallSprite*>(event->getCurrentTarget());
		auto delta = touch->getDelta();
		auto ballTouchCurrentPosition=target->getPosition();
		auto adress=target->getAddress();
		auto ballBoundingBoxSize=target->getContentSize();
		auto x = (ballTouchCurrentPosition.x-20)*120/(70*ballBoundingBoxSize.width);
		auto y = (ballTouchCurrentPosition.y-30)*120/(70*ballBoundingBoxSize.height);
		//floor向下 ceil向上
		auto column = (int)round(x);
		auto row = (int)round(y);
		auto position = Point(column*ballBoundingBoxSize.width*75/120 + 20,row*ballBoundingBoxSize.height*75/120 + 30);
		//x->column y->row
		if(delta.x>0 && delta.y>0) //↗
		{
			if(column+1>=7 || row+1>=9 || m_arrBall[row+1][column+1] != NULL)
			{
				target->setPosition(position);
				return;
			}
		}
		else if(delta.x==0&&delta.y>0) //↑
		{
			if(row+1>=9 || m_arrBall[row+1][column] != NULL)
			{
				target->setPosition(position);
				return;
			}
		}
		else if(delta.x<0&&delta.y>0) //↖
		{
			if(column-1<0 || row+1>=9 || m_arrBall[row+1][column-1] != NULL)
			{
				target->setPosition(position);
				return;
			}
		}
		else if(delta.x<0&&delta.y==0) //←
		{
			if(column-1<0 || m_arrBall[row][column-1] != NULL)
			{
				target->setPosition(position);
				return;
			}
		}
		else if(delta.x<0&&delta.y<0) //↙
		{
			if(column-1<0 || row-1<0 || m_arrBall[row-1][column-1] != NULL)
			{
				target->setPosition(position);
				return;
			}
		}
		else if(delta.x==0&&delta.y<0) //↓
		{
			if(row-1<0 || m_arrBall[row-1][column] != NULL)
			{
				target->setPosition(position);
				return;
			}
		}
		else if(delta.x>0&&delta.y<0) //↘
		{
			if(column+1>=7 || row-1<0 || m_arrBall[row-1][column+1] != NULL)
			{
				target->setPosition(position);
				return;
			}
		}
		else if(delta.x>0&&delta.y==0) //→
		{
			if(column+1>=7 || m_arrBall[row][column+1] != NULL)
			{
				target->setPosition(position);
				return;
			}
		}
		target->setPosition(target->getPosition() + touch->getDelta());
		//this->setBallExchange(target);

	};

	m_listener1->onTouchEnded = [=](Touch* touch, Event* event){
		auto target = static_cast<BallSprite*>(event->getCurrentTarget());
		//target->getPhysicsBody()->setDynamic(false);
		log("sprite onTouchesEnded.. ");
		/*target->setZOrder(0);
		target->setOpacity(255);*/

		auto ballTouchCurrentPosition=target->getPosition();
		auto adress=target->getAddress();
		auto ballBoundingBoxSize=target->getContentSize();
		auto column = (int)round((ballTouchCurrentPosition.x-20)*120/(75*ballBoundingBoxSize.width));
		auto row = (int)round((ballTouchCurrentPosition.y-30)*120/(75*ballBoundingBoxSize.height));
		target->setPosition(Point(column*ballBoundingBoxSize.width*75/120 + 20,row*ballBoundingBoxSize.height*75/120 + 30));
		m_arrBall[adress.row][adress.column] = NULL;
		for (auto i=row;i>=-1;i--)
		{
			if(i==-1 || m_arrBall[i][column]!=NULL)
			{
				float speed = (row-i+1)/4;
				target->runAction(MoveTo::create(speed, Point(column*ballBoundingBoxSize.width*75/120 + 20,(i+1)*ballBoundingBoxSize.height*75/120 + 30)));
				target->feather();
				target->setAddress(i+1, column);
				m_arrBall[i+1][column]=target;
				break;
			}
		}
		

		/*this->checkThreeAndAboveSameBall();

		Node* node=Node::create();
		addChild(node);
		node->runAction(Sequence::create(DelayTime::create(0.3f),CallFunc::create([this](){this->produceNewBallFill();}), NULL));*/
	};

	//将小球增加到监听器
	for (auto i=0; i<3; i++) {
		for (auto j=0; j<7; j++) {
			if (i==0&&j==0) {
				_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener1, m_arrBall[i][j]);
			} else {
				_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener1->clone(), m_arrBall[i][j]);
			}
		}
	}

}

void GameLayer::checkThreeAndAboveSameBall()
{
	
}

void GameLayer::removeAndMoveBall()
{

}

void GameLayer::produceNewBallFill()
{
	
}

void GameLayer::onExit()
{
	_eventDispatcher->removeEventListener(m_listener1);

	Layer::onExit();
}
