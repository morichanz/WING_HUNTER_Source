/**
* @file LargeEnemyAction.h.h
*/
#ifndef COMPONENT_LARGEENEMYACTION_H_INCLUDED
#define COMPONENT_LARGEENEMYACTION_H_INCLUDED
#include "../../Component/Component.h"
#include "../../GameObject/GameObject.h"
#include"../../Audio/AudioEvent.h"
#include"../../Audio/AudioSetting.h"
#include"../../Easing.h"
#include"../../VecMath/VecMath.h"
#include"../MeshRenderer.h"
#include"../Color.h"
#include"../Rotation/Rotation.h"

/**
* ��^�@���ړ�����
*/
class LargeEnemyAction : public Component
{
public:
	LargeEnemyAction() = default;
	virtual ~LargeEnemyAction() = default;

	virtual void Start(GameObject& gameObject)override
	{
		SpawnPos(gameObject);
		gameObject.health = gameObject.engine->j["LargeEnemy"]["hp"];
		gameObject.z = 500;
	}

	virtual void Update(GameObject& gameObject, float deltaTime) override
	{
		ColorChange(gameObject, deltaTime);

		if (val < 1)
		{
			if(movecnt == 0) val += deltaTime / 3;
			else val += deltaTime / 2;
		}

		if (stageCnt == 1) Stage1Move(gameObject, deltaTime);
		else EnemyMove(gameObject, deltaTime);

		if (moveFlg)gameObject.AddCollision({ -50, -50, 45, 37 });
	}

