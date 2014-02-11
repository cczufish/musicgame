#include "Note.h"
#include "GameScene.h"

const int TIME_PRELOAD = 60;//���ڷ�Ӧ��ʱ��

Note::Note()
{
}

Note::~Note()
{
}

Note* Note::createNote(int type, int length, int pos, int des)
{
	Note *note = new Note();
	if (note)
	{
		note->initNote(type, length, pos, des);
		note->scheduleUpdate();
		note->autorelease();
		return note;
	}
	CC_SAFE_DELETE(note);
	return NULL;
}

void Note::initNote(int type, int length, int pos, int des)
{
	this->type = type;
	this->life = TIME_PRELOAD;
	this->lifeSpan = length;
	this->touched = false;
	this->inLifeSpan = false;
	switch (type)
	{
	case 0:
		this->initWithFile("gameSceneUI/note0.png");
		//this->setScale(2.5);
		//this->runAction(ScaleTo::create(life / 60.0, 0));//������Ч
		break;
	case 1:
		this->initWithFile("gameSceneUI/note1.png");
		this->runAction(ScaleTo::create(life / 60.0, 1.25));//������Ч
		break;
	case 2:
		this->initWithFile("gameSceneUI/note2.png");
		this->runAction(RotateBy::create(life / 60.0, 360));//������Ч
		break;
	}
	this->setPositionX(120 * (pos / 10) + 80);
	this->setPositionY(60 * (pos % 10) + 5);
	this->destX = 120 * (des / 10) + 80;
	this->destY = 60 * (des % 10) + 5;
}

void Note::removeNote(float dt)
{
	this->removeFromParentAndCleanup(true);
}

void Note::update(float dt)
{
	life--;//��������
	if (life <= 0)
	{
		if (!touched)//���������û����������϶���miss
		{
			GameScene::judgeNote(0);
			this->removeFromParentAndCleanup(true);//ɾ����note
		}
		else if (!inLifeSpan)//����ʼ��������
		{
			life = lifeSpan;
			inLifeSpan = true;
			if (type == 1)
				this->runAction(RotateBy::create(lifeSpan / 60.0, 360));//����������Ч
			else
				this->runAction(MoveTo::create(lifeSpan / 60.0, Point(destX, destY)));//����������Ч
		}
		else//�������ڽ�����δ���־�ȥ����
		{
			this->judge();
			this->removeFromParentAndCleanup(true);//ɾ����note
		}
	}
}

void Note::judge()
{
	float lifePercent;
	switch (this->getType())
	{
	case 0:
		lifePercent = (float)this->getLife() / TIME_PRELOAD;
		if (lifePercent >= 0.8 || lifePercent <= 0.4)//̫���̫��������ֻ����good
			GameScene::judgeNote(1);
		else
			GameScene::judgeNote(2);
		break;
	case 1:
		lifePercent = 1 - (float)life / lifeSpan;
		if (!inLifeSpan || lifePercent <= 0.4)//�����Ĵ���ʱ��̫�����ж�Ϊmiss
			GameScene::judgeNote(0);
		else if (lifePercent <= 0.8)//�����Ĵ���ʱ�䲻������good
			GameScene::judgeNote(1);
		else
			GameScene::judgeNote(2);
		break;
	case 2:
		break;
	}
}

int Note::getType(){ return this->type; }
int Note::getDestX(){ return this->destX; }
int Note::getDestY(){ return this->destY; }
int Note::getLife(){ return this->life; }
int Note::getLifeSpan(){ return this->lifeSpan; }
void Note::setTouched(){ this->touched = true; }
bool Note::isTouched(){ return this->touched; }
bool Note::isInLifeSpan(){ return this->inLifeSpan; }