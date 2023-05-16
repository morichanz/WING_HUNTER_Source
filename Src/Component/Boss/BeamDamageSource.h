/**
* @file BeamDamageSource.h
*/
#ifndef COMPONENT_BOSSDAMAGESOURCE_H_INSTANCE
#define COMPONENT_BOSSDAMAGESOURCE_H_INSTANCE
#include "../Component.h"
#include"../../GameObject/GameObject.h"
#include<string>

/**
* �_���[�W���R���|�[�l���g
*/
class BeamDamageSource : public Component
{
public:
	BeamDamageSource() = default;
	virtual ~BeamDamageSource() = default;
	virtual void OnCollision(GameObject& gameObject, GameObject& other) override
	{
		//�^�[�Q�b�g���Ɠ������O�̃Q�[���I�u�W�F�N�g�Ƀ_���[�W��^����
		if (other.name == targetName)
		{
			other.TakeDamage(gameObject, { amount, 0 });
		}
	}
	//�_���[�W��^������Q�[���I�u�W�F�N�g��
	std::string targetName;

	int amount = 1;		//�_���[�W��
	bool isOnce = true;		//��x�_���[�W��^�����玀�ʂ��H
};


#endif // !COMPONENT_BOSSDAMAGESOURCE_H_INSTANCE