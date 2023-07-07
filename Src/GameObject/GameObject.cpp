/**
* @file GameObject.cpp
*/
#include"GameObject.h"
#include<algorithm>
#include "../ProgramPipeline/ProgramPipeline.h"
#include "../Component/MeshRenderer.h"
#include "../VecMath/VecMath.h"

using namespace VecMath;

/**
* �X�^�[�g�C�x���g
*/
void GameObject::Start()
{
	if (isStarted)
	{
		return;
	}
	for (auto& e : componentList)
	{
		e->Start(*this);
	}
	isStarted = true;
}

/**
* �X�V�C�x���g
*/
void GameObject::Update(float deltaTime)
{
	for (auto& e : componentList)
	{
		e->Update(*this, deltaTime);
	}
}

/**
* �R���W�����C�x���g
*/
void GameObject::OnCollision(GameObject& other)
{
	for (auto& e : componentList)
	{
		e->OnCollision(*this,other);
	}
}

/**
* �폜�C�x���g
*/
void GameObject::OnDestroy()
{
	for (auto& e : componentList)
	{
		e->OnDestroy(*this);
	}
}

/**
* �_���[�W�C�x���g
*/
void GameObject::TakeDamage(GameObject& other, const Damage& damage)
{
	for (auto& e : componentList)
	{
		e->OnTakeDamage(*this, other, damage);
	}
}

/**
* �X�v���C�g�f�[�^��ǉ�����
*/
void GameObject::AddSprite(const Texcoord& tc, float x, float y, float scalex,float scaley, float scale, float rotation,float alpha, float color)
{
	spriteList.push_back({ x,y,rotation,alpha,scalex,scaley,scale,scale,color,color,color,1,tc });
}

void GameObject::ClearSprite()
{
	spriteList.clear();
}

void GameObject::RemoveCollision()
{
	collisionList.clear();
}

/**
* �R���W�����f�[�^��ǉ�����
*/
void GameObject::AddCollision(const Rect& r)
{
	collisionList.push_back(r);
	
	//���E�{�b�N�X���X�V
	aabb.min[0] = std::min(aabb.min[0], r.min[0]);
	aabb.min[1] = std::min(aabb.min[1], r.min[1]);
	aabb.max[0] = std::max(aabb.max[0], r.max[0]);
	aabb.max[1] = std::max(aabb.max[1], r.max[1]);
}

/**
* �Q�[���I�u�W�F�N�g��`�悷��
*
* @param program �`��Ɏg�p����V�F�[�_�v���O����
*/
void GameObject::DrawStaticMesh(const ProgramPipeline & program) const
{
	if (meshRendererList.empty()) 
	{
		return;
	}
	
	mat4 mt = mat4::Translate(vec3(x, y, z));
	mat4 mrx = mat4::RotateX(rotation[0]);
	mat4 mry = mat4::RotateY(rotation[1]);
	mat4 mrz = mat4::RotateZ(rotation[2]);
	const mat4 matGameObject = mt * mry * mrx * mrz;

	for (const auto& e : meshRendererList) 
	{
		if (!e->mesh)
		{
			continue;
		}
		
		//���s�ړ��A��]�A�g�嗦��ݒ�
		mat4 mt = mat4::Translate(e->translate);
		mat4 mrx = mat4::RotateX(e->rotation.x);
		mat4 mry = mat4::RotateY(e->rotation.y);
		mat4 mrz = mat4::RotateZ(e->rotation.z);
		mat4 ms = mat4::Scale(e->scale);
		mat4 matMesh = mt * mry * mrx * mrz * ms;
		mat4 matModel = matGameObject * matMesh;
		glProgramUniformMatrix4fv(program, 0, 1, GL_FALSE, &matModel[0][0]);
		if (e->materials.empty()) {
			Mesh::Draw(*e->mesh, e->mesh->materials, program);
			
		}
		else {
			Mesh::Draw(*e->mesh, e->materials, program);
			
		}
	} // for
}