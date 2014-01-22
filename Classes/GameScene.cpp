#include "GameScene.h"

Scene* GameScene::createScene()
{
	auto scene = Scene::createWithPhysics();//�������������������
	scene->getPhysicsWorld()->setGravity(Vect(0,-980));
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);//�򿪵��Կ���
	auto layer = GameScene::create();
	layer->setPhyWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

void GameScene::addNewSpriteAtPosition(Point p)
{
	auto sp = Sprite::create("1.png");
	sp->setTag(1);
	auto body = PhysicsBody::createBox(Size(80, 40));
	sp->setPhysicsBody(body);
	sp->setPosition(p);
	this->addChild(sp);
}

void GameScene::onTouchesEnded(const std::vector<Touch*>& touches, Event *event)
{
	for (auto touch : touches)
	{
		auto location = touch->getLocation();
		addNewSpriteAtPosition(location);
	}
}

void GameScene::onEnter()
{
	Layer::onEnter();
	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = [=](EventCustom* event, const PhysicsContact& contact)
	{
		auto sp = (Sprite*)contact.getShapeA()->getBody()->getNode();
		int tag = sp->getTag();
		if (tag == 1)
		{
			Texture2D *texture = TextureCache::getInstance()->addImage("2.png");
			sp->setTexture(texture);
		}

		sp = (Sprite*)contact.getShapeB()->getBody()->getNode();
		tag = sp->getTag();
		if (tag == 1)
		{
			Texture2D *texture = TextureCache::getInstance()->addImage("1.png");
			sp->setTexture(texture);
		}
		return true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 10);
}

bool GameScene::init()
{
	//super init
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////////////////////////////
	auto edgeSp = Sprite::create();
	auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 1);
	edgeSp->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	edgeSp->setPhysicsBody(body);
	this->addChild(edgeSp);
	edgeSp->setTag(0);
	setTouchEnabled(true);

	return true;
}

void GameScene::menuCloseCallback(Object* pSender)
{
	Director::getInstance()->end();

	if (m_world->getDebugDrawMask() != PhysicsWorld::DEBUGDRAW_NONE)
	{
		m_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
	}
	else
	{
		m_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}