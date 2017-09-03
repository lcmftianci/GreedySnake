#include "RandomBall.h"

bool RandomBall::init()
{
	Sprite::initWithFile("GameUi.jpg");
	visibleSize = Director::getInstance()->getVisibleSize();

	//这里用这个宏报错
	speedX = CCRANDOM_0_1() * 10 - 5;
	speedY = CCRANDOM_0_1() * 10 - 5;
	//speedX = cocos2d::rand_0_1() * 10 - 5;
	//speedY = cocos2d::rand_0_1() * 10 - 5;

	scheduleUpdate();
	return true;
}

void RandomBall::update(float dt)
{
	setPosition(getPositionX() + speedX, getPositionY() + speedY);
	if (getPositionX() < getContentSize().width / 2)
	{
		speedX = fabs(speedX);
	}
	if (getPositionX() > visibleSize.width - getContentSize().width / 2)
	{
		speedX = -fabs(speedX);
	}
	if (getPositionY() < getContentSize().height / 2)
	{
		speedY = fabs(speedY);
	}
	if (getPositionY() > visibleSize.height - getContentSize().height / 2)
	{
		speedY = -fabs(speedY);
	}
}