#include "BeamCharge.h"

#include "../BossWeapon/BossWeaponRight.h"

void BeamCharge::Execute(GameObjectPtr weapon, GameObjectPtr boss, float deltaTime, bool animFlg)
{
	Charge(boss, deltaTime);
	if (charge->isDead)
	{
		//weapon->GetComponent<BossWeaponRight>()->SetState<BeamShot>();
	}
}