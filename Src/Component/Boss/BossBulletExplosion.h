/**
* @file BossBulletExprosion.h
*/
#ifndef COMPONENT_BOSSBULLETEXPROSION_H_INCLUDE
#define COMPONENT_BOSSBULLETEXPROSION_H_INCLUDE
#include"../Component.h"
#include"../../Sprite/SpritePriority.h"
#include"../../GameObject/GameObject.h"
#include"../../Engine/Engine.h"
#include"../Animator/Animator2D.h"
#include"../../Audio/EasyAudio.h"
#include"../../Audio/AudioSetting.h"

/**
* 爆発コンポーネント
*/
class BossBulletExprosion : public Component
{
public:
	BossBulletExprosion() = default;
	virtual ~BossBulletExprosion() = default;
	virtual void OnDestroy(GameObject& gameObject) override
	{
		gameObject.priority = prioExplosion;

		//爆発アニメーション
		const Texcoord tc[] = {
			MakeTexcoord(161,465,14,14),
			MakeTexcoord(177,465,14,14),
			MakeTexcoord(193,465,14,14),
			MakeTexcoord(209,465,14,14),
		};

		//爆発の表示座標データ
		const float x[] = { 0,-32,40,-36,32 };
		const float y[] = { 0,40,32,-36,-40 };

		//爆発を表示
		if (gameObject.scoreFlg || gameObject.name == "player")
		{
			Audio::PlayOneShot(SE::bossShot, 2);
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

#endif // !COMPONENT_BOSSBULLETEXPROSION_H_INCLUDE
