#pragma once
//#include <iostream>
//#include "cocos2d.h"
//using namespace std;
//
//USING_NS_CC;
#include "CommonHead.h"

//ö�����ͣ���ʾ����ҡ��������ҡ�˱���
typedef enum {
	tag_rocker,
	tag_rockerBG
}tagForRocker;

//���ڱ�ʾҡ����ҡ�˱���,
typedef enum
{
	rocker_stay,
	rocker_right,
	rocker_up,
	rocker_left,
	rocker_down,
}tagDirection;

//����������12��
typedef enum
{
	snape_right = 1,		//��
	snape_rightups,		//����С
	snape_rightupl,		//���ϴ�
	snape_up,			//��
	snape_leftups,		//����С
	snape_leftupl,		//���ϴ� ������Сֻ������С
	snape_left,  //��
	snape_leftdowns, //����С
	snape_leftdownl,	//���´�
	snape_down,		//��
	snape_rightdowns,	//����С
	snape_rightdownl	//���´�
}lesTagDirection;


//��������ҡ���࣬�̳���Layer
//create joystick class extends from Layer
class VirtualHandle :public Layer
{
public:
	//���캯��
	VirtualHandle(void);
	//create joystick/joystick image/joystick background image/vector
	//����ҡ�ˣ�ҡ�˵Ĳ���ͼƬ��Դ��ҡ�˱�������ʵ���꣩
	static VirtualHandle* createJoystick(const char* joystickImage, const char* joystickBgImage, Vec2 pos);

	//����ҡ�ˣ���ʾҡ�ˡ�����ҡ�˴����¼���
	void startJoystick(bool _isStopOther);

	//ֹͣҡ�ˣ�����ҡ�ˡ�ȡ�����������¼���
	void stopJoystick();

	//�жϿ��Ƹ˷��������жϾ������������˶�
	int rockerDirection;

	//��ͷ����
	int snapeDirection;
	//��ǰ�������߷��������жϾ��鳯�򣬾���������
	bool rockerRun;

	//�������ĳ���ֹ��һ����12������
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
	  ���û���������ֻ���˸����£�
	  16������Ƕ��Ͼͷ�Ϊ12�ȷݣ�360/12 = 30
	  �ֱ��ǣ�
	  ��һ����
	  0~PI/6,PI/6~PI/3,PI/3~PI/2
	  �ڶ�����
	  PI/2~2PI/3,2PI/3~5PI/6,5PI/6~PI
	  ��������
	  -PI~-5PI/6,
	*/



	CREATE_FUNC(VirtualHandle);
private:
	//�Զ����ʼ������
	void JoystickInit(const char* joystickImage, const char* joystickBgImage, Vec2 pos);

	//get x,y
	Vec2 getAnglePosition(float r, float angle);

	//�Ƿ�ɲ���ҡ��
	bool isCanMove;

	//�õ�ҡ�����û������ĽǶ�
	float GetRad(Vec2 pos1, Vec2 pos2);

	//ҡ�˱�������
	Vec2 joystickBgPosition;

	//ҡ�˱���ͼ�뾶
	float joystickBgRadius;

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
};