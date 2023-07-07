/**
* @file DamageSource.h
*/
#ifndef COMPONENT_DAMAGESOURCE_H_INSTANCE
#define COMPONENT_DAMAGESOURCE_H_INSTANCE
#include "Component.h"
#include"../GameObject/GameObject.h"
#include<string>

/**
* ダメージ減コンポーネント
*/
class DamageSource : public Component
{
public:
	DamageSource() = default;
	virtual ~DamageSource() = default;
	virtual void OnCollision(GameObject& gameObject, GameObject& other) override
	{
		//ターゲット名と同じ名前のゲームオブジェクトにダメージを与える
		if (other.name == targetName)
		{
			other.TakeDamage(gameObject, { amount, 0 });

			if (isOnce)
			{
				gameObject.isDead = true;
			}
		}

		for (int i = 0; i < targetNameList.size(); i++)
		{
			if (other.name == targetNameList[i])
			{
				other.TakeDamage(gameObject, { amount, 0 });

				if (isOnce)
				{
					gameObject.isDead = true;
				}
			}
		}
	}

	//ダメージを与えられるゲームオブジェクト名
	std::string targetName;
	std::vector<std::string> targetNameList;
	int amount = 1;		//ダメージ量
	bool isOnce = true;		//一度ダメージを与えたら死ぬか？
};
#endif // !COMPONENT_DAMAGESOURCE_H_INSTANCE
