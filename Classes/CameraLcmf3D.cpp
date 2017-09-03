#include "CameraLcmf3D.h"
#include <algorithm>	//包含stl库算法
#include "VisibleRect.h"

class DrawLine3D : public Node 
{
public:
	//静态创建函数
	static DrawLine3D* create();

	//画线函数，参数1：起点；参数2：终点；参数3:线段颜色
	void drawLine(const Vec3& from, const Vec3& to, const Color4F& color);

	void clear() { _buffer.clear(); }

	//真正的绘图函数
	void onDraw(const Mat4& transform, uint32_t flags);

	//绘制节点
	virtual void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags) override;

CC_CONSTRUCTOR_ACCESS:
	DrawLine3D(){}
	virtual ~DrawLine3D() {}

	virtual bool init();
protected:
	//自定义线段存放结构体
	struct V3F_C4B
	{
		Vec3 vectices;	//端点坐标
		Color4F colors; //线段颜色
	};
	//存放线段数据的容器
	std::vector<V3F_C4B> _buffer;
	CustomCommand _customCommand;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(DrawLine3D);//定义一个类，禁止拷贝和赋值运算符重载功能
};

DrawLine3D* DrawLine3D::create()
{
	auto ret = new (std::nothrow)DrawLine3D();
	if (ret && ret->init())
		return ret;
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool DrawLine3D::init()
{
	//设置节点着色程序
	setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_LABEL_DISTANCEFIELD_GLOW));
	return true;
}

//划线函数
void DrawLine3D::drawLine(const Vec3& from, const Vec3& to, const Color4F& color)
{
	Color4F col = Color4F(color);//线段颜色
	DrawLine3D::V3F_C4B vertex;
	vertex.vectices = from;//start
	vertex.colors = col;//color
	_buffer.push_back(vertex);
	vertex.vectices = to;
	_buffer.push_back(vertex);
}

//绘制节点
void DrawLine3D::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	//调用绘制函数
	_customCommand.func = CC_CALLBACK_0(DrawLine3D::onDraw, this, transform, flags);

	//将着色器程序加入到renderer对象
	renderer->addCommand(&_customCommand);
}

//调用OpenGl接口绘图，涉及opengl知识，
void DrawLine3D::onDraw(const Mat4& transform, uint32_t flags)
{
	auto glProgram = getGLProgram();//返回当前节点GLProgram(shader)
	glProgram->use();//调用glUseProgram()函数使用shader绘图
	glProgram->setUniformsForBuiltins(transform);
	glEnable(GL_DEPTH_TEST);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V3F_C4B), &(_buffer[0].colors));
	glDrawArrays(GL_LINES, 0, static_cast<int>(_buffer.size()));
	glDisable(GL_DEPTH_TEST);
}

//下面是camare的实现
enum {
	IDC_NEXT = 100,
	IDC_BACK,
	IDC_RESTART,
};

CameraLcmf3D::CameraLcmf3D(void) :Layer(), _camera(nullptr), _incRot(nullptr), _decRot(nullptr), _bZoomOut(false), _bZoomIn(false), _bRotateLeft(false), _bRotateRight(false)
{

}

CameraLcmf3D::~CameraLcmf3D(void)
{

}

