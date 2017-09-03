#include "VirtualHandle.h"

//����PI
const double PI = 3.1415;
VirtualHandle::VirtualHandle(void)
{
	rockerRun = false;
}

//create joystick/joystick image/joystick background image/vector
VirtualHandle* VirtualHandle::createJoystick(const char* joystickImage, const char* joystickBgImage, Vec2 pos)
{
	//���ø����create
	VirtualHandle* layer = VirtualHandle::create();
	if (layer)
	{
		layer->JoystickInit(joystickImage, joystickBgImage, pos);
		return layer;
	}

	CC_SAFE_DELETE(layer);
	return NULL;
}

//����ҡ��
void VirtualHandle::startJoystick(bool _isStopOther)
{
	//�������ÿɼ�
	Sprite* joystick = (Sprite*)getChildByTag(tag_rocker);
	joystick->setVisible(true);

	//�Ǹ���ʹ��
	//Sprite* sprite = (Sprite*)this->getChildByTag(tag_rocker);

	Sprite* joystickBg = (Sprite*)getChildByTag(tag_rockerBG);
	joystickBg->setVisible(true);

	//cocos2d-x2.x
	//Director::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, _isStopOther);

	//ע������¼�
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

//ֹͣҡ��
void VirtualHandle::stopJoystick()
{
	//�������ò��ɼ�ȡ������
	Sprite* joystick = (Sprite*)getChildByTag(tag_rocker);
	joystick->setVisible(false);

	Sprite* joystickBg = (Sprite*)getChildByTag(tag_rockerBG);
	joystickBg->setVisible(false);
	
	//ȡ�������¼�
	_eventDispatcher->pauseEventListenersForTarget(this);
}

void VirtualHandle::JoystickInit(const char* joystickImage, const char* joystickBgImage, Vec2 pos)
{
	//��ӱ���ͼƬ
	Sprite* sprideBg = Sprite::create(joystickBgImage);
	sprideBg->setPosition(pos);
	sprideBg->setVisible(false);
	addChild(sprideBg, 0, tag_rockerBG);

	//���ҡ���Ϸ����ƶ���ͼ
	Sprite* spriteJoy = Sprite::create(joystickImage);
	sprideBg->setVisible(false);
	spriteJoy->setPosition(pos);
	addChild(spriteJoy, 1, tag_rocker);


	//ң�б���ͼ����
	joystickBgPosition = pos;

	//ҡ�˱����뾶
	joystickBgRadius = sprideBg->getContentSize().width / 2;

	//��ʾҡ�˷��򲻱�
	rockerDirection = -1;
}

//get x,y
//�õ���Ƕȶ�Ӧ�İ뾶Ϊr��Բ��һ����㣬r�뾶��angle�Ƕ�
Vec2 VirtualHandle::getAnglePosition(float r, float angle)
{
	//ccp��б��
	return ccp(r*cos(angle), r*sin(angle));
}

bool VirtualHandle::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	Sprite* sprite = (Sprite*)getChildByTag(tag_rocker);

	//�õ�����������
	Vec2 pos = touch->getLocation();

	//�ж��Ƿ��������飬 boundingBox()�����С֮�ڵ�����
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
	
	//�ж�ҡ���봥��ĽǶ�
	float angle = GetRad(joystickBgPosition, pos);
	
	//�ж�Բ��֮��ľ����Ƿ����ҡ�˱����İ뾶
	if (sqrt(pow(pos.x - joystickBgPosition.x, 2) + pow(pos.y - joystickBgPosition.y, 2)) >= joystickBgRadius)
	{ 
		//ȷ��СԲ�˶��뾶�ڱ���Բ��//ccp��б��
		sprite->setPosition(ccpAdd(getAnglePosition(joystickBgRadius, angle), ccp(joystickBgPosition.x, joystickBgPosition.y)));
	}
	else
	{
		//�����ڱ���԰�����������˶�
		sprite->setPosition(pos);
	}

	//�жϷ���
	//�����-45��45֮��
	if (angle >= -PI / 4 && angle < PI / 4)
	{
		rockerDirection = rocker_right;
		rockerRun = false;
	}
	else if (angle >= PI / 4 && angle < 3 * PI / 4)//�����45~135֮��
	{
		rockerDirection = rocker_up;
	}
	else if ((angle >= 3 * PI / 4 && angle <= PI) || (angle >= -PI&&angle < -3 * PI / 4))//�����135��225֮��
	{
		rockerDirection = rocker_left;
		rockerRun = true;
	}
	else if (angle >= -3 * PI / 4 && angle < -PI / 4)//225~315֮��
	{
		rockerDirection = rocker_down;
	}


	//�����-15��15֮��
	if (angle >= -PI / 12 && angle < PI / 12)//1
	{
		snapeDirection = snape_right;//��
		/**rockerRun = false; */
	}
	else if (angle >= PI / 12&& angle < 3 * PI / 12)//�����15~45֮��2
	{
		snapeDirection = snape_rightups;	//����С						
	}
	else if (angle >= 3 * PI / 12 && angle < 5 * PI / 12)//�����45��75֮��3
	{
		snapeDirection = snape_rightupl;//���ϴ�
		/*rockerRun = true;*/
	}
	else if (angle >= 5 * PI / 12 && angle < 7 * PI / 12)//75~105֮��4
	{
		snapeDirection = snape_up;//��

	}
	else if (angle >= 7 * PI / 12 && angle < 9 * PI / 12)//105~135֮��5
	{
		snapeDirection = snape_leftups;//����С

	}
	else if (angle >= 9 * PI / 12 && angle < 11 * PI / 12)//135~165֮��6
	{
		snapeDirection = snape_leftupl;//���ϴ� ������Сֻ������С

	}
	else if ((angle >= 11 * PI / 12 && angle < PI) ||(angle < -11 * PI / 12 && angle >= -PI))//165~195֮��7
	{
		snapeDirection = snape_left;//��

	}
	else if (angle >= -11 * PI / 12 && angle < -9 * PI / 12)//195~225֮��8
	{
		snapeDirection = snape_leftdowns;//����С

	}
	else if (angle >= -9 * PI / 12 && angle < -7 * PI / 12)//225~255֮��9
	{
		snapeDirection = snape_leftdownl;	//���´�

	}
	else if (angle >= -7 * PI / 12 && angle < -5 * PI / 12)//255~285֮��10
	{
		snapeDirection = snape_down;		//��

	}
	else if (angle >= -5 * PI / 12 && angle < -3 * PI / 12)//285~315֮��11
	{
		snapeDirection = snape_rightdowns;	//����С
	}
	else if (angle >= -3 * PI / 12 && angle < -PI / 12)//315~345֮��12
	{
		snapeDirection = snape_rightdownl;//���´�
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

//�õ�ҡ�����û������ĽǶ�,��һ���������꣬�ڶ���ң������
float VirtualHandle::GetRad(Vec2 pos1, Vec2 pos2)
{
	//�õ���������x,yֵ
	float px1 = pos1.x;
	float py1 = pos1.y;

	float px2 = pos2.x;
	float py2 = pos2.y;

	//������߳���
	float x = px2 - px1;
	float y = py2 - py1;

	//�����뼸�η�
	float bevel = sqrt(pow(x, 2) + pow(y, 2));
	float cos = x / bevel;

	//�����Ҷ�����ýǶ�
	float rad = acos(cos);

	//������y����С��ҡ��y������ȡ��ֵ
	if (py1 > py2)
		rad = -rad;

	return rad;
}