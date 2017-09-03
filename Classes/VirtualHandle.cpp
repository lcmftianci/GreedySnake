#include "VirtualHandle.h"

//定义PI
const double PI = 3.1415;
VirtualHandle::VirtualHandle(void)
{
	rockerRun = false;
}

//create joystick/joystick image/joystick background image/vector
VirtualHandle* VirtualHandle::createJoystick(const char* joystickImage, const char* joystickBgImage, Vec2 pos)
{
	//调用父类的create
	VirtualHandle* layer = VirtualHandle::create();
	if (layer)
	{
		layer->JoystickInit(joystickImage, joystickBgImage, pos);
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return NULL;
}

//启用摇杆
void VirtualHandle::startJoystick(bool _isStopOther)
{
	//精灵设置可见
	Sprite* joystick = (Sprite*)getChildByTag(tag_rocker);
	joystick->setVisible(true);

	//那个好使呢
	//Sprite* sprite = (Sprite*)this->getChildByTag(tag_rocker);

	Sprite* joystickBg = (Sprite*)getChildByTag(tag_rockerBG);
	joystickBg->setVisible(true);

	//cocos2d-x2.x
	//Director::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, _isStopOther);

	//注册监听事件
	//Director::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(VirtualHandle::onTouchBegan,
		this);
	listener->onTouchMoved = CC_CALLBACK_2(VirtualHandle::onTouchMoved,
		this);
	listener->onTouchEnded = CC_CALLBACK_2(VirtualHandle::onTouchEnded,
		this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,
		this);
}

//停止摇杆
void VirtualHandle::stopJoystick()
{
	//精灵设置不可见取消监听
	Sprite* joystick = (Sprite*)getChildByTag(tag_rocker);
	joystick->setVisible(false);

	Sprite* joystickBg = (Sprite*)getChildByTag(tag_rockerBG);
	joystickBg->setVisible(false);
	
	//取消监听事件
	_eventDispatcher->pauseEventListenersForTarget(this);
}

void VirtualHandle::JoystickInit(const char* joystickImage, const char* joystickBgImage, Vec2 pos)
{
	//添加背景图片
	Sprite* sprideBg = Sprite::create(joystickBgImage);
	sprideBg->setPosition(pos);
	sprideBg->setVisible(false);
	addChild(sprideBg, 0, tag_rockerBG);

	//添加摇杆上方可移动的图
	Sprite* spriteJoy = Sprite::create(joystickImage);
	sprideBg->setVisible(false);
	spriteJoy->setPosition(pos);
	addChild(spriteJoy, 1, tag_rocker);


	//遥感背景图坐标
	joystickBgPosition = pos;

	//摇杆背景半径
	joystickBgRadius = sprideBg->getContentSize().width / 2;

	//标示摇杆方向不变
	rockerDirection = -1;
}

//get x,y
//得到与角度对应的半径为r的圆上一坐标点，r半径，angle角度
Vec2 VirtualHandle::getAnglePosition(float r, float angle)
{
	//ccp求斜边
	return ccp(r*cos(angle), r*sin(angle));
}

bool VirtualHandle::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	Sprite* sprite = (Sprite*)getChildByTag(tag_rocker);

	//得到触屏点坐标
	Vec2 pos = touch->getLocation();

	//判断是否点击到精灵， boundingBox()精灵大小之内的坐标
	if (sprite->boundingBox().containsPoint(pos))
	{
		isCanMove = true;
		log("begin");
	}
	return true;
}

