#include "Global.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "Note.h"
#include <fstream>

const int TIME_PRELOAD = 60;//���ڷ�Ӧ��ʱ��
const std::string FILENAME = "test.gnm";//������������

std::ifstream fin;
int framecounter;
int counterTotal, counterPerfect, counterGood, counterMiss, counterCombo, counterMaxcombo;
TextBMFont *labelInfo, *labelCombo, *labelJudge;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);

	framecounter;//֡��������
	counterTotal = 0;//��������
	counterPerfect = 0;//������
	counterGood = 0;//��ͨ������
	counterMiss = 0;//�����
	counterCombo = 0;//������
	counterMaxcombo = 0;//���������
	return scene;
}

bool GameScene::init()
{
	//super init
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////////////////////////////
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/game.mp3");
	auto sceneNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("gameScene.json");
	sceneNode->setLocalZOrder(-1);
	addChild(sceneNode);
	auto UINode = sceneNode->getChildByTag(10004);
	auto UIComponent = (cocostudio::ComRender*) UINode->getComponent("GUIComponent");
	auto UIlayer = UIComponent->getNode();
	auto buttonPause = dynamic_cast<Button*>(UIlayer->getChildByTag(GAMESCENE_PAUSE));
	buttonPause->addTouchEventListener(this, toucheventselector(GameScene::touchEvent));
	labelInfo = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(GAMESCENE_INFO));
	labelCombo = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(GAMESCENE_COMBO));
	labelJudge = dynamic_cast<TextBMFont*>(UIlayer->getChildByTag(GAMESCENE_JUDGE));
	labelInfo->setText(FILENAME.substr(0,FILENAME.find('.')).c_str());
	fin.open(FileUtils::getInstance()->getWritablePath() + FILENAME);//�򿪲�������
	return true;
}

void GameScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	/////////////////////////////////////////////////////
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/game.mp3");
	this->scheduleUpdate();
}

void GameScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameScene::update(float dt)
{
	framecounter++;
	std::string notefile;
	fin.clear();
	fin.seekg(0);
	while (getline(fin, notefile))//ÿ�ζ�����һ���ѵ�����Ӱ������ô������Ҫ��vector����İɡ�
	{
		int type = atoi(notefile.substr(0, 1).c_str());
		int time = atoi(notefile.substr(2, 6).c_str());
		int length = atoi(notefile.substr(8, 10).c_str());
		int pos = atoi(notefile.substr(12, 13).c_str());
		int des = atoi(notefile.substr(15, 16).c_str());
		if (framecounter + TIME_PRELOAD *0.6 == time)//��ǰһ�����ɸ�NOTE
			addNewNote(type, length, pos, des);
	}
	/*switch (framecounter % 720)//������ɵ�note�����Űɡ�
	{
	case 180:addRandomNote(2); break;
	case 360:addRandomNote(1); break;
	case 540:addRandomNote(0); break;
	}*/
	if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())//һ�׸�������л����������
	{
		this->unscheduleUpdate();
		if (counterMaxcombo == 0 && counterMiss != counterTotal)
			counterMaxcombo = counterTotal;//ȫ����miss
		auto scene = ClearScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
	}
}

void GameScene::touchEvent(Object* obj, gui::TouchEventType eventType)
{
	auto button = dynamic_cast<Button*>(obj);
	int tag = button->getTag();
	switch (eventType)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (tag == GAMESCENE_PAUSE)
		{//��ʱ����ͣ��Ϊ��ת�����������
			this->unscheduleUpdate();
			if (counterMaxcombo == 0 && counterMiss != counterTotal)
				counterMaxcombo = counterTotal;//ȫ����miss
			auto scene = ClearScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
			/*if (!Director::getInstance()->isPaused())
			{
			Director::getInstance()->pause();
			CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
			}
			else
			{
			Director::getInstance()->resume();
			CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
			}*/
		}
		break;
	}
}

void GameScene::addNewNote(int type, int length, int pos, int des)
{
	auto note = Note::createNote(type, length, pos, des);
	auto noteListener = EventListenerTouchOneByOne::create();
	noteListener->setSwallowTouches(true);
	noteListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	noteListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	noteListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(noteListener, note);
	addChild(note);
	if (note->getType() == 2)
		addArrow(note->getPositionX(), note->getPositionY(), note->getDestX(), note->getDestY());
}

