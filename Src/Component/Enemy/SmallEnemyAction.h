/**
* @file SmallEnemyAction.h
*/
#ifndef COMPONENT_SMALLENEMYACTION_H_INCLUDE
#define COMPONENT_SMALLENEMYACTION_H_INCLUDE
#include"../../Component/Component.h"
#include"../../GameObject/GameObject.h"
#include"../../Component/MeshRenderer.h"
#include"../Bullet/FireBullet.h"
#include"../../Easing.h"
#include "../VecMath/VecMath.h"
#include"../Color.h"

/**
* ���^�@���ړ�����
*/
class SmallEnemyAction : public Component
{
public:
	SmallEnemyAction() = default;
	virtual ~SmallEnemyAction() = default;

	virtual void Start(GameObject& gameObject)override
	{
		gameObject.y = 0;
		gameObject.z = -200;
	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//��ʊO�ɏo�Ȃ��悤�ɐݒ�
		const float viewSizeY = 22 * 32;

		if (gameObject.z < 0)gameObject.z += deltaTime * 150;

		ColorChange(gameObject, deltaTime);

		//�{�̂̓���
		gameObject.priority = prioSmallEnemyMachine;
		if (!enemyUpFlg)
		{
			if(val < 1)val += deltaTime;
			EnemyMove(gameObject, gameObject.x, viewSizeY - 50);
			if (gameObject.z >= 0)
			{
				gameObject.z = 0;
				enemyUpFlg = true;
			}
		}
		else
		{
			gameObject.AddCollision({ -15, -15, 15, 15 });
			gameObject.shotFlg = true;
			gameObject.y -= vy * deltaTime;
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

	//����
	void EnemyMove(GameObject& gameObject, float disx, float disy)
	{
		if (!disFlg)
		{
			enemyPosX = gameObject.x;
			enemyPosY = gameObject.y;

			disX = (disx - gameObject.x);
			disY = (disy - gameObject.y);
			disFlg = true;
		}
		gameObject.x = enemyPosX + disX * Easing::EaseOutBack(val);
		gameObject.y = enemyPosY + disY * Easing::EaseOutBack(val);
	}

	GameObjectPtr target;
	GameObjectPtr player;

	std::shared_ptr<Color> enemyColor;

	float vy = 0;
	float val = 0.0f;

private:
	float health = 2;	//�ϋv�l
	float disX = 0;		//�����x�N�g��
	float disY = 0;		//�����x�N�g��
	float enemyPosX = 0;//�O��̃|�W�V����
	float enemyPosY = 0;//�O��̃|�W�V����
	bool disFlg = false;//��x���������𑪂�
	bool enemyUpFlg = false;
	float colorInterval = 0.05f; //�_���[�W���󂯂��Ƃ��ɂ̐F�ύX
	float moveRotation = 0;

};
#endif // !COMPONENT_SMALLENEMYACTION_H_INCLUDE