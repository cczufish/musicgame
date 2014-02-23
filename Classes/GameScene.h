#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "CocosGUI.h"  
#include "cocostudio/CocoStudio.h"   

#include "Global.h"
#include "AudioEngine.h" 

USING_NS_CC;
using namespace gui;

class GameScene : public Layer
{
public:
	static Scene* createScene(std::string filename);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExitTransitionDidStart();
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
