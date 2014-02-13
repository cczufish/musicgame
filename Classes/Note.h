#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

enum NoteStatus{ UNTOUCHED_PRELOAD = 0, UNTOUCHED_DEADLINE = 1, TOUCHED_UNACTIVATED = 2, TOUCHED_ACTIVATED = 3 };//����״̬ö��

class Note : public cocos2d::Sprite
{
public:
	Note();
	virtual ~Note();
	static Note* createNote(int type, int length, int pos, int des);
	void removeNote(float dt);
	void judge();

	int getType();
	int getLength();
	int getDestX();
	int getDestY();
	int getLife();
	NoteStatus getStatus();
	void setStatus(NoteStatus status);
	void setLife(int life);
private:
	void update(float dt);
	void initNote(int type, int length, int pos, int des);
	int type;//0Ϊ��ͨ������1Ϊ����������2Ϊ��������
	int length;//����������
	int destX, destY;//����������Ŀ�ĵ�
	int life;//ʣ����������
	NoteStatus status;//��ǰ״̬

};

#endif 