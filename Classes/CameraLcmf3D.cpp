#include "CameraLcmf3D.h"
#include <algorithm>	//����stl���㷨
#include "VisibleRect.h"

class DrawLine3D : public Node 
{
public:
	//��̬��������
	static DrawLine3D* create();

	//���ߺ���������1����㣻����2���յ㣻����3:�߶���ɫ
	void drawLine(const Vec3& from, const Vec3& to, const Color4F& color);

	void clear() { _buffer.clear(); }

	//�����Ļ�ͼ����
	void onDraw(const Mat4& transform, uint32_t flags);

	//���ƽڵ�
	virtual void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags) override;

CC_CONSTRUCTOR_ACCESS:
	DrawLine3D(){}
	virtual ~DrawLine3D() {}

	virtual bool init();
protected:
	//�Զ����߶δ�Žṹ��
	struct V3F_C4B
	{
		Vec3 vectices;	//�˵�����
		Color4F colors; //�߶���ɫ
	};
	//����߶����ݵ�����
	std::vector<V3F_C4B> _buffer;
	CustomCommand _customCommand;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(DrawLine3D);//����һ���࣬��ֹ�����͸�ֵ��������ع���
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
	//���ýڵ���ɫ����
	setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_LABEL_DISTANCEFIELD_GLOW));
	return true;
}

//���ߺ���
void DrawLine3D::drawLine(const Vec3& from, const Vec3& to, const Color4F& color)
{
	Color4F col = Color4F(color);//�߶���ɫ
	DrawLine3D::V3F_C4B vertex;
	vertex.vectices = from;//start
	vertex.colors = col;//color
	_buffer.push_back(vertex);
	vertex.vectices = to;
	_buffer.push_back(vertex);
}

//���ƽڵ�
void DrawLine3D::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	//���û��ƺ���
	_customCommand.func = CC_CALLBACK_0(DrawLine3D::onDraw, this, transform, flags);

	//����ɫ��������뵽renderer����
	renderer->addCommand(&_customCommand);
}

//����OpenGl�ӿڻ�ͼ���漰opengl֪ʶ��
void DrawLine3D::onDraw(const Mat4& transform, uint32_t flags)
{
	auto glProgram = getGLProgram();//���ص�ǰ�ڵ�GLProgram(shader)
	glProgram->use();//����glUseProgram()����ʹ��shader��ͼ
	glProgram->setUniformsForBuiltins(transform);
	glEnable(GL_DEPTH_TEST);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V3F_C4B), &(_buffer[0].colors));
	glDrawArrays(GL_LINES, 0, static_cast<int>(_buffer.size()));
	glDisable(GL_DEPTH_TEST);
}

//������camare��ʵ��
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

//��ת�ӽǻص�����
void CameraLcmf3D::SwitchViewCallback(Ref* sender, CameraType cameratype)
{
	//�����������͵��������ӽ�
	if (_cameraType == CameraType::FreeCamera)
	{
		_camera->setPosition3D(Vec3(0, 130, 0) + _sprite3D->getPosition3D());
		_camera->lookAt(_sprite3D->getPosition3D(), Vec3(0, 1, 0));
		_RotateRightLabel->setColor(Color3B::WHITE);
		_RotateLeftLabel->setColor(Color3B::WHITE);
		_ZoomInLabel->setColor(Color3B::WHITE);
		_ZoomOutLabel->setColor(Color3B::WHITE);
	}
	//������������Ϊ��һ�ӽ�
	else if (_cameraType == CameraType::FirstPerson)
	{
		Vec3 newFaceDir;
		//�������������任�������صľ��󣬲���ȡfowrward���󣬲���ΪĿ������
		_sprite3D->getNodeToWorldTransform().getForwardVector(&newFaceDir);
		//Ŀ��������׼������Vec3�����x,y,xת��Ϊ��������x��ƽ��+y
		newFaceDir.normalize();
		_camera->setPosition3D(Vec3(0, 35, 0) + _sprite3D->getPosition3D());

		//�����������;���󣬲���1��targetλ�ã� ����2�����ϵ�����
		_camera->lookAt(_sprite3D->getPosition3D() + newFaceDir * 50, Vec3(0, 1, 0));

		_RotateRightLabel->setColor(Color3B::WHITE);
		_RotateLeftLabel->setColor(Color3B::WHITE);
		_ZoomInLabel->setColor(Color3B::GRAY);
		_ZoomOutLabel->setColor(Color3B::GRAY);
	}
	//��������������ǵ����ӽ�
	else if (_cameraType == CameraType::ThirdPerson)
	{
		_camera->setPosition3D(Vec3(0, 130, 130) + _sprite3D->getPosition3D());
		//�����������ͼ����
		_camera->lookAt(_sprite3D->getPosition3D(), Vec3(3, 0, 0));
		_RotateRightLabel->setColor(Color3B::GRAY);
		_RotateLeftLabel->setColor(Color3B::GRAY);
		_ZoomInLabel->setColor(Color3B::WHITE);
		_ZoomOutLabel->setColor(Color3B::WHITE);
	}
}

