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
* �G�e���˃R���|�[�l���g
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

		//�e�𔭎�
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
					anim->loopTime = true;		//���[�v�Đ������Ȃ�
					anim->killWhenEnd = false;	//�Đ��I�����ɃQ�[���I�u�W�F�N�g���폜
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

	//���������ɐݒ肷��ϐ�
	GameObjectPtr target;	//�W�I
	GameObjectPtr enemy;
	float intervalA = 1;	//���ˊԊu
	float intervalB = 0.1f;	//�A�ˊԊu
	int numberOfFire = 10;	//�A�ː�
	float speed = 300;		//�e�̑��x
	float x = 0;			//���ˈʒu��X���W
	float y = 0;			//���ˈʒu��Y���W
	float angle = 0;
	bool shotFlg = false;	//�e�𔭎˂����Ȃ��悤�ɐݒ肷��(�m�F�p)

	//���s���ɕω�����ϐ�
	bool isFiring = false;	//���˃t���O
	float timerA = 0;		//���˗p�^�C�}�[
	float timerB = 0;		//�A�˗p�^�C�}�[
	int fireCounter = 10;	//�A�ː��J�E���^�[
	float vx = 0;			//���˃x�N�g��X
	float vy = 0;			//���˃x�N�g��Y
	float shotSize = 1.3f;
	float shotCnt = 0;
};
#endif // !COMPONENT_BOSSBULLET_H_INCLUDE