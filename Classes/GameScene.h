#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"

class GameScene : public cocos2d::LayerColor
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	void menuCloseCallback(Ref * pRef);

	void chanScene(Ref * pRef);
};



#endif