/**
* WeaponAction.h
*/
#ifndef WEAPONACTION_H_COMPONENT
#define WEAPONACTION_H_COMPONENT
#include"../../GameObject/GameObject.h"
#include"../../Component.h"
#include"../WeaponState/WeaponState.h"
#include"../../Color.h"

class WeaponAction : public Component
{
public:
	WeaponAction() = default;
	virtual ~WeaponAction() = default;
	virtual void Start(GameObject& gameObject)override
	{
		weaponRight = gameObject.engine->FindGameObjectWithTag("weaponRight");
		weaponLeft = gameObject.engine->FindGameObjectWithTag("weaponLeft");
		weaponCenter = gameObject.engine->FindGameObjectWithTag("weaponCenter");

		if (gameObject.name == str_weaponRight)
		{
			base = std::make_shared<FactoryBase>(weaponRight);
			base->SetState<Charge>();
			base->Initialize();
		}
		else if(gameObject.name == str_weaponLeft)
		{
			base = std::make_shared<FactoryBase>(weaponLeft);
			base->SetState<Charge>();
			base->Initialize();
		}
		else
		{
			base = std::make_shared<FactoryBase>(weaponCenter);
			base->SetState<FireShot>();
			base->Initialize();
		}
	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		gameObject.x = bossPtr->x + weaponPosX;
		gameObject.y = bossPtr->y + weaponPosY;
		gameObject.z = bossPtr->z;
		base->StateExecute(deltaTime);

		if (bossPtr->collisionFlg && !appearanceFlg)
		{
			if(gameObject.name != str_weaponCenter)gameObject.AddCollision({ -15,-50,15,50 });
			else gameObject.AddCollision({ -25,-30,25,30 });
			appearanceFlg = true;
		}

		if (!bossPtr->collisionFlg && appearanceFlg)
		{
			gameObject.RemoveCollision();
			appearanceFlg = false;
		}

		if (gameObject.damageFlg)
		{
			if (colorInterval < 0)
			{
				color->SetMaterialColor(meshrenderer, color->objectColor.baceColor);
				colorInterval = timer;
				gameObject.damageFlg = false;
			}
			else
			{
				colorInterval -= deltaTime;
				color->SetMaterialColor(meshrenderer, color->objectColor.white);
			}
		}

		if (bossPtr->isDead)
		{
			gameObject.isDead = true;
		}
	}

	virtual void OnDestroy(GameObject& gameObject) override
	{
		base->Finalize();
	}

	//void Finalize()
	//{
	//	base->FInalize();
	//}

	GameObjectPtr bossPtr;
	float weaponPosX = 0.0f;
	float weaponPosY = 0.0f;

	std::shared_ptr<Color> color;
	MeshRendererPtr meshrenderer;

private:
	FactoryBasePtr base;
	GameObjectPtr weaponRight;
	GameObjectPtr weaponLeft;
	GameObjectPtr weaponCenter;
	std::string str_weaponRight = "weaponRight";
	std::string str_weaponLeft = "weaponLeft";
	std::string str_weaponCenter = "weaponCenter";
	bool appearanceFlg = false;
	float colorInterval = 0.05f;
	float timer = 0.05f;
	const Texcoord tcBullet = MakeTexcoord(16, 432, 16, 32);
};
#endif // !WEAPONACTION_H_COMPONENT
