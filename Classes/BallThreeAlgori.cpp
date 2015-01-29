#include "BallThreeAlgori.h"
#include <functional>

USING_NS_CC;

CBallThreeAlgori::CBallThreeAlgori()
{
	m_rowThreeAboveSameBall.clear();
	m_colThreeAboveSameBall.clear();
}

CBallThreeAlgori::~CBallThreeAlgori()
{

}

Scene* CBallThreeAlgori::createScene()
{
	auto scene = Scene::create();

	auto layer = CBallThreeAlgori::create();

	scene->addChild(layer);

	return scene;
}

bool CBallThreeAlgori::init()
{
	if (!Layer::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

	this->initBall();
	this->registerTouchBall();

	return true;
}

void CBallThreeAlgori::initBall()
{
	//lambda表达式
	std::function< void(enum enum_ball& temp_ballColor)> randProduceBall = [](enum enum_ball& temp_ballColor){
		enum enum_ball tp_arrBall[5];
		for (auto i=0,j=0; i<6; i++) {
			enum enum_ball tempp=BALLNULL;
			switch (i) {
				case 0:
					tempp=BALL0;
					break;
				case 1:
					tempp=BALL1;
					break;
				case 2:
					tempp=BALL2;
					break;
				case 3:
					tempp=BALL3;
					break;
				case 4:
					tempp=BALL4;
					break;
				case 5:
					tempp=BALL5;
					break;
				default:
					break;
			};
			if (tempp!=temp_ballColor) {
				tp_arrBall[j++]=tempp;
			}
		}

		temp_ballColor=tp_arrBall[abs(rand()%5)];
	};

	//初始化小球布局算法
	for (auto i=0; i<5; i++) {
		for (auto j=0; j<6; j++) {
			enum enum_ball temp_ballColor=(enum enum_ball)(abs(rand()%(int)BALLNULL));
			if (j-1<0 && i-1<0) {
				m_arrBall[j][i].sp_ball=Sprite::create(StringUtils::format("ball_%d.png",(int)temp_ballColor));
				m_arrBall[j][i].ballColor=temp_ballColor;
			} else if (j-1>=0 && i-1<0){
				if (j-2>=0) {
					if(temp_ballColor==m_arrBall[j-1][i].ballColor){
						if(temp_ballColor==m_arrBall[j-2][i].ballColor){
							randProduceBall(temp_ballColor);
						}
					}
				}
				m_arrBall[j][i].sp_ball=Sprite::create(StringUtils::format("ball_%d.png",(int)temp_ballColor));
				m_arrBall[j][i].ballColor=temp_ballColor;
			} else if (j-1<0 && i-1>=0){
				if (i-2>=0) {
					if(temp_ballColor==m_arrBall[j][i-1].ballColor){
						if(temp_ballColor==m_arrBall[j][i-2].ballColor){
							randProduceBall(temp_ballColor);
						}
					}
				}
				m_arrBall[j][i].sp_ball=Sprite::create(StringUtils::format("ball_%d.png",(int)temp_ballColor));
				m_arrBall[j][i].ballColor=temp_ballColor;
			}else if (j-1>=0 && i-1>=0){
				if (j-2>=0 && i-2<0) {
					if(temp_ballColor==m_arrBall[j-1][i].ballColor){
						if(temp_ballColor==m_arrBall[j-2][i].ballColor){
							randProduceBall(temp_ballColor);
						}
					}
				}else if (i-2>=0 && j-2<0) {
					if(temp_ballColor==m_arrBall[j][i-1].ballColor){
						if(temp_ballColor==m_arrBall[j][i-2].ballColor){
							randProduceBall(temp_ballColor);
						}
					}
				}else if (i-2<0 && j-2<0) {
				}else{
KK_1:
					if(temp_ballColor==m_arrBall[j-1][i].ballColor){
						if(temp_ballColor==m_arrBall[j-2][i].ballColor){
							randProduceBall(temp_ballColor);
						}
					}
					if(temp_ballColor==m_arrBall[j][i-1].ballColor){
						if(temp_ballColor==m_arrBall[j][i-2].ballColor){
							randProduceBall(temp_ballColor);
							goto KK_1;
						}
					}
				}
				m_arrBall[j][i].sp_ball=Sprite::create(StringUtils::format("ball_%d.png",(int)temp_ballColor));
				m_arrBall[j][i].ballColor=temp_ballColor;

			}
		}
	}

	//将小球显示出来
	for (auto i=0; i<5; i++) {
		for (auto j=0; j<6; j++) {
			auto ballBoundingBoxSize=m_arrBall[j][i].sp_ball->getBoundingBox();
			auto position=Point(j*ballBoundingBoxSize.size.width+ballBoundingBoxSize.size.width/2,i*ballBoundingBoxSize.size.height+ballBoundingBoxSize.size.height/2);
			m_arrBall[j][i].sp_ball->setPosition(position);
			this->addChild(m_arrBall[j][i].sp_ball,0);
			int *arr=new int[2];
			arr[0] = j;
			arr[1] = i;
			m_arrBall[j][i].sp_ball->setUserData(arr);
		}
	}
}

void CBallThreeAlgori::registerTouchBall()
{
	m_listener1 = EventListenerTouchOneByOne::create();  //注册单点触摸
	m_listener1->setSwallowTouches(true);

	m_listener1->onTouchBegan = [](Touch* touch, Event* event){
		auto target = static_cast<Sprite*>(event->getCurrentTarget());

		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		if (rect.containsPoint(locationInNode))
		{
			log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
			target->setZOrder(1);
			target->setOpacity(180);
			return true;
		}
		return false;
	};

	m_listener1->onTouchMoved = [this](Touch* touch, Event* event){
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		target->setPosition(target->getPosition() + touch->getDelta());
		this->setBallExchange(target);

	};

	m_listener1->onTouchEnded = [=](Touch* touch, Event* event){
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		log("sprite onTouchesEnded.. ");
		target->setZOrder(0);
		target->setOpacity(255);

		auto arr=(int*)target->getUserData();
		auto ballBoundingBoxSize=m_arrBall[arr[0]][arr[1]].sp_ball->getBoundingBox();
		target->setPosition(Point(arr[0]*ballBoundingBoxSize.size.width+ballBoundingBoxSize.size.width/2,arr[1]*ballBoundingBoxSize.size.height+ballBoundingBoxSize.size.height/2));

		this->checkThreeAndAboveSameBall();

		Node* node=Node::create();
		addChild(node);
		node->runAction(Sequence::create(DelayTime::create(0.3f),CallFunc::create([this](){this->produceNewBallFill();}), NULL));
	};

	//将小球增加到监听器
	for (auto i=0; i<5; i++) {
		for (auto j=0; j<6; j++) {
			if (i==0 && j==0) {
				_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener1, m_arrBall[j][i].sp_ball);
			} else {
				_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener1->clone(), m_arrBall[j][i].sp_ball);
			}
		}
	}

}

