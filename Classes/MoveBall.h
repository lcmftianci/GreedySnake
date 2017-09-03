#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "Ball.h"

USING_NS_CC;

class MoveBall : public LayerColor {
public:
	virtual bool init();

	CREATE_FUNC(MoveBall);

	virtual void update(float dt);

	static Scene* createScene();

private:
	//Ball *ball;
	cocos2d::Vector<Ball*> balls;

	void AddBall(float x, float y);
	
};
