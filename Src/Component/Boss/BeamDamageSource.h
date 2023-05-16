/**
* @file BeamDamageSource.h
*/
#ifndef COMPONENT_BOSSDAMAGESOURCE_H_INSTANCE
#define COMPONENT_BOSSDAMAGESOURCE_H_INSTANCE
#include "../Component.h"
#include"../../GameObject/GameObject.h"
#include<string>

/**
* ダメージ減コンポーネント
*/
class BeamDamageSource : public Component
{
public:
	BeamDamageSource() = default;
	virtual ~BeamDamageSource() = default;
	virtual void OnCollision(GameObject& gameObject, GameObject& other) override
	{
		//ターゲット名と同じ名前のゲームオブジェクトにダメージを与える
		if (other.name == targetName)
		{
			other.TakeDamage(gameObject, { amount, 0 });
		}
	}
	//ダメージを与えられるゲームオブジェクト名
	std::string targetName;

	int amount = 1;		//ダメージ量
	bool isOnce = true;		//一度ダメージを与えたら死ぬか？
};


#endif // !COMPONENT_BOSSDAMAGESOURCE_H_INSTANCE