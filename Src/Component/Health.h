/**
* @file Healht.h
*/
#ifndef COMPONENT_HEALTH_H_INCLUDE
#define COMPONENT_HEALTH_H_INCLUDE
#include"../Component/Component.h"
#include"../GameObject/GameObject.h"
#include"../Audio/EasyAudio.h"
#include"../Audio/AudioSetting.h"

/**
* �ϋv�l�R���|�[�l���g
*/
class Health : public Component
{
public:
	Health() = default;
	virtual ~Health() = default;
	virtual void OnTakeDamage(GameObject& gameObject, GameObject& other, const Damage& damage) override
	{
		health -= damage.amount;
		gameObject.health -= damage.amount;
		gameObject.damageFlg = true;
		if (health <= 0)
		{
			gameObject.scoreFlg = true;
			gameObject.isDead = true;
		}
	}
	int health = 1;	//�ϋv�l
};
#endif // !COMPONENT_HEALTH_H_INCLUDE

