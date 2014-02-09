#include "AppDelegate.h"
#include "IntroScene.h"
#include "GameScene.h"

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
	auto director = Director::getInstance();
	auto eglView = EGLView::getInstance();
	director->setOpenGLView(eglView);
	director->setDisplayStats(true);
	director->setAnimationInterval(1.0 / 60);//����֡��
	auto scene = IntroScene::createScene();
	director->runWithScene(scene);//���г���
	return true;
}
//�����̨
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}
//�л�ǰ̨
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
