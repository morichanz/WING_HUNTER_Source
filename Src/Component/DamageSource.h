/**
* @file DamageSource.h
*/
#ifndef COMPONENT_DAMAGESOURCE_H_INSTANCE
#define COMPONENT_DAMAGESOURCE_H_INSTANCE
#include "Component.h"
#include"../GameObject/GameObject.h"
#include<string>

/**
* �_���[�W���R���|�[�l���g
*/
class DamageSource : public Component
{
public:
	DamageSource() = default;
	virtual ~DamageSource() = default;
	virtual void OnCollision(GameObject& gameObject, GameObject& other) override
	{
		//�^�[�Q�b�g���Ɠ������O�̃Q�[���I�u�W�F�N�g�Ƀ_���[�W��^����
		if (other.name == targetName)
		{
			other.TakeDamage(gameObject, { amount, 0 });

			if (isOnce)
			{
				gameObject.isDead = true;
			}
		}

		for (int i = 0; i < targetNameList.size(); i++)
		{
			if (other.name == targetNameList[i])
			{
				other.TakeDamage(gameObject, { amount, 0 });

				if (isOnce)
				{
					gameObject.isDead = true;
				}
			}
		}
	}

	//�_���[�W��^������Q�[���I�u�W�F�N�g��
	std::string targetName;
	std::vector<std::string> targetNameList;
	int amount = 1;		//�_���[�W��
	bool isOnce = true;		//��x�_���[�W��^�����玀�ʂ��H
};
#endif // !COMPONENT_DAMAGESOURCE_H_INSTANCE