void CBallThreeAlgori::setBallExchange(Sprite* target)
{
	//lambda表达式
	std::function< void(int* &arr,int* &arr_t)> temp_exchangeArr = [this](int* &arr,int* &arr_t){
		m_arrBall[arr[0]][arr[1]].sp_ball->setPosition(Point(arr_t[0]*m_arrBall[arr_t[0]][arr_t[1]].sp_ball->getBoundingBox().size.width+m_arrBall[arr_t[0]][arr_t[1]].sp_ball->getBoundingBox().size.width/2,arr_t[1]*m_arrBall[arr_t[0]][arr_t[1]].sp_ball->getBoundingBox().size.height+m_arrBall[arr_t[0]][arr_t[1]].sp_ball->getBoundingBox().size.height/2));

		struct ballInfo temp_ballInfo=m_arrBall[arr_t[0]][arr_t[1]];
		m_arrBall[arr_t[0]][arr_t[1]]=m_arrBall[arr[0]][arr[1]];
		m_arrBall[arr[0]][arr[1]]=temp_ballInfo;
	};

	auto arr=(int*)target->getUserData();
	auto ballBoundingBoxSize=m_arrBall[arr[0]][arr[1]].sp_ball->getBoundingBox();
	auto ballTouchBeganPosition=Point(arr[0]*ballBoundingBoxSize.size.width+ballBoundingBoxSize.size.width/2,arr[1]*ballBoundingBoxSize.size.height+ballBoundingBoxSize.size.height/2);

	auto ballTouchCurrentPosition=target->getPosition();

	//判断是否行交换2小球
	if (fabs(ballTouchCurrentPosition.x-ballTouchBeganPosition.x)>=ballBoundingBoxSize.size.width/2) {
		if (ballTouchCurrentPosition.x-ballTouchBeganPosition.x>0) {
			auto arr_t=(int*)m_arrBall[arr[0]+1][arr[1]].sp_ball->getUserData();
			arr_t[0]--;
			arr[0]++;
			temp_exchangeArr(arr,arr_t);
		} else {
			auto arr_t=(int*)m_arrBall[arr[0]-1][arr[1]].sp_ball->getUserData();
			arr_t[0]++;
			arr[0]--;
			temp_exchangeArr(arr,arr_t);
		}
	}else if(fabs(ballTouchCurrentPosition.y-ballTouchBeganPosition.y)>=ballBoundingBoxSize.size.height/2){  //判断是否列交换2小球
		if (ballTouchCurrentPosition.y-ballTouchBeganPosition.y>0) {
			auto arr_t=(int*)m_arrBall[arr[0]][arr[1]+1].sp_ball->getUserData();
			arr_t[1]--;
			arr[1]++;
			temp_exchangeArr(arr,arr_t);
		} else {
			auto arr_t=(int*)m_arrBall[arr[0]][arr[1]-1].sp_ball->getUserData();
			arr_t[1]++;
			arr[1]--;
			temp_exchangeArr(arr,arr_t);
		}

	}
}