//旋转视角回调函数
void CameraLcmf3D::SwitchViewCallback(Ref* sender, CameraType cameratype)
{
	//如果摄像机类型等于自由视角
	if (_cameraType == CameraType::FreeCamera)
	{
		_camera->setPosition3D(Vec3(0, 130, 0) + _sprite3D->getPosition3D());
		_camera->lookAt(_sprite3D->getPosition3D(), Vec3(0, 1, 0));
		_RotateRightLabel->setColor(Color3B::WHITE);
		_RotateLeftLabel->setColor(Color3B::WHITE);
		_ZoomInLabel->setColor(Color3B::WHITE);
		_ZoomOutLabel->setColor(Color3B::WHITE);
	}
	//如果摄像机类型为第一视角
	else if (_cameraType == CameraType::FirstPerson)
	{
		Vec3 newFaceDir;
		//返回逆世界仿射变换矩阵，像素的矩阵，并获取fowrward矩阵，参数为目的向量
		_sprite3D->getNodeToWorldTransform().getForwardVector(&newFaceDir);
		//目标向量标准化，将Vec3对象的x,y,x转换为分数，且x的平方+y
		newFaceDir.normalize();
		_camera->setPosition3D(Vec3(0, 35, 0) + _sprite3D->getPosition3D());

		//设置相机的仕途矩阵，参数1：target位置， 参数2，向上的向量
		_camera->lookAt(_sprite3D->getPosition3D() + newFaceDir * 50, Vec3(0, 1, 0));

		_RotateRightLabel->setColor(Color3B::WHITE);
		_RotateLeftLabel->setColor(Color3B::WHITE);
		_ZoomInLabel->setColor(Color3B::GRAY);
		_ZoomOutLabel->setColor(Color3B::GRAY);
	}
	//设置摄像机类型是第三视角
	else if (_cameraType == CameraType::ThirdPerson)
	{
		_camera->setPosition3D(Vec3(0, 130, 130) + _sprite3D->getPosition3D());
		//设置相机的视图矩阵
		_camera->lookAt(_sprite3D->getPosition3D(), Vec3(3, 0, 0));
		_RotateRightLabel->setColor(Color3B::GRAY);
		_RotateLeftLabel->setColor(Color3B::GRAY);
		_ZoomInLabel->setColor(Color3B::WHITE);
		_ZoomOutLabel->setColor(Color3B::WHITE);
	}
}

//触摸事件
void CameraLcmf3D::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
	for(auto &item :touches)
	{
		auto touch = item;
		auto location = touch->getLocation();
	}
}

//多点触摸移动，即点击线段区域，精灵移动过去 
void CameraLcmf3D::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
	if (touches.size() == 1)
	{
		auto touch = touches[0];
		//获取第一个触摸点的OpenGL坐标系的坐标，原点在屏幕左下角。  
		//补充：屏幕坐标系，原点在屏幕左上角  
		//世界坐标系：与OpenGL坐标系一致，位于屏幕左下角  
		//节点坐标系：父节点的左下角，与OpenGL一致  
		auto location = touch->getLocation();
		Point newPos = touch->getPreviousLocation() - location;
		if (_cameraType == CameraType::FreeCamera || _cameraType == CameraType::FirstPerson)
		{
			Vec3 cameraDir;
			Vec3 cameraRightDir;
			//返回世界仿射变换矩阵。矩阵单位是像素。并获取Forward矩阵，参数为目标向量。注意逆世界仿射变换矩阵  
			_camera->getNodeToWorldTransform().getForwardVector(&cameraDir);
			//将返回的矩阵向量规范化  
			cameraDir.normalize();
			//将返回的矩阵向量y轴赋值为0  
			cameraDir.y = 0;
			//返回正确的举证向量的世界仿射变化矩阵  
			_camera->getNodeToWorldTransform().getRightVector(&cameraRightDir);
			//向量规范化  
			cameraRightDir.normalize();
			//向量y轴赋值0  
			cameraRightDir.y = 0;
			Vec3 cameraPos = _camera->getPosition3D();
			cameraPos += cameraDir*newPos.y*0.1f;
			cameraPos += cameraRightDir*newPos.x*0.1f;
			//重新设置摄像机的3D坐标  
			_camera->setPosition3D(cameraPos);
			//如果精灵存在，且相机的类型为第一视角，执行下面代码  
			if (_sprite3D &&  _cameraType == CameraType::FirstPerson)
			{
				//设置精灵的3D坐标，并保存当前精灵坐标值，用于更新任务运动状态  
				_sprite3D->setPosition3D(Vec3(_camera->getPositionX(), 0, _camera->getPositionZ()));
				_targetPos = _sprite3D->getPosition3D();
			}
		}
	}
}
void CameraLcmf3D::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
	for (auto &item : touches)
	{
		auto touch = item;
		auto location = touch->getLocationInView();
		if (_camera)
		{
			if (_sprite3D && _cameraType == CameraType::ThirdPerson && _bZoomOut == false && _bZoomIn == false && _bRotateLeft == false && _bRotateRight == false)
			{
				Vec3 nearP(location.x, location.y, -1.0f), farP(location.x, location.y, 1.0f);

				auto size = Director::getInstance()->getWinSize();
				_camera->unproject(size, &nearP, &nearP);
				_camera->unproject(size, &farP, &farP);
				Vec3 dir(farP - nearP);
				float dist = 0.0f;
				float ndd = Vec3::dot(Vec3(0, 1, 0), dir);
				if (ndd == 0)
					dist = 0.0f;
				float ndo = Vec3::dot(Vec3(0, 1, 0), nearP);
				dist = (0 - ndo) / ndd;
				Vec3 p = nearP + dist *  dir;

				if (p.x > 100)
					p.x = 100;
				if (p.x < -100)
					p.x = -100;
				if (p.z > 100)
					p.z = 100;
				if (p.z < -100)
					p.z = -100;

				_targetPos = p;
			}
		}
	}
}

