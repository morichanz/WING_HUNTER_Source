/**
* @file OutOfScreen.h
*/
#ifndef COMPONENT_OUTOFSCREEN_H_INCLUDED
#define COMPONENT_OUTOFSCREEN_H_INCLUDED
#include "../Component/Component.h"
#include "../GameObject/GameObject.h"

/**
* 画面外と判定されたオブジェクトを削除する
*/
class OutOfScreen : public Component
{
public:
	OutOfScreen() = default;
	virtual ~OutOfScreen() = default;

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{	
		const float	xMin = -64;
		const float xMax = 18 * 32;
		const float yMin = -32;
		const float yMax = 32 * 32;
		if (gameObject.x <= xMin || gameObject.x >= xMax ||
			gameObject.y <= yMin || gameObject.y >= yMax)
		{
			gameObject.scoreFlg = false;
			gameObject.isDead = true;
		}
		else
		{
			gameObject.scoreFlg = true;
		}
	}

};
#endif // COMPONENT_OUTOFSCREEN_H_INCLUDED