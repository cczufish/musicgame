#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

enum NoteType{ CLICK = 0, LONGPRESS = 1, SLIDE = 2 };//��������ö��
enum NoteStatus{ UNTOUCHED = 0, TOUCHED_UNACTIVATED = 1, TOUCHED_ACTIVATED = 2 };//����״̬ö��

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
	void setTouchPoint(Point point);
	void setLifeTouchBegan(int lifeTouchBegan);
private:
	void update(float dt);
	void initNote(int type, int length, int pos, int des);
	NoteType type;//��������
	NoteStatus status;//��ǰ״̬
	Point touchpoint;//������
	int length;//����������
	int destX, destY;//����������Ŀ�ĵ�
	int life;//ʣ����������
	int lifeTouchBegan;//������ʼʱ������ֵ
};

#endif 