	//�_���[�W���󂯂��Ƃ�
	virtual void OnTakeDamage(GameObject& gameObject, GameObject& other, const Damage& damage) override
	{
		Audio::PlayOneShot(SE::weaponDamage, 1);
		gameObject.health -= damage.amount;
		gameObject.damageFlg = true;
		if (gameObject.health <= 0)
		{
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

	//�C�[�W���O
	void EaseRoate(GameObject& gameObject,float disy)
	{
		if (!roatDisFlg)
		{
			roatPosY = gameObject.rotation[1];
			roatY = (disy - gameObject.rotation[1]);
		}
		roatDisFlg = true;
	}

	//�X�|�[�����̃|�W�V����
	void SpawnPos(GameObject& gameObject)
	{
		if (gameObject.x > 300 && !side)
		{
			gameObject.x = -120;
			gameObject.y = 200;
			side = true;
			left = true;
		}
		if (gameObject.x < 300 && !side)
		{
			gameObject.x = 650;
			gameObject.y = 200;
			side = true;
			right = true;
		}
	}

	//�_���[�W���󂯂��Ƃ��ɐF��ς���
	void ColorChange(GameObject& gameObject, float deltaTime)
	{
		if (gameObject.damageFlg)
		{
			if (colorInterval > 0)
			{
				enemyColor->SetMaterialColor(gameObject.meshRenderer, enemyColor->objectColor.red);
				colorInterval -= deltaTime;
			}
			else
			{
				enemyColor->SetMaterialColor(gameObject.meshRenderer, enemyColor->objectColor.baceColor);
				colorInterval = 0.05f;
				gameObject.damageFlg = false;
			}
		}
	}

	//�X�e�[�W1�̂ݍs�����Ⴄ
	void Stage1Move(GameObject& gameObject, float deltaTime)
	{
		switch (movecnt)
		{
		case 0:
			//�o�ꎞ�T�C�Y������������
			if (gameObject.z > 0)
			{
				if (gameObject.z > 0) gameObject.z -= deltaTime * 150;

				//���Ɉړ�
				Dis(gameObject, 380, 450);
				gameObject.x = enemyPosX + disX * Easing::EaseOutExpo(val);
				gameObject.y = enemyPosY + disY * Easing::EaseOutExpo(val);
			}
			else
			{
				gameObject.shotFlg = true;
				movecnt = 1;
				moveFlg = true;
			}
			break;
		case 1:
			if (val < 1)
			{
				//�E�Ɉړ�
				EaseRoate(gameObject, 20.0f);
				Dis(gameObject, 500, 430);
				gameObject.x = enemyPosX + disX * Easing::EaseInOutCubic(val);
				gameObject.y = enemyPosY + disY * Easing::EaseInOutCubic(val);
			}
			else
			{
				val = 0.0f;
				movecnt = 2;
				disFlg = false;
				roatDisFlg = false;
			}
			break;
		case 2:
			if (val < 1)
			{
				//���Ɉړ�
				EaseRoate(gameObject, -20.0f);
				Dis(gameObject, 50, 430);
				gameObject.x = enemyPosX + disX * Easing::EaseInOutCubic(val);
				gameObject.y = enemyPosY + disY * Easing::EaseInOutCubic(val);
			}
			else
			{
				val = 0.0f;
				movecnt = 1;
				disFlg = false;
				roatDisFlg = false;
			}
			break;
		}
	}

	//�X�e�[�W�Q�`�̓G�̍s��
	void EnemyMove(GameObject& gameObject, float deltaTime)
	{
		//������X�|�[���������̓���
		if (left)
		{
			switch (movecnt)
			{
			case 0:
				if (gameObject.z > 0)
				{
					gameObject.z -= deltaTime * 100;
					Dis(gameObject, 450, 550);
					if (val < 0.5)
					{
						gameObject.x = enemyPosX + disX * Easing::EaseOutExpo(val);
						gameObject.y = enemyPosY + disY * Easing::EaseOutExpo(val);
					}
				}
				else
				{
					gameObject.shotFlg = true;
					movecnt = 2;
					moveFlg = true;
				}
				break;
			case 1:
				if (val < 1)
				{
					Dis(gameObject, 450, 550);
					gameObject.x = enemyPosX + disX * Easing::EaseInOutCubic(val);
					gameObject.y = enemyPosY + disY * Easing::EaseInOutCubic(val);
				}
				else
				{
					val = 0.0f;
					movecnt = 2;
					disFlg = false;
				}
				break;
			case 2:
				if (val < 1)
				{
					Dis(gameObject, 50, 550);
					gameObject.x = enemyPosX + disX * Easing::EaseInOutCubic(val);
					gameObject.y = enemyPosY + disY * Easing::EaseInOutCubic(val);
				}
				else
				{
					val = 0.0f;
					movecnt = 3;
					disFlg = false;
				}
				break;
			case 3:
				if (val < 1)
				{
					Dis(gameObject, 50, 300);
					gameObject.x = enemyPosX + disX * Easing::EaseInOutCubic(val);
					gameObject.y = enemyPosY + disY * Easing::EaseInOutCubic(val);
				}
				else
				{
					val = 0.0f;
					movecnt = 4;
					disFlg = false;
				}
				break;
			case 4:
				if (val < 1)
				{
					Dis(gameObject, 450, 300);
					gameObject.x = enemyPosX + disX * Easing::EaseInOutCubic(val);
					gameObject.y = enemyPosY + disY * Easing::EaseInOutCubic(val);
				}
				else
				{
					val = 0.0f;
					movecnt = 1;
					disFlg = false;
				}
				break;
			}
		}
		//�E����X�|�[���������̓���
		if (right)
		{
			switch (movecnt)
			{
			case 0:
				if (gameObject.z > 0)
				{
					gameObject.z -= deltaTime * 100;
					Dis(gameObject, 50, 550);
					gameObject.x = enemyPosX + disX * Easing::EaseOutCirc(val);
					gameObject.y = enemyPosY + disY * Easing::EaseOutCirc(val);
				}
				else
				{
					gameObject.shotFlg = true;
					movecnt = 2;
					moveFlg = true;
				}
				break;
			case 1:
				if (val < 1)
				{
					Dis(gameObject, 50, 550);
					gameObject.x = enemyPosX + disX * Easing::EaseInOutCubic(val);
					gameObject.y = enemyPosY + disY * Easing::EaseInOutCubic(val);
				}
				else
				{
					val = 0.0f;
					movecnt = 2;
					disFlg = false;
				}
				break;
			case 2:
				if (val < 1)
				{
					Dis(gameObject, 450, 550);
					gameObject.x = enemyPosX + disX * Easing::EaseInOutCubic(val);
					gameObject.y = enemyPosY + disY * Easing::EaseInOutCubic(val);
				}
				else
				{
					val = 0.0f;
					movecnt = 3;
					disFlg = false;
				}
				break;
			case 3:
				if (val < 1)
				{
					Dis(gameObject, 450, 300);
					gameObject.x = enemyPosX + disX * Easing::EaseInOutCubic(val);
					gameObject.y = enemyPosY + disY * Easing::EaseInOutCubic(val);
				}
				else
				{
					val = 0.0f;
					movecnt = 4;
					disFlg = false;
				}
				break;
			case 4:
				if (val < 1)
				{
					Dis(gameObject, 50, 300);
					gameObject.x = enemyPosX + disX * Easing::EaseInOutCubic(val);
					gameObject.y = enemyPosY + disY * Easing::EaseInOutCubic(val);
				}
				else
				{
					val = 0.0f;
					movecnt = 1;
					disFlg = false;
				}
				break;
			}
		}
	}

	GameObjectPtr target;
	std::shared_ptr<Color> enemyColor;

	bool moveFlg = false;	//�ړ��J�n�t���O
	float val = 0.0f;
	bool sideFlg = false;
	int movecnt = 0;		//�ړ����J�E���g
	int stageCnt = 0;
private:

	float colorInterval = 0.05f; //�_���[�W���󂯂��Ƃ��ɂ̐F�ύX
	float disX = 0.0f;		//�����x�N�g��
	float disY = 0.0f;		//�����x�N�g��
	float enemyPosX = 0.0f;	//�O��̃|�W�V����
	float enemyPosY = 0.0f;	//����̃|�W�V����
	bool disFlg = false;	//�|�W�W������1��̂ݎ擾����悤�ɂ���

	bool side = false;	//�����_���t���O
	bool right = false;	//�E�ړ��t���O
	bool left = false;	//���ړ��t���O

	float moveRotation = 0.0f;//�X��
	float roatY = 0.0f;	//�����x�N�g��
	float roatPosY = 0.0f;	//����̃|�W�V����
	bool roatDisFlg = false;
	bool roatchangeFlg = false;

	int amount = 0;
};
#endif //COMPONENT_LARGEENEMYACTION_H_INCLUDED