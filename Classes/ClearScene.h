#ifndef __CLEAR_SCENE_H__
#define __CLEAR_SCENE_H__

#include "CocosGUI.h"  
#include "cocostudio/CocoStudio.h"  
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h" 

USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;

class ClearScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(ClearScene);
private:
	void touchEvent(Object* obj, gui::TouchEventType eventType);
};

#endif 