void CBallThreeAlgori::checkThreeAndAboveSameBall()
{
	m_rowThreeAboveSameBall.clear();
	m_colThreeAboveSameBall.clear();
	//行检测
	for (auto i=0; i<5; i++) {
		std::vector<Sprite*> rowSameBall;
		for (auto j=0; j<6; j++) {
			if (rowSameBall.size()==0) {
				if (m_arrBall[j][i].ballColor!=BALLNULL) {
					rowSameBall.push_back(m_arrBall[j][i].sp_ball);
				}

			} else {
				if (m_arrBall[j][i].ballColor!=BALLNULL) {
					auto arr=(int*)(*(rowSameBall.end()-1))->getUserData();
					if (m_arrBall[j][i].ballColor==m_arrBall[arr[0]][arr[1]].ballColor) {
						if (abs(j-arr[0])==1) {
							rowSameBall.push_back(m_arrBall[j][i].sp_ball);
						}

					}else{
						if (rowSameBall.size()<3) {
							rowSameBall.clear();
							rowSameBall.push_back(m_arrBall[j][i].sp_ball);
						}else{
							m_rowThreeAboveSameBall.push_back(rowSameBall);
							rowSameBall.clear();
							rowSameBall.push_back(m_arrBall[j][i].sp_ball);
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
	for (auto i=0; i<6; i++) {
		std::vector<Sprite*> colSameBall;
		for (auto j=0; j<5; j++) {
			if (colSameBall.size()==0) {
				if (m_arrBall[i][j].ballColor!=BALLNULL) {
					colSameBall.push_back(m_arrBall[i][j].sp_ball);
				}

			} else {
				if (m_arrBall[i][j].ballColor!=BALLNULL) {
					auto arr=(int*)(*(colSameBall.end()-1))->getUserData();
					if (m_arrBall[i][j].ballColor==m_arrBall[arr[0]][arr[1]].ballColor) {
						if (abs(j-arr[1])==1) {
							colSameBall.push_back(m_arrBall[i][j].sp_ball);
						}

					}else{
						if (colSameBall.size()<3) {
							colSameBall.clear();
							colSameBall.push_back(m_arrBall[i][j].sp_ball);
						}else{
							m_colThreeAboveSameBall.push_back(colSameBall);
							colSameBall.clear();
							colSameBall.push_back(m_arrBall[i][j].sp_ball);
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

void CBallThreeAlgori::removeAndMoveBall()
{
	for(auto& rowSameBall : m_rowThreeAboveSameBall){
		for(auto& sp : rowSameBall){
			sp->setVisible(false);
			auto arr=(int*)sp->getUserData();
			m_arrBall[arr[0]][arr[1]].ballColor=BALLNULL;
		}
	}

	for(auto& colSameBall : m_colThreeAboveSameBall){
		for(auto& sp : colSameBall){
			sp->setVisible(false);
			auto arr=(int*)sp->getUserData();
			m_arrBall[arr[0]][arr[1]].ballColor=BALLNULL;
		}
	}

	//将上面的球移动到消除后的球位置处，并消除的球上移
	for (auto i=0; i<5; i++) {
		for (auto j=0; j<6; j++) {
			if (m_arrBall[j][i].ballColor==BALLNULL && i+1<5 ) {
				int n=1,k=i+1;
				while (k<5) {
					if(m_arrBall[j][k].ballColor==BALLNULL){
						n++;
						k++;
					}else{
						break;
					}
				}

				if (k!=5) {
					int m=k,p=0;
					std::vector<struct ballInfo> temp_ballInfo1;
					do {
						auto arr=(int*)m_arrBall[j][m].sp_ball->getUserData();
						arr[1]-=n;
						p++;
						temp_ballInfo1.push_back(m_arrBall[j][m]);
					} while (++m<5);

					m=i;
					std::vector<struct ballInfo> temp_ballInfo2;
					do {
						auto arr=(int*)m_arrBall[j][m].sp_ball->getUserData();
						arr[1]+=p;
						temp_ballInfo2.push_back(m_arrBall[j][m]);
					} while (++m<k);

					for (auto& ball : temp_ballInfo2) {
						auto arr=(int*)ball.sp_ball->getUserData();
						m_arrBall[arr[0]][arr[1]]=ball;

						auto ballBoundingBoxSize=ball.sp_ball->getBoundingBox();
						ball.sp_ball->setPosition(Point(arr[0]*ballBoundingBoxSize.size.width+ballBoundingBoxSize.size.width/2,arr[1]*ballBoundingBoxSize.size.height+ballBoundingBoxSize.size.height/2));
					}

					for (auto& ball : temp_ballInfo1) {
						auto arr=(int*)ball.sp_ball->getUserData();
						m_arrBall[arr[0]][arr[1]]=ball;

						auto ballBoundingBoxSize=ball.sp_ball->getBoundingBox();

						ball.sp_ball->runAction(MoveTo::create(0.2f, Point(arr[0]*ballBoundingBoxSize.size.width+ballBoundingBoxSize.size.width/2,arr[1]*ballBoundingBoxSize.size.height+ballBoundingBoxSize.size.height/2)));

					}

				}
			}
		}
	}
	Node* node=Node::create();
	addChild(node);
	node->runAction(Sequence::create(DelayTime::create(0.25f),CallFunc::create([this](){this->checkThreeAndAboveSameBall();}), NULL));

}

void CBallThreeAlgori::produceNewBallFill()
{
	//lambda表达式
	std::function< void(enum enum_ball& temp_ballColor)> randProduceBall = [this](enum enum_ball& temp_ballColor){
		m_tempBallMask.push_back(temp_ballColor);
		enum enum_ball tp_arrBall[5];
		auto j=0;
		for (auto i=0; i<6; i++) {
			enum enum_ball tempp=BALLNULL;
			switch (i) {
			case 0:
				tempp=BALL0;
				break;
			case 1:
				tempp=BALL1;
				break;
			case 2:
				tempp=BALL2;
				break;
			case 3:
				tempp=BALL3;
				break;
			case 4:
				tempp=BALL4;
				break;
			case 5:
				tempp=BALL5;
				break;
			default:
				break;
			};
			int mm=0;
			for(auto& ballColor : m_tempBallMask){
				if (tempp==ballColor) {
					mm++;
					break;
				}
			}
			if (mm==0) {
				tp_arrBall[j++]=tempp;
			}
		}

		if (j>0) {
			temp_ballColor=tp_arrBall[abs(rand()%j)];
		}

	};

	for (auto i=0; i<5; i++) {
		for (auto j=0; j<6; j++) {
			if(m_arrBall[j][i].ballColor==BALLNULL){
				enum enum_ball temp_ballColor=(enum enum_ball)(abs(rand()%(int)BALLNULL));
				std::vector<struct ballInfo> temp_rowBall;
				std::vector<struct ballInfo> temp_colBall;
				m_tempBallMask.clear();
				while (true) {
					temp_rowBall.clear();
					temp_colBall.clear();
					if (j-1>=0 && m_arrBall[j-1][i].ballColor!=BALLNULL) {
						if (j-2>=0 && m_arrBall[j-2][i].ballColor!=BALLNULL) {
							if (m_arrBall[j-1][i].ballColor==m_arrBall[j-2][i].ballColor) {
								if (temp_ballColor==m_arrBall[j-1][i].ballColor) {
									randProduceBall(temp_ballColor);
								}
							}else{
								if (temp_ballColor==m_arrBall[j-1][i].ballColor) {
									temp_rowBall.push_back(m_arrBall[j-1][i]);
								}
							}
						}else{
							if (temp_ballColor==m_arrBall[j-1][i].ballColor) {
								temp_rowBall.push_back(m_arrBall[j-1][i]);
							}
						}
					}

					if (j+1>=0 && m_arrBall[j+1][i].ballColor!=BALLNULL) {
						if (j+2>=0 && m_arrBall[j+2][i].ballColor!=BALLNULL) {
							if (m_arrBall[j+1][i].ballColor==m_arrBall[j+2][i].ballColor) {
								if (temp_ballColor==m_arrBall[j+1][i].ballColor) {
									randProduceBall(temp_ballColor);
									continue;
								}
							}else{
								if (temp_ballColor==m_arrBall[j+1][i].ballColor) {
									temp_rowBall.push_back(m_arrBall[j+1][i]);
								}
							}
						}else{
							if (temp_ballColor==m_arrBall[j+1][i].ballColor) {
								temp_rowBall.push_back(m_arrBall[j+1][i]);
							}
						}
					}

					if (i-1>=0 && m_arrBall[j][i-1].ballColor!=BALLNULL) {
						if (i-2>=0 && m_arrBall[j][i-2].ballColor!=BALLNULL) {
							if (m_arrBall[j][i-1].ballColor==m_arrBall[j][i-2].ballColor) {
								if (temp_ballColor==m_arrBall[j][i-1].ballColor) {
									randProduceBall(temp_ballColor);
									continue;
								}
							}else{
								if (temp_ballColor==m_arrBall[j][i-1].ballColor) {
									temp_colBall.push_back(m_arrBall[j][i-1]);
								}
							}
						}else{
							if (temp_ballColor==m_arrBall[j][i-1].ballColor) {
								temp_colBall.push_back(m_arrBall[j][i-1]);
							}
						}
					}

					if (i+1>=0 && m_arrBall[j][i+1].ballColor!=BALLNULL) {
						if (i+2>=0 && m_arrBall[j][i+2].ballColor!=BALLNULL) {
							if (m_arrBall[j][i+1].ballColor==m_arrBall[j][i+2].ballColor) {
								if (temp_ballColor==m_arrBall[j][i+1].ballColor) {
									randProduceBall(temp_ballColor);
									continue;
								}
							}else{
								if (temp_ballColor==m_arrBall[j][i+1].ballColor) {
									temp_colBall.push_back(m_arrBall[j][i+1]);
								}
							}
						}else{
							if (temp_ballColor==m_arrBall[j][i+1].ballColor) {
								temp_colBall.push_back(m_arrBall[j][i+1]);
							}
						}
					}

					if (temp_colBall.size()<2 && temp_rowBall.size()<2) {
						break;
					}else{
						randProduceBall(temp_ballColor);
						continue;
					}

				}

				m_arrBall[j][i].sp_ball->setTexture(StringUtils::format("ball_%d.png",(int)temp_ballColor));
				m_arrBall[j][i].ballColor=temp_ballColor;
				m_arrBall[j][i].sp_ball->runAction(Sequence::create(DelayTime::create(0.3f), Show::create(),NULL));
			}
		}
	}

}

void CBallThreeAlgori::onExit()
{
	_eventDispatcher->removeEventListener(m_listener1);

	Layer::onExit();
}