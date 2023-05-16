/**
* @file Component.h
*/

#ifndef COMPONENT_H_INCLUDED
#define COMPONENT_H_INCLUDED
#include<string>
#include<vector>
#include<memory>

//��s�錾
class GameObject;
class Engine;
class Component;
using ComponentPtr = std::shared_ptr<Component>;	//�R���|�[�l���g�̃|�C���^
using ComponentList = std::vector<ComponentPtr>;	//�R���|�[�l���g�|�C���^�̔z��

//�_���[�W���
struct Damage
{
	int amount = 0;	//�_���[�W��
	int type = 0;		//�_���[�W�̎��
};

/**
* �R���|�[�l���g�̊��N���X
*/
class Component
{
public:
	Component() = default;
	virtual ~Component() = default;

	//�ŏ���Update�̒��O�ŌĂяo�����
	virtual void Start(GameObject& gameObject){}

	//���t���[��1��Ăяo�����
	virtual void Update(GameObject& gameObject,float deltaTime){}

	//�Փ˂��N�����Ƃ��ɌĂяo�����
	virtual void OnCollision(GameObject & gameObject, GameObject & other) {}

	//�_���[�W���󂯂����ɌĂяo�����
	virtual void OnTakeDamage(GameObject& gameObject,GameObject& other,const Damage& damage){}

	//�Q�[���I�u�W�F�N�g���G���W������폜����鎞�ɌĂяo�����
	virtual void OnDestroy(GameObject& gameObject){}

	std::string name;	//�R���|�[�l���g��
};

#endif	//COMPONENT_H_INCLUDED
