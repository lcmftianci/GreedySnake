#include "Hero.h"


Hero::Hero(void)
{
	IsRunning = false;//没有动
	HeroDirection = false; //向右运动
	HeroName = NULL;
}
Hero::~Hero(void)
{

}

//根据图片创建英雄
void Hero::InitHeroSprite(char* chHeroPathName)
{
	HeroName = chHeroPathName;
	this->m_HeroSprite = Sprite::create(chHeroPathName);
	this->addChild(m_HeroSprite);
}

//设置动画，num为图片的数目，run_direction为精灵脸朝向，false朝右，name_each为name_png的每一张小图片的公共名称部分
void Hero::SetAnimation(const char* name_plist, const char* name_png, const unsigned int num, bool run_direction)
{
	if (HeroDirection != run_direction)
	{
		HeroDirection = run_direction;
		m_HeroSprite->setFlipX(run_direction);
	}
	if (IsRunning)
		return;

	SpriteFrameCache* m_frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_frameCache->addSpriteFramesWithFile(name_plist, name_png);

	Vector<SpriteFrame*> arrFrame;

	unsigned int i;
	for (i = 2; i <= num; i++)
	{
		SpriteFrame* frame = m_frameCache->spriteFrameByName(CCString::createWithFormat("run_%d.png", i)->getCString());
		arrFrame.pushBack(frame);
	}
	//创建动画
	Animation* ani = Animation::createWithSpriteFrames(arrFrame);
	if (HeroDirection != run_direction)
		HeroDirection = run_direction;

	ani->setLoops(-1);
	ani->setDelayPerUnit(0.1f);

	Animate* act = Animate::create(ani);

	m_HeroSprite->runAction(act);
	IsRunning = true;
}

//停止动画
void Hero::StopAnimation()
{
	if (!IsRunning)
		return;
	m_HeroSprite->stopAllActions();
								   
	this->removeChild(m_HeroSprite, TRUE);
	m_HeroSprite = Sprite::create(HeroName);
	m_HeroSprite->setFlipX(HeroDirection);
	this->addChild(m_HeroSprite);
	IsRunning = false;
}