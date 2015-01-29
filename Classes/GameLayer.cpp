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
			int *arr=new int[2];
			arr[0] = i;
			arr[1] = j;
			m_arrBall[i][j]->setUserData(arr);
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
		auto arr=(int*)target->getUserData();
		auto ballBoundingBoxSize=m_arrBall[arr[0]][arr[1]]->getContentSize();
		auto x = (ballTouchCurrentPosition.x-20)*120/(75*ballBoundingBoxSize.width);
		auto y = (ballTouchCurrentPosition.y-30)*120/(75*ballBoundingBoxSize.height);
		auto column = delta.x>0 ? (int)floor(x) : (int)ceil(x);
		auto row = delta.y>0 ? (int)floor(y) : (int)ceil(y);
		//x->column y->row
		if(delta.x>0 && delta.y>0) //↗
		{
			if(column+1>=7 || row+1>=9 || m_arrBall[row+1][column+1] != NULL)
			{
				return;
			}
		}
		else if(delta.x==0&&delta.y>0) //↑
		{
			if(row+1>=9 || m_arrBall[row+1][column] != NULL)
			{
				return;
			}
		}
		else if(delta.x<0&&delta.y>0) //↖
		{
			if(column-1<0 || row+1>=9 || m_arrBall[row+1][column-1] != NULL)
			{
				return;
			}
		}
		else if(delta.x<0&&delta.y==0) //←
		{
			if(column-1<0 || m_arrBall[row][column-1] != NULL)
			{
				return;
			}
		}
		else if(delta.x<0&&delta.y<0) //↙
		{
			if(column-1<0 || row-1<0 || m_arrBall[row-1][column-1] != NULL)
			{
				return;
			}
		}
		else if(delta.x==0&&delta.y<0) //↓
		{
			if(row-1<0 || m_arrBall[row-1][column] != NULL)
			{
				return;
			}
		}
		else if(delta.x>0&&delta.y<0) //↘
		{
			if(column+1>=7 || row-1<0 || m_arrBall[row-1][column+1] != NULL)
			{
				return;
			}
		}
		else if(delta.x>0&&delta.y==0) //→
		{
			if(column+1>=7 || m_arrBall[row][column+1] != NULL)
			{
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
		auto arr=(int*)target->getUserData();
		auto ballBoundingBoxSize=m_arrBall[arr[0]][arr[1]]->getContentSize();
		auto column = (int)round((ballTouchCurrentPosition.x-20)*120/(75*ballBoundingBoxSize.width));
		auto row = (int)round((ballTouchCurrentPosition.y-30)*120/(75*ballBoundingBoxSize.height));
		target->setPosition(Point(column*ballBoundingBoxSize.width*75/120 + 20,row*ballBoundingBoxSize.height*75/120 + 30));
		m_arrBall[arr[0]][arr[1]] = NULL;
		for (auto i=row;i>0;i--)
		{
			if(m_arrBall[i][column]!=NULL)
			{
				float speed = (row-i+1)/4;
				target->runAction(MoveTo::create(speed, Point(column*ballBoundingBoxSize.width*75/120 + 20,(i+1)*ballBoundingBoxSize.height*75/120 + 30)));
				target->feather();
				m_arrBall[i+1][column]=target;
				int *arr=new int[2];
				arr[0] = i+1;
				arr[1] = column;
				m_arrBall[i+1][column]->setUserData(arr);
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

void GameLayer::setBallExchange(BallSprite* target)
{
	//lambda表达式
	auto temp_exchangeArr = [this](int* &arr,int* &arr_t){
		auto ballBoundingBoxSize=m_arrBall[arr_t[0]][arr_t[1]]->getContentSize();
		m_arrBall[arr[0]][arr[1]]->setPosition(Point(arr_t[0]*ballBoundingBoxSize.width*75/120 + 20,arr_t[1]*ballBoundingBoxSize.height*75/120 + 30));

		auto temp = m_arrBall[arr_t[0]][arr_t[1]];
		m_arrBall[arr_t[0]][arr_t[1]]=m_arrBall[arr[0]][arr[1]];
		m_arrBall[arr[0]][arr[1]]=temp;
	};

	auto arr=(int*)target->getUserData();
	auto ballBoundingBoxSize=m_arrBall[arr[0]][arr[1]]->getContentSize();
	auto ballTouchBeganPosition=Point(arr[0]*ballBoundingBoxSize.width*75/120 + 20,arr[1]*ballBoundingBoxSize.height*75/120 + 30);

	auto ballTouchCurrentPosition=target->getPosition();

	//判断是否行交换2小球
	if (fabs(ballTouchCurrentPosition.x-ballTouchBeganPosition.x)>=ballBoundingBoxSize.width/2) {
		if (ballTouchCurrentPosition.x-ballTouchBeganPosition.x>0) {
			auto arr_t=(int*)m_arrBall[arr[0]+1][arr[1]]->getUserData();
			arr_t[0]--;
			arr[0]++;
			temp_exchangeArr(arr,arr_t);
		} else {
			auto arr_t=(int*)m_arrBall[arr[0]-1][arr[1]]->getUserData();
			arr_t[0]++;
			arr[0]--;
			temp_exchangeArr(arr,arr_t);
		}
	}else if(fabs(ballTouchCurrentPosition.y-ballTouchBeganPosition.y)>=ballBoundingBoxSize.height/2){  //判断是否列交换2小球
		if (ballTouchCurrentPosition.y-ballTouchBeganPosition.y>0) {
			auto arr_t=(int*)m_arrBall[arr[0]][arr[1]+1]->getUserData();
			arr_t[1]--;
			arr[1]++;
			temp_exchangeArr(arr,arr_t);
		} else {
			auto arr_t=(int*)m_arrBall[arr[0]][arr[1]-1]->getUserData();
			arr_t[1]++;
			arr[1]--;
			temp_exchangeArr(arr,arr_t);
		}

	}
}

void GameLayer::checkThreeAndAboveSameBall()
{
	m_rowThreeAboveSameBall.clear();
	m_colThreeAboveSameBall.clear();
	//行检测
	for (auto i=0; i<9; i++) {
		vector<BallSprite*> rowSameBall;
		for (auto j=0; j<7; j++) {
			if (rowSameBall.size()==0 && m_arrBall[i][j]!=NULL) {
				rowSameBall.push_back(m_arrBall[i][j]);
			} else {
				if (m_arrBall[i][j]!=NULL) {
					auto arr=(int*)(*(rowSameBall.end()-1))->getUserData();
					if (m_arrBall[i][j]->getName()==m_arrBall[arr[0]][arr[1]]->getName()) {
						if (abs(i-arr[0])==1) {
							rowSameBall.push_back(m_arrBall[i][j]);
						}

					}else{
						if (rowSameBall.size()<3) {
							rowSameBall.clear();
							rowSameBall.push_back(m_arrBall[i][j]);
						}else{
							m_rowThreeAboveSameBall.push_back(rowSameBall);
							rowSameBall.clear();
							rowSameBall.push_back(m_arrBall[i][j]);
						}
					}
				}else{
					if (rowSameBall.size()<3) {
						rowSameBall.clear();
					}else{
						m_rowThreeAboveSameBall.push_back(rowSameBall);
						rowSameBall.clear();
					}
				}
			}
		}
		if (rowSameBall.size()>=3) {
			m_rowThreeAboveSameBall.push_back(rowSameBall);
		}
		rowSameBall.clear();
	}

	//列检测
	for (auto j=0; j<7; j++) {
		std::vector<BallSprite*> colSameBall;
		for (auto i=0; i<9; i++) {
			if (colSameBall.size()==0) {
				if (m_arrBall[i][j]!=NULL) {
					colSameBall.push_back(m_arrBall[i][j]);
				}
			} else {
				if (m_arrBall[i][j]!=NULL) {
					auto arr=(int*)(*(colSameBall.end()-1))->getUserData();
					if (m_arrBall[i][j]->getName()==m_arrBall[arr[0]][arr[1]]->getName()) {
						if (abs(j-arr[1])==1) {
							colSameBall.push_back(m_arrBall[i][j]);
						}

					}else{
						if (colSameBall.size()<3) {
							colSameBall.clear();
							colSameBall.push_back(m_arrBall[i][j]);
						}else{
							m_colThreeAboveSameBall.push_back(colSameBall);
							colSameBall.clear();
							colSameBall.push_back(m_arrBall[i][j]);
						}
					}
				}else{
					if (colSameBall.size()<3) {
						colSameBall.clear();
					}else{
						m_colThreeAboveSameBall.push_back(colSameBall);
						colSameBall.clear();
					}
				}
			}
		}
		if (colSameBall.size()>=3) {
			m_colThreeAboveSameBall.push_back(colSameBall);
		}
		colSameBall.clear();
	}

	if (m_rowThreeAboveSameBall.size()==0 && m_colThreeAboveSameBall.size()==0) {
		return;
	} else {
		this->removeAndMoveBall();
	}

}

void GameLayer::removeAndMoveBall()
{
	for(auto& rowSameBall : m_rowThreeAboveSameBall){
		for(auto& sp : rowSameBall){
			sp->setVisible(false);
			auto arr=(int*)sp->getUserData();
			m_arrBall[arr[0]][arr[1]]=NULL;
		}
	}

	for(auto& colSameBall : m_colThreeAboveSameBall){
		for(auto& sp : colSameBall){
			sp->setVisible(false);
			auto arr=(int*)sp->getUserData();
			m_arrBall[arr[0]][arr[1]]=NULL;
		}
	}

	//将上面的球移动到消除后的球位置处，并消除的球上移
	for (auto i=0; i<9; i++) {
		for (auto j=0; j<7; j++) {
			if (m_arrBall[i][j]==NULL && i+1<9 ) {
				int n=1,k=i+1;
				while (k<9) {
					if(m_arrBall[k][j]==NULL){
						n++;
						k++;
					}else{
						break;
					}
				}

				if (k!=9) {
					int m=k,p=0;
					std::vector<BallSprite*> temp_ballInfo1;
					do {
						auto arr=(int*)m_arrBall[m][j]->getUserData();
						arr[1]-=n;
						p++;
						temp_ballInfo1.push_back(m_arrBall[m][j]);
					} while (++m<5);

					m=i;
					std::vector<BallSprite*> temp_ballInfo2;
					do {
						auto arr=(int*)m_arrBall[m][j]->getUserData();
						arr[1]+=p;
						temp_ballInfo2.push_back(m_arrBall[m][j]);
					} while (++m<k);

					for (auto& ball : temp_ballInfo2) {
						auto arr=(int*)ball->getUserData();
						m_arrBall[arr[0]][arr[1]]=ball;

						auto ballBoundingBoxSize=ball->getContentSize();
						ball->setPosition(Point(arr[0]*ballBoundingBoxSize.width*75/120 + 20,arr[1]*ballBoundingBoxSize.height*75/120 + 30));
					}

					for (auto& ball : temp_ballInfo1) {
						auto arr=(int*)ball->getUserData();
						m_arrBall[arr[0]][arr[1]]=ball;

						auto ballBoundingBoxSize=ball->getContentSize();

						ball->runAction(MoveTo::create(0.2f, Point(arr[0]*ballBoundingBoxSize.width*75/120 + 20,arr[1]*ballBoundingBoxSize.height*75/120 + 30)));

					}

				}
			}
		}
	}
	Node* node=Node::create();
	addChild(node);
	node->runAction(Sequence::create(DelayTime::create(0.25f),CallFunc::create([this](){this->checkThreeAndAboveSameBall();}), NULL));

}

void GameLayer::produceNewBallFill()
{
	
}

void GameLayer::onExit()
{
	_eventDispatcher->removeEventListener(m_listener1);

	Layer::onExit();
}
