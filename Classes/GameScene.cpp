#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MoveBall.h"
#include "RandomBall.h"

USING_NS_CC;

using namespace cocostudio::timeline;

cocos2d::Scene* GameScene::createScene()
{
	auto scene = Scene::create();

	auto layer = GameScene::create();

	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	///**  you can create scene with following comment code instead of using csb file.
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(255,255,255,255)))
	{
		return false;
	}

	auto sprite = Sprite::create("meng.jpg");
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(GameScene::menuCloseCallback, this));
	closeItem->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu,1);

	auto changeBall = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(GameScene::chanScene, this));
	changeBall->setPosition(Vec2(Vec2(visibleSize.width / 2 + 20, visibleSize.height / 2 + 20)));
	auto chan = Menu::create(changeBall, NULL);
	chan->setPosition(Vec2::ZERO);
	this->addChild(chan, 1);

	//这里直接添加自己继承的精灵
	for (int i = 0; i < 30; i++)
	{
		auto b = RandomBall::create();
		b->setPosition(CCRANDOM_0_1() * 200 + 100, CCRANDOM_0_1() * 200 + 100);
		addChild(b, 1);
	}

		/*	auto closeItem = MenuItemImage::create(
				"CloseNormal.png",
				"CloseSelected.png",
				CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));*/
	
	sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	addChild(sprite);
	return true;
}

void GameScene::menuCloseCallback(Ref * pRef)
{
	//auto ballScene = MoveBall::createScene();
	//auto transition = TransitionZoomFlipX::create(2.0f, ballScene);
	//Director::getInstance()->replaceScene(transition);
	Director::getInstance()->end();
}

void GameScene::chanScene(Ref * pRef)
{
	//auto Scene = GameScene::createScene();
	//auto transition = TransitionZoomFlipY::create(1.0f, Scene);
	//Director::getInstance()->replaceScene(transition);

	//一定要自己动手动脑子写
	auto ballScene = MoveBall::createScene();
	auto transition = TransitionZoomFlipX::create(2.0f, ballScene);
	Director::getInstance()->replaceScene(transition);

}