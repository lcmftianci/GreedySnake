#ifndef _CAMERALCMF3D_H_
#define _CAMERALCMF3D_H_

#include "cocos2d.h"
#include <string.h>

USING_NS_CC;

//在HelloWorld的createScene中，直接创建Camera3DDemo的Layer对象，加入Scene

namespace cocos2d {
	class Sprite3D;
	class Delay;
}

//状态
enum State
{
	State_None = 0,
	State_Idle = 0x01,
	State_Move = 0x02,
	State_Rotate = 0x04,
	State_Speak = 0x08,
	State_MeleeAttack = 0x10,
	State_RemoveAttack = 0x20,
	State_Attack = 0x40,
};

//相机类型
enum class CameraType
{
	FreeCamera = 0,
	FirstPerson = 1,
	ThirdPerson = 2,
};

//相机操作类型
enum class OperateCamType
{
	MoveCamera = 0,
	RotateCamera = 1,
};

class CameraLcmf3D : public Layer
{
public:
	CREATE_FUNC(CameraLcmf3D);
	CameraLcmf3D(void);
	virtual ~CameraLcmf3D(void);

	virtual void onEnter() override;
	virtual void onExit() override;

	void addNewSpriteWithCoords(Vec3 pos, std::string fileName, bool playAnimation = false, float scale = 1.0f, bool bindCamera = false);

	//触摸事件
	void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
	void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);

	void SwitchViewCallback(Ref* sender, CameraType cameratype);

	void updateCamera(float fDelta);
	void move3D(float elapsedTime);
	void updateState(float elapsedTime);
	bool isState(unsigned int nState, unsigned int bit) const;
	
	//缩放
	bool onTouchesZoomOut(Touch* touch, Event* event);
	void onTouchesZoomOutEnd(Touch* touch, Event* event);
	bool onTouchesZoomIn(Touch* touch, Event* event);
	void onTouchesZoomInEnd(Touch* touch, Event* event);

	//旋转
	bool onTouchesRotateLeft(Touch* touch, Event* event);
	void onTouchesRotateLeftEnd(Touch* touch, Event* event);
	bool onTouchesRotateRight(Touch* touch, Event* event);
	void onTouchesRotateRightEnd(Touch* touch, Event* event);

protected:
	std::string _title;
	Layer* _layer3D;
	Sprite3D* _sprite3D;
	Vec3 _targetPos;
	CameraType _cameraType;
	MenuItem* _incRot;
	MenuItem* _decRot;
	unsigned int _curState;
	Camera* _camera;
	MoveTo* _moveAction;
	bool _bZoomOut;
	bool _bZoomIn;
	bool _bRotateLeft;
	bool _bRotateRight;
	Label* _RotateRightLabel;
	Label* _RotateLeftLabel;
	Label* _ZoomInLabel;
	Label* _ZoomOutLabel;
};





#endif // _CAMERA3D_H_
