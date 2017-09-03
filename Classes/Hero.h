#pragma once

#include "cocos2d.h"
//#include "cocos-ext.h"

USING_NS_CC;
//USING_NS_CC_EXT;
USING_NS_CC_MATH;

class Hero :public cocos2d::Node
{
public:
	Hero(void);
	~Hero(void);

	//根据图片创建英雄
	void InitHeroSprite(char* chHeroPathName);

	//设置动画，num为图片的数目，run_direction为精灵脸朝向，false朝右，name_each为name_png的每一张小图片的公共名称部分
	void SetAnimation(const char* name_plist, const char* name_png, const unsigned int num, bool run_direction);

	//停止动画
	void StopAnimation();

	//判断是否在跑动
	bool IsRunning;

	//任务运动方向
	bool HeroDirection;

	CREATE_FUNC(Hero);

private:
	Sprite* m_HeroSprite;//精灵
	char* HeroName;//用于保存初始状态的精灵图片的名称
};
