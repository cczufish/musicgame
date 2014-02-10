#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

class Note : public cocos2d::Sprite
{
public:
	Note();
	virtual ~Note();
	static Note* createNote(int type, int length, int pos, int des);
	void removeNote(float dt);
	void judge();

	int getType();
	int getDestX();
	int getDestY();
	int getLife();
	int getLifeSpan();
	void setTouched();
	bool isTouched();
	bool isInLifeSpan();
private:
	void update(float dt);
	void initNote(int type, int length, int pos, int des);
	int type;//0Ϊ��ͨ������1Ϊ����������2Ϊ��������
	int destX, destY;//����������Ŀ�ĵ�
	bool touched;//�Ƿ񱻴�����
	bool inLifeSpan;//�Ƿ�������������
	int lifeSpan;//����������
	int life;//ʣ����������
};

#endif 