#include "Note.h"
#include "GameScene.h"

const int TIME_PRELOAD = 60;//���ڷ�Ӧ��ʱ��


const int POS_X1 = 200;
const int POS_X2 = 320;
const int POS_X3 = 440;
const int POS_X4 = 560;
const int POS_X5 = 680;
const int POS_X6 = 800;
const int POS_X7 = 920;
const int POS_X8 = 1040;
const int POS_X9 = 1160;

const int POS_Y1 = 470;
const int POS_Y2 = 380;
const int POS_Y3 = 290;
const int POS_Y4 = 200;
const int POS_Y5 = 110;

Note::Note()
{
}

Note::~Note()
{
}

Note* Note::createNote(int type, int length,int pos, int des)
{
	Note *note = new Note();
	if (note)
	{
		note->initNote(type,length, pos, des);
		note->scheduleUpdate();
		note->autorelease();
		return note;
	}
	CC_SAFE_DELETE(note);
	return NULL;
}

void Note::initNote(int type,int length, int pos, int des)
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
	switch (pos / 10)
	{
	case 1:this->setPositionX(POS_X1); break;
	case 2:this->setPositionX(POS_X2); break;
	case 3:this->setPositionX(POS_X3); break;
	case 4:this->setPositionX(POS_X4); break;
	case 5:this->setPositionX(POS_X5); break;
	case 6:this->setPositionX(POS_X6); break;
	case 7:this->setPositionX(POS_X7); break;
	case 8:this->setPositionX(POS_X8); break;
	case 9:this->setPositionX(POS_X9); break;
	default: break;
	}
	switch (pos % 10)
	{
	case 1:this->setPositionY(POS_Y1); break;
	case 2:this->setPositionY(POS_Y2); break;
	case 3:this->setPositionY(POS_Y3); break;
	case 4:this->setPositionY(POS_Y4); break;
	case 5:this->setPositionY(POS_Y5); break;
	default: break;
	}
	switch (des / 10)
	{
	case 1:this->destX=POS_X1; break;
	case 2:this->destX=POS_X2; break;
	case 3:this->destX=POS_X3; break;
	case 4:this->destX=POS_X4; break;
	case 5:this->destX=POS_X5; break;
	case 6:this->destX=POS_X6; break;
	case 7:this->destX=POS_X7; break;
	case 8:this->destX=POS_X8; break;
	case 9:this->destX=POS_X9; break;
	default: break;
	}
	switch (des % 10)
	{
	case 1:this->destY = POS_Y1; break;
	case 2:this->destY = POS_Y2; break;
	case 3:this->destY = POS_Y3; break;
	case 4:this->destY = POS_Y4; break;
	case 5:this->destY = POS_Y5; break;
	default: break;
	}
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