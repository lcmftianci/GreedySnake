#include "Ball.h"

bool Ball::init()
{
	Sprite::initWithFile("GameUi.jpg");
	visibleSize = Director::getInstance()->getVisibleSize();
	speedX = 3;
	return true;
}

void Ball::move()
{
	setPositionX(getPositionX() + speedX);
	if (getPositionX() > visibleSize.width)
		setPositionX(0);
}