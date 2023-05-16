/**
* @file Move.h
*/

#ifndef COMPONENT_BULLETMOVE_H_INCLUDED
#define COMPONENT_BULLETMOVE_H_INCLUDED
#include "../Component.h"
#include "../../GameObject/GameObject.h"

/**
* ゲームオブジェクトを移動する
*/
class BulletMove : public Component
{
public:
	BulletMove() = default;
	virtual ~BulletMove() = default;
	
	virtual void Start(GameObject& gameObject)override
	{
		vx = sin(angle);
		vy = cos(angle);
	}

	virtual void Update(GameObject& gameObject, float deltaTime) override
	{
		gameObject.x += vx * speed * deltaTime;
		gameObject.y += vy * speed * deltaTime;
	}

	float angle = 0;
	float speed = 0;

	//実行中に変化する変数
	float vx = 0;			//発射ベクトルX
	float vy = 0;			//発射ベクトルY
};
#endif // COMPONENT_BULLETMOVE_H_INCLUDED