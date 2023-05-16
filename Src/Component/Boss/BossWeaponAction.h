/**
* @file BossWeaponAction.h
*/
#ifndef COMPONENT_BOSSWEAPONACTION_H_INCLUDED
#define COMPONENT_BOSSWEAPONACTION_H_INCLUDED
#include "../Component.h"
#include "../../GameObject/GameObject.h"
#include"Beam.h"
#include"../Bullet/BoundBullet.h"
#include"BossBulletExplosion.h"

const Texcoord tca[] =
{
	MakeTexcoord(96, 465, 14, 14),
	MakeTexcoord(113, 465, 14, 14),
	MakeTexcoord(129, 465, 14, 14),
	MakeTexcoord(145, 465, 14, 14),
};

/**
* ゲームオブジェクトを移動する
*/
class BossWeaponAction : public Component
{
public:
	BossWeaponAction() = default;
	virtual ~BossWeaponAction() = default;
	virtual void Update(GameObject& gameObject, float deltaTime) override
	{
		gameObject.x = bossPtr->x + weaponPosX;
		gameObject.y = bossPtr->y + weaponPosY;
		gameObject.z = bossPtr->z;
	}

	GameObjectPtr bossPtr;
	float weaponPosX = 0.0f;
	float weaponPosY = 0.0f;
};
#endif // COMPONENT_BOSSSWEAPON_H_INCLUDED