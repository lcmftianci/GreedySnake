#pragma once

#include "cocos2d.h"
#include <stdio.h>


USING_NS_CC;


class Ball :public Sprite {
public:
	virtual bool init();

	void move();

	CREATE_FUNC(Ball);

private:
	float speedX;
	Size visibleSize;
};