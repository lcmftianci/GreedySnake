#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class VirtualHandle;
class Hero;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	void loadANewScene(Ref * ref);

	virtual void update(float dt);

	//closeMenu
	void menuCloseCallback(Ref * pSender);

private:
	cocos2d::Size visibleSize;
	cocos2d::Sprite *ball;


	//�������ҡ��
	VirtualHandle* pVirtualHandle;
	//�������
	Hero* hero;
	float m_HeroSpeed;//�����˶��ٶȣ�����
	float m_HeroFastSpeed;//���������ٶȣ��죩

	//���Ʒ���ı�־λ0 = ��ֹ��1 = ��2 = �ң�3 = �ϣ�4 = ��
	int m_dirFlag = 1;
};

#endif // __HELLOWORLD_SCENE_H__
