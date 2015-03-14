#include "GameLayer.h"

GameLayer::GameLayer(){}

GameLayer::~GameLayer(){}

bool GameLayer::init(){
	if (!Layer::init()) {
		return false;
	}

	this->visiableSize = Director::getInstance()->getVisibleSize();
	this->visibleOrigin = Director::getInstance()->getVisibleOrigin();

	this->gameStatus = GAME_STATUS_READY;
	this->score = 0;
	this->popBirds = 0;

	Sprite *ttree = Sprite::create("stage_tree@2x.png");
	ttree->setPosition(Point(visiableSize.width/2 , (visiableSize.height*7)/16));
	this->addChild(ttree);
	this->tree = ttree;

	Sprite *grass = Sprite::create("stage_tree_grass_r@2x.png");
	grass->setAnchorPoint(Point::ZERO);
	grass->setPosition(Point(visiableSize.width/2-grass->getBoundingBox().size.width/2, 0));
	grass->setGlobalZOrder(90);
	this->addChild(grass);
	this->initBall();
	for (auto j=0; j<7; j++) {
		m_newBall[j]=nullptr;
	}
	this->produceNewBall();

	shared = this;

	//create the quit menu
	Sprite* quitBtn = Sprite::create("pauseButton@2x.png");
	auto  quitMenuItem = MenuItemSprite::create(quitBtn,nullptr,nullptr,CC_CALLBACK_1(GameLayer::menuPauseCallback,this));

	auto menu = Menu::create(quitMenuItem, NULL);
	menu->alignItemsHorizontallyWithPadding(20.0f);
	menu->setPosition(Point((visiableSize.width*3)/40 , (visiableSize.height * 22) / 23));
	this->addChild(menu, 100);

	return true;
}

void GameLayer::menuPauseCallback(Object *sender){
	this->pause();
	this->delegator->onGamePause();
}

GameLayer* GameLayer::shared = nullptr;

GameLayer* GameLayer::getInstance(){
	return shared;
}

