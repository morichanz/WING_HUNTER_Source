/**
* @file WeaponFire
*/
#ifndef COMPONENT_WEAPONFIRE_H_INCLUDE
#define COMPONENT_WEAPONFIRE_H_INCLUDE
#include"../../Component.h"
#include"../../GameObject/GameObject.h"
#include"../../Engine/Engine.h"
#include"../../Animator/Animator2D.h"
#include"../../Bullet/BoundBullet.h"
#include"../BossBulletExplosion.h"
#include"../../OutOfScreen.h"
#include"../../DamageSource.h"

const Texcoord weaponFireTc[] =
{
	MakeTexcoord(96, 465, 14, 14),
	MakeTexcoord(113, 465, 14, 14),
	MakeTexcoord(129, 465, 14, 14),
	MakeTexcoord(145, 465, 14, 14),
};

/**
* 敵弾発射コンポーネント
*/
class WeaponFire : public Component
{
public:
	WeaponFire() = default;
	virtual ~WeaponFire() = default;

	void Fire(GameObject& gameObject, GameObjectPtr weaponCenter, GameObjectPtr target, float deltaTime)
	{

		if (isDead) return;
		if (weaponCenter->isDead) isDead = true;
		//発射用タイマーが発射間隔を超えたら、発射用変数を初期化
		timerA += deltaTime;
		if (timerA >= intervalA)
		{
			isFiring = true;
			timerA -= intervalA;
			timerB = intervalB;	//1発目が即座に発射されるようにする
			weaponFireCounter = numberOfFire;

			//ターゲット指定があれば、発射ベクトルをターゲットに向ける
			if (target)
			{
				vx = target->x - gameObject.x;
				vy = target->y - gameObject.y;
				const float v = sqrt(vx * vx + vy * vy);
				if (vy > 0)
				{
					vyFlg = true;
				}
				else
				{
					vyFlg = false;
					vx = vx / v * speed;
					vy = vy / v * speed;
				}
			}
			else
			{
				vx = 0;
				vy = -speed;
			}
		}

		//発射モードでなければ何もしない
		if (!isFiring)
		{
			return;
		}

		//連射用タイマーが連射間隔を超えたら、弾を発射
		timerB += deltaTime;
		if (timerB < intervalB)
		{
			return;
		}
		timerB -= intervalB;

		if (!vyFlg)
		{

			if (isFiring)
			{
				Audio::PlayOneShot(SE::enemyShot, 2);
				auto bullet = gameObject.engine->Create<GameObject>("enemy bullet", weaponCenter->x + 10, weaponCenter->y - 30);
				bullet->AddSprite(weaponFireTc[0]);
				bullet->priority = prioWaaponBullet;
				bullet->AddCollision({ -4,-4,4,4 });
				auto move = bullet->AddComponent<BoundBullet>();
				move->boundCounter = 2;
				move->vx = vx;
				move->vy = vy;
				auto exploaion = bullet->AddComponent<BossBulletExprosion>();

				bullet->AddComponent<OutOfScreen>();
				auto damageSource = bullet->AddComponent<DamageSource>();
				damageSource->targetName = "player";

				auto anim = bullet->AddComponent<Animator2D>();
				anim->loopTime = true;		//ループ再生をしない
				anim->killWhenEnd = false;	//再生終了時にゲームオブジェクトを削除
				anim->clip.resize(std::size(weaponFireTc));
				for (size_t frameNo = 0; frameNo < std::size(weaponFireTc); ++frameNo)
				{
					anim->AddSprite(frameNo, weaponFireTc[frameNo], 1, 1, shotSize, shotSize, shotSize);
				}

				auto bullet1 = gameObject.engine->Create<GameObject>("enemy bullet", weaponCenter->x - 10, weaponCenter->y - 30);
				bullet1->AddSprite(weaponFireTc[0]);
				bullet1->priority = prioWaaponBullet;
				bullet1->AddCollision({ -4,-4,4,4 });
				auto move1 = bullet1->AddComponent<BoundBullet>();
				move1->boundCounter = 2;
				move1->vx = vx;
				move1->vy = vy;
				auto exploaion1 = bullet1->AddComponent<BossBulletExprosion>();
				bullet1->AddComponent<OutOfScreen>();
				auto damageSource1 = bullet1->AddComponent<DamageSource>();
				damageSource1->targetName = "player";

				auto anim1 = bullet1->AddComponent<Animator2D>();
				anim1->loopTime = true;		//ループ再生をしない
				anim1->killWhenEnd = false;	//再生終了時にゲームオブジェクトを削除
				anim1->clip.resize(std::size(weaponFireTc));
				for (size_t frameNo = 0; frameNo < std::size(weaponFireTc); ++frameNo)
				{
					anim1->AddSprite(frameNo, weaponFireTc[frameNo], 1, 1, shotSize, shotSize, shotSize);
				}
			}
		}

		//連射カウンターを減らし、0以下になったら発射モードを解除
		--weaponFireCounter;
		if (weaponFireCounter <= 0)
		{
			isFiring = false;
		}	
	}

private:
	float numberOfFire = 1.0f;

	float vx = 0.0f;
	float vy = 0.0f;
	float speed = 400.0f;

	float weaponFireCounter = 0.0f;
	float shotSize = 1.0f;

	float timerA = 0.0f;
	float timerB = 0.0f;
	float intervalA = 0.4f;
	float intervalB = 0.2f;
	bool isFiring = false;
	bool vyFlg  = false;
	bool isDead = false;
};
#endif // !COMPONENT_WEAPONFIRE_H_INCLUDE