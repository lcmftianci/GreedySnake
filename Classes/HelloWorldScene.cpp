#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameScene.h"
#include "VirtualHandle.h"
#include "Hero.h"
#include "CameraLcmf3D.h"

USING_NS_CC;
const double PI = 3.1415926;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

	//auto layerCamera = CameraLcmf3D::create();

	//scene->addChild(layerCamera);

    // add layer as a child to scene

	//auto camera = Camera::create();

	//camera->setCameraFlag(CameraFlag::USER1);

	////hero->setCameraMask(2);    //因为2 & CameraFlag::USER1 !=0，所以cameraMask=2与CameraFlag::USER1匹配，sprite将使用上面创建的camera
	//layer->setCameraMask(2);

	//this->addChild(camera);
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    ///**  you can create scene with following comment code instead of using csb file.
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	/*
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    **/
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    //auto rootNode = CSLoader::createNode("MainScene.csb");

	//add btn to change a new Scene
	//auto Sprite = 
	auto rootNode = Sprite::create("HelloWorld.png");
	
	rootNode->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.x + visibleSize.height / 2));
	addChild(rootNode);

	auto newBtn = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(HelloWorld::loadANewScene, this));
	newBtn->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2 - 20,
		+20 + origin.y + closeItem->getContentSize().height / 2));

	auto newScene = Menu::create(newBtn, NULL);
	newScene->setPosition(Vec2::ZERO);
	this->addChild(newScene, 1);

	ball = Sprite::create("GameUI.jpg");
	
	addChild(ball);
	ball->setPosition(200, 200);

	//初始化任务运动速度
	m_HeroSpeed = 2;//人物运动速度
	m_HeroFastSpeed = 4;//人物运动速度（快）
	//repeate

	//添加虚拟摇杆
	//VirtualHandle* handle = VirtualHandle::createJoystick("CloseNormal.png", "GameUI.jpg", ccp(240, 160));
	//addChild(handle);
	//handle->startJoystick(true);

	//方法二
	pVirtualHandle = VirtualHandle::createJoystick("Direction_bt.png", "Direction_bc.png", ccp(300, 200));
	this->addChild(pVirtualHandle, 2);
	pVirtualHandle->startJoystick(true);

	//设置英雄动画
	hero = Hero::create();
	hero->InitHeroSprite("zhaoyun.png");
	hero->setPosition(ccp(200, 200));
	this->addChild(hero, 1);

	//获取窗口大小
	auto winSize = Director::sharedDirector()->getWinSize();

	//设置相机
	//Camera* pCamera = Camera::create();
	//pCamera->setCameraFlag(CameraFlag::USER1);
	//this->addChild(pCamera);
	//hero->setCameraMask(2);
	//Vec3 eyesPosOld = pCamera->getPosition3D();
	//Vec3 eyesPos = Vec3(hero->getPositionX(), hero->getPositionY(), eyesPosOld.z);
	//pCamera->setPosition3D(eyesPos);
	//assert(eyesPos.z > 0);
	//pCamera->lookAt(Vec3(eyesPos.x, eyesPos.y, 0), Vec3(0, 1, 0));

	//auto camera = Camera::createPerspective(60, float(visibleSize.width / visibleSize.height), 1, 1000);
	//camera->setCameraFlag(CameraFlag::USER1);
	//this->setCameraMask((unsigned short)CameraFlag::USER1);
	//camera->setPosition3D(Vec3());
	//camera->lookAt(Vec3(0, 0, 0), Vec3::UNIT_Y);
	//this->addChild(camera);

	//Camera* camera = Camera::createPerspective(60, float(visibleSize.width / visibleSize.height), 1, 200);
	//this->addChild(camera);
	//camera->setCameraFlag(CameraFlag::USER1);
	//this->setCameraMask((unsigned short)CameraFlag::USER1);

	//auto _camera = Camera::createPerspective(60, winSize.width / winSize.height, 1, 1000);
	//_camera->setCameraFlag(CameraFlag::USER1);
	//_camera->setPosition3D(Vec3());
	//this->setCameraMask((unsigned short)CameraFlag::USER1);
	//_camera->lookAt(Vec3(0, 0, 0), Vec3::UNIT_Y);
	//this->addChild(_camera);

	//Size winCameSize = Director::sharedDirector()->getWinSize();

	//Node中有个_cameraMask的属性，当相机的_cameraFlag & _cameraMask为true时，该Node可以被该相机看到。
	//	所以在上述相机的创建代码中，camera的CameraFlag设置为CameraFlag::USER1，并且该layer的CameraMask为2，
	//	则表示该layer只能被CameraFlag::USER1相机看到。如果你设置的精灵的cameraMask是3的话，
	//	它也是能被cameraFlag为CameraFlag::USER1和CameraFlag::USER2的相机看到的。
	//	我们还要注意如果你的精灵是在layer设置cameraMask之后添加的，它是不会被看到的，
	//	还需要手动再设置精灵的cameraMask。不要以为这样就可以了，最后我们还要把相机添加到场景中，
	//	不然我们还是看不到效果的，一定要记住呀，下图就是把相机加到场景中的代码：

	//auto camera = Camera::create();

	//camera->setCameraFlag(CameraFlag::USER1);

	////hero->setCameraMask(2);    //因为2 & CameraFlag::USER1 !=0，所以cameraMask=2与CameraFlag::USER1匹配，sprite将使用上面创建的camera
	//rootNode->setCameraMask(2);

	//this->addChild(camera);

	//Vec3 eyePosOld = camera->getPosition3D();

	//Vec3 eyePos = Vec3(eyePosOld.x, eyePosOld.y, eyePosOld.z);

	//camera->setPosition3D(eyePos);

	//assert(eyePos.z > 0);

	//camera->lookAt(Vec3(eyePos.x, eyePos.y, 0), Vec3(0, 1, 0));

	//注意，这里有个坑：camera->lookAt必须在camera->setPostion3D之后，因为lookAt中有一句

	//Vec3::subtract(this->getPosition3D(), lookAtPos, &zaxis)，即相减得出相机空间z轴，

	//使用了getPosition3D。所以必须先设定好position3D再调lookAt才能得到正确结果。

	//创建透视相机
	//Camera* cameraExample = Camera::createPerspective(30, (GLfloat)visibleSize.width / visibleSize.height, 10, 200);
	//创建正交相机
	//Camera* cameraExample = Camera::createOrthographic(100, 100, 10, 200);
	//设置相机位置
	//Vec2 hPos = hero->getPosition();
	//cameraExample->setPosition3D(Vec3(hPos.x, hPos.y, -50));
	//cameraExample->lookAt(Vec3(hPos.x, hPos.y, 0), Vec3(0, 1, 0));
	//cameraExample->setCameraFlag(CameraFlag::USER1);
	
	//测试跟随代码
	//定义一个MoveBy动作
	//auto move = MoveBy::create(5, Vec2(visibleSize.width * 3, 0));
	////定义一个和moveBy动作相反的动作
	//auto move_back = move->reverse();
	////定义一个组合动作
	//auto seq = Sequence::create(move, move_back, NULL);
	////定义一个重复动作
	//auto rep = RepeatForever::create(seq);
	//精灵执行重复动作
	/*hero->runAction(rep);*/
	//当前层执行一个跟随动作
	//this->runAction(Follow::create(hero, Rect(0, 0, visibleSize.width * 2 - 100, visibleSize.height)));
	this->runAction(Follow::create(hero));
	//pVirtualHandle->runAction(Follow::create(hero));
	scheduleUpdate();

    return true;
}


