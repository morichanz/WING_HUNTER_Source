	/**
* @file ScaleManager.h
*/

#ifndef SCALEMANAGER_H_INCLUDED
#define SCALEMANAGER_H_INCLUDED
#include"../Component/Component.h"
#include"../GameObject/GameObject.h"

/**
* ゲーム内に登場する様々なオブジェクトを表す基本クラス
*/
	class ScaleManager : public Component
	{
	public:
		ScaleManager() = default;
		virtual ~ScaleManager() = default;

		//イベントの操作
		virtual void Start(GameObject& gameObject)override
		{
			gameObject.scale = scale;
		};
		virtual void Update(GameObject& gameObject, float deltaTime) override
		{
			for (auto& s : gameObject.spriteList)
			{
				s.sx = gameObject.scale;
				s.sy = gameObject.scale;
				s.sz = gameObject.scale;
			}

			if (scaleFlg)
			{
				if (scale > 1)scale -= deltaTime;
			}
		}
		float scale = 1.0f;
		bool scaleFlg = false;
	};
#endif //SCALEMANAGER_H_INCLUDED
