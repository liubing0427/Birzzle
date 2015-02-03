#include "cocos2d.h"
#include "AtlasLoader.h"
using namespace cocos2d;

typedef enum{
	ACTION_STATE_NORMAL,
	ACTION_STATE_EXPLODE,//��ը 4��
	ACTION_STATE_FIRE, //��һ�е�С�����鱻������������β����һ�α��� 5��
	ACTION_STATE_LIGHTNING,//������ ͬ��ͬ������ 6��
	ACTION_STATE_WHIRLWIND,//ȫ����ͬ��ɫ��ʧ 7��
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
	//Ч��
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