#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

class Note : public cocos2d::Sprite
{
public:
	static Note* createAtPoint(Point p, int tag);
	void removeSelf(float dt);
	void update(float dt);
	int getLife();
	void setTouched();
private:
	int type;//0Ϊ��ͨ������1Ϊ����������2Ϊ��������
	int life;//ʣ��֡��
	bool touched;//�Ƿ񱻴�����
};

#endif 