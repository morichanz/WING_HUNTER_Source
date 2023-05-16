/**
* @file Animator2D.h
*/

#ifndef COMPONENT_ANIMATOR2D_H_INCLUDE
#define COMPONENT_ANIMATOR2D_H_INCLUDE
#include "../Component.h"
#include"../../GameObject/GameObject.h"
#include"../../Sprite/Sprite.h"
#include<vector>
#include<algorithm>

/**
* 2Dアニメーションコンポーネント
*/
class Animator2D : public Component
{
public:
	Animator2D() = default;
	virtual ~Animator2D() = default;
	virtual void Update(GameObject& gameObject, float deltaTime) override
	{
		for (auto& s : gameObject.spriteList)
		{
			s.w = w;
		}

		if (destroyAnim) gameObject.isDead = true;


		//データがない場合は何もしない
		if (clip.empty())
		{
			return;
		}

		//経過時間を更新
		timer += deltaTime;

		//再生位置を計算
		const int size = static_cast<int>(clip.size());
		int i = static_cast<int>(timer / interval);
		if (loopTime)
		{
			i %= size;	//ループ再生
		}
		else
		{
			//アニメ終了時、killwhenEndフラグがtrueならゲームオブジェクトを削除
			if (i >= size && killWhenEnd)
			{
				gameObject.isDead = true;
			}
			i = std::clamp(i, 0, size - 1);
		}
		//スプライトデータを再生位置のデータに更新
		gameObject.spriteList = clip[i];
	}

	//フレームにスプライトを追加
	void AddSprite(size_t frameNo, const Texcoord& tc, float x = 1, float y = 1, float scalex = 1, float scaley = 1, float scale = 1, float rotation = 0, float alpha = 1, float color = 1)
	{
		if (frameNo >= clip.size())
		{
			clip.resize(frameNo + 1);
		}
		clip[frameNo].push_back({ x,y,0,alpha,scalex,scaley,scale,scale,color,color,color,1,tc });
	}

	bool destroyAnim = false;
	bool loopTime = true;			//ループ再生の有無
	bool killWhenEnd = false;		//アニメ終了時にゲームオブジェクトを削除
	float timer = 0;				//経過時間(秒)
	float interval = 0.1f;			//フレームを進める感覚(秒)
	bool beamEndFlg = false;
	float w = 1;
	std::vector<SpriteList> clip;	//アニメーションクリップ
};

#endif // !COMPONENT_ANIMATOR2D_H_INCLUDE