void VirtualHandle::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (!isCanMove)
		return;
	
	Sprite* sprite = (Sprite*)getChildByTag(tag_rocker);
	Vec2 pos = touch->getLocation();
	
	//判断摇杆与触点的角度
	float angle = GetRad(joystickBgPosition, pos);
	
	//判断圆心之间的距离是否大于摇杆背景的半径
	if (sqrt(pow(pos.x - joystickBgPosition.x, 2) + pow(pos.y - joystickBgPosition.y, 2)) >= joystickBgRadius)
	{ 
		//确保小圆运动半径在背景圆内//ccp求斜边
		sprite->setPosition(ccpAdd(getAnglePosition(joystickBgRadius, angle), ccp(joystickBgPosition.x, joystickBgPosition.y)));
	}
	else
	{
		//出点在背景园内则跟随出点运动
		sprite->setPosition(pos);
	}

	//判断方向
	//如果在-45到45之间
	if (angle >= -PI / 4 && angle < PI / 4)
	{
		rockerDirection = rocker_right;
		rockerRun = false;
	}
	else if (angle >= PI / 4 && angle < 3 * PI / 4)//如果在45~135之间
	{
		rockerDirection = rocker_up;
	}
	else if ((angle >= 3 * PI / 4 && angle <= PI) || (angle >= -PI&&angle < -3 * PI / 4))//如果在135到225之间
	{
		rockerDirection = rocker_left;
		rockerRun = true;
	}
	else if (angle >= -3 * PI / 4 && angle < -PI / 4)//225~315之间
	{
		rockerDirection = rocker_down;
	}


	//如果在-15到15之间
	if (angle >= -PI / 12 && angle < PI / 12)//1
	{
		snapeDirection = snape_right;//右
		/**rockerRun = false; */
	}
	else if (angle >= PI / 12&& angle < 3 * PI / 12)//如果在15~45之间2
	{
		snapeDirection = snape_rightups;	//右上小						
	}
	else if (angle >= 3 * PI / 12 && angle < 5 * PI / 12)//如果在45到75之间3
	{
		snapeDirection = snape_rightupl;//右上大
		/*rockerRun = true;*/
	}
	else if (angle >= 5 * PI / 12 && angle < 7 * PI / 12)//75~105之间4
	{
		snapeDirection = snape_up;//上

	}
	else if (angle >= 7 * PI / 12 && angle < 9 * PI / 12)//105~135之间5
	{
		snapeDirection = snape_leftups;//左上小

	}
	else if (angle >= 9 * PI / 12 && angle < 11 * PI / 12)//135~165之间6
	{
		snapeDirection = snape_leftupl;//左上大 ，，大小只度数大小

	}
	else if ((angle >= 11 * PI / 12 && angle < PI) ||(angle < -11 * PI / 12 && angle >= -PI))//165~195之间7
	{
		snapeDirection = snape_left;//左

	}
	else if (angle >= -11 * PI / 12 && angle < -9 * PI / 12)//195~225之间8
	{
		snapeDirection = snape_leftdowns;//左下小

	}
	else if (angle >= -9 * PI / 12 && angle < -7 * PI / 12)//225~255之间9
	{
		snapeDirection = snape_leftdownl;	//左下大

	}
	else if (angle >= -7 * PI / 12 && angle < -5 * PI / 12)//255~285之间10
	{
		snapeDirection = snape_down;		//下

	}
	else if (angle >= -5 * PI / 12 && angle < -3 * PI / 12)//285~315之间11
	{
		snapeDirection = snape_rightdowns;	//右下小
	}
	else if (angle >= -3 * PI / 12 && angle < -PI / 12)//315~345之间12
	{
		snapeDirection = snape_rightdownl;//右下大
	}
}

void VirtualHandle::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (!isCanMove)
		return;

	Sprite* sprite = (Sprite*)getChildByTag(tag_rocker);
	Sprite* joystickBg = (Sprite*)getChildByTag(tag_rockerBG);
	sprite->stopAllActions();
	sprite->runAction(MoveTo::create(0.08, joystickBg->getPosition()));
	isCanMove = false;
	rockerDirection = rocker_stay;
	log("derection + %d", rockerDirection);
}

//得到摇杆与用户触屏的角度,第一个触屏坐标，第二个遥感坐标
float VirtualHandle::GetRad(Vec2 pos1, Vec2 pos2)
{
	//得到两点坐标x,y值
	float px1 = pos1.x;
	float py1 = pos1.y;

	float px2 = pos2.x;
	float py2 = pos2.y;

	//求出两边长度
	float x = px2 - px1;
	float y = py2 - py1;

	//开发与几次方
	float bevel = sqrt(pow(x, 2) + pow(y, 2));
	float cos = x / bevel;

	//反余弦定理，求得角度
	float rad = acos(cos);

	//当触屏y坐标小于摇杆y坐标是取反值
	if (py1 > py2)
		rad = -rad;

	return rad;
}