/**
* @file FireBullet
*/
#ifndef COMPONENT_FIREBULLET_H_INCLUDE
#define COMPONENT_FIREBULLET_H_INCLUDE
#include"../Component/Component.h"
#include"../../Sprite/SpritePriority.h"
#include"../../GameObject/GameObject.h"
#include"../../Engine/Engine.h"
#include"../Component/Move.h"
#include"../Component/OutOfScreen.h"
#include"DamageSource.h"

/**
* 敵弾発射コンポーネント
*/
class FireBullet : public Component
{
public:
	FireBullet() = default;
	virtual ~FireBullet() = default;
	virtual void Update(GameObject& gameObject, float deltaTime) override
	{
		//発射用タイマーが発射間隔を超えたら、発射用変数を初期化
		timerA += deltaTime;
		if (timerA >= intervalA)
		{
			isFiring = true;
			timerA -= intervalA;
			timerB = intervalB;	//1発目が即座に発射されるようにする
			fireCounter = numberOfFire;

			//ターゲット指定があれば、発射ベクトルをターゲットに向ける
			if (target)
			{
				vx = target->x - gameObject.x;
				vy = target->y - gameObject.y;
				const float v = sqrt(vx * vx + vy * vy);

				vx = vx / v * speed;
				vy = vy / v * speed;
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

		//弾を発射
		if (shotFlg)
		{
			auto bullet = gameObject.engine->Create<GameObject>("enemy bullet", gameObject.x + x, gameObject.y + y);
			bullet->AddSprite(tc);
			bullet->priority = prioEnemyBullet;
			bullet->AddCollision({ -4,-4,4,4 });
			auto move = bullet->AddComponent<Move>();
			move->vx = vx;
			move->vy = vy;
			bullet->AddComponent<OutOfScreen>();
			auto damageSource = bullet->AddComponent<DamageSource>();
			damageSource->targetName = "player";
		}

		//連射カウンターを減らし、0以下になったら発射モードを解除
		--fireCounter;
		if (fireCounter <= 0)
		{
			isFiring = false;
		}
	}

	//初期化時に設定する変数
	GameObjectPtr target;	//標的
	Texcoord tc = MakeTexcoord(336, 448, 16, 16);	//テクスチャ座標
	float intervalA = 1;	//発射間隔
	float intervalB = 0.1f;	//連射間隔
	int numberOfFire = 1;	//連射数
	float speed = 200;		//弾の速度
	float x = 0;			//発射位置のX座標
	float y = 0;			//発射位置のY座標

	bool shotFlg = false;	//弾を発射させないように設定する(確認用)

	//実行中に変化する変数
	bool isFiring = false;		//発射フラグ
	float timerA = 0;	//発射用タイマー
	float timerB = 0;	//連射用タイマー
	int fireCounter = 1;//連射数カウンター
	float vx = 0;		//発射ベクトルX
	float vy = 0;		//発射ベクトルY
};
#endif // !COMPONENT_FIREBULLET_H_INCLUDE
