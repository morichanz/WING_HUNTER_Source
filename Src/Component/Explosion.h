/**
* @file Exprosion.h
*/
#ifndef COMPONENT_EXPROSION_H_INCLUDE
#define COMPONENT_EXPROSION_H_INCLUDE
#include"Component.h"
#include"../Sprite/SpritePriority.h"
#include"../GameObject/GameObject.h"
#include"../Engine/Engine.h"
#include"../Component/Animator/Animator2D.h"

/**
* 爆発コンポーネント
*/
class Explosion : public Component
{
public:
	Explosion() = default;
	virtual ~Explosion() = default;
	virtual void OnDestroy(GameObject& gameObject) override
	{
		gameObject.priority = prioExplosion;

		//爆発アニメーション
		const Texcoord tc[] = {
			MakeTexcoord(320,368,32,32),
			MakeTexcoord(352,368,32,32),
			MakeTexcoord(384,368,32,32),
			MakeTexcoord(416,368,32,32),
			MakeTexcoord(448,368,32,32),
			MakeTexcoord(480,368,32,32),
		};

		//爆発の表示座標データ
		const float x[] = { 0,-12,20,-16,12 };
		const float y[] = { 0,20,12,-16,-20 };

		//爆発を表示
		if (gameObject.scoreFlg || gameObject.name == "player")
		{
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
				anim->loopTime = false;		//ループ再生をしない
				anim->killWhenEnd = true;	//再生終了時にゲームオブジェクトを削除
				anim->clip.resize(std::size(tc));
				for (size_t frameNo = 0; frameNo < std::size(tc); ++frameNo)
				{
					anim->AddSprite(frameNo, tc[frameNo], x[i], y[i],3,3,3);
				}
			}//for a
		}
	}
	int priority = 0;
	int count = 1;
};

#endif // !COMPONENT_EXPROSION_H_INCLUDE
