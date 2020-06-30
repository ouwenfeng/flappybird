#include "cocos2d.h"
#include "GameDefine.h"

USING_NS_CC;

class GameScene : public Scene
{
public:
	static Scene *createScene();

	virtual bool init();

	CREATE_FUNC(GameScene);

	virtual void update(float delta);
	virtual bool onTouchBeganButton(Touch *touch, Event *event);
	virtual bool onTouchScene(Touch *touch, Event *event);
	virtual void birdUpAction(float delta);

private:
	void gameStart();
	void gameOver();
	void gameRestart();
	Vector<Sprite *> pipes;
	Vector<SpriteFrame *> animFrames;
	int gameStatus;
	int downSpeed;
	int upSpeed;
	int score;
};
