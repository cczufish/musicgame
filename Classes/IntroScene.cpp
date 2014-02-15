#include "Global.h"
#include "IntroScene.h"
#include "MainScene.h"

Scene* IntroScene::createScene()
{
	auto scene = Scene::create();
	auto layer = IntroScene::create();
	scene->addChild(layer);
	return scene;
}

bool IntroScene::init()
{
	//super init
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////////////////////////////
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/intro.mp3");
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("introScene.json");
	addChild(sceneNode);
	auto UINode = sceneNode->getChildByTag(10003);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("GUIComponent");
	auto UIlayer = UIComponent->getNode();
	auto buttonStart = dynamic_cast<Button*>(UIlayer->getChildByTag(INTROSCENE_START));
	buttonStart->addTouchEventListener(this, toucheventselector(IntroScene::touchEvent));

	auto logo = Sprite::create("introSceneUI/logo.png");
	logo->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	logo->setLocalZOrder(1);
	sceneNode->addChild(logo);
	logo->runAction(Sequence::create(DelayTime::create(1), CallFunc::create(CC_CALLBACK_0(IntroScene::playIntro, this)), FadeOut::create(2), NULL));//logo�л���Ч
	return true;
}

void IntroScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void IntroScene::playIntro()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/intro.mp3");
}

void IntroScene::touchEvent(Object* obj, gui::TouchEventType eventType)
{
	auto button = dynamic_cast<Button*>(obj);
	int tag = button->getTag();
	switch (eventType)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (tag == INTROSCENE_START)
		{
			auto scene = MainScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
		}
		break;
	}
}