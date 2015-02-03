#include "cocos2d.h"
#include "AtlasLoader.h"
using namespace cocos2d;

typedef enum{
	ACTION_STATE_NORMAL,
	ACTION_STATE_EXPLODE,//爆炸 4个
	ACTION_STATE_FIRE, //下一列的小鸟或冰块被消除并且在列尾发生一次爆破 5个
	ACTION_STATE_LIGHTNING,//闪电鸟 同行同列消除 6个
	ACTION_STATE_WHIRLWIND,//全屏相同颜色消失 7个
}ActionState;

typedef enum{
	ACTION_STATE_BLINK,
	ACTION_STATE_FEATHER,
	ACTION_STATE_DIE
}State;

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
	bool changeState(State state);
	static string createBallByRandom();
	string getName();
	void blink();
	void MoveToAction(ActionInterval* action, bool isYChange);
	void clearFeather();
	Address getAddress();
	void setAddress(int row, int column);
	bool init();
private:
	//效果
	ActionState actionState;

	State state;

	//the bird name will be created by random
	string birdName;

	//the bird name format depends on the bird name we have rendom created before
	string featherName;

	Action* blinkAction;

	vector<Sprite*> feathers;

	Address address;

	void removeFeather(Node* sender);
};