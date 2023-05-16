/**
* @file VortexShot
*/
#ifndef COMPONENT_VORTEXSHOT_H_INCLUDE
#define COMPONENT_VORTEXSHOT_H_INCLUDE
#include"../../Component.h"
#include"../../GameObject/GameObject.h"
#include"../../Engine/Engine.h"
#include"../../Animator/Animator2D.h"
#include"../../Bullet/BoundBullet.h"
#include"../BossBulletExplosion.h"
#include"../../OutOfScreen.h"
#include"../../DamageSource.h"
#include"../../Bullet/BulletMove.h"

/**
* 敵弾発射コンポーネント
*/
class VortexShot : public Component
{
public:
	VortexShot() = default;
	virtual ~VortexShot() = default;

	void Shot(GameObject& gameObject, float deltaTime)
	{
		if (timer > 0)
		{
			if (angle > 360)angle = 0;
			timer -= deltaTime;
		}
		else
		{
			isFiring = true;
		}

		//弾を発射
		if (isFiring)
		{
			float rad = angle * 3.14f / 180;
			auto bullet = gameObject.engine->Create<GameObject>("enemy bullet", gameObject.x, gameObject.y - 55);
			bullet->AddSprite(tc);
			bullet->priority = prioBossBullet;
			bullet->AddCollision({ -4,-4,4,4 });
			auto move = bullet->AddComponent<BulletMove>();
			move->angle = rad;
			move->speed = speed;
			bullet->AddComponent<OutOfScreen>();
			auto damageSource = bullet->AddComponent<DamageSource>();
			damageSource->targetName = "player";
			angle += 10;
			isFiring = false;
			timer = interval;
		}
	}

	GameObjectPtr target;	//ターゲット
	float speed = 400;		//弾速

private:
	Texcoord tc = MakeTexcoord(336, 448, 16, 16);	//テクスチャ座標
	float vx = 0;
	float vy = 0;
	float angle = 0;

	float timer = 0;
	float interval = 0;
	bool isFiring = false;
};
#endif // !COMPONENT_VORTEXSHOT_H_INCLUDE