/**
* @file GameObject.h
*/

#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include"../Sprite/Sprite.h"
#include"../Component/Component.h"

#include"../Rect.h"
#include<string>
#include<vector>
#include<memory>
//#include <type_traits>

//��s�錾
class Engine;
class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>;	//�R���|�[�l���g�̃|�C���^
using GameObjectList = std::vector<GameObjectPtr>;	//�R���|�[�l���g�|�C���^�̔z��
class MeshRenderer;
using MeshRendererPtr = std::shared_ptr<MeshRenderer>;
using MeshRendererList = std::vector<MeshRendererPtr>;
class ProgramPipeline;


/**
* �Q�[�����ɓo�ꂷ��l�X�ȃI�u�W�F�N�g��\����{�N���X
*/
class GameObject:public std::enable_shared_from_this<GameObject>
{
public:
	GameObject() = default;
	virtual ~GameObject() = default;

	//�C�x���g�̑���
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void OnCollision(GameObject& object);
	virtual void OnDestroy();
	virtual void TakeDamage(GameObject& other, const Damage& damage);
	bool IsStarted() const { return isStarted; }

	//�X�v���C�g�̑���
	void AddSprite(const Texcoord& tc, float x = 0, float y = 0, float scalex = 1, float scaley = 1, float scale = 1, float rotation = 0, float alpha = 1,float color = 1);
	void ClearSprite();
	const SpriteList& GetSprite() const { return spriteList; }

	//�R���W�����̑���
	void AddCollision(const Rect& r);
	void RemoveCollision();

	const RectList& GetCollision() const { return collisionList; }
	const Rect& GetAabb() const { return aabb; }
	void SetAabb(Rect aabb) {
		this->aabb = aabb;
	}

	//�R���|�[�l���g�̑���
	template<typename T>
	std::shared_ptr<T> AddComponent()
	{
		std::shared_ptr<T> p = std::make_shared<T>();
		componentList.push_back(p);
		//���b�V�������_���̏ꍇ�͐�p���X�g�ɒǉ�
		if constexpr(std::is_base_of<MeshRenderer, T>::value)
		{
			meshRendererList.push_back(p);
		}
		return p;
	}
	
	template <typename T>
	std::shared_ptr<T> GetComponent()
	{
		auto component = std::make_shared<T>();
		for (auto itr : componentList)
		{
			if (component->name == itr->name)
			{
				return std::dynamic_pointer_cast<T>(itr) ;
			}
		}
		return nullptr;
	}

	//�`��
	void DrawStaticMesh(const ProgramPipeline & program) const;
public:

	Engine* engine = nullptr;	//�G���W���̃A�h���X
	std::string name;			//�I�u�W�F�N�g��
	float x = 0.0f;				//x���W
	float y = 0.0f;				//y���W
	float z = 0.0f;                 //Z���W
	float rotation[3] = { 0, 0, 0 }; //��]
	int priority = 0;			//�\���D�揇��
	SpriteList spriteList;		//�X�v���C�g�z��
	ComponentList componentList;//�R���|�[�l���g�z��
	MeshRendererPtr meshRenderer;
	bool isDead = false;		//���S�t���O
	int health = 10;			//�̗�
	float scale = 0.0f;			//�T�C�Y
	bool clearFlg = false;		//�N���A�t���O
	bool scoreFlg = false;		//�X�R�A�t���O(��ʊO�ɏo���Ƃ��̓X�R�A�����Z����Ȃ��悤�ɂ���)
	float clearTimer = 1;		//�N���A�ɍs���^�C�����O
	bool bossFlg = false;	//�{�X�o���t���O
	int AI = 0;
	bool deleteFlg = false;
	float alpha = 1.0f;
	bool shotFlg = false;
	float size = 1.0f;
	bool collisionFlg = false;
	float invincible = 0.0f; //���G����
	float rotate = 0.0f;
	bool damageFlg = false;
	bool beamEndFlg = false;
	bool push = false;
	float translate = 0.0f;
	bool chargeFlg = false;
private:
	bool isStarted = false;		//�X�^�[�Ǝ��s�ς݃t���O
	RectList collisionList;		//�R���W�����z��
	Rect aabb = { 0,0,0,0 };	//�R���W�������E�{�b�N�X
	std::vector<MeshRendererPtr> meshRendererList;
};
#endif //GAMEOBJECT_H_INCLUDED
