/**
* @file BoundBullet.h
*/
#ifndef COMPONENT_BOUNDBULLET_H_INCLUDED
#define COMPONENT_BOUNDBULLET_H_INCLUDED
#include "../Component.h"
#include"../../GameObject/GameObject.h"

/**
* ゲームオブジェクトを移動する
*/
class BoundBullet : public Component
{
public:
	BoundBullet() = default;
	virtual ~BoundBullet() = default;
	virtual void Update(GameObject& gameObject, float deltaTime) override
	{
		if (boundCnt < boundCounter)
		{
			if (gameObject.x <= 0 || gameObject.x >= 525)
			{
				x = -1;
				boundCnt++;
			}
			if (gameObject.y <= 0 || gameObject.y >= 685)
			{
				y = -1;
				boundCnt++;
			}
		}
		gameObject.x += x * vx * deltaTime;
		gameObject.y += y * vy * deltaTime;
	}
	float vx = 0;
	float vy = 0;
	float x = 1;
	float y = 1;
	int boundCounter = 0;

private:
	int boundCnt = 0;
};
#endif // COMPONENT_BOUNDBULLET_H_INCLUDED