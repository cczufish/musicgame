#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "AudioEngine.h" 
#include "cocostudio/CocoStudio.h"  
#include "CocosGUI.h"  

USING_NS_CC;
using namespace gui;

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(GameScene);
	static void judgeNote(int judge);
private:
	void update(float dt);
	void addNewNote(int type, int length, int pos, int des);//������note
	void addRandomNote(float dt);//�������note
	void addArrow(int posX, int posY, int desX, int desY);//Ϊ����note���ɼ�ͷ��Ŀ���
	void getNoteline();//�õ��µ�һ������
	void startGame();//��ʼ��Ϸ
	void touchEvent(Object* obj, TouchEventType eventType);
};

struct Noteline//���׽ṹ
{
	int time;//ʱ��
	int difficulty;//�Ѷ�
	int type;//����
	int length;//�������뻬��������������
	int pos;//����λ��
	int des;//��������Ŀ��λ��
};

#endif 
