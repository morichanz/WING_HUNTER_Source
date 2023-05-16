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
* 2D�A�j���[�V�����R���|�[�l���g
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


		//�f�[�^���Ȃ��ꍇ�͉������Ȃ�
		if (clip.empty())
		{
			return;
		}

		//�o�ߎ��Ԃ��X�V
		timer += deltaTime;

		//�Đ��ʒu���v�Z
		const int size = static_cast<int>(clip.size());
		int i = static_cast<int>(timer / interval);
		if (loopTime)
		{
			i %= size;	//���[�v�Đ�
		}
		else
		{
			//�A�j���I�����AkillwhenEnd�t���O��true�Ȃ�Q�[���I�u�W�F�N�g���폜
			if (i >= size && killWhenEnd)
			{
				gameObject.isDead = true;
			}
			i = std::clamp(i, 0, size - 1);
		}
		//�X�v���C�g�f�[�^���Đ��ʒu�̃f�[�^�ɍX�V
		gameObject.spriteList = clip[i];
	}

	//�t���[���ɃX�v���C�g��ǉ�
	void AddSprite(size_t frameNo, const Texcoord& tc, float x = 1, float y = 1, float scalex = 1, float scaley = 1, float scale = 1, float rotation = 0, float alpha = 1, float color = 1)
	{
		if (frameNo >= clip.size())
		{
			clip.resize(frameNo + 1);
		}
		clip[frameNo].push_back({ x,y,0,alpha,scalex,scaley,scale,scale,color,color,color,1,tc });
	}

	bool destroyAnim = false;
	bool loopTime = true;			//���[�v�Đ��̗L��
	bool killWhenEnd = false;		//�A�j���I�����ɃQ�[���I�u�W�F�N�g���폜
	float timer = 0;				//�o�ߎ���(�b)
	float interval = 0.1f;			//�t���[����i�߂銴�o(�b)
	bool beamEndFlg = false;
	float w = 1;
	std::vector<SpriteList> clip;	//�A�j���[�V�����N���b�v
};

#endif // !COMPONENT_ANIMATOR2D_H_INCLUDE
