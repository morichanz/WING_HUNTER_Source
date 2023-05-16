/**
* @file MiddleEnemyAction.h
*/
#ifndef COMPONENT_MIDDLENEMYACTION_H_INCLUDE
#define COMPONENT_MIDDLENEMYACTION_H_INCLUDE
#include"../Component.h"
#include"../../GameObject/GameObject.h"
#include"../Bullet/FireBullet.h"
#include"../../Easing.h"
#include"../MeshRenderer.h"
#include<time.h>

/**
* ���^�@���ړ�����
*/
class MiddleEnemyAction :public Component
{
public:
	MiddleEnemyAction() = default;
	virtual ~MiddleEnemyAction() = default;

	virtual void Start(GameObject& gameObject)override
	{
		srand(static_cast<unsigned int>(time(NULL)));
		_rand = rand() % 2;

		std::random_device rnd;     // �񌈒�I�ȗ���������𐶐�
		std::mt19937 mt(rnd());     //  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
		std::uniform_int_distribution<> rand3(0, 300);        // [0, 300] �͈͂̈�l����
		_randPosition = rand3(mt);

		if (rand == 0)
		{
			gameObject.x = -32;
			gameObject.y = 550;
		}
		else
		{
			gameObject.x = 500;
			gameObject.y = 550;
		}
	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		EnemyMove(gameObject, deltaTime);
		Shot(gameObject, deltaTime);
	}

	//�_���[�W���󂯂��Ƃ�
	virtual void OnTakeDamage(GameObject& gameObject, GameObject& other, const Damage& damage) override
	{
		health -= damage.amount;
		if (health <= 0)
		{
			Audio::PlayOneShot(SE::enemyExplosionM);
			gameObject.scoreFlg = true;
			gameObject.isDead = true;
		}
	}

	//����
	void Dis(GameObject& gameObject, float disx, float disy)
	{
		if (!disFlg)
		{
			enemyPosX = gameObject.x;
			enemyPosY = gameObject.y;
			disX = (disx - gameObject.x);
			disY = (disy - gameObject.y);
		}
		disFlg = true;
	}
	
	//�G�̒e
	void Shot(GameObject& gameObject, float deltaTime)
	{
		if (!shadowFlg)
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
				if (player)
				{
					shotvx = player->x - gameObject.x;
					shotvy = player->y - gameObject.y;
					const float v = sqrt(shotvx * shotvx + shotvy * shotvy);

					shotvx = shotvx / v * speed;
					shotvy = shotvy / v * speed;
				}
				else
				{
					shotvx = 0;
					shotvy = -speed;
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

			//�e�𔭎�
			if (shotFlg)
			{
				auto bullet = gameObject.engine->Create<GameObject>("enemy bullet", gameObject.x + x, gameObject.y + y);
				bullet->AddSprite(tc);
				bullet->priority = prioEnemyBullet;
				bullet->AddCollision({ -4,-4,4,4 });
				auto move = bullet->AddComponent<Move>();
				move->vx = shotvx;
				move->vy = shotvy;
				bullet->AddComponent<OutOfScreen>();
				auto damageSource = bullet->AddComponent<DamageSource>();
				damageSource->targetName = "player";
			}

			//�A�˃J�E���^�[�����炵�A0�ȉ��ɂȂ����甭�˃��[�h������
			--fireCounter;
			if (fireCounter <= 0)
			{
				isFiring = false;
			}
		}
	}

	//�G�̓���
	void EnemyMove(GameObject& gameObject, float deltaTime)
	{
		//�{�̂̓���
		if (_rand == 0)
		{
			switch (moveCnt)
			{
			case 0:
				Dis(gameObject, static_cast<float>(_randPosition), 450);
				gameObject.x = enemyPosX + disX * Easing::EaseOutExpo(val);
				gameObject.y = enemyPosY + disY * Easing::EaseOutExpo(val);
				val += deltaTime;
				if (val > 1)
				{
					disFlg = false;
					val = 0.0f;
					moveCnt = 1;
				}
				break;

			case 1:
				Dis(gameObject, static_cast<float>(_randPosition), -150);
				gameObject.x = enemyPosX + disX * Easing::EaseOutExpo(val);
				gameObject.y = enemyPosY + disY * Easing::EaseOutExpo(val);
				val += deltaTime / 2;
				if (val > 1)
				{
					disFlg = false;
					val = 0.0f;
				}
				break;
			}
		}
		else
		{
			switch (moveCnt)
			{
			case 0:
				Dis(gameObject, static_cast<float>(_randPosition), 450);
				gameObject.x = enemyPosX + disX * Easing::EaseOutExpo(val);
				gameObject.y = enemyPosY + disY * Easing::EaseOutExpo(val);
				val += deltaTime / 2;
				if (val > 1)
				{
					disFlg = false;
					val = 0.0f;
					moveCnt = 1;
				}
				break;

			case 1:
				Dis(gameObject, static_cast<float>(_randPosition), -150);
				gameObject.x = enemyPosX + disX * Easing::EaseOutExpo(val);
				gameObject.y = enemyPosY + disY * Easing::EaseOutExpo(val);
				val += deltaTime / 2;

				if (val > 1)
				{
					disFlg = false;
					val = 0.0f;
				}
				break;
			}
		}
	}
	GameObjectPtr target;
	GameObjectPtr player;

	bool shotFlg = false;
	float vy = 0.0f;
	float vx = 0.0f;
	float shotvy = 100.0f;
	float shotvx = 100.0f;
	float val = 0.0f;
	bool DownMoveTimer = false;
	bool addBulletFlg = false;
	bool shadowFlg = false;

	Texcoord tc = MakeTexcoord(336, 448, 16, 16);	//�e�N�X�`�����W
	float intervalA = 1.0f;	//���ˊԊu
	float intervalB = 0.1f;	//�A�ˊԊu
	float numberOfFire = 1.0f;	//�A�ː�
	float speed = 200.0f;		//�e�̑��x
	bool isFiring = false;		//���˃t���O
	float timerA = 0.0f;	//���˗p�^�C�}�[
	float timerB = 0.0f;	//�A�˗p�^�C�}�[
	float fireCounter = 1.0f;//�A�ː��J�E���^�[
	float x = 0.0f;			//���ˈʒu��X���W
	float y = 0.0f;			//���ˈʒu��Y���W

private:
	int health = 2;	//�ϋv�l
	float disX = 0.0f;
	float disY = 0.0f;
	float enemyPosX = 0.0f;
	float enemyPosY = 0.0f;
	bool disFlg = false;
	bool enemyUpFlg = false;
	float scale = 0.3f;

	float shadowX = 32.0f;
	float shadowY = -16.0f;

	bool side = false;
	bool right = false;
	bool left = false;

	int moveCnt = 0;
	int _rand = 0;
	int _randPosition = 0;
};
#endif //!COMPONENT_MIDDLENEMYACTION_H_INCLUDE