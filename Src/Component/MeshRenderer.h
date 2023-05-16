/**
* @file MeshRenderer.h
*/
#ifndef COMPONENT_MESHRENDERER_H_INCLUDED
#define COMPONENT_MESHRENDERER_H_INCLUDED
#include "../Component/Component.h"
#include "../Mesh/Mesh.h"
#include "../VecMath/VecMath.h"

/**
* メッシュ描画コンポーネント
*/
class MeshRenderer : public Component
{
public:
	MeshRenderer() = default;
	virtual ~MeshRenderer() = default;
	
	Mesh::StaticMeshPtr mesh;

	//この配列にマテリアルを設定すると、メッシュのマテリアルの代わりに使われる
	std::vector<Mesh::MaterialPtr> materials;
	VecMath::vec3 translate = VecMath::vec3(0); // 平行移動
	VecMath::vec3 rotation = VecMath::vec3(0);  // 回転
	VecMath::vec3 scale = VecMath::vec3(1);     // 拡大率
};
using MeshRendererPtr = std::shared_ptr<MeshRenderer>;
#endif // COMPONENT_MESHRENDERER_H_INCLUDED