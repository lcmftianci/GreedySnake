/************************************************************************/
/*
//	贪吃蛇头部类头文件
//	创建人：许立龙
//	创建时间：2017年01月12日
//	最后修改时间：2017年01月12日
*/
/************************************************************************/

#include "SnakeHead.h"
//定义PI
const double PI = 3.1415;

bool SnakeHead::init()
{
	//创建头部
	Sprite::initWithFile("GameUi.jpg");
	visibleSize = Director::getInstance()->getVisibleSize();
	speed = 3;
	fastSpeed = 6;
	return true;
}

//创建蛇的运动方式
/*
	蛇的运动方式多种多样，我们定义12个方向
	nDirection 方向从 1~~12;
	1 = 0度；
	2 = PI/6;
	3 = PI/3;
	4 = PI/2;
	5 = 2PI/3;
	6 = 5PI/6;
	7 = PI;
	8 = 7PI/6;
	9 = 4PI/3;
	10 = 3PI/2;
	11 = 5PI/3;
	12 = 11PI/6;
	nSpeed 有两个档
	1 = 3m/s；
	2 = 6m/s；
*/
void SnakeHead::move(int nDirection, int nSpeed)
{
	switch (nDirection)//运动方向
	{
		case 1://向左运动
			switch (nSpeed)//速度控制
			{
			case 1:
				setPositionX(getPositionX() + speed);
				break;
			case 2:
				setPositionX(getPositionX() + fastSpeed);
				break;
			default:
				break;
			}
			break;
		case 2:
			switch (nSpeed)
			{
			case 1:
				setPosition(getPositionX() + speed*cos(PI/6), getPositionY() + speed*sin(PI/6));
				break;
			case 2:
				setPosition(getPositionX() + fastSpeed*cos(PI / 6), getPositionY() + fastSpeed*sin(PI / 6));
			default:
				break;
			}
		case 3:
			switch (nSpeed)
			{
			case 1:
				setPosition(getPositionX() + speed*cos(PI / 3), getPositionY() + speed*sin(PI / 3));
				break;
			case 2:
				setPosition(getPositionX() + fastSpeed*cos(PI / 3), getPositionY() + fastSpeed*sin(PI / 3));
			default:
				break;
			}
		default:
			break;
	}
	//setPositionX(getPositionX() + speedX);
	//if (getPositionX() > visibleSize.width)
	//	setPositionX(0);
}