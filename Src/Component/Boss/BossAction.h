/**
* @file BossAction.h
*/
#ifndef COMPONENT_BOSSMOVE_H_INCLUDED
#define COMPONENT_BOSSMOVE_H_INCLUDED
#include "../../GameObject/GameObject.h"
#include "../../Component/Component.h"
#include"../../Component/MeshRenderer.h"
#include"../../VecMath/VecMath.h"
#include"../Bullet/BulletMove.h"
#include"../ScaleManager.h"
#include"../Health.h"
#include"../Explosion.h"
#include"../Move.h"
#include"../Bullet/BoundBullet.h"
#include"BeamDamageSource.h"
#include"../Color.h"
#include"../../Easing.h"
#include"../DamageSource.h"
#include"../Score/ScoreManager.h"
#include"../Score/Score.h"
#include"../OutOfScreen.h"
#include"../Rotation/Rotation.h"
#include"FactoryBase.h"
#include"BossMove/BossState.h"
#include"WeaponAction/WeaponAction.h"

#include "../../../ImGui/ImGui/imgui.h"
#include "../../../ImGui/ImGui/imgui_impl_opengl3.h"
#include "../../../ImGui/ImGui/imgui_impl_glfw.h"

#include<time.h>

/**
* ゲームオブジェクトを移動する
*/
class BossAction : public Component
{
public:
	BossAction() = default;
	virtual ~BossAction() = default;

	virtual void Start(GameObject& gameObject) override
	{
		bossPtr = gameObject.engine->FindGameObjectWithTag("boss");
		base = std::make_shared<FactoryBase>(bossPtr);
		base->SetState<Appearance>();
		base->Initialize();
		SetBossWeapon(gameObject);
		StartPos(gameObject);
		gameObject.health = gameObject.engine->j["Boss"]["hp"];
		if(stageCnt == 6) gameObject.z = 500;
	}

	virtual void Update(GameObject& gameObject, float deltaTime) override
	{
		base->StateExecute(deltaTime);
		ColorChange(gameObject, deltaTime);
	}

	//ボスポジション
	void StartPos(GameObject& gameObject)
	{
		if (stageCnt == 1)
		{
			gameObject.y = -400;
		}
		else if (stageCnt == 2)
		{
			gameObject.y = 100;
			gameObject.x = -300;
		}
		else if (stageCnt == 3)
		{
			gameObject.y = 800;
		}
		else if (stageCnt == 4)
		{
			gameObject.x = 700;
			gameObject.y = -400;
		}
		else if (stageCnt == 5)
		{
			gameObject.x = -300;
		}
		else if (stageCnt == 6)
		{
			gameObject.y = -400;
		}
	}

	void SetBossWeapon(GameObject& gameObject)
	{
		if (stageCnt == 3)
		{
			CreateWeaponRight(gameObject);
			CreateWeaponLeft(gameObject);
		}
		else if (stageCnt == 4)
		{
			CreateWeaponCenter(gameObject);
		}
		else if (stageCnt == 5)
		{
			CreateWeaponCenter(gameObject);
		}
		else if (stageCnt == 6)
		{
			CreateWeaponCenter(gameObject);
			CreateWeaponRight(gameObject);
			CreateWeaponLeft(gameObject);
		}
	}

	void CreateWeaponRight(GameObject& gameObject)
	{
		auto weapon = gameObject.engine->Create<GameObject>("weaponRight", gameObject.x, gameObject.y);
		auto action = weapon->AddComponent<WeaponAction>();
		action->bossPtr = bossPtr;
		action->weaponPosX = weaponPosX[0];
		action->weaponPosY = -160;
		auto score = weapon->AddComponent<Score>();
		score->scoreManager = scoreManager;
		score->score = 500;
		auto health = weapon->AddComponent<Health>();
		health->health = gameObject.engine->j["Boss"]["weaponHp"];
		auto explosion = weapon->AddComponent<Explosion>();
		explosion->count = 5;	//爆発の表示数を増やす

		auto renderer = weapon->AddComponent<MeshRenderer>();
		renderer->mesh = gameObject.engine->meshList[10];
		renderer->scale = VecMath::vec3(8);
		renderer->rotation.x = VecMath::radians(90);
		renderer->rotation.y = VecMath::radians(180);
		auto rotation = weapon->AddComponent<Rotation>();
		rotation->roatey = 360;

		auto materialColor = weapon->AddComponent<Color>();
		materialColor->MaterialColor(renderer);
		action->color = materialColor;
		action->meshrenderer = renderer;
	}