void GameLayer::initBall()
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
	for (auto i=0; i<GAME_ROW; i++) {
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
			//auto ballBoundingBoxSize=m_arrBall[i][j]->getContentSize();
			auto position=Point(j*BIRD_WIDTH + X_SKEWING,i*BIRD_WIDTH + Y_SKEWING);
			m_arrBall[i][j]->setAnchorPoint(Point::ZERO);
			m_arrBall[i][j]->setPosition(position);
			m_arrBall[i][j]->blink();
			m_arrBall[i][j]->setGlobalZOrder(GAME_ROW-1-i);
			/*PhysicsBody *body = PhysicsBody::create();
			auto visiableSize = Director::getInstance()->getVisibleSize();
			auto bodyPosition = Point(ballBoundingBoxSize.width*BIRD_WIDTH/WIDTH+(visiableSize.width-tree->getBoundingBox().size.width)/2-15*j,ballBoundingBoxSize.height*BIRD_WIDTH/WIDTH+((visiableSize.height*7)/16-tree->getBoundingBox().size.height/2)-5*i);
			body->addShape(PhysicsShapeBox::create(Size(ballBoundingBoxSize.width*BIRD_WIDTH/WIDTH,ballBoundingBoxSize.height*BIRD_WIDTH/WIDTH),PHYSICSBODY_MATERIAL_DEFAULT,bodyPosition));
			body->setDynamic(false);
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
	if(target->getActionState()!=ACTION_STATE_SHAKE){
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
	}
	return false;
}

void GameLayer::onTouchMoved(Touch* touch, Event* event)
{
	auto target = static_cast<BallSprite*>(event->getCurrentTarget());
	auto delta = touch->getDelta();
	auto ballTouchCurrentPosition=target->getPosition();
	auto address=target->getAddress();
	//auto ballBoundingBoxSize=target->getContentSize();
	auto x = (ballTouchCurrentPosition.x-X_SKEWING)/BIRD_WIDTH;
	auto y = (ballTouchCurrentPosition.y-Y_SKEWING)/BIRD_WIDTH;
	//floor向下 ceil向上
	auto column = (int)round(x);
	auto row = (int)round(y);
	column = column<0?0:(column>6?6:column);
	row = row<0?0:(row>(GAME_ROW-1)?(GAME_ROW-1):row);
	if(column!=address.column||row!=address.row){
		m_arrBall[address.row][address.column] = nullptr;
	}
	auto position = Point((column)*BIRD_WIDTH + X_SKEWING, row*BIRD_WIDTH + Y_SKEWING);
	//x->column y->row
	if(delta.x>0 && delta.y>0) //↗
	{
		if(column+1>=7 || row+1>=GAME_ROW || m_arrBall[row+1][column+1] != nullptr)
		{
			target->setPosition(position);
			return;
		}
	}
	else if(delta.x==0&&delta.y>0) //↑
	{
		if(row+1>=GAME_ROW || m_arrBall[row+1][column] != nullptr)
		{
			target->setPosition(position);
			return;
		}
	}
	else if(delta.x<0&&delta.y>0) //↖
	{
		if(column-1<0 || row+1>=GAME_ROW || m_arrBall[row+1][column-1] != nullptr)
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
	//auto ballBoundingBoxSize=target->getContentSize();
	auto column = (int)round((ballTouchCurrentPosition.x-X_SKEWING)/BIRD_WIDTH);
	auto row = (int)round((ballTouchCurrentPosition.y-Y_SKEWING)/BIRD_WIDTH);
	target->setPosition(Point(column*BIRD_WIDTH + X_SKEWING,row*BIRD_WIDTH + Y_SKEWING));
	if(row==address.row&&column==address.column)
	{
		if(m_arrBall[address.row][address.column]==nullptr||m_arrBall[address.row][address.column]==target)
		{
			m_arrBall[address.row][address.column] = target;
		}
		else
		{
			auto p=1;
			while (m_arrBall[address.row+p][address.column]!=nullptr)
			{
				p++;
			}

			target->setPosition(Point(column*BIRD_WIDTH + X_SKEWING,(address.row+p)*BIRD_WIDTH + Y_SKEWING));
			target->setGlobalZOrder((GAME_ROW-1)-(address.row+p));
			target->setAddress(address.row+p, column);
			m_arrBall[address.row+p][column]=target;
		}
		return;
	}
	for (auto i=row;i>=-1;i--)
	{
		if(i==-1 || m_arrBall[i][column]!=nullptr)
		{
			float speed = (row-(i+1))/4;
			target->setGlobalZOrder((GAME_ROW-1)-(i+1));
			target->setAddress(i+1, column);
			m_arrBall[i+1][column]=target;
			target->MoveToAction(MoveTo::create(speed, Point(column*BIRD_WIDTH + X_SKEWING,(i+1)*BIRD_WIDTH + Y_SKEWING)), 
			[&](Node* ball)
			{
				auto bird = (BallSprite*)ball;
				checkThreeAndAboveSameBall(bird);
			}
			,row!=(i+1));

			auto j=1;
			while(address.row + j<GAME_ROW&&m_arrBall[address.row + j][address.column]!=nullptr&&m_arrBall[address.row + j][address.column]->getActionState()!=ACTION_STATE_SHAKE)
			{
				auto p = Point(address.column*BIRD_WIDTH + X_SKEWING,(address.row + j-1)*BIRD_WIDTH + Y_SKEWING);

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
					m_arrBall[address.row + j][address.column]->setGlobalZOrder((GAME_ROW-1)-(address.row + j-1));
				}
				m_arrBall[address.row + j-1][address.column] = m_arrBall[address.row + j][address.column];
				m_arrBall[address.row + j][address.column] = nullptr;
				j++;
			}

			break;
		}
	}
}

void GameLayer::checkThreeAndAboveSameBall(BallSprite* sprite)
{
	queue<BallSprite*> Q;
	vector<BallSprite*> sameBall;
	BallSprite* lastBall = nullptr;
	sprite->setVisited(true);
	Q.push(sprite);
	sameBall.push_back(sprite);
	while(!Q.empty()) 
	{
		int i,j;
		auto ball=Q.front();         //取队首顶点
		ball->isLast = false;
		auto address = ball->getAddress();
		if(lastBall ==nullptr || (lastBall->getSkillState()!= SKILL_STATE_FIRE && lastBall->getSkillState()<ball->getSkillState())){
			lastBall = ball;
		}
		auto name = ball->getName();
		Q.pop();
		if(address.row<(GAME_ROW-1) && m_arrBall[address.row+1][address.column]&&m_arrBall[address.row+1][address.column]->getVisited()==false
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
		lastBall->isLast = true;
		auto count = sameBall.size();
		auto k=0;
		for(auto& bird : sameBall){
			k++;
			switch (count){
			case 3:
				bird->remove(true, SKILL_STATE_NORMAL);
				break;
			case 4:
				bird->remove(true, SKILL_STATE_BOMB);
				break;
			case 5:
				bird->remove(true, SKILL_STATE_FIRE);
				break;
			case 6:
				bird->remove(true, SKILL_STATE_LIGHTNING);
				break;
			case 7:
			default:
				bird->remove(true, SKILL_STATE_BLACKHOLE);
				break;
			}
		}
	}
}

void GameLayer::produceNewBall()
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
	auto isGameOver = true;
	for (auto j=0; j<7; j++) {
		if(m_newBall[j]==nullptr){
			isGameOver = false;
			string ballname = BallSprite::createBallByRandom();
			if (j>=2){
				if(ballname==m_newBall[j-1]->getName()&&ballname==m_newBall[j-2]->getName()){
					ballname = randProduceBall(ballname);//重新生成
				}
			}
			BallSprite* temp_ball = BallSprite::createBall(ballname);
			m_newBall[j]=temp_ball;
			//将小球显示出来
			auto position=Point(j*BIRD_WIDTH + X_SKEWING,10*BIRD_WIDTH + 45);
			m_newBall[j]->setAnchorPoint(Point::ZERO);
			m_newBall[j]->setPosition(position);
			m_newBall[j]->blink();
			m_newBall[j]->setGlobalZOrder(10);

			tree->addChild(m_newBall[j],0);
		}
	}
	if (isGameOver)
	{
		this->gameOver();
	}
}

void GameLayer::fallDown(float dt){
	for (auto j=0; j<7; j++) {
		if(m_arrBall[8][j]==nullptr){
			auto k = 0;
			while (m_arrBall[k][j]!=nullptr)
			{
				k++;
			}
			m_arrBall[k][j] = m_newBall[j];
			m_newBall[j] = nullptr;
			m_arrBall[k][j]->setAddress(k,j);
			m_arrBall[k][j]->setGlobalZOrder(GAME_ROW-1-k);
			auto p = Point(j*BIRD_WIDTH + X_SKEWING,k*BIRD_WIDTH + Y_SKEWING);
			m_arrBall[k][j]->MoveToAction(MoveTo::create((8-k)/8.0f, p), 
				[&](Node* ball)
			{
				auto bird = (BallSprite*)ball;
				_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener1->clone(), bird);
				checkThreeAndAboveSameBall(bird);
			}
			,true);
		}
	}
	produceNewBall();
}

void GameLayer::removeFromArray(int row, int column){
	if(m_arrBall[row][column]!=nullptr){
		m_arrBall[row][column]->setVisible(false);
	}
	m_arrBall[row][column]=nullptr;
}

BallSprite* GameLayer::getBall(int row, int column){
	return m_arrBall[row][column];
}

void GameLayer::swithBall(int row, int column, int destrow, int destcolumn){
	auto p = Point(destcolumn*BIRD_WIDTH + X_SKEWING,destrow*BIRD_WIDTH + Y_SKEWING);
	m_arrBall[row][column]->setPosition(p);
	m_arrBall[row][column]->setAddress(destrow, destcolumn);
	if(m_arrBall[row][column]->getSkillState()!=SKILL_STATE_NORMAL)
	{
		m_arrBall[row][column]->setGlobalZOrder(19);
	}
	else
	{
		m_arrBall[row][column]->setGlobalZOrder(GAME_ROW-1-destrow);
	}
	m_arrBall[destrow][destcolumn] = m_arrBall[row][column];
	m_arrBall[row][column] = nullptr;
}

void GameLayer::onExit(){
	_eventDispatcher->removeEventListener(m_listener1);

	Layer::onExit();
}

void GameLayer::gameOver(){
	if(this->gameStatus == GAME_STATUS_OVER) {
		return;
	}
	_eventDispatcher->removeEventListener(m_listener1);
	tree->runAction(Sequence::create(MoveTo::create(0.3f, Point(visiableSize.width/2 , -(visiableSize.height* 3)/16)), CallFunc::create([&](){
		this->delegator->onGameEnd(this->score, this->popBirds);
	}), nullptr));
	this->unschedule(schedule_selector(GameLayer::fallDown));  
	//SimpleAudioEngine::getInstance()->playEffect("sfx_die.ogg");
	this->gameStatus = GAME_STATUS_OVER;
}

void GameLayer::onTouch() {
	if(this->gameStatus == GAME_STATUS_OVER){
		return;
	}
	if(this->gameStatus == GAME_STATUS_READY) {
		if(this->delegator->isGameStart()){
			this->registerTouchBall();
			for (auto j=0;j<7;j++)
			{
				checkThreeAndAboveSameBall(m_arrBall[1][j]);
			}

			schedule(schedule_selector(GameLayer::fallDown), 10.0f);

			this->gameStatus = GAME_STATUS_START;
		}
	}
	else if(this->gameStatus == GAME_STATUS_START) {
	}
}
