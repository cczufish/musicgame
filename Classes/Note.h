#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

class Note : public cocos2d::Sprite
{
public:
	Note();
	virtual ~Note();
	static Note* createNote(int type, int pos, int des);
	void removeNote(float dt);

	int getType();
	int getDest();
	int getLife();
	int getLifeSpan();
	int getLifeTouched();
	void setTouched();
	bool isTouched();
	void setNotMissed();
	bool isMissed();
private:
	void update(float dt);
	void initNote(int type, int pos, int des);

	int type;//0Ϊ��ͨ������1Ϊ����������2Ϊ��������
	int dest;//����������Ŀ�ĵ�
	bool touched;//�Ƿ񱻴������������򲻽��ܶ��δ����¼�
	bool missed;//�Ƿ񱻴��
	int lifeSpan;//����������
	int life;//ʣ����������
	int lifeTouched;//���ִ���״̬�ĳ���
};

#endif 