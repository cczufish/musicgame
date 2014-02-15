#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

enum NoteType{CLICK=0,LONGPRESS=1,SLIDE=2};//��������ö��
enum NoteStatus{ UNTOUCHED_PRELOAD = 0, UNTOUCHED_DEADLINE = 1, TOUCHED_UNACTIVATED = 2, TOUCHED_ACTIVATED = 3 };//����״̬ö��

class Note : public cocos2d::Sprite
{
public:
	Note();
	virtual ~Note();
	static Note* createNote(int type, int length, int pos, int des);
	void removeNote();
	void judge();

	int getLength();
	int getDestX();
	int getDestY();
	int getLife();
	NoteType getType();
	NoteStatus getStatus();
	void setStatus(NoteStatus status);
	void setLife(int life);
	void setTouchPoint(Point point);
private:
	void update(float dt);
	void initNote(int type, int length, int pos, int des);
	NoteType type;//��������
	NoteStatus status;//��ǰ״̬
	Point touchpoint;//������
	int length;//����������
	int destX, destY;//����������Ŀ�ĵ�
	int life;//ʣ����������
	

};

#endif 