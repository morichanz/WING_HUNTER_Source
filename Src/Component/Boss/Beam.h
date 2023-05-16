/**
* @file Beam.h
*/
#ifndef COMPONENT_BEAM_H_INCLUDE
#define COMPONENT_BEAM_H_INCLUDE
#include"../../Component/Component.h"
#include"../../GameObject/GameObject.h"

class Beam : public Component
{
public:
	Beam() = default;
	virtual ~Beam() = default;
	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		gameObject.x = weapon->x;
		gameObject.y = weapon->y;
	}
	float vy = 0;
	GameObjectPtr weapon;
};
#endif // !COMPONENT_BEAM_H_INCLUDE
