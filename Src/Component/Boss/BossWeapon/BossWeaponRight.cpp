#include "BossWeaponRight.h"

void BossWeaponRight::StateMode(float deltaTime)
{
	if (!firstFlg)
	{
		//SetState<BeamCharge>();
		firstFlg = true;
	}
	else
	{
		state->Execute(weaponRightPtr, bossPtr, deltaTime);
	}
}