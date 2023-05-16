/**
* @file BossCicleShot
*/
#ifndef COMPONENT_BOSSCICLESHOT_H_INCLUDE
#define COMPONENT_BOSSCICLESHOT_H_INCLUDE
#include"../../Component.h"
#include"../../GameObject/GameObject.h"
#include"../../Engine/Engine.h"
#include"../../Animator/Animator2D.h"
#include"../../Bullet/BoundBullet.h"
#include"../BossBulletExplosion.h"
#include"../../OutOfScreen.h"
#include"../../DamageSource.h"
#include"../../Bullet/BulletMove.h"

const Texcoord bossCircleTc[] = {
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
class BossCicleShot : public Component
{
public:
	BossCicleShot() = default;
	virtual ~BossCicleShot() = default;

	void Shot(GameObject& gameObject, float deltaTime)
	{
		if (timer > 0)
		{
			timer -= deltaTime;
		}
		else
		{
			isFiring = true;
		}

		//弾を発射
		if (isFiring)
		{
			for (angle = 0; angle < 360; angle += 18)
			{
				float rad = angle * 3.14f / 180;
				auto bullet = gameObject.engine->Create<GameObject>("enemy bullet", gameObject.x, gameObject.y - 55);
				bullet->AddSprite(bossCircleTc[0]);
				bullet->priority = prioBossBullet;
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
				anim->clip.resize(std::size(bossCircleTc));
				for (size_t frameNo = 0; frameNo < std::size(bossCircleTc); ++frameNo)
				{
					anim->AddSprite(frameNo, bossCircleTc[frameNo], 1, 1, shotSize, shotSize, shotSize);
				}
			}
			if (angle >= 360)
			{
				timer = interval;
				isFiring = false;
			}
		}
	}

private:
	float timer = 0;
	float interval = 1;
	float speed = 400;
	float angle = 0;
	float shotSize = 1;

	bool isFiring = false;

};
#endif // !COMPONENT_BOSSCICLESHOT_H_INCLUDE