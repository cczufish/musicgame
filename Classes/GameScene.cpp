#include "Global.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "Note.h"
#include <fstream>

const int TIME_PRELOAD = 60;//������ǰ���ֵ�ʱ��
const int DIFFICULTY = 0;//��ǰ�Ѷ�
const std::string FILENAME = "test.gnm";//������������

std::ifstream fin;//������
Noteline noteline;//��ǰ����
int framecounter;
int counterTotal, counterPerfect, counterGood, counterMiss, counterCombo, counterMaxcombo;
TextBMFont *labelInfo, *labelCombo, *labelJudge;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);

	framecounter = 0;//֡��������
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
	labelInfo->setText(FILENAME.substr(0, FILENAME.find('.')).c_str());
	return true;
}

void GameScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	/////////////////////////////////////////////////////
	AudioEngine::getInstance()->create("music/test.mp3");
	fin.open(FileUtils::getInstance()->fullPathForFilename(FILENAME));//�򿪲�������
	getNoteline();//��ȡ��һ��

	labelJudge->setText("Get Ready");
	labelJudge->runAction(FadeOut::create(2));
	Sprite* progress = Sprite::create("gameSceneUI/note.png");
	addChild(progress);
	progress->setPosition(-40, 605);
	progress->runAction(Sequence::create(MoveTo::create(3, Point(1340, 605)), CallFunc::create(CC_CALLBACK_0(GameScene::startGame, this)), NULL));//׼����Ч
}

void GameScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameScene::startGame()
{
	AudioEngine::getInstance()->play();
	this->scheduleUpdate();
	//this->schedule(schedule_selector(GameScene::addRandomNote), 120 / 115.65f);
}

void GameScene::update(float dt)
{
	framecounter++;
	while ((framecounter + TIME_PRELOAD >= noteline.time&&noteline.type != 0)
		|| (framecounter + TIME_PRELOAD / 2 >= noteline.time&&noteline.type == 0))//��ǰһЩ����
	{
		if (noteline.time == 0)break;//�����������
		if (DIFFICULTY >= noteline.difficulty)//��ǰ�Ѷȷ��������ɷ�������
			addNewNote(noteline.type, noteline.length, noteline.pos, noteline.des);
		getNoteline();//��ȡ�¸�����
	}
	if (!AudioEngine::getInstance()->isPlaying())//һ�׸�������л����������
	{
		this->unscheduleUpdate();
		fin.close();
		if (counterMaxcombo == 0 && counterMiss != counterTotal)
			counterMaxcombo = counterTotal;//ȫ����miss
		auto scene = ClearScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
	}
}

void GameScene::getNoteline()
{
	std::string notestring;
	if (getline(fin, notestring))
	{
		noteline.time = atoi(notestring.substr(0, 5).c_str());
		noteline.difficulty = atoi(notestring.substr(6, 7).c_str());
		noteline.type = atoi(notestring.substr(8, 9).c_str());
		noteline.length = atoi(notestring.substr(10, 13).c_str());
		noteline.pos = atoi(notestring.substr(14, 16).c_str());
		noteline.des = atoi(notestring.substr(17, 19).c_str());
	}
	else
		noteline.time = 0;//������ʶ��
}

void GameScene::addNewNote(int type, int length, int pos, int des)
{
	auto note = Note::createNote(type, length, pos, des);
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
	addChild(arrow);
	addChild(dest);
}

void GameScene::addRandomNote(float dt)
{
	int randomT = CCRANDOM_0_1() * 3;
	int randomX = CCRANDOM_0_1() * 8 + 1;
	int randomY = CCRANDOM_0_1() * 8 + 1;
	int randomA = CCRANDOM_0_1() * 8 + 1;
	int randomB = CCRANDOM_0_1() * 8 + 1;
	addNewNote(randomT, 60, randomX * 10 + randomY, randomA * 10 + randomB);
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
	//sprintf(temp, "%f", AudioEngine::getInstance()->getPosition());
	//labelCombo->setText(temp);
	labelJudge->runAction(FadeOut::create(1));
	labelCombo->runAction(FadeOut::create(1));//��ʧ��Ч
}

void GameScene::touchEvent(Object* obj, gui::TouchEventType eventType)
{
	auto button = dynamic_cast<Button*>(obj);
	int tag = button->getTag();
	switch (eventType)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (tag == GAMESCENE_PAUSE)
		{
			if (!Director::getInstance()->isPaused())
			{
				Director::getInstance()->pause();
				AudioEngine::getInstance()->pause();
			}
			else
			{
				Director::getInstance()->resume();
				AudioEngine::getInstance()->resume();
			}
		}
		break;
	}
}

