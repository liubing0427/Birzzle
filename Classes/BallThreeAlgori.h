#ifndef __MyGameDemo__BallThreeAlgori__
#define __MyGameDemo__BallThreeAlgori__

#include <iostream>
#include "cocos2d.h"
#include <vector>

enum enum_ball{
	BALL0 = 0,
	BALL1,
	BALL2,
	BALL3,
	BALL4,
	BALL5,
	BALLNULL
};

struct ballInfo {
	cocos2d::Sprite* sp_ball;
	enum enum_ball ballColor;

};

class CBallThreeAlgori : public cocos2d::Layer
{
public:
	CBallThreeAlgori();
	virtual ~CBallThreeAlgori();

	static cocos2d::Scene* createScene();

	virtual bool init() override;
	virtual void onExit() override;

	CREATE_FUNC(CBallThreeAlgori);

private:
	void initBall();   //初始化球，并显示到界面
	void registerTouchBall();  //注册球触摸并进行移动的处理
	void setBallExchange(cocos2d::Sprite* target);  //交换2个球

	void checkThreeAndAboveSameBall();  //筛选出3个及以上颜色相同的球，放到m_rowThreeAboveSameBall、m_colThreeAboveSameBall

	void removeAndMoveBall();  //移除可消除的球并上面的球向下移动填充，它与checkThreeAndAboveSameBall函数递归调用

	void produceNewBallFill();  //产生新球去填充

private:
	struct ballInfo m_arrBall[6][5]; //保存小球，为5行6列
	std::vector<std::vector<cocos2d::Sprite*>> m_rowThreeAboveSameBall; //保存行相同的球
	std::vector<std::vector<cocos2d::Sprite*>> m_colThreeAboveSameBall; //保存列相同的球

	std::vector<enum enum_ball> m_tempBallMask;

	cocos2d::EventListenerTouchOneByOne* m_listener1;
};

#endif /* defined(__MyGameDemo__BallThreeAlgori__) */