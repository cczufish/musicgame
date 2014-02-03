#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

class Note : public cocos2d::Sprite
{
public:
	Note();
	virtual ~Note();
	static Note* createNote(int posX, int posY, int type);
	void removeNote(float dt);

	int getType();
	int getLife();
	int getLifeSpan();
	int getLifeTouched();
	void setTouched();
	bool isTouched();
	void setTouchEnded();
	bool isTouchEnded();
private:
	void update(float dt);
	void initNote(int posX, int posY, int type);
	
	int type;//0Ϊ��ͨ������1Ϊ����������2Ϊ��������
	bool touched;//�Ƿ񱻴�����
	bool touchEnded;//�Ƿ����ѽ���
	int lifeSpan;//����������
	int life;//ʣ����������
	int lifeTouched;//���ִ���״̬�ĳ���
};

#endif 