void CameraLcmf3D::onEnter()
{
	Layer::onEnter();
	_sprite3D = nullptr;

	////注册事件
	auto winSize = Director::getInstance()->getWinSize();
	auto listner = EventListenerTouchAllAtOnce::create();
	//
	////注册触摸其实事件
	listner->onTouchesBegan = CC_CALLBACK_2(CameraLcmf3D::onTouchesBegan, this);
	listner->onTouchesMoved = CC_CALLBACK_2(CameraLcmf3D::onTouchesMoved, this);
	listner->onTouchesEnded = CC_CALLBACK_2(CameraLcmf3D::onTouchesBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listner, this);

	//创建新的层，用于加载3D精灵和线段
	auto layer3D = Layer::create();
	addChild(layer3D, 0);
	_layer3D = layer3D;
	_curState = State_None;

	//添加精灵参数依次为:Vec3向量，精灵文件路径，是否执行动作，缩放系数，是否绑定摄像机
	addNewSpriteWithCoords(Vec3(0, 0, 0), "ReskinGirl.c3b", true, 0.2f, true);

	TTFConfig ttfConfig("fonts/arial.ttf", 30);
	//依次添加4个Label对象到对应的Node节点上，并对4个Label对象注册监听事件  
	auto containerForLabel1 = Node::create();
	_ZoomOutLabel = Label::createWithTTF(ttfConfig, "zoom out");
	_ZoomOutLabel->setPosition(winSize.width - 150, VisibleRect::top().y - 30);
	containerForLabel1->addChild(_ZoomOutLabel);
	addChild(containerForLabel1, 10);
	
	auto listener1 = EventListenerTouchOneByOne::create();
	listener1->setSwallowTouches(true);

	listener1->onTouchBegan = CC_CALLBACK_2(CameraLcmf3D::onTouchesZoomOut, this);
	listener1->onTouchEnded = CC_CALLBACK_2(CameraLcmf3D::onTouchesZoomOutEnd, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, _ZoomOutLabel);

	auto containerForLabel2 = Node::create();
	_ZoomInLabel = Label::createWithTTF(ttfConfig, "zoom in");
	_ZoomInLabel->setPosition(winSize.width - 150, VisibleRect::top().y - 100);
	containerForLabel2->addChild(_ZoomInLabel);
	addChild(containerForLabel2, 10);

	auto listener2 = EventListenerTouchOneByOne::create();
	listener2->setSwallowTouches(true);

	listener2->onTouchBegan = CC_CALLBACK_2(CameraLcmf3D::onTouchesZoomIn, this);
	listener2->onTouchEnded = CC_CALLBACK_2(CameraLcmf3D::onTouchesZoomInEnd, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, _ZoomInLabel);

	auto containerForLabel3 = Node::create();
	_RotateLeftLabel = Label::createWithTTF(ttfConfig, "rotate left");
	_RotateLeftLabel->setPosition(winSize.width - 150, VisibleRect::top().y - 170);
	containerForLabel3->addChild(_RotateLeftLabel);
	addChild(containerForLabel3, 10);

	auto listener3 = EventListenerTouchOneByOne::create();
	listener3->setSwallowTouches(true);

	listener3->onTouchBegan = CC_CALLBACK_2(CameraLcmf3D::onTouchesRotateLeft, this);
	listener3->onTouchEnded = CC_CALLBACK_2(CameraLcmf3D::onTouchesRotateLeftEnd, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener3, _RotateLeftLabel);

	auto containerForLabel4 = Node::create();
	_RotateRightLabel = Label::createWithTTF(ttfConfig, "rotate right");
	_RotateRightLabel->setPosition(winSize.width - 150, VisibleRect::top().y - 240);
	containerForLabel4->addChild(_RotateRightLabel);
	addChild(containerForLabel4, 10);

	auto listener4 = EventListenerTouchOneByOne::create();
	listener4->setSwallowTouches(true);

	listener4->onTouchBegan = CC_CALLBACK_2(CameraLcmf3D::onTouchesRotateRight, this);
	listener4->onTouchEnded = CC_CALLBACK_2(CameraLcmf3D::onTouchesRotateRightEnd, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener4, _RotateRightLabel);

	//设置摄像机视角菜单选项，并回调转换视角函数  
	auto label1 = Label::createWithTTF(ttfConfig, "free ");
	auto menuItem1 = MenuItemLabel::create(label1, CC_CALLBACK_1(CameraLcmf3D::SwitchViewCallback, this, CameraType::FreeCamera));
	auto label2 = Label::createWithTTF(ttfConfig, "third person");
	auto menuItem2 = MenuItemLabel::create(label2, CC_CALLBACK_1(CameraLcmf3D::SwitchViewCallback, this, CameraType::ThirdPerson));
	auto label3 = Label::createWithTTF(ttfConfig, "first person");
	auto menuItem3 = MenuItemLabel::create(label3, CC_CALLBACK_1(CameraLcmf3D::SwitchViewCallback, this, CameraType::FirstPerson));
	auto menu = Menu::create(menuItem1, menuItem2, menuItem3, nullptr);

	menu->setPosition(Vec2::ZERO);

	menuItem1->setPosition(VisibleRect::left().x + 100, VisibleRect::top().y - 50);
	menuItem2->setPosition(VisibleRect::left().x + 100, VisibleRect::top().y - 100);
	menuItem3->setPosition(VisibleRect::left().x + 100, VisibleRect::top().y - 150);
	addChild(menu, 0);
	//设置自定义更新器，时间间隔为0.0秒  
	schedule(CC_SCHEDULE_SELECTOR(CameraLcmf3D::updateCamera), 0.0f);
	if (_camera == nullptr)
	{
		//初始化相机  
		_camera = Camera::createPerspective(60, (GLfloat)winSize.width / winSize.height, 1, 1000);
		_camera->setCameraFlag(CameraFlag::USER1);
		_layer3D->addChild(_camera);
	}
	//首先执行视角切换回调函数，先切换到第三视角  
	SwitchViewCallback(this, CameraType::ThirdPerson);

	//创建3D画线类对象  
	DrawLine3D* line = DrawLine3D::create();
	//draw x  
	for (int j = -20; j <= 20; j++)
	{
		//将线段信息存入vector，调用draw绘图，实质是onDraw进行绘制  
		line->drawLine(Vec3(-100, 0, 5 * j), Vec3(100, 0, 5 * j), Color4F(1, 0, 0, 1));
	}
	//draw z  
	for (int j = -20; j <= 20; j++)
	{
		//将线段信息存入vector，调用draw绘图，实质是onDraw进行绘制  
		line->drawLine(Vec3(5 * j, 0, -100), Vec3(5 * j, 0, 100), Color4F(0, 0, 1, 1));
	}
	//draw y //绘制y轴方向的两条线段  
	line->drawLine(Vec3(0, -50, 0), Vec3(0, 0, 0), Color4F(0, 0.5, 0, 1));
	line->drawLine(Vec3(0, 0, 0), Vec3(0, 50, 0), Color4F(0, 1, 0, 1));
	_layer3D->addChild(line);
	//设置mask，使得其对相机可见  
	_layer3D->setCameraMask(2);
}

