/**
* @file Move.h
*/

#ifndef COMPONENT_MOVE_H_INCLUDED
#define COMPONENT_MOVE_H_INCLUDED
#include "Component.h"
#include "../GameObject/GameObject.h"

/**
* ゲームオブジェクトを移動する
*/
class Move : public Component
{
public:
	Move() = default;
	virtual ~Move() = default;
	virtual void Update(GameObject& gameObject, float deltaTime) override
	{
		if (!jetFlg)
		{
			gameObject.x += vx * deltaTime;
			gameObject.y += vy * deltaTime;
		}
		else
		{
			gameObject.x = player->x;
			gameObject.y = player->y;
			gameObject.z = player->z;
		}
	}
	float vx = 0;
	float vy = 0;

	GameObjectPtr player;
	bool jetFlg = false;
};
#endif // COMPONENT_MOVE_H_INCLUDED