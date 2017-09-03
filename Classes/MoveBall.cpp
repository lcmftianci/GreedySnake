#include "MoveBall.h"


Scene* MoveBall::createScene()
{
	auto pScene = Scene::create();
	auto childScene = MoveBall::create();
	pScene->addChild(childScene);
	return pScene;
}

bool MoveBall::init()
{
	LayerColor::initWithColor(Color4B(255, 255, 255, 255));

	for (auto num = 0; num < 100; num++)
	{
		//Sleep(1000);
		AddBall(num + 100, num + 100);
	}

	for (auto num = 0; num < 100; num++)
	{
		//Sleep(1000);
		AddBall(num + 100, num + 200);
	}

	for (auto num = 0; num < 100; num++)
	{
		//Sleep(1000);
		AddBall(num + 100, num + 300);
	}
	scheduleUpdate();
	return true;
}
void MoveBall::update(float dt)
{
	//ball->move();
	for (auto it = balls.begin(); it != balls.end(); it++)
	{
		(*it)->move();
	}
}

void MoveBall::AddBall(float x, float y)
{
	auto ball = Ball::create();
	ball->setPosition(Vec2(x, y));
	addChild(ball);
	balls.pushBack(ball);
}