//�����¼�
void CameraLcmf3D::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
	for(auto &item :touches)
	{
		auto touch = item;
		auto location = touch->getLocation();
	}
}

//��㴥���ƶ���������߶����򣬾����ƶ���ȥ 
void CameraLcmf3D::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
	if (touches.size() == 1)
	{
		auto touch = touches[0];
		//��ȡ��һ���������OpenGL����ϵ�����꣬ԭ������Ļ���½ǡ�  
		//���䣺��Ļ����ϵ��ԭ������Ļ���Ͻ�  
		//��������ϵ����OpenGL����ϵһ�£�λ����Ļ���½�  
		//�ڵ�����ϵ�����ڵ�����½ǣ���OpenGLһ��  
		auto location = touch->getLocation();
		Point newPos = touch->getPreviousLocation() - location;
		if (_cameraType == CameraType::FreeCamera || _cameraType == CameraType::FirstPerson)
		{
			Vec3 cameraDir;
			Vec3 cameraRightDir;
			//�����������任���󡣾���λ�����ء�����ȡForward���󣬲���ΪĿ��������ע�����������任����  
			_camera->getNodeToWorldTransform().getForwardVector(&cameraDir);
			//�����صľ��������淶��  
			cameraDir.normalize();
			//�����صľ�������y�ḳֵΪ0  
			cameraDir.y = 0;
			//������ȷ�ľ�֤�������������仯����  
			_camera->getNodeToWorldTransform().getRightVector(&cameraRightDir);
			//�����淶��  
			cameraRightDir.normalize();
			//����y�ḳֵ0  
			cameraRightDir.y = 0;
			Vec3 cameraPos = _camera->getPosition3D();
			cameraPos += cameraDir*newPos.y*0.1f;
			cameraPos += cameraRightDir*newPos.x*0.1f;
			//���������������3D����  
			_camera->setPosition3D(cameraPos);
			//���������ڣ������������Ϊ��һ�ӽǣ�ִ���������  
			if (_sprite3D &&  _cameraType == CameraType::FirstPerson)
			{
				//���þ����3D���꣬�����浱ǰ��������ֵ�����ڸ��������˶�״̬  
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

	////ע���¼�
	auto winSize = Director::getInstance()->getWinSize();
	auto listner = EventListenerTouchAllAtOnce::create();
	//
	////ע�ᴥ����ʵ�¼�
	listner->onTouchesBegan = CC_CALLBACK_2(CameraLcmf3D::onTouchesBegan, this);
	listner->onTouchesMoved = CC_CALLBACK_2(CameraLcmf3D::onTouchesMoved, this);
	listner->onTouchesEnded = CC_CALLBACK_2(CameraLcmf3D::onTouchesBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listner, this);

	//�����µĲ㣬���ڼ���3D������߶�
	auto layer3D = Layer::create();
	addChild(layer3D, 0);
	_layer3D = layer3D;
	_curState = State_None;

	//��Ӿ����������Ϊ:Vec3�����������ļ�·�����Ƿ�ִ�ж���������ϵ�����Ƿ�������
	addNewSpriteWithCoords(Vec3(0, 0, 0), "ReskinGirl.c3b", true, 0.2f, true);

	TTFConfig ttfConfig("fonts/arial.ttf", 30);
	//�������4��Label���󵽶�Ӧ��Node�ڵ��ϣ�����4��Label����ע������¼�  
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

	//����������ӽǲ˵�ѡ����ص�ת���ӽǺ���  
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
	//�����Զ����������ʱ����Ϊ0.0��  
	schedule(CC_SCHEDULE_SELECTOR(CameraLcmf3D::updateCamera), 0.0f);
	if (_camera == nullptr)
	{
		//��ʼ�����  
		_camera = Camera::createPerspective(60, (GLfloat)winSize.width / winSize.height, 1, 1000);
		_camera->setCameraFlag(CameraFlag::USER1);
		_layer3D->addChild(_camera);
	}
	//����ִ���ӽ��л��ص����������л��������ӽ�  
	SwitchViewCallback(this, CameraType::ThirdPerson);

	//����3D���������  
	DrawLine3D* line = DrawLine3D::create();
	//draw x  
	for (int j = -20; j <= 20; j++)
	{
		//���߶���Ϣ����vector������draw��ͼ��ʵ����onDraw���л���  
		line->drawLine(Vec3(-100, 0, 5 * j), Vec3(100, 0, 5 * j), Color4F(1, 0, 0, 1));
	}
	//draw z  
	for (int j = -20; j <= 20; j++)
	{
		//���߶���Ϣ����vector������draw��ͼ��ʵ����onDraw���л���  
		line->drawLine(Vec3(5 * j, 0, -100), Vec3(5 * j, 0, 100), Color4F(0, 0, 1, 1));
	}
	//draw y //����y�᷽��������߶�  
	line->drawLine(Vec3(0, -50, 0), Vec3(0, 0, 0), Color4F(0, 0.5, 0, 1));
	line->drawLine(Vec3(0, 0, 0), Vec3(0, 50, 0), Color4F(0, 1, 0, 1));
	_layer3D->addChild(line);
	//����mask��ʹ���������ɼ�  
	_layer3D->setCameraMask(2);
}

//onExit�������˳���ǰ��ʱִ��
void CameraLcmf3D::onExit()
{
	Layer::onExit();
	if (_camera)
		_camera = nullptr;
}

//��Ӿ��飬����1:�������꣬����2:�����ļ�·��������3:�Ƿ�ִ�ж���������4:����ϵ��������5:�Ƿ������� 
void CameraLcmf3D::addNewSpriteWithCoords(Vec3 pos, std::string fileName, bool playAnimation/* = false*/, float scale/* = 1.0f*/, bool bindCamera /*= false*/)
{
	//����3D����  
	auto sprite = Sprite3D::create(fileName);
	_layer3D->addChild(sprite);
	float globalZOrder = sprite->getGlobalZOrder();
	//���þ���������ȫ��Z��˳��  
	sprite->setPosition3D(Vec3(pos.x, pos.y, pos.z));
	sprite->setGlobalZOrder(globalZOrder);
	if (playAnimation)
	{
		//����ͨ�������ڵ㴴����������  
		auto animation = Animation3D::create(fileName, "Take 001");
		if (animation)
		{
			//ִ��3D����  
			auto animate = Animate3D::create(animation);
			sprite->runAction(RepeatForever::create(animate));
		}
	}
	//��������������sprite��ֵ��_sprite3D����  
	if (bindCamera)
	{
		_sprite3D = sprite;
	}
	sprite->setScale(scale);
}

//�жϾ�����˶�״̬
bool CameraLcmf3D::isState(unsigned int nState, unsigned int bit) const
{

	return (nState & bit) == bit;
}

//��С�����¼�
bool CameraLcmf3D::onTouchesZoomOut(Touch* touch, Event* event)
{
	auto target = static_cast<Label*>(event->getCurrentTarget());
	//���������λ��ת��Ϊ�ڵ�����λ��  
	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	//��������ڽڵ���  
	if (rect.containsPoint(locationInNode))
	{
		_bZoomOut = true;
		return true;
	}
	return false;
}

//��С��������
void CameraLcmf3D::onTouchesZoomOutEnd(Touch* touch, Event* event)
{
	_bZoomOut = false;
}

//�Ŵ����¼� 
bool CameraLcmf3D::onTouchesZoomIn(Touch* touch, Event* event)
{
	auto target = static_cast<Label*>(event->getCurrentTarget());

	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	//����ڽڵ��ڣ����÷Ŵ��ж�ֵΪtrue  
	if (rect.containsPoint(locationInNode))
	{
		_bZoomIn = true;
		return true;
	}
	return false;
}

//�Ŵ�������
void CameraLcmf3D::onTouchesZoomInEnd(Touch* touch, Event* event)
{
	_bZoomIn = false;
}

//��ת
bool CameraLcmf3D::onTouchesRotateLeft(Touch* touch, Event* event)
{

	auto target = static_cast<Label*>(event->getCurrentTarget());

	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	//������ת����Ϊtrue  
	if (rect.containsPoint(locationInNode))
	{

		_bRotateLeft = true;
		return true;
	}
	return false;
}
void CameraLcmf3D::onTouchesRotateLeftEnd(Touch* touch, Event* event)
{
	//��ת�����������ָ��ж�ֵΪfalse  
	_bRotateLeft = false;
}

//��ת
bool CameraLcmf3D::onTouchesRotateRight(Touch* touch, Event* event)
{
	auto target = static_cast<Label*>(event->getCurrentTarget());

	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	//������ת�����ж�ֵΪtrue��  
	if (rect.containsPoint(locationInNode))
	{
		_bRotateRight = true;
		return true;
	}
	return false;
}

//�����������ָ���ת�жϴ���ֵΪfalse 
void CameraLcmf3D::onTouchesRotateRightEnd(Touch* touch, Event* event)
{
	_bRotateRight = false;
}

//������ͼ  
void CameraLcmf3D::updateCamera(float fDelta)
{
	if (_sprite3D)
	{
		if (_cameraType == CameraType::ThirdPerson)
		{
			//���������Ϊ�����ӽǣ��жϾ�����˶�״̬  
			updateState(fDelta);
			//���������������  
			if (isState(_curState, State_Move))
			{
				//ʹ��������Ϊ����״̬  
				move3D(fDelta);
				//���������ת��  
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
					//���ö���Ľڵ�任����  
					_sprite3D->setAdditionalTransform(&mat);
				}
			}
		}
		//���ִ����С����  
		if (_bZoomOut == true)
		{
			if (_camera)
			{
				//���������Ϊ�����ӽ�  
				if (_cameraType == CameraType::ThirdPerson)
				{
					Vec3 lookDir = _camera->getPosition3D() - _sprite3D->getPosition3D();
					Vec3 cameraPos = _camera->getPosition3D();
					//�������뾫��Ŀռ����  
					if (lookDir.length() <= 300)
					{
						//���λ�����ӣ��Ҳ��ı�����뾫������λ��  
						cameraPos += lookDir.getNormalized();
						_camera->setPosition3D(cameraPos);
					}
				}
				//����������ӽ�  
				else if (_cameraType == CameraType::FreeCamera)
				{
					Vec3 cameraPos = _camera->getPosition3D();
					if (cameraPos.length() <= 300)
					{
						//�����������  
						cameraPos += cameraPos.getNormalized();
						_camera->setPosition3D(cameraPos);
					}
				}
			}
		}
		//����ǷŴ�Ч��  
		if (_bZoomIn == true)
		{
			if (_camera)
			{
				//����ǵ����ӽ�  
				if (_cameraType == CameraType::ThirdPerson)
				{
					Vec3 lookDir = _camera->getPosition3D() - _sprite3D->getPosition3D();
					Vec3 cameraPos = _camera->getPosition3D();
					if (lookDir.length() >= 50)
					{
						//����뾫��ľ������50���Ż�ִ���������  
						//���λ������  
						cameraPos -= lookDir.getNormalized();
						_camera->setPosition3D(cameraPos);
					}
				}
				//���Ϊ�����ӽ�  
				else if (_cameraType == CameraType::FreeCamera)
				{
					Vec3 cameraPos = _camera->getPosition3D();
					if (cameraPos.length() >= 50)
					{
						//���λ������  
						cameraPos -= cameraPos.getNormalized();
						_camera->setPosition3D(cameraPos);
					}
				}
			}
		}
		//�������תЧ��  
		if (_bRotateLeft == true)
		{
			//����������Ϊ�����ӽǺ͵�һ�ӽ�  
			if (_cameraType == CameraType::FreeCamera || _cameraType == CameraType::FirstPerson)
			{
				//y��������ת  
				Vec3  rotation3D = _camera->getRotation3D();
				rotation3D.y += 1;
				_camera->setRotation3D(rotation3D);
			}
		}
		//�������תЧ��  
		if (_bRotateRight == true)
		{
			if (_cameraType == CameraType::FreeCamera || _cameraType == CameraType::FirstPerson)
			{
				//���������Ϊ�����ӽǺ͵�һ�ӽǣ�y�������ת  
				Vec3  rotation3D = _camera->getRotation3D();
				rotation3D.y -= 1;
				_camera->setRotation3D(rotation3D);
			}
		}
	}
}

//3D�����ƶ�
void CameraLcmf3D::move3D(float elapsedTime)
{
	if (_sprite3D)
	{
		//��ȡ��ǰ�����3D����  
		Vec3 curPos = _sprite3D->getPosition3D();
		//����ԭ�������������������  
		Vec3 newFaceDir = _targetPos - curPos;
		//��y�ḳֵΪ0�������������淶��  
		newFaceDir.y = 0.0f;
		newFaceDir.normalize();
		//����ƫ��  
		Vec3 offset = newFaceDir * 25.0f * elapsedTime;
		curPos += offset;
		//���þ����������  
		_sprite3D->setPosition3D(curPos);
		offset.x = offset.x;
		offset.z = offset.z;
		if (_cameraType == CameraType::ThirdPerson)
		{
			//��������������Ϊ�����ӽǣ�ͬʱ�����������3D����  
			Vec3 cameraPos = _camera->getPosition3D();
			cameraPos.x += offset.x;
			cameraPos.z += offset.z;
			_camera->setPosition3D(cameraPos);
		}
	}
}

//�жϾ����״̬
void CameraLcmf3D::updateState(float elapsedTime)
{
	if (_sprite3D)
	{
		Vec3 curPos = _sprite3D->getPosition3D();
		Vec3 curFaceDir;
		//�����������任���󡣾���λ�����ء�����ȡForward���󣬲���ΪĿ��������ע�����������任����  
		_sprite3D->getNodeToWorldTransform().getForwardVector(&curFaceDir);
		curFaceDir = -curFaceDir;
		curFaceDir.normalize();
		Vec3 newFaceDir = _targetPos - curPos;
		newFaceDir.y = 0.0f;
		newFaceDir.normalize();
		//�������������ĵ����ͨ�����ص�ֵ�жϾ����˶�������Ƿ�ֹͣ  
		float cosAngle = std::fabs(Vec3::dot(curFaceDir, newFaceDir) - 1.0f);
		//��������������ƽ������  
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