//onExit函数，退出当前层时执行
void CameraLcmf3D::onExit()
{
	Layer::onExit();
	if (_camera)
		_camera = nullptr;
}

//添加精灵，参数1:精灵坐标，参数2:精灵文件路径，参数3:是否执行动作，参数4:缩放系数，参数5:是否绑定摄像机 
void CameraLcmf3D::addNewSpriteWithCoords(Vec3 pos, std::string fileName, bool playAnimation/* = false*/, float scale/* = 1.0f*/, bool bindCamera /*= false*/)
{
	//创建3D精灵  
	auto sprite = Sprite3D::create(fileName);
	_layer3D->addChild(sprite);
	float globalZOrder = sprite->getGlobalZOrder();
	//设置精灵的坐标和全局Z轴顺序  
	sprite->setPosition3D(Vec3(pos.x, pos.y, pos.z));
	sprite->setGlobalZOrder(globalZOrder);
	if (playAnimation)
	{
		//首先通过骨骼节点创建骨骼动画  
		auto animation = Animation3D::create(fileName, "Take 001");
		if (animation)
		{
			//执行3D动画  
			auto animate = Animate3D::create(animation);
			sprite->runAction(RepeatForever::create(animate));
		}
	}
	//如果绑定摄像机，将sprite赋值给_sprite3D对象  
	if (bindCamera)
	{
		_sprite3D = sprite;
	}
	sprite->setScale(scale);
}

