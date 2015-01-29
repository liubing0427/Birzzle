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
	void initBall();   //��ʼ���򣬲���ʾ������
	void registerTouchBall();  //ע�������������ƶ��Ĵ���
	void setBallExchange(cocos2d::Sprite* target);  //����2����

	void checkThreeAndAboveSameBall();  //ɸѡ��3����������ɫ��ͬ���򣬷ŵ�m_rowThreeAboveSameBall��m_colThreeAboveSameBall

	void removeAndMoveBall();  //�Ƴ�����������������������ƶ���䣬����checkThreeAndAboveSameBall�����ݹ����

	void produceNewBallFill();  //��������ȥ���

private:
	struct ballInfo m_arrBall[6][5]; //����С��Ϊ5��6��
	std::vector<std::vector<cocos2d::Sprite*>> m_rowThreeAboveSameBall; //��������ͬ����
	std::vector<std::vector<cocos2d::Sprite*>> m_colThreeAboveSameBall; //��������ͬ����

	std::vector<enum enum_ball> m_tempBallMask;

	cocos2d::EventListenerTouchOneByOne* m_listener1;
};

#endif /* defined(__MyGameDemo__BallThreeAlgori__) */