#include "cocos2d.h"
#include "AtlasLoader.h"
#include "Shake.h"
using namespace cocos2d;

const int SHAKE_TIME = 2.0f;

typedef enum{
	SKILL_STATE_NORMAL,
	SKILL_STATE_BOMB,//发光的小鸡 会爆破周围的6只鸡不等 4个 
	SKILL_STATE_FIRE, //燃火的小鸡 会从激活处燃烧到底，最后爆炸 5个
	SKILL_STATE_LIGHTNING,//闪电小鸡 犹如一道霹雳消除横竖向的小鸡
	SKILL_STATE_BLACKHOLE,//黑旋风小鸡 会吸走屏幕上所有的鸡，意思就是清屏 7个
}SkillState;

typedef enum{
	ACTION_STATE_BLINK,//眨眼
	ACTION_STATE_FEATHER,//掉羽毛
	ACTION_STATE_SHAKE,//晃动
	ACTION_STATE_SKILL,//带技能状态
}ActionState;

typedef struct
{
	int row;
	int column;
}Address;

class BallSprite:public Sprite{
public:
	BallSprite();
	~BallSprite();

	static BallSprite* createBall(string birdName);

	static string createBallByRandom();

	string getName();

	/**
	*执行眨眼动作
	*/
	void blink();

	void changeTo(SkillState skillState);

	void MoveToAction(ActionInterval* action, const std::function<void(Node*)> &func, bool isYChange);

	void clearFeather();

	Address getAddress();

	void setAddress(int row, int column);

	bool getVisited();

	void setVisited(bool isVisited);

	void remove(const std::function<void(Node*)> &func);

	ActionState getActionState();

	SkillState getSkillState();

protected:

	bool init();
	
	/**
	*改变动作状态
	*/
	bool changeActionState(ActionState state);

	/**
	*改变技能状态
	*/
	bool changeSkillState(SkillState state);

private:
	//技能
	SkillState skillState;
	//动作
	ActionState actionState;

	//the bird name will be created by random
	string birdName;

	//the bird name format depends on the bird name we have rendom created before
	string featherName;

	Action* blinkAction;

	vector<Sprite*> feathers;

	Address address;

	bool isVisited;

	void removeFeather(Node* sender);

	void changeToCallBack(SkillState skillState);
};