void HelloWorld::update(float dt)
{
	ball->setPositionX(ball->getPositionX() + 5);
	//pVirtualHandle->setPosition(hero->getPosition().x, hero->getPosition().y);

	if (ball->getPositionX() > visibleSize.width + ball->getContentSize().width / 2)
	{
		ball->setPositionX(-ball->getContentSize().width / 2);
	}

	switch (pVirtualHandle->snapeDirection)
	{
	case 1:
		////其实这里我们需要做更多的是不仅仅是跟随控制走，只要有方向走就不能停
		//hero->SetAnimation("run_animation.plist", "run_animation.png", 8, pVirtualHandle->rockerRun);
		//hero->setPosition(ccp(hero->getPosition().x + 1, hero->getPosition().y));
		////如何跳出循环，就在用户按下别的方向时,回调函数判断如果用户有了新的动作就跳出循环
		m_dirFlag = 1;
		break;
	case  2:
		//hero->SetAnimation("run_animation.plist", "run_animation.png", 8, pVirtualHandle->rockerRun);
		//hero->setPosition(ccp(hero->getPosition().x, hero->getPosition().y + 1));
		m_dirFlag = 2;
		break;
	case 3:
		hero->SetAnimation("run_animation.plist", "run_animation.png", 8, pVirtualHandle->rockerRun);
		hero->setPosition(ccp(hero->getPosition().x - 1, hero->getPosition().y));
		m_dirFlag = 3;
		break;
	case 4:
		/*hero->SetAnimation("run_animation.plist", "run_animation.png", 8, pVirtualHandle->rockerRun);
		hero->setPosition(ccp(hero->getPosition().x, hero->getPosition().y - 1));*/
		m_dirFlag = 4;
		break;
	case 5:
		m_dirFlag = 5;
		break;
	case 6:
		m_dirFlag = 6;
		break;
	case 7:
		m_dirFlag = 7;
		break;
	case 8:
		m_dirFlag = 8;
		break;
	case 9:
		m_dirFlag = 9;
		break;
	case 10:
		m_dirFlag = 10;
		break;
	case 11:
		m_dirFlag = 11;
		break;
	case 12:
		m_dirFlag = 12;
		break;
	default:
		/*hero->StopAnimation();*/
		int i = 0;
		break;
	}

	//如果执行一定到底
	if (m_dirFlag == 1)
	{
		hero->SetAnimation("run_animation.plist", "run_animation.png", 8, pVirtualHandle->rockerRun);
		hero->setPosition(ccp(hero->getPosition().x + m_HeroSpeed, hero->getPosition().y));
		log("取得virtualhand = %d", pVirtualHandle->snapeDirection);
	}
	else if (m_dirFlag == 2)
	{
		hero->SetAnimation("run_animation.plist", "run_animation.png", 8, pVirtualHandle->rockerRun);
		hero->setPosition(ccp(hero->getPosition().x + cos(PI/6)*m_HeroSpeed, hero->getPosition().y + cos(PI / 6)*m_HeroSpeed));
		log("取得virtualhand = %d", pVirtualHandle->snapeDirection);
	}
	else if (m_dirFlag == 3)
	{
		hero->SetAnimation("run_animation.plist", "run_animation.png", 8, pVirtualHandle->rockerRun);
		hero->setPosition(ccp(hero->getPosition().x + cos(PI / 3)*m_HeroSpeed, hero->getPosition().y + cos(PI / 3)*m_HeroSpeed));
		log("取得virtualhand = %d", pVirtualHandle->snapeDirection);
	}
	else if (m_dirFlag == 4)
	{
		hero->SetAnimation("run_animation.plist", "run_animation.png", 8, pVirtualHandle->rockerRun);
		hero->setPosition(ccp(hero->getPosition().x, hero->getPosition().y + m_HeroSpeed));
		log("取得virtualhand = %d", pVirtualHandle->snapeDirection);
	}
	else if (m_dirFlag == 5)
	{
		hero->SetAnimation("run_animation.plist", "run_animation.png", 8, pVirtualHandle->rockerRun);
		hero->setPosition(ccp(hero->getPosition().x - sin(PI/6) * m_HeroSpeed, hero->getPosition().y + cos(PI/6) * m_HeroSpeed));
		log("取得virtualhand = %d", pVirtualHandle->snapeDirection);
	}
	else if (m_dirFlag == 6)
	{
		hero->SetAnimation("run_animation.plist", "run_animation.png", 8, pVirtualHandle->rockerRun);
		hero->setPosition(ccp(hero->getPosition().x - sin(PI/3) * m_HeroSpeed, hero->getPosition().y + cos(PI / 3) * m_HeroSpeed));
		log("取得virtualhand = %d", pVirtualHandle->snapeDirection);
	}
	else if (m_dirFlag == 7)
	{
		hero->SetAnimation("run_animation.plist", "run_animation.png", 8, pVirtualHandle->rockerRun);
		hero->setPosition(ccp(hero->getPosition().x - m_HeroSpeed, hero->getPosition().y - 1));
		log("取得virtualhand = %d", pVirtualHandle->snapeDirection);
	}
	else if (m_dirFlag == 8)
	{
		hero->SetAnimation("run_animation.plist", "run_animation.png", 8, pVirtualHandle->rockerRun);
		hero->setPosition(ccp(hero->getPosition().x - cos(PI/6) * m_HeroSpeed, hero->getPosition().y - sin(PI / 6) * m_HeroSpeed));
		log("取得virtualhand = %d", pVirtualHandle->snapeDirection);
	}
	else if (m_dirFlag == 9)
	{
		hero->SetAnimation("run_animation.plist", "run_animation.png", 8, pVirtualHandle->rockerRun);
		hero->setPosition(ccp(hero->getPosition().x - cos(PI / 3) * m_HeroSpeed, hero->getPosition().y - sin(PI / 3) * m_HeroSpeed));
		log("取得virtualhand = %d", pVirtualHandle->snapeDirection);
	}
	else if (m_dirFlag == 10)
	{
		hero->SetAnimation("run_animation.plist", "run_animation.png", 8, pVirtualHandle->rockerRun);
		hero->setPosition(ccp(hero->getPosition().x, hero->getPosition().y - m_HeroSpeed));
		log("取得virtualhand = %d", pVirtualHandle->snapeDirection);
	}
	else if (m_dirFlag == 11)
	{
		hero->SetAnimation("run_animation.plist", "run_animation.png", 8, pVirtualHandle->rockerRun);
		hero->setPosition(ccp(hero->getPosition().x + sin(PI/6) * m_HeroSpeed, hero->getPosition().y - cos(PI / 6) * m_HeroSpeed));
		log("取得virtualhand = %d", pVirtualHandle->snapeDirection);
	}
	else if (m_dirFlag == 12)
	{
		hero->SetAnimation("run_animation.plist", "run_animation.png", 8, pVirtualHandle->rockerRun);
		hero->setPosition(ccp(hero->getPosition().x + sin(PI / 3) * m_HeroSpeed, hero->getPosition().y - cos(PI / 3) * m_HeroSpeed));
		log("取得virtualhand = %d", pVirtualHandle->snapeDirection);
	/*	hero->StopAnimation();*/
	}
	else if (m_dirFlag == 0)
	{
		hero->StopAnimation();
	}

	if (hero->getPositionX() > visibleSize.width + hero->getContentSize().width / 2)
	{
		
		m_dirFlag = 0;
		hero->setPositionX(-hero->getContentSize().width / 2);
	}
}


void HelloWorld::menuCloseCallback(Ref * pSender)
{
	//close application
	Director::getInstance()->end();
}

void HelloWorld::loadANewScene(Ref * ref)
{
	//auto nextNode = CSLoader::createNode("NextScene.csb");
	//addChild(nextNode);
	//GameScene game;
	auto Scene = GameScene::createScene();
	auto transition = TransitionZoomFlipY::create(1.0f, Scene);
	Director::getInstance()->replaceScene(transition);
}