/**
* @file Move.h
*/

#ifndef COMPONENT_BULLETMOVE_H_INCLUDED
#define COMPONENT_BULLETMOVE_H_INCLUDED
#include "../Component.h"
#include "../../GameObject/GameObject.h"

/**
* �Q�[���I�u�W�F�N�g���ړ�����
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

	//���s���ɕω�����ϐ�
	float vx = 0;			//���˃x�N�g��X
	float vy = 0;			//���˃x�N�g��Y
};
#endif // COMPONENT_BULLETMOVE_H_INCLUDED