//判断精灵的运动状态
bool CameraLcmf3D::isState(unsigned int nState, unsigned int bit) const
{

	return (nState & bit) == bit;
}

//缩小触摸事件
bool CameraLcmf3D::onTouchesZoomOut(Touch* touch, Event* event)
{
	auto target = static_cast<Label*>(event->getCurrentTarget());
	//将触摸点的位置转化为节点坐标位置  
	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	//如果触摸在节点内  
	if (rect.containsPoint(locationInNode))
	{
		_bZoomOut = true;
		return true;
	}
	return false;
}

//缩小触摸结束
void CameraLcmf3D::onTouchesZoomOutEnd(Touch* touch, Event* event)
{
	_bZoomOut = false;
}

//放大触摸事件 
bool CameraLcmf3D::onTouchesZoomIn(Touch* touch, Event* event)
{
	auto target = static_cast<Label*>(event->getCurrentTarget());

	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	//如果在节点内，设置放大判断值为true  
	if (rect.containsPoint(locationInNode))
	{
		_bZoomIn = true;
		return true;
	}
	return false;
}

//放大触摸结束
void CameraLcmf3D::onTouchesZoomInEnd(Touch* touch, Event* event)
{
	_bZoomIn = false;
}

//旋转
bool CameraLcmf3D::onTouchesRotateLeft(Touch* touch, Event* event)
{

	auto target = static_cast<Label*>(event->getCurrentTarget());

	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	//设置左转触摸为true  
	if (rect.containsPoint(locationInNode))
	{

		_bRotateLeft = true;
		return true;
	}
	return false;
}
void CameraLcmf3D::onTouchesRotateLeftEnd(Touch* touch, Event* event)
{
	//左转触摸结束，恢复判断值为false  
	_bRotateLeft = false;
}

