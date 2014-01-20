#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MainScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(MainScene);// implement the "static create()" method manually
	void addNewSpriteAtPosition(Point p);
	void setPhyWorld(PhysicsWorld* world){ m_world = world; }
	virtual void onEnter() override;
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)override;
private:
	PhysicsWorld* m_world;
};

#endif 
