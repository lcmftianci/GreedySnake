#pragma once
//#include <iostream>
//#include "cocos2d.h"
//using namespace std;
//
//USING_NS_CC;
#include "CommonHead.h"

//枚举类型：标示虚拟摇杆与虚拟摇杆背景
typedef enum {
	tag_rocker,
	tag_rockerBG
}tagForRocker;

//用于标示摇杆与摇杆背景,
typedef enum
{
	rocker_stay,
	rocker_right,
	rocker_up,
	rocker_left,
	rocker_down,
}tagDirection;

//方向扩充至12个
typedef enum
{
	snape_right = 1,		//右
	snape_rightups,		//右上小
	snape_rightupl,		//右上大
	snape_up,			//上
	snape_leftups,		//左上小
	snape_leftupl,		//左上大 ，，大小只度数大小
	snape_left,  //左
	snape_leftdowns, //左下小
	snape_leftdownl,	//左下大
	snape_down,		//下
	snape_rightdowns,	//右下小
	snape_rightdownl	//右下大
}lesTagDirection;


//创建虚拟摇杆类，继承自Layer
//create joystick class extends from Layer
class VirtualHandle :public Layer
{
public:
	//构造函数
	VirtualHandle(void);
	//create joystick/joystick image/joystick background image/vector
	//创建摇杆（摇杆的操作图片资源，摇杆背景，其实坐标）
	static VirtualHandle* createJoystick(const char* joystickImage, const char* joystickBgImage, Vec2 pos);

	//启用摇杆（显示摇杆、监听摇杆触屏事件）
	void startJoystick(bool _isStopOther);

	//停止摇杆（隐藏摇杆、取消监听触屏事件）
	void stopJoystick();

	//判断控制杆方向，用来判断精灵上下左右运动
	int rockerDirection;

	//舌头方向
	int snapeDirection;
	//当前人物行走方向，用来判断精灵朝向，精灵脸朝向
	bool rockerRun;

	//精灵脸的朝向不止有一个是12个方向
	/*
		.        .            .
		  .      .         .
			.    .     .
			  .  .  .
	   .  .  .   .  .  .  .  .
			 .   .  .
		  .      .    .
		.        .      .
	  .          .        .
	  不好画所以这里只画八个放下，
	  16个方向角度上就分为12等份，360/12 = 30
	  分别是：
	  第一象限
	  0~PI/6,PI/6~PI/3,PI/3~PI/2
	  第二象限
	  PI/2~2PI/3,2PI/3~5PI/6,5PI/6~PI
	  第三象限
	  -PI~-5PI/6,
	*/



	CREATE_FUNC(VirtualHandle);
private:
	//自定义初始化函数
	void JoystickInit(const char* joystickImage, const char* joystickBgImage, Vec2 pos);

	//get x,y
	Vec2 getAnglePosition(float r, float angle);

	//是否可操作摇杆
	bool isCanMove;

	//得到摇杆与用户触屏的角度
	float GetRad(Vec2 pos1, Vec2 pos2);

	//摇杆背景坐标
	Vec2 joystickBgPosition;

	//摇杆背景图半径
	float joystickBgRadius;

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
};