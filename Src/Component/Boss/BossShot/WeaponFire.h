/**
* @file WeaponFire
*/
#ifndef COMPONENT_WEAPONFIRE_H_INCLUDE
#define COMPONENT_WEAPONFIRE_H_INCLUDE
#include"../../Component.h"
#include"../../GameObject/GameObject.h"
#include"../../Engine/Engine.h"
#include"../../Animator/Animator2D.h"
#include"../../Bullet/BoundBullet.h"
#include"../BossBulletExplosion.h"
#include"../../OutOfScreen.h"
#include"../../DamageSource.h"

const Texcoord weaponFireTc[] =
{
	MakeTexcoord(96, 465, 14, 14),
	MakeTexcoord(113, 465, 14, 14),
	MakeTexcoord(129, 465, 14, 14),
	MakeTexcoord(145, 465, 14, 14),
};

/**
* �G�e���˃R���|�[�l���g
*/
class WeaponFire : public Component
{
public:
	WeaponFire() = default;
	virtual ~WeaponFire() = default;

	void Fire(GameObject& gameObject, GameObjectPtr weaponCenter, GameObjectPtr target, float deltaTime)
	{

		if (isDead) return;
		if (weaponCenter->isDead) isDead = true;
		//���˗p�^�C�}�[�����ˊԊu�𒴂�����A���˗p�ϐ���������
		timerA += deltaTime;
		if (timerA >= intervalA)
		{
			isFiring = true;
			timerA -= intervalA;
			timerB = intervalB;	//1���ڂ������ɔ��˂����悤�ɂ���
			weaponFireCounter = numberOfFire;

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

			if (isFiring)
			{
				Audio::PlayOneShot(SE::enemyShot, 2);
				auto bullet = gameObject.engine->Create<GameObject>("enemy bullet", weaponCenter->x + 10, weaponCenter->y - 30);
				bullet->AddSprite(weaponFireTc[0]);
				bullet->priority = prioWaaponBullet;
				bullet->AddCollision({ -4,-4,4,4 });
				auto move = bullet->AddComponent<BoundBullet>();
				move->boundCounter = 2;
				move->vx = vx;
				move->vy = vy;
				auto exploaion = bullet->AddComponent<BossBulletExprosion>();

				bullet->AddComponent<OutOfScreen>();
				auto damageSource = bullet->AddComponent<DamageSource>();
				damageSource->targetName = "player";

				auto anim = bullet->AddComponent<Animator2D>();
				anim->loopTime = true;		//���[�v�Đ������Ȃ�
				anim->killWhenEnd = false;	//�Đ��I�����ɃQ�[���I�u�W�F�N�g���폜
				anim->clip.resize(std::size(weaponFireTc));
				for (size_t frameNo = 0; frameNo < std::size(weaponFireTc); ++frameNo)
				{
					anim->AddSprite(frameNo, weaponFireTc[frameNo], 1, 1, shotSize, shotSize, shotSize);
				}

				auto bullet1 = gameObject.engine->Create<GameObject>("enemy bullet", weaponCenter->x - 10, weaponCenter->y - 30);
				bullet1->AddSprite(weaponFireTc[0]);
				bullet1->priority = prioWaaponBullet;
				bullet1->AddCollision({ -4,-4,4,4 });
				auto move1 = bullet1->AddComponent<BoundBullet>();
				move1->boundCounter = 2;
				move1->vx = vx;
				move1->vy = vy;
				auto exploaion1 = bullet1->AddComponent<BossBulletExprosion>();
				bullet1->AddComponent<OutOfScreen>();
				auto damageSource1 = bullet1->AddComponent<DamageSource>();
				damageSource1->targetName = "player";

				auto anim1 = bullet1->AddComponent<Animator2D>();
				anim1->loopTime = true;		//���[�v�Đ������Ȃ�
				anim1->killWhenEnd = false;	//�Đ��I�����ɃQ�[���I�u�W�F�N�g���폜
				anim1->clip.resize(std::size(weaponFireTc));
				for (size_t frameNo = 0; frameNo < std::size(weaponFireTc); ++frameNo)
				{
					anim1->AddSprite(frameNo, weaponFireTc[frameNo], 1, 1, shotSize, shotSize, shotSize);
				}
			}
		}

		//�A�˃J�E���^�[�����炵�A0�ȉ��ɂȂ����甭�˃��[�h������
		--weaponFireCounter;
		if (weaponFireCounter <= 0)
		{
			isFiring = false;
		}	
	}

private:
	float numberOfFire = 1.0f;

	float vx = 0.0f;
	float vy = 0.0f;
	float speed = 400.0f;

	float weaponFireCounter = 0.0f;
	float shotSize = 1.0f;

	float timerA = 0.0f;
	float timerB = 0.0f;
	float intervalA = 0.4f;
	float intervalB = 0.2f;
	bool isFiring = false;
	bool vyFlg  = false;
	bool isDead = false;
};
#endif // !COMPONENT_WEAPONFIRE_H_INCLUDE