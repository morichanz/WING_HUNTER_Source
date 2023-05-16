/**
* @file BossExprosion.h
*/
#ifndef COMPONENT_BOSSEXPROSION_H_INCLUDE
#define COMPONENT_BOSSEXPROSION_H_INCLUDE
#include"../../Component/Component.h"
#include"../../Sprite/SpritePriority.h"
#include"../../GameObject/GameObject.h"
#include"../../Engine/Engine.h"
#include"../Animator/Animator2D.h"
#include"../../Audio/EasyAudio.h"
#include"../../Audio/AudioSetting.h"

/**
* 爆発コンポーネント
*/
class BossExprosion : public Component
{
public:
	BossExprosion() = default;
	virtual ~BossExprosion() = default;
	virtual void OnDestroy(GameObject& gameObject) override
	{
		gameObject.priority = prioExplosion;
		//爆発アニメーション

		const Texcoord tc[] = {
			MakeTexcoord(449,412,32,32),
			MakeTexcoord(449 + 32,412,32,32),
			MakeTexcoord(449 + (32 * 2),412,32,32),
			MakeTexcoord(449 + (32 * 3),412,32,32),
			MakeTexcoord(449 + (32 * 4),412,32,32),
			MakeTexcoord(449 + (32 * 5),412,32,32),
			MakeTexcoord(449 + (32 * 6),412,32,32),
			MakeTexcoord(449 + (32 * 7),412,32,32),
			MakeTexcoord(449 + (32 * 8),412,32,32),
			MakeTexcoord(449 + (32 * 9),412,32,32),
			MakeTexcoord(449 + (32 * 10),412,32,32),
			MakeTexcoord(449 + (32 * 11),412,32,32),
			MakeTexcoord(449 + (32 * 12),412,32,32),
			MakeTexcoord(449 + (32 * 13),412,32,32),
			MakeTexcoord(449 + (32 * 14),412,32,32),
			MakeTexcoord(449 + (32 * 15),412,32,32),
			MakeTexcoord(449 + (32 * 16),412,32,32),
			MakeTexcoord(449 + (32 * 17),412,32,32),
		};

		//爆発の表示座標データ
		const float x[] = { 0,-32,40,-36,32 };
		const float y[] = { 0,40,32,-36,-40 };

		//爆発を表示
		if (gameObject.scoreFlg || gameObject.name == "player")
		{
			Audio::PlayOneShot(SE::enemyExplosionM, 3);
			int i = static_cast<int>(std::size(x));
			if (count < i)
			{
				i = count;
			}
			for (--i; i >= 0; --i)
			{
				auto explosion = gameObject.engine->Create<GameObject>("explosion", gameObject.x, gameObject.y);
				explosion->AddSprite(tc[0]);
				explosion->priority = prioExplosion;
				auto anim = explosion->AddComponent<Animator2D>();
				anim->interval = 0.05f;
				anim->loopTime = false;		//ループ再生をしない
				anim->killWhenEnd = true;	//再生終了時にゲームオブジェクトを削除
				anim->clip.resize(std::size(tc));
				for (size_t frameNo = 0; frameNo < std::size(tc); ++frameNo)
				{
					anim->AddSprite(frameNo, tc[frameNo], x[i], y[i], 4, 4, 4);
				}
			}//for a
		}
	}
	int priority = 0;
	int count = 1;
};

#endif // !COMPONENT_BOSSEXPROSION_H_INCLUDE
