#include "MainScene.h"
#include "GameScene.h"
#include "MapGenerator.h"

Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

bool MainScene::init()
{
	//super init
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->createLoop("music/main.mp3");
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("mainScene.json");
	addChild(sceneNode);

	UINode = sceneNode->getChildByTag(10005);
	ExitNode = sceneNode->getChildByTag(10004);
	DialogNode = sceneNode->getChildByTag(10006);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("mainSceneUI");
	auto ExitComponent = (cocostudio::ComRender*) ExitNode->getComponent("exitSelectUI");
	auto DialogComponent = (cocostudio::ComRender*) DialogNode->getComponent("dialogBoxUI");
	auto UILayer = UIComponent->getNode();
	auto ExitLayer = ExitComponent->getNode();
	auto DialogLayer = DialogComponent->getNode();
	auto objectTable = dynamic_cast<ImageView*>(UILayer->getChildByTag(MAINSCENE_TABLE));
	auto objectPaper = dynamic_cast<Button*>(UILayer->getChildByTag(MAINSCENE_PAPER));
	auto objectShelf = dynamic_cast<Button*>(UILayer->getChildByTag(MAINSCENE_SHELF));
	auto objectClock = dynamic_cast<ImageView*>(UILayer->getChildByTag(MAINSCENE_CLOCK));
	auto objectCat = dynamic_cast<ImageView*>(UILayer->getChildByTag(MAINSCENE_CAT));
	auto objectCharacter = dynamic_cast<ImageView*>(UILayer->getChildByTag(MAINSCENE_CHARACTER));
	objectBox = dynamic_cast<ImageView*>(DialogLayer->getChildByTag(MAINSCENE_BOX));
	buttonOption = dynamic_cast<Button*>(UILayer->getChildByTag(MAINSCENE_OPTION));
	buttonHelp = dynamic_cast<Button*>(UILayer->getChildByTag(MAINSCENE_HELP));
	buttonExit = dynamic_cast<Button*>(UILayer->getChildByTag(MAINSCENE_EXIT));
	buttonCheck = dynamic_cast<Button*>(ExitLayer->getChildByTag(MAINSCENE_CHECKMARK));
	buttonCross = dynamic_cast<Button*>(ExitLayer->getChildByTag(MAINSCENE_CROSSMARK));
	labelWord = dynamic_cast<TextBMFont*>(DialogLayer->getChildByTag(MAINSCENE_WORD));
	objectTable->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	objectPaper->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	objectShelf->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	objectClock->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	objectCat->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	objectCharacter->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	buttonOption->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	buttonHelp->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	buttonExit->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	buttonCheck->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	buttonCross->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	objectBox->addTouchEventListener(this, toucheventselector(MainScene::touchEvent));
	objectBox->setEnabled(false);
	buttonCheck->setEnabled(false);
	buttonCross->setEnabled(false);
	return true;
}

void MainScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->play();
}

void MainScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->stop();
}

void MainScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MainScene::createDialog(std::string key)
{
	ValueMap strings = FileUtils::getInstance()->getValueMapFromFile("fonts/strings.xml");
	DialogNode->setVisible(true);
	objectBox->setEnabled(true);
	labelWord->setText(strings.at(key).asString().c_str());
}

void MainScene::touchEvent(Object* obj, gui::TouchEventType eventType)
{
	auto widget = dynamic_cast<Widget*>(obj);
	int tag = widget->getTag();
	std::string musicname = "music/" + FILENAME + ".mp3";
	std::string mapname = FileUtils::getInstance()->getWritablePath() + FILENAME + ".gnm";
	if (eventType == TouchEventType::TOUCH_EVENT_ENDED)
	{
		switch (tag)
		{
		case MAINSCENE_TABLE:
			this->createDialog("dialogTable");
			break;
		case MAINSCENE_PAPER:
			MapGenerator::generateMap(musicname.c_str());
			this->createDialog("dialogMapCreated");
			break;
		case MAINSCENE_SHELF:
			if (FileUtils::getInstance()->isFileExist(mapname))
			{
				auto scene = GameScene::createScene(FILENAME);
				Director::getInstance()->replaceScene(TransitionPageTurn::create(2, scene,true));
			}
			else
				this->createDialog("dialogNoMap");
			break;
		case MAINSCENE_CLOCK:
			this->createDialog("dialogClock");
			break;
		case MAINSCENE_CAT:
			this->createDialog("dialogCat");
			break;
		case MAINSCENE_OPTION:
			UserDefault::getInstance()->setIntegerForKey("difficulty", 0);
			break;
		case MAINSCENE_HELP:
			break;
		case MAINSCENE_EXIT:
			ExitNode->setVisible(true);
			buttonOption->setTouchEnabled(false);
			buttonHelp->setTouchEnabled(false);
			buttonExit->setTouchEnabled(false);
			buttonCheck->setEnabled(true);
			buttonCross->setEnabled(true);
			break;
		case MAINSCENE_CHECKMARK:
			Director::getInstance()->popScene();
			break;
		case MAINSCENE_CROSSMARK:
			ExitNode->setVisible(false);
			buttonOption->setTouchEnabled(true);
			buttonHelp->setTouchEnabled(true);
			buttonExit->setTouchEnabled(true);
			buttonCheck->setEnabled(false);
			buttonCross->setEnabled(false);
			break;
		case MAINSCENE_CHARACTER:
			this->createDialog("dialogCharacter");
			break;
		case MAINSCENE_BOX:
			DialogNode->setVisible(false);
			objectBox->setEnabled(false);
			break;
		}
	}
}