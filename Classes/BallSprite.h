#ifndef __BALLSPRITE_H__
#define __BALLSPRITE_H__

#include "cocos2d.h"
#include "AtlasLoader.h"
#include "Shake.h"
using namespace cocos2d;

const int SHAKE_TIME = 1.5f;

typedef enum{
	SKILL_STATE_NORMAL,
	//�����С�� �ᱬ����Χ��6ֻ������ 4�� 
	SKILL_STATE_BOMB,
	//ȼ���С�� ��Ӽ��ȼ�յ��ף����ը 5��
	SKILL_STATE_FIRE,
	//����С�� ����һ�����������������С��
	SKILL_STATE_LIGHTNING,
	//������С�� ��������Ļ�����еļ�����˼�������� 7��
	SKILL_STATE_BLACKHOLE,
}SkillState;

typedef enum{
	//գ��
	ACTION_STATE_BLINK,
	//����ë
	ACTION_STATE_FEATHER,
	//��ը֮������
	ACTIOn_STATE_STAR,
	//�ƶ�
	ACTION_STATE_MOVE,
	//�ζ�
	ACTION_STATE_SHAKE,
	//������״̬
	ACTION_STATE_SKILL,
	//��������
	ACTION_STATE_FIRE,
	//�ڶ�
	ACTION_STATE_HOLE,
}ActionState;

typedef struct
{
	int row;
	int column;
}Address;

class GameLayer;

class BallSprite:public Sprite{
public:
	BallSprite();
	~BallSprite();

	static BallSprite* createBall(string birdName);

	static string createBallByRandom();

	string getName();

	/**
	*ִ��գ�۶���
	*/
	void blink();

	/**
	*��ë����
	*/
	void feather();

	void star();

	void bone();

	void burn();

	void fire();

	void blackHole();

	void changeTo(SkillState skillState);

	void MoveToAction(ActionInterval* action, const std::function<void(Node*)> &func, bool isYChange);

	void clearFeather();

	Address getAddress();

	void setAddress(int row, int column);

	bool getVisited();

	void setVisited(bool isVisited);

	void remove(bool isshake, SkillState skill);

	ActionState getActionState();

	SkillState getSkillState();

	bool isLast;

protected:

	bool init();
	
	/**
	*�ı䶯��״̬
	*/
	bool changeActionState(ActionState state);

	/**
	*�ı似��״̬
	*/
	bool changeSkillState(SkillState state);

private:
	//����
	SkillState skillState;
	//����
	ActionState actionState;

	//the bird name will be created by random
	string birdName;

	//the bird name format depends on the bird name we have rendom created before
	string featherName;

	Action* blinkAction;

	vector<Sprite*> feathers;

	Address address;

	bool isVisited;

	void changeToCallBack(SkillState skillState);
};
#endif