//右转
bool CameraLcmf3D::onTouchesRotateRight(Touch* touch, Event* event)
{
	auto target = static_cast<Label*>(event->getCurrentTarget());

	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	//设置右转触摸判断值为true；  
	if (rect.containsPoint(locationInNode))
	{
		_bRotateRight = true;
		return true;
	}
	return false;
}

//触摸结束，恢复右转判断触摸值为false 
void CameraLcmf3D::onTouchesRotateRightEnd(Touch* touch, Event* event)
{
	_bRotateRight = false;
}

//更新视图  
void CameraLcmf3D::updateCamera(float fDelta)
{
	if (_sprite3D)
	{
		if (_cameraType == CameraType::ThirdPerson)
		{
			//摄像机类型为第三视角，判断精灵的运动状态  
			updateState(fDelta);
			//如果精灵正在行走  
			if (isState(_curState, State_Move))
			{
				//使精灵和相机为行走状态  
				move3D(fDelta);
				//如果行走在转身  
				if (isState(_curState, State_Rotate))
				{
					Vec3 curPos = _sprite3D->getPosition3D();

					Vec3 newFaceDir = _targetPos - curPos;
					newFaceDir.y = 0;
					newFaceDir.normalize();
					Vec3 up;
					_sprite3D->getNodeToWorldTransform().getUpVector(&up);
					up.normalize();
					Vec3 right;
					Vec3::cross(-newFaceDir, up, &right);
					right.normalize();
					Vec3 pos = Vec3(0, 0, 0);
					Mat4 mat;
					mat.m[0] = right.x;
					mat.m[1] = right.y;
					mat.m[2] = right.z;
					mat.m[3] = 0.0f;

					mat.m[4] = up.x;
					mat.m[5] = up.y;
					mat.m[6] = up.z;
					mat.m[7] = 0.0f;

					mat.m[8] = newFaceDir.x;
					mat.m[9] = newFaceDir.y;
					mat.m[10] = newFaceDir.z;
					mat.m[11] = 0.0f;

					mat.m[12] = pos.x;
					mat.m[13] = pos.y;
					mat.m[14] = pos.z;
					mat.m[15] = 1.0f;
					//设置额外的节点变换矩阵  
					_sprite3D->setAdditionalTransform(&mat);
				}
			}
		}
		//如果执行缩小动作  
		if (_bZoomOut == true)
		{
			if (_camera)
			{
				//摄像机类型为第三视角  
				if (_cameraType == CameraType::ThirdPerson)
				{
					Vec3 lookDir = _camera->getPosition3D() - _sprite3D->getPosition3D();
					Vec3 cameraPos = _camera->getPosition3D();
					//求得相机与精灵的空间距离  
					if (lookDir.length() <= 300)
					{
						//相机位置增加，且不改变相机与精灵的相对位置  
						cameraPos += lookDir.getNormalized();
						_camera->setPosition3D(cameraPos);
					}
				}
				//如果是自由视角  
				else if (_cameraType == CameraType::FreeCamera)
				{
					Vec3 cameraPos = _camera->getPosition3D();
					if (cameraPos.length() <= 300)
					{
						//相机距离增加  
						cameraPos += cameraPos.getNormalized();
						_camera->setPosition3D(cameraPos);
					}
				}
			}
		}
		//如果是放大效果  
		if (_bZoomIn == true)
		{
			if (_camera)
			{
				//相机是第三视角  
				if (_cameraType == CameraType::ThirdPerson)
				{
					Vec3 lookDir = _camera->getPosition3D() - _sprite3D->getPosition3D();
					Vec3 cameraPos = _camera->getPosition3D();
					if (lookDir.length() >= 50)
					{
						//相机与精灵的距离大于50，才会执行下面操作  
						//相机位置增加  
						cameraPos -= lookDir.getNormalized();
						_camera->setPosition3D(cameraPos);
					}
				}
				//如果为自由视角  
				else if (_cameraType == CameraType::FreeCamera)
				{
					Vec3 cameraPos = _camera->getPosition3D();
					if (cameraPos.length() >= 50)
					{
						//相机位置增加  
						cameraPos -= cameraPos.getNormalized();
						_camera->setPosition3D(cameraPos);
					}
				}
			}
		}
		//如果是左转效果  
		if (_bRotateLeft == true)
		{
			//如果相机类型为自由视角和第一视角  
			if (_cameraType == CameraType::FreeCamera || _cameraType == CameraType::FirstPerson)
			{
				//y轴增加旋转  
				Vec3  rotation3D = _camera->getRotation3D();
				rotation3D.y += 1;
				_camera->setRotation3D(rotation3D);
			}
		}
		//如果是右转效果  
		if (_bRotateRight == true)
		{
			if (_cameraType == CameraType::FreeCamera || _cameraType == CameraType::FirstPerson)
			{
				//摄像机类型为自由视角和第一视角，y轴减少旋转  
				Vec3  rotation3D = _camera->getRotation3D();
				rotation3D.y -= 1;
				_camera->setRotation3D(rotation3D);
			}
		}
	}
}

