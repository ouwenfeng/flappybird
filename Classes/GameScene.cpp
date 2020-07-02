#include "GameScene.h"

Scene *GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	//随机种子//
	srand((int)time(0));
	//背景//
	auto bgSprite = Sprite::create("bg_day.png");
	bgSprite->setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	this->addChild(bgSprite);

	//地板//
	auto land1 = Sprite::create("land.png");
	land1->setAnchorPoint(Point::ZERO);
	land1->setPosition(Point::ZERO);
	land1->setName("land1");
	this->addChild(land1);
	auto land2 = Sprite::create("land.png");
	land2->setAnchorPoint(Point::ZERO);
	land2->setPosition(Point::ZERO);
	land2->setName("land2");
	this->addChild(land2);

	//设置初始变量//
	gameStatus = GAME_INIT;
	score = 0;

	//标题//
	auto title = Sprite::create("title.png");
	title->setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2 + 100);
	title->setName("title");
	this->addChild(title);

	//小鸟//
	auto birdSprite = Sprite::create("bird1.png");
	birdSprite->setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2 + 50);
	birdSprite->setVisible(false);
	birdSprite->setName("birdSprite");
	this->addChild(birdSprite, 9);

	//小鸟飞帧动画//
	animFrames.pushBack(SpriteFrame::create("bird1.png", Rect(0, 0, 38, 27)));
	animFrames.pushBack(SpriteFrame::create("bird2.png", Rect(0, 0, 38, 27)));
	animFrames.pushBack(SpriteFrame::create("bird3.png", Rect(0, 0, 38, 27)));

	//上下抖动//
	//auto up = MoveBy::create(0.4f, Point(0, 8));
	//swingAction = RepeatForever::create(Sequence::create(up, up->reverse(), NULL));
	//birdSprite->runAction(swingAction);

	//开始按钮//
	auto beginSprite = Sprite::create("button_play.png");
	beginSprite->setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	beginSprite->setName("beginSprite");
	//绑定事件//
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBeganButton, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, beginSprite);
	this->addChild(beginSprite, 10);

	//分数面板//
	auto scoreSprite = Sprite::create("score.png");
	scoreSprite->setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	scoreSprite->setName("scoreSprite");
	scoreSprite->setVisible(false);
	this->addChild(scoreSprite, 9);
	//结束text//
	auto textGameOver = Sprite::create("text_game_over.png");
	textGameOver->setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2 + 100);
	textGameOver->setName("textGameOver");
	textGameOver->setVisible(false);
	this->addChild(textGameOver, 10);
	//分数//
	auto scoreLabel = Label::createWithSystemFont("0", "黑体", 20);
	scoreLabel->setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2 + 10);
	scoreLabel->setName("scoreLabel");
	scoreLabel->setVisible(false);
	this->addChild(scoreLabel, 10);
	//最好分数//
	auto maxScoreLabel = Label::createWithSystemFont("0", "黑体", 20);
	maxScoreLabel->setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2 - 30);
	maxScoreLabel->setName("maxScoreLabel");
	maxScoreLabel->setVisible(false);
	this->addChild(maxScoreLabel, 10);
	//显示在游戏中的分数//
	auto nowScoreLabel = Label::createWithSystemFont("0", "黑体", 50);
	nowScoreLabel->setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2 + 200);
	nowScoreLabel->setName("nowScoreLabel");
	nowScoreLabel->setVisible(false);
	this->addChild(nowScoreLabel, 10);
	//绑定事件//
	auto eventListener = EventListenerTouchOneByOne::create();
	eventListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchScene, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
	//启动定时器//
	this->scheduleUpdate();

	return true;
}
bool GameScene::onTouchScene(cocos2d::Touch *touch, cocos2d::Event *event)
{
	if (gameStatus == GAME_START)
	{
		log("click Scene");
		//每点击一次速度重新设定//
		downSpeed = 0;
		upSpeed = 10;
		//点击动作//
		auto rotateUp = RotateTo::create(0.1f, -40.0f);
		auto stop = RotateTo::create(0.4f, -40.0f);
		auto rotateDown = RotateTo::create(0.1f, 40.0f);
		auto touchActionSeq = Sequence::create(rotateUp, stop, rotateDown, nullptr);
		auto birdSprite = this->getChildByName("birdSprite");
		birdSprite->runAction(touchActionSeq);
		//上升加速度事件//
		this->schedule(schedule_selector(GameScene::birdUpAction), 0.0f);
	}
	return true;
}

