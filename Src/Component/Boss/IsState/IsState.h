/**
* IsState.h
*/
#ifndef ISSTATE_H_INCLUDE
#define ISSTATE_H_INCLUDE
#include<memory>
#include"../../Easing.h"

class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>;
class FactoryBase;
using FactoryBasePtr = std::shared_ptr<FactoryBase>;

namespace STATE
{
	class IsState
	{
	public:
		virtual void Initialize() = 0;
		virtual void Execute(float deltaTime) = 0;
		virtual void Finalize() = 0;
		virtual void GiveGameObject(GameObjectPtr gameObject) = 0;
		virtual void GiveBaseFactory(FactoryBasePtr base) = 0;

	protected:
		float val = 0.0f;
		int moveRand = 0;
		int bossMove = 0;
	};
}
#endif // !ISSTATE_H_INCLUDE