#include "Note.h"
#include "GameScene.h"

const int TIME_PRELOAD = 30;//������ǰ���ֵ�ʱ��
const int TIME_DEADLINE = 30;//������������ӳٰ��µ�ʱ��

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
	this->length = length;
	this->status = UNTOUCHED_PRELOAD;
	switch (type)
	{
	case 0:
		this->initWithFile("gameSceneUI/note0.png");
		this->setScale(2);
		this->runAction(ScaleTo::create(TIME_PRELOAD / 60.0, 1));//������Ч
		break;
	case 1:
		this->initWithFile("gameSceneUI/note1.png");
		this->setScale(0.5);
		this->runAction(ScaleTo::create(TIME_PRELOAD / 60.0, 1));//������Ч
		break;
	case 2:
		this->initWithFile("gameSceneUI/note2.png");
		this->runAction(RotateBy::create(TIME_PRELOAD / 60.0, 360));//������Ч
		break;
	}
	this->setPositionX(120 * (pos / 10) + 80);
	this->setPositionY(60 * (10 - pos % 10) + 5);
	this->destX = 120 * (des / 10) + 80;
	this->destY = 60 * (10 - des % 10) + 5;
}

void Note::removeNote()
{
	this->removeFromParent();
}

void Note::update(float dt)
{
	life--;//��������
	if (life <= 0)
	{
		switch (status)
		{
		case UNTOUCHED_PRELOAD://Ԥ��ʱ����˻�û�д������ٵȴ��ӳ�ʱ��
			life = TIME_DEADLINE;
			status = UNTOUCHED_DEADLINE;
			if (type == 0)
				this->runAction(ScaleTo::create(TIME_DEADLINE / 60.0, 0));//����������Ч
			else if (type == 1)
				this->runAction(RotateBy::create(length / 60.0, 360));//����������Ч
			else
				this->runAction(MoveTo::create(length / 60.0, Point(destX, destY)));//����������Ч
			break;
		case UNTOUCHED_DEADLINE://�ȴ�����δ���������Ϊmiss
			GameScene::judgeNote(0);
			this->removeNote();
			break;
		case TOUCHED_UNACTIVATED://Ԥ��ʱ������Ѵ�������ʼ��������
			life = length;
			status = TOUCHED_ACTIVATED;
			if (type == 1)
				this->runAction(RotateBy::create(length / 60.0, 360));//����������Ч
			else
				this->runAction(MoveTo::create(length / 60.0, Point(destX, destY)));//����������Ч
			break;
		case TOUCHED_ACTIVATED://�������ڽ�������δֹͣ������ֱ��ȥ����
			this->judge();
			break;
		}
	}
}

void Note::judge()
{
	float lifePercent;

	this->stopAllActions();//ֹͣ���ж���
	this->unscheduleAllSelectors();//ֹͣ���м���
	this->runAction(Sequence::create(FadeOut::create(0.2f), CallFunc::create(CC_CALLBACK_0(Note::removeNote, this)), NULL));//��ʧ��Ч

	switch (this->getType())
	{
	case 0:
		if (status == TOUCHED_UNACTIVATED)
			lifePercent = (float)life / TIME_PRELOAD;
		else
			lifePercent = (float)(TIME_DEADLINE - life) / TIME_DEADLINE;
		if (lifePercent >= 0.4)//���е�ǰ��40%���ⶼֻ����GOOD
			GameScene::judgeNote(1);
		else
			GameScene::judgeNote(2);
		break;
	case 1:
		lifePercent = 1 - (float)life / length;
		if (status != TOUCHED_ACTIVATED || lifePercent <= 0.4)//�����Ĵ���ʱ��̫�����ж�Ϊmiss
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
int Note::getLength(){ return this->length; }
NoteStatus Note::getStatus(){ return this->status; }
void Note::setLife(int life){ this->life = life; }
void Note::setStatus(NoteStatus status){ this->status = status; }