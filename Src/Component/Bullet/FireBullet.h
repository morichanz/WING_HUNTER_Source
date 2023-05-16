/**
* @file FireBullet
*/
#ifndef COMPONENT_FIREBULLET_H_INCLUDE
#define COMPONENT_FIREBULLET_H_INCLUDE
#include"../Component.h"
#include"../Sprite/SpritePriority.h"
#include "../GameObject/GameObject.h"
#include"../Engine/Engine.h"
#include"../Move.h"
#include"../OutOfScreen.h"
#include"../Bullet/BoundBullet.h"

const Texcoord fireTc[] = {
MakeTexcoord(585, 41, 17, 16),
MakeTexcoord(602, 41, 17, 16),
MakeTexcoord(619, 41, 17, 16),
MakeTexcoord(636, 41, 17, 16),
};

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
			//弾を発射
			if (enemy->shotFlg)
			{
				auto bullet = gameObject.engine->Create<GameObject>("enemy bullet", gameObject.x + x, gameObject.y + y);
				bullet->AddSprite(fireTc[0]);
				bullet->priority = prioEnemyBullet;
				bullet->AddCollision({ -4,-4,4,4 });
				auto move = bullet->AddComponent<BoundBullet>();
				if (!boundFlg)
				{
					move->boundCounter = 0;
				}
				else if (boundFlg && gameObject.engine->stageCnt == 6)
				{
					move->boundCounter = 2;
				}
				else if(boundFlg && gameObject.engine->stageCnt != 1)
				{
					move->boundCounter = 3;
				}
				move->vx = vx;
				move->vy = vy;
				bullet->AddComponent<OutOfScreen>();
				auto damageSource = bullet->AddComponent<DamageSource>();
				damageSource->targetName = "player";

				auto anim = bullet->AddComponent<Animator2D>();
				anim->loopTime = true;		//ループ再生をしない
				anim->killWhenEnd = false;	//再生終了時にゲームオブジェクトを削除
				anim->clip.resize(std::size(fireTc));
				for (size_t frameNo = 0; frameNo < std::size(fireTc); ++frameNo)
				{
					anim->AddSprite(frameNo, fireTc[frameNo], 1, 1, shotSize, shotSize, shotSize);
				}
			}
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
	GameObjectPtr enemy;
	float intervalA = 0.9f;	//発射間隔
	float intervalB = 0.1f;	//連射間隔
	int numberOfFire = 1;	//連射数
	float speed = 200;		//弾の速度
	float x = 0;			//発射位置のX座標
	float y = 0;			//発射位置のY座標
	int tecNum = 0;
	bool shotFlg = false;	//弾を発射させないように設定する(確認用)
	float shotSize = 1.3f;
	bool boundFlg = false;
	bool vyFlg = false;

	//実行中に変化する変数
	bool isFiring = false;		//発射フラグ
	float timerA = 0;	//発射用タイマー
	float timerB = 0;	//連射用タイマー
	int fireCounter = 1;//連射数カウンター
	float vx = 0;		//発射ベクトルX
	float vy = 0;		//発射ベクトルY
};
#endif // !COMPONENT_FIREBULLET_H_INCLUDE
