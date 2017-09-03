#pragma once

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;

class RandomBall:public Sprite
{
public:
	virtual bool init();

	CREATE_FUNC(RandomBall);

	virtual void update(float dt);

	

private:
	float speedX, speedY;
	Size visibleSize;
};
