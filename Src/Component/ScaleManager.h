	/**
* @file ScaleManager.h
*/

#ifndef SCALEMANAGER_H_INCLUDED
#define SCALEMANAGER_H_INCLUDED
#include"../Component/Component.h"
#include"../GameObject/GameObject.h"

/**
* �Q�[�����ɓo�ꂷ��l�X�ȃI�u�W�F�N�g��\����{�N���X
*/
	class ScaleManager : public Component
	{
	public:
		ScaleManager() = default;
		virtual ~ScaleManager() = default;

		//�C�x���g�̑���
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
