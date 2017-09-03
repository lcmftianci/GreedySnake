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


	//添加虚拟摇杆
	VirtualHandle* pVirtualHandle;
	//添加人物
	Hero* hero;
	float m_HeroSpeed;//人物运动速度（慢）
	float m_HeroFastSpeed;//人物运行速度（快）

	//控制方向的标志位0 = 静止，1 = 左，2 = 右，3 = 上，4 = 下
	int m_dirFlag = 1;
};

#endif // __HELLOWORLD_SCENE_H__