	void CreateWeaponLeft(GameObject& gameObject)
	{
		auto weapon = gameObject.engine->Create<GameObject>("weaponLeft", gameObject.x, gameObject.y);
		weapon->priority = prioWeapon;
		auto action = weapon->AddComponent<WeaponAction>();
		action->bossPtr = bossPtr;
		action->weaponPosX = weaponPosX[2];
		action->weaponPosY = -160;
		auto score = weapon->AddComponent<Score>();
		score->scoreManager = scoreManager;
		score->score = 500;
		auto health = weapon->AddComponent<Health>();
		health->health = gameObject.engine->j["Boss"]["weaponHp"];
		auto explosion = weapon->AddComponent<Explosion>();
		explosion->count = 5;	//爆発の表示数を増やす

		auto renderer = weapon->AddComponent<MeshRenderer>();
		renderer->mesh = gameObject.engine->meshList[10];
		renderer->scale = VecMath::vec3(8);
		renderer->rotation.x = VecMath::radians(90);
		renderer->rotation.y = VecMath::radians(180);
		auto rotation = weapon->AddComponent<Rotation>();
		rotation->roatey = 360;

		auto MaterialColor = weapon->AddComponent<Color>();
		MaterialColor->MaterialColor(renderer);
		action->color = MaterialColor;
		action->meshrenderer = renderer;
	}

	void CreateWeaponCenter(GameObject& gameObject)
	{
		auto weapon = gameObject.engine->Create<GameObject>("weaponCenter", gameObject.x, gameObject.y);
		weapon->priority = prioWeapon;
		auto action = weapon->AddComponent<WeaponAction>();
		action->bossPtr = bossPtr;
		action->weaponPosY = -140;
		auto score = weapon->AddComponent<Score>();
		score->scoreManager = scoreManager;
		score->score = 500;
		auto weaponTransform = weapon->AddComponent<ScaleManager>();
		weaponTransform->scale = weaponScale;
		auto health = weapon->AddComponent<Health>();
		health->health = gameObject.engine->j["Boss"]["weaponHp"];
		auto explosion = weapon->AddComponent<Explosion>();
		explosion->count = 5;	//爆発の表示数を増やす
	
		auto renderer = weapon->AddComponent<MeshRenderer>();
		renderer->mesh = gameObject.engine->meshList[11];
		renderer->scale = VecMath::vec3(8);
		renderer->rotation.x = VecMath::radians(90);
		renderer->rotation.y = VecMath::radians(0);
	
		auto materialColor = weapon->AddComponent<Color>();
		materialColor->MaterialColor(renderer);
		action->color = materialColor;
		action->meshrenderer = renderer;
	}

	//ボスがダメージを受けたとき
	virtual void OnTakeDamage(GameObject& gameObject, GameObject& other, const Damage& damage) override
	{
		gameObject.health -= damage.amount;
		enemyColor->SetMaterialColor(gameObject.meshRenderer, enemyColor->objectColor.red);
		damageColorFlg = true;
		colorInterval = 0.05f;
		if (gameObject.health <= 0)
		{
			gameObject.scoreFlg = true;
		}
	}

	//攻撃を受けたときに色を変える
	void ColorChange(GameObject& gameObject, float deltaTime)
	{
		if (damageColorFlg)
		{
			if (colorInterval > 0)
			{
				colorInterval -= deltaTime;
			}
			else
			{
				enemyColor->SetMaterialColor(gameObject.meshRenderer, enemyColor->objectColor.baceColor);
				Audio::PlayOneShot(SE::bossDamage, Audio::GetMasterVolume());
				damageColorFlg = false;
			}
		}
	}

	GameObjectPtr target;	//標的
	ScoreManagerPtr scoreManager;
	std::shared_ptr<Color> enemyColor;

	FactoryBasePtr base;
	float weaponPosX[5] = { 60, 0, -60 , 160, -160 };//武器のポジション
	int stageCnt = 1;			//ステージカウント

private:
	GameObjectPtr bossPtr;
	bool centerFlg = true;		//ボスを中心に移動するフラグ
	float weaponScale = 1.7f;
	float weaponScaleDown = 2.5f;
	bool damageColorFlg = false;
	float colorInterval = 0.0f;
};
#endif // COMPONENT_BOSSMOVE_H_INCLUDED