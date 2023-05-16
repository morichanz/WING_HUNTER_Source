/**
* @file Rotation.h
*/
#ifndef COMPONENT_ROTATION_H_INCLUDE
#define COMPONENT_ROTATION_H_INCLUDE
#include"../Component.h"
#include"../../GameObject/GameObject.h"
#include"../../VecMath/VecMath.h"

/**
* 耐久値コンポーネント
*/
class Rotation : public Component
{
public:
	Rotation() = default;
	virtual ~Rotation() = default;
	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		gameObject.rotation[0] += VecMath::radians(roatex) * deltaTime;
		gameObject.rotation[1] += VecMath::radians(roatey) * deltaTime;
		gameObject.rotation[2] += VecMath::radians(roatez) * deltaTime;
	}

	float roatex = 0;
	float roatey = 0;
	float roatez = 0;
};

#endif // !COMPONENT_ROTATION_H_INCLUDE