/**
* @file MeshRenderer.h
*/
#ifndef COMPONENT_MESHRENDERER_H_INCLUDED
#define COMPONENT_MESHRENDERER_H_INCLUDED
#include "../Component/Component.h"
#include "../Mesh/Mesh.h"
#include "../VecMath/VecMath.h"

/**
* ���b�V���`��R���|�[�l���g
*/
class MeshRenderer : public Component
{
public:
	MeshRenderer() = default;
	virtual ~MeshRenderer() = default;
	
	Mesh::StaticMeshPtr mesh;

	//���̔z��Ƀ}�e���A����ݒ肷��ƁA���b�V���̃}�e���A���̑���Ɏg����
	std::vector<Mesh::MaterialPtr> materials;
	VecMath::vec3 translate = VecMath::vec3(0); // ���s�ړ�
	VecMath::vec3 rotation = VecMath::vec3(0);  // ��]
	VecMath::vec3 scale = VecMath::vec3(1);     // �g�嗦
};
using MeshRendererPtr = std::shared_ptr<MeshRenderer>;
#endif // COMPONENT_MESHRENDERER_H_INCLUDED