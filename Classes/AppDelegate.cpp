#include "AppDelegate.h"
#include "GameDefine.h"
#include "GameScene.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview)
    {
        glview = GLViewImpl::createWithRect("FlappyBird", cocos2d::Rect(0, 0, GAME_WIDTH, GAME_HEIGHT));
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    //director->setDisplayStats(true);

    //// set FPS. the default value is 1.0/60 if you don't call this
    //director->setAnimationInterval(1.0f / 60);

    // create a scene. it's an autorelease object
    auto scene = GameScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
}