//3D精灵移动
void CameraLcmf3D::move3D(float elapsedTime)
{
	if (_sprite3D)
	{
		//获取当前精灵的3D坐标  
		Vec3 curPos = _sprite3D->getPosition3D();
		//计算原来坐标与新坐标的向量  
		Vec3 newFaceDir = _targetPos - curPos;
		//将y轴赋值为0，并将新向量规范化  
		newFaceDir.y = 0.0f;
		newFaceDir.normalize();
		//计算偏移  
		Vec3 offset = newFaceDir * 25.0f * elapsedTime;
		curPos += offset;
		//设置精灵的新坐标  
		_sprite3D->setPosition3D(curPos);
		offset.x = offset.x;
		offset.z = offset.z;
		if (_cameraType == CameraType::ThirdPerson)
		{
			//如果摄像机的类型为第三视角，同时更新摄像机的3D坐标  
			Vec3 cameraPos = _camera->getPosition3D();
			cameraPos.x += offset.x;
			cameraPos.z += offset.z;
			_camera->setPosition3D(cameraPos);
		}
	}
}

//判断精灵的状态
void CameraLcmf3D::updateState(float elapsedTime)
{
	if (_sprite3D)
	{
		Vec3 curPos = _sprite3D->getPosition3D();
		Vec3 curFaceDir;
		//返回世界仿射变换矩阵。矩阵单位是像素。并获取Forward矩阵，参数为目标向量。注意逆世界仿射变换矩阵  
		_sprite3D->getNodeToWorldTransform().getForwardVector(&curFaceDir);
		curFaceDir = -curFaceDir;
		curFaceDir.normalize();
		Vec3 newFaceDir = _targetPos - curPos;
		newFaceDir.y = 0.0f;
		newFaceDir.normalize();
		//计算两个向量的点积，通过返回的值判断精灵运动方向和是否停止  
		float cosAngle = std::fabs(Vec3::dot(curFaceDir, newFaceDir) - 1.0f);
		//返回两个向量的平方距离  
		float dist = curPos.distanceSquared(_targetPos);
		if (dist <= 4.0f)
		{
			if (cosAngle <= 0.01f)
				_curState = State_Idle;
			else
				_curState = State_Rotate;
		}
		else
		{
			if (cosAngle > 0.01f)
				_curState = State_Rotate | State_Move;
			else
				_curState = State_Move;
		}
	}
}
