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

	//����ͼƬ����Ӣ��
	void InitHeroSprite(char* chHeroPathName);

	//���ö�����numΪͼƬ����Ŀ��run_directionΪ����������false���ң�name_eachΪname_png��ÿһ��СͼƬ�Ĺ������Ʋ���
	void SetAnimation(const char* name_plist, const char* name_png, const unsigned int num, bool run_direction);

	//ֹͣ����
	void StopAnimation();

	//�ж��Ƿ����ܶ�
	bool IsRunning;

	//�����˶�����
	bool HeroDirection;

	CREATE_FUNC(Hero);

private:
	Sprite* m_HeroSprite;//����
	char* HeroName;//���ڱ����ʼ״̬�ľ���ͼƬ������
};