void GameScene::addArrow(int posX, int posY, int desX, int desY)
{
	auto arrow = Sprite::create("gameSceneUI/arrow.png");
	auto dest = Sprite::create("gameSceneUI/note.png");
	arrow->setPosition(posX / 2 + desX / 2, posY / 2 + desY / 2);
	dest->setPosition(desX, desY);
	arrow->setRotation(atan2(desX - posX, desY - posY) * 180 / M_PI);
	dest->runAction(FadeOut::create(2));
	arrow->runAction(FadeOut::create(2));//��ʧ��Ч
	this->addChild(arrow);
	this->addChild(dest);
}

void GameScene::addRandomNote(int type)
{
	int randomX = CCRANDOM_0_1() * 6 + 1;
	int randomY = CCRANDOM_0_1() * 5 + 1;
	int randomA = CCRANDOM_0_1() * 6 + 1;
	int randomB = CCRANDOM_0_1() * 5 + 1;
	addNewNote(type, 120, randomX * 10 + randomY, randomA * 10 + randomB);
}

bool GameScene::onTouchBegan(Touch *touch, Event  *event)
{
	auto target = static_cast<Note*>(event->getCurrentTarget());
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode) && !Director::getInstance()->isPaused() && !target->isTouched())
	{
		target->setTouched();//��Ϊ������
		if (target->getType() == 0)//����ͨnote��ֱ�ӽ����ж�
		{
			target->stopAllActions();
			target->unscheduleAllSelectors();
			target->runAction(RotateBy::create(0.2f, 360));//��ʧ��Ч
			target->scheduleOnce(schedule_selector(Note::removeNote), 0.2f);
			target->judge();
		}
	}
	return true;
}
void GameScene::onTouchMoved(Touch *touch, Event  *event)
{
	auto target = static_cast<Note*>(event->getCurrentTarget());
	Size s = target->getContentSize();
	Point pos = touch->getLocation();
	Point t = target->getPosition();
	Rect rect = Rect(pos.x - s.width / 2, pos.y - s.height / 2, s.width, s.height);
	Rect rect2 = Rect(pos.x - s.width, pos.y - s.height, s.width * 2, s.height * 2);
	if (!Director::getInstance()->isPaused() && target->getType() == 2 && target->isInLifeSpan() && target->getLife() % (target->getLifeSpan() / 3) == 0)
	{
		if (rect.containsPoint(target->getPosition()))//perfect
			judgeNote(2);
		else if (rect2.containsPoint(target->getPosition()))//good
			judgeNote(1);
		else//miss 
			judgeNote(0);
	}
}
void GameScene::onTouchEnded(Touch *touch, Event  *event)
{
	auto target = static_cast<Note*>(event->getCurrentTarget());
	if (!Director::getInstance()->isPaused() && target->getType() != 0)
	{//�뿪ʱ�����ж�
		target->stopAllActions();
		target->unscheduleAllSelectors();
		target->runAction(FadeOut::create(0.2f));//��ʧ��Ч
		target->scheduleOnce(schedule_selector(Note::removeNote), 0.2f);
		target->judge();
	}
}
void GameScene::judgeNote(int judge)
{
	counterTotal++;
	char temp[64];
	switch (judge)
	{
	case 0:
		if (counterMaxcombo < counterCombo)
			counterMaxcombo = counterCombo;
		counterCombo = 0;
		counterMiss++;
		labelJudge->setText("Miss!");
		labelCombo->setText("");
		break;
	case 1:
		counterCombo++;
		counterGood++;
		labelJudge->setText("Good!");
		sprintf(temp, "%d", counterCombo);
		labelCombo->setText(temp);
		break;
	case 2:
		counterCombo++;
		counterPerfect++;
		labelJudge->setText("Perfect!");
		sprintf(temp, "%d", counterCombo);
		labelCombo->setText(temp);
		break;
	}
	labelJudge->setVisible(true);
	labelCombo->setVisible(true);
	labelJudge->runAction(Sequence::create(ScaleTo::create(0.2f, 1.25), ScaleTo::create(0.2f, 1), NULL));
	labelCombo->runAction(FadeOut::create(1));//��ʧ��Ч
}

