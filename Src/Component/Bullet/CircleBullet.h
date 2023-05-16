/**
* @file BossBullet
*/
#ifndef COMPONENT_BOSSBULLET_H_INCLUDE
#define COMPONENT_BOSSBULLET_H_INCLUDE
#include"../Component.h"
#include"../../Sprite/SpritePriority.h"
#include"../../GameObject/GameObject.h"
#include"../../Engine/Engine.h"
#include"BulletMove.h"
#include"../OutOfScreen.h"

const Texcoord circleTc[] = {
MakeTexcoord(586,123,17,16),
MakeTexcoord(603,123,17,16),
MakeTexcoord(620,123,17,16),
MakeTexcoord(637,123,17,16),
MakeTexcoord(654,123,17,16),
MakeTexcoord(671,123,17,16),
};

/**
* 敵弾発射コンポーネント
*/
class CircleBullet : public Component
{
public:
	CircleBullet() = default;
	virtual ~CircleBullet() = default;
	virtual void Update(GameObject& gameObject, float deltaTime) override
	{
		if (timerA > 0)
		{
			timerA -= deltaTime;
		}
		else
		{
			isFiring = true;
		}

		//弾を発射
		if (enemy->shotFlg)
		{
			if (isFiring)
			{
				for (angle = 0; angle < 360; angle += 18)
				{
					float rad = angle * 3.14f / 180;
					auto bullet = gameObject.engine->Create<GameObject>("enemy bullet", gameObject.x + x, gameObject.y + y);
					bullet->AddSprite(circleTc[0]);
					bullet->priority = prioEnemyBullet;
					bullet->AddCollision({ -4,-4,4,4 });
					auto move = bullet->AddComponent<BulletMove>();
					move->angle = rad;
					move->speed = speed;
					bullet->AddComponent<OutOfScreen>();
					auto damageSource = bullet->AddComponent<DamageSource>();
					damageSource->targetName = "player";

					auto anim = bullet->AddComponent<Animator2D>();
					anim->loopTime = true;		//ループ再生をしない
					anim->killWhenEnd = false;	//再生終了時にゲームオブジェクトを削除
					anim->clip.resize(std::size(circleTc));
					for (size_t frameNo = 0; frameNo < std::size(circleTc); ++frameNo)
					{
						anim->AddSprite(frameNo, circleTc[frameNo], 1, 1, shotSize, shotSize, shotSize);
					}
				}
				if (angle >= 360)
				{
					timerA = intervalA;
					isFiring = false;
				}
			}
		}
	}

	//初期化時に設定する変数
	GameObjectPtr target;	//標的
	GameObjectPtr enemy;
	float intervalA = 1;	//発射間隔
	float intervalB = 0.1f;	//連射間隔
	int numberOfFire = 10;	//連射数
	float speed = 300;		//弾の速度
	float x = 0;			//発射位置のX座標
	float y = 0;			//発射位置のY座標
	float angle = 0;
	bool shotFlg = false;	//弾を発射させないように設定する(確認用)

	//実行中に変化する変数
	bool isFiring = false;	//発射フラグ
	float timerA = 0;		//発射用タイマー
	float timerB = 0;		//連射用タイマー
	int fireCounter = 10;	//連射数カウンター
	float vx = 0;			//発射ベクトルX
	float vy = 0;			//発射ベクトルY
	float shotSize = 1.3f;
	float shotCnt = 0;
};
#endif // !COMPONENT_BOSSBULLET_H_INCLUDE