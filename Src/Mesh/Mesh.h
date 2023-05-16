/**
* @file Mesh.h
*/
#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include "glad/glad.h"
#include "../Texture/Texture.h"
#include "../VecMath/VecMath.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

// 先行宣言
class VertexArray;
using VertexArrayPtr = std::shared_ptr<VertexArray>;

/**
* 3Dモデル関係の機能を格納する名前空間
*/
namespace Mesh
{
	// 先行宣言
	class PrimitiveBuffer;
	using PrimitiveBufferPtr = std::shared_ptr<PrimitiveBuffer>;
	class MeshBuffer;
	using MeshBufferPtr = std::shared_ptr<MeshBuffer>;

	/**
	* 頂点データ形式
	*/
	struct Vertex
	{
		VecMath::vec3 position;
		VecMath::vec2 texcoord;
		VecMath::vec3 normal;
	};
	
	/**
	* マテリアル
	*/
	struct Material
	{
		std::string name = "<Default>";             // マテリアル名
		VecMath::vec4 baseColor = VecMath::vec4(1); // 基本色+アルファ
		float specularPower = 16;                   // 鏡面反射指数
		float normalizeFactor = 24.0f / 25.13274f;  // 正規化係数
		TexturePtr texBaseColor;                    // 基本色テクスチャ
	};
	using MaterialPtr = std::shared_ptr<Material>;


	/**
	* プリミティブ
	*/
	struct Primitive
	{
		GLenum mode = GL_TRIANGLES; //プリミティブの種類
		GLsizei count = 0;          //描画するインデックス数
		const void* indices = 0;    //描画開始インデックスのバイトオフセット
		GLint baseVertex = 0;       //インデックス0となる頂点配列内の位置
		int materialNo = -1;        // マテリアル
	};
	void Draw(const Primitive& prim);

	/**
	* プリミティブを管理するクラス
	*/
	class PrimitiveBuffer
	{
	public:
		//プリミティブバッファを作成する
		static PrimitiveBufferPtr Create(size_t vboCapacity, size_t iboCapacity)
		{
			return std::make_shared<PrimitiveBuffer>(vboCapacity, iboCapacity);
		}
		//コンストラクタ・デストラクタ
		PrimitiveBuffer(size_t vboCapacity, size_t iboCapacity);
		~PrimitiveBuffer();

		//コピーと代入を禁止
		PrimitiveBuffer(const PrimitiveBuffer&) = delete;
		PrimitiveBuffer& operator=(const PrimitiveBuffer&) = delete;

		//プリミティブの追加
		void AddPrimitive(const Vertex* vertices, size_t vertexCount,
			const uint16_t* indices, size_t indexCount, GLenum mode = GL_TRIANGLES);

		//プリミティブの取得
		const Primitive& GetPrimitive(size_t index) const { return primitives[index]; }

		//VAOの取得
		VertexArrayPtr GetVAO() const { return vao; }

		//頂点データ、インデックスデータの追加
		GLint AddVertices(const Vertex* vertices, size_t count);
		const void* AddIndices(const uint16_t* indices, size_t count);

		//全てのプリミティブを削除
		void Clear();

	private:
		std::vector<Primitive> primitives;
		VertexArrayPtr vao;

		GLuint vbo = 0;          // VBO管理番号
		uint8_t* pVbo = nullptr; // VBOの書き込み先アドレス
		size_t vboCapacity = 0;  // VBOの全容量(バイト)
		size_t vboSize = 0;      // VBOの使用済み容量(バイト)

		GLuint ibo = 0;          // IBO管理番号
		uint8_t* pIbo = nullptr; // IBOの書き込み先アドレス
		size_t iboCapacity = 0;  // IBOの全容量(バイト)
		size_t iboSize = 0;      // IBOの使用済み容量(バイト)
	};

	/**
	* 3Dモデル
	*/
	struct StaticMesh
	{
		std::string name;    //メッシュ名
		std::vector<Primitive> primitives;
		std::vector<MaterialPtr> materials;
	};
	using StaticMeshPtr = std::shared_ptr<StaticMesh>;
	using StaticMeshList = std::vector<StaticMeshPtr>;

	void Draw(const StaticMesh& mesh,
		const std::vector<MaterialPtr>&materials, GLuint program = 0);

	/**
	* メッシュを管理するクラス
	*/
	class MeshBuffer
	{
	public:
		//メッシュバッファを作成する
		static MeshBufferPtr Create(size_t vboCapacity, size_t iboCapacity) 
		{
			return std::make_shared<MeshBuffer>(vboCapacity, iboCapacity);
		}
		
		MeshBuffer(size_t vboCapacity, size_t iboCapacity);
		~MeshBuffer() = default;
		
		StaticMeshPtr LoadOBJ(const char* filename);
		VertexArrayPtr GetVAO() const;
		void Clear();
		
	private:
		PrimitiveBuffer primitiveBuffer;
		std::unordered_map<std::string, StaticMeshPtr> meshes;
	};
} //namespace Mesh
#endif //MESH_H_INCLUDED