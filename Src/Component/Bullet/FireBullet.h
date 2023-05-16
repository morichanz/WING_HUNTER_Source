/**
* @file FireBullet
*/
#ifndef COMPONENT_FIREBULLET_H_INCLUDE
#define COMPONENT_FIREBULLET_H_INCLUDE
#include"../Component.h"
#include"../Sprite/SpritePriority.h"
#include "../GameObject/GameObject.h"
#include"../Engine/Engine.h"
#include"../Move.h"
#include"../OutOfScreen.h"
#include"../Bullet/BoundBullet.h"

const Texcoord fireTc[] = {
MakeTexcoord(585, 41, 17, 16),
MakeTexcoord(602, 41, 17, 16),
MakeTexcoord(619, 41, 17, 16),
MakeTexcoord(636, 41, 17, 16),
};

/**
* �G�e���˃R���|�[�l���g
*/
class FireBullet : public Component
{
public:
	FireBullet() = default;
	virtual ~FireBullet() = default;
	virtual void Update(GameObject& gameObject, float deltaTime) override
	{
		//���˗p�^�C�}�[�����ˊԊu�𒴂�����A���˗p�ϐ���������
		timerA += deltaTime;
		if (timerA >= intervalA)
		{
			isFiring = true;
			timerA -= intervalA;
			timerB = intervalB;	//1���ڂ������ɔ��˂����悤�ɂ���
			fireCounter = numberOfFire;

			//�^�[�Q�b�g�w�肪����΁A���˃x�N�g�����^�[�Q�b�g�Ɍ�����
			if (target)
			{
				vx = target->x - gameObject.x;
				vy = target->y - gameObject.y;
				const float v = sqrt(vx * vx + vy * vy);
				if (vy > 0)
				{
					vyFlg = true;
				}
				else
				{
					vyFlg = false;
					vx = vx / v * speed;
					vy = vy / v * speed;
				}
			}
			else
			{
				vx = 0;
				vy = -speed;
			}
		}

		//���˃��[�h�łȂ���Ή������Ȃ�
		if (!isFiring)
		{
			return;
		}

		//�A�˗p�^�C�}�[���A�ˊԊu�𒴂�����A�e�𔭎�
		timerB += deltaTime;
		if (timerB < intervalB)
		{
			return;
		}
		timerB -= intervalB;

		if (!vyFlg)
		{
			//�e�𔭎�
			if (enemy->shotFlg)
			{
				auto bullet = gameObject.engine->Create<GameObject>("enemy bullet", gameObject.x + x, gameObject.y + y);
				bullet->AddSprite(fireTc[0]);
				bullet->priority = prioEnemyBullet;
				bullet->AddCollision({ -4,-4,4,4 });
				auto move = bullet->AddComponent<BoundBullet>();
				if (!boundFlg)
				{
					move->boundCounter = 0;
				}
				else if (boundFlg && gameObject.engine->stageCnt == 6)
				{
					move->boundCounter = 2;
				}
				else if(boundFlg && gameObject.engine->stageCnt != 1)
				{
					move->boundCounter = 3;
				}
				move->vx = vx;
				move->vy = vy;
				bullet->AddComponent<OutOfScreen>();
				auto damageSource = bullet->AddComponent<DamageSource>();
				damageSource->targetName = "player";

				auto anim = bullet->AddComponent<Animator2D>();
				anim->loopTime = true;		//���[�v�Đ������Ȃ�
				anim->killWhenEnd = false;	//�Đ��I�����ɃQ�[���I�u�W�F�N�g���폜
				anim->clip.resize(std::size(fireTc));
				for (size_t frameNo = 0; frameNo < std::size(fireTc); ++frameNo)
				{
					anim->AddSprite(frameNo, fireTc[frameNo], 1, 1, shotSize, shotSize, shotSize);
				}
			}
		}

		//�A�˃J�E���^�[�����炵�A0�ȉ��ɂȂ����甭�˃��[�h������
		--fireCounter;
		if (fireCounter <= 0)
		{
			isFiring = false;
		}
	}

	//���������ɐݒ肷��ϐ�
	GameObjectPtr target;	//�W�I
	GameObjectPtr enemy;
	float intervalA = 0.9f;	//���ˊԊu
	float intervalB = 0.1f;	//�A�ˊԊu
	int numberOfFire = 1;	//�A�ː�
	float speed = 200;		//�e�̑��x
	float x = 0;			//���ˈʒu��X���W
	float y = 0;			//���ˈʒu��Y���W
	int tecNum = 0;
	bool shotFlg = false;	//�e�𔭎˂����Ȃ��悤�ɐݒ肷��(�m�F�p)
	float shotSize = 1.3f;
	bool boundFlg = false;
	bool vyFlg = false;

	//���s���ɕω�����ϐ�
	bool isFiring = false;		//���˃t���O
	float timerA = 0;	//���˗p�^�C�}�[
	float timerB = 0;	//�A�˗p�^�C�}�[
	int fireCounter = 1;//�A�ː��J�E���^�[
	float vx = 0;		//���˃x�N�g��X
	float vy = 0;		//���˃x�N�g��Y
};
#endif // !COMPONENT_FIREBULLET_H_INCLUDE