bool GameScene::onTouchBeganButton(cocos2d::Touch *touch, cocos2d::Event *event)
{
	auto bounds = event->getCurrentTarget()->getBoundingBox();
	if (bounds.containsPoint(touch->getLocation()))
	{
		log("click BeganButton");
		if (gameStatus == GAME_INIT)
		{
			this->removeChildByName("title");
			auto birdSprite = this->getChildByName("birdSprite");
			birdSprite->setVisible(true);
			gameStart();
		}
		if (gameStatus == GAME_OVER)
		{
			gameRestart();
		}
	}
	return true;
}

void GameScene::gameRestart()
{
	//小鸟位置重新设定//
	auto birdSprite = this->getChildByName("birdSprite");
	birdSprite->setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2 + 50);

	//显示开始按钮//
	auto beginSprite = this->getChildByName("beginSprite");
	beginSprite->setVisible(false);
	//分数重新设定//
	Label *nowScoreLabel = (Label *)this->getChildByName("nowScoreLabel");
	auto scoreLabel = this->getChildByName("scoreLabel");
	auto maxScoreLabel = this->getChildByName("maxScoreLabel");
	score = 0;
	nowScoreLabel->setVisible(true);
	scoreLabel->setVisible(false);
	maxScoreLabel->setVisible(false);
	nowScoreLabel->setString(StringUtils::format("%d", score));

	this->getChildByName("textGameOver")->setVisible(false);
	this->getChildByName("scoreSprite")->setVisible(false);
	//管道移除//
	for (auto &singlePipe : pipes)
	{
		this->removeChild(singlePipe);
	}

	gameStart();
}
void GameScene::gameStart()
{

	//隐藏开始按钮//
	auto beginSprite = this->getChildByName("beginSprite");
	beginSprite->setVisible(false);
	//小鸟飞行//
	auto birdSprite = this->getChildByName("birdSprite");
	auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	auto animate = Animate::create(animation);
	auto swingAnimate = RepeatForever::create(animate);
	swingAnimate->setTag(99);
	birdSprite->runAction(swingAnimate);

	Label *nowScoreLabel = (Label *)this->getChildByName("nowScoreLabel");
	nowScoreLabel->setVisible(true);
	//设置管道按钮//
	pipes.clear();
	for (int i = 0; i < 2; i++)
	{
		int r = rand() % PIPE_VARIATION_RANGE;
		auto pipeUp = Sprite::create("pipe_up.png");
		pipeUp->setPosition(PIPE_START_WIDTH + i * PIPE_BETWEEN_WIDTH, GAME_HEIGHT - PIPE_SPACE + r);
		pipeUp->setName("newPipe");
		auto pipeDown = Sprite::create("pipe_down.png");
		pipeDown->setPosition(PIPE_START_WIDTH + i * PIPE_BETWEEN_WIDTH, r);
		pipeDown->setName("newPipe");
		this->addChild(pipeUp);
		this->addChild(pipeDown);
		pipes.pushBack(pipeUp);
		pipes.pushBack(pipeDown);
	}
	gameStatus = GAME_START;
}

