/**
* @file BossHealht.h
*/
#ifndef COMPONENT_BOSSHEALTH_H_INCLUDE
#define COMPONENT_BOSSHEALTH_H_INCLUDE
#include"../Component.h"
#include"../../GameObject/GameObject.h"

/**
* �ϋv�l�R���|�[�l���g
*/
class BossHealth : public Component
{
public:
	BossHealth() = default;
	virtual ~BossHealth() = default;
	virtual void OnTakeDamage(GameObject& gameObject, GameObject& other, const Damage& damage) override
	{
		health -= damage.amount;
		if (health <= 0)
		{
			gameObject.isDead = true;
		}
	}
	float health = 1;	//�ϋv�l
};

#endif // !COMPONENT_BOSSHEALTH_H_INCLUDE