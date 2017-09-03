/************************************************************************/
/*
//	̰����ͷ����ͷ�ļ�
//	�����ˣ�������
//	����ʱ�䣺2017��01��12��
//	����޸�ʱ�䣺2017��01��12��
*/
/************************************************************************/

#include "SnakeHead.h"
//����PI
const double PI = 3.1415;

bool SnakeHead::init()
{
	//����ͷ��
	Sprite::initWithFile("GameUi.jpg");
	visibleSize = Director::getInstance()->getVisibleSize();
	speed = 3;
	fastSpeed = 6;
	return true;
}

//�����ߵ��˶���ʽ
/*
	�ߵ��˶���ʽ���ֶ��������Ƕ���12������
	nDirection ����� 1~~12;
	1 = 0�ȣ�
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
	nSpeed ��������
	1 = 3m/s��
	2 = 6m/s��
*/
void SnakeHead::move(int nDirection, int nSpeed)
{
	switch (nDirection)//�˶�����
	{
		case 1://�����˶�
			switch (nSpeed)//�ٶȿ���
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