void GameScene::gameOver()
{
	//显示分数面版//
	this->getChildByName("textGameOver")->setVisible(true);
	this->getChildByName("scoreSprite")->setVisible(true);

	Label *nowScoreLabel = (Label *)this->getChildByName("nowScoreLabel");
	Label *scoreLabel = (Label *)this->getChildByName("scoreLabel");
	Label *maxScoreLabel = (Label *)this->getChildByName("maxScoreLabel");
	scoreLabel->setVisible(true);
	maxScoreLabel->setVisible(true);
	nowScoreLabel->setVisible(false);
	scoreLabel->setString(StringUtils::format("%d", score));

	if (UserDefault::getInstance()->getIntegerForKey("maxScore") < score)
	{
		maxScoreLabel->setString(StringUtils::format("%d", score));
		UserDefault::getInstance()->setIntegerForKey("maxScore", score);
	}
	else
	{
		maxScoreLabel->setString(StringUtils::format("%d", UserDefault::getInstance()->getIntegerForKey("maxScore")));
	}

	//停止飞动作//
	auto birdSprite = this->getChildByName("birdSprite");
	birdSprite->stopAction(birdSprite->getActionByTag(99));

	auto beginSprite = this->getChildByName("beginSprite");
	beginSprite->setVisible(true);
	beginSprite->setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2 - 100);

	gameStatus = GAME_OVER;
}

void GameScene::birdUpAction(float delta)
{
	upSpeed--;
	if (upSpeed < 0)
	{
		this->unschedule(schedule_selector(GameScene::birdUpAction));
	}
	auto birdSprite = this->getChildByName("birdSprite");
	birdSprite->setPositionY(birdSprite->getPositionY() + upSpeed * 1.0f);
}

void GameScene::update(float delta)
{
	//每一帧下落速度都增加//
	downSpeed++;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto land1 = this->getChildByName("land1");
	auto land2 = this->getChildByName("land2");
	auto birdSprite = this->getChildByName("birdSprite");
	//游戏中//
	if (gameStatus == GAME_START)
	{
		//两个地板向左平移//
		land1->setPositionX(land1->getPositionX() - 1.0f);
		land2->setPositionX(land1->getPositionX() + land1->getContentSize().width - 2.0f);
		if (land2->getPositionX() <= 0)
		{
			land1->setPosition(Point::ZERO);
		}
		//小鸟重力//
		birdSprite->setPositionY(birdSprite->getPositionY() - downSpeed * 1.0f / 10);

		int index = 0;
		int r = 100;
		//管道移动和生成//
		for (auto &singlePipe : pipes)
		{
			singlePipe->setPositionX(singlePipe->getPositionX() - 1.0f);
			if (singlePipe->getName() == "newPipe")
			{
				if (birdSprite->getPositionX() > singlePipe->getPositionX())
				{
					score++;
					Label *nowScoreLabel = (Label *)this->getChildByName("nowScoreLabel");
					nowScoreLabel->setString(StringUtils::format("%d", score));
					singlePipe->setName("passed");
				}
			}
			if (singlePipe->getPositionX() < -PIPE_WIDTH / 2)
			{
				singlePipe->setPositionX(visibleSize.width + PIPE_WIDTH / 2);
				singlePipe->setName("newPipe");
				if (index % 2 == 0)
				{
					r = rand() % PIPE_VARIATION_RANGE;
					singlePipe->setPositionY(GAME_HEIGHT - PIPE_SPACE + r);
				}
				else
				{
					singlePipe->setPositionY(r);
				}
			}
			index++;
		}
		//碰撞检测//
		//地板
		if (birdSprite->boundingBox().intersectsRect(land2->boundingBox()) || birdSprite->boundingBox().intersectsRect(land1->boundingBox()))
		{
			gameOver();
		}
		//管道
		for (auto &singlePipe : pipes)
		{
			if (birdSprite->boundingBox().intersectsRect(singlePipe->boundingBox()) || birdSprite->boundingBox().intersectsRect(singlePipe->boundingBox()))
			{
				gameOver();
			}
		}
		if (birdSprite->getPositionY() > GAME_HEIGHT)
		{
			gameOver();
		}
	}
	//游戏结束时掉落地板//
	if (gameStatus == GAME_OVER)
	{
		if (!birdSprite->boundingBox().intersectsRect(land2->boundingBox()) && !birdSprite->boundingBox().intersectsRect(land1->boundingBox()))
		{
			birdSprite->setPositionY(birdSprite->getPositionY() - downSpeed * 1.0f);
		}
	}
}