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

// ��s�錾
class VertexArray;
using VertexArrayPtr = std::shared_ptr<VertexArray>;

/**
* 3D���f���֌W�̋@�\���i�[���閼�O���
*/
namespace Mesh
{
	// ��s�錾
	class PrimitiveBuffer;
	using PrimitiveBufferPtr = std::shared_ptr<PrimitiveBuffer>;
	class MeshBuffer;
	using MeshBufferPtr = std::shared_ptr<MeshBuffer>;

	/**
	* ���_�f�[�^�`��
	*/
	struct Vertex
	{
		VecMath::vec3 position;
		VecMath::vec2 texcoord;
		VecMath::vec3 normal;
	};
	
	/**
	* �}�e���A��
	*/
	struct Material
	{
		std::string name = "<Default>";             // �}�e���A����
		VecMath::vec4 baseColor = VecMath::vec4(1); // ��{�F+�A���t�@
		float specularPower = 16;                   // ���ʔ��ˎw��
		float normalizeFactor = 24.0f / 25.13274f;  // ���K���W��
		TexturePtr texBaseColor;                    // ��{�F�e�N�X�`��
	};
	using MaterialPtr = std::shared_ptr<Material>;


	/**
	* �v���~�e�B�u
	*/
	struct Primitive
	{
		GLenum mode = GL_TRIANGLES; //�v���~�e�B�u�̎��
		GLsizei count = 0;          //�`�悷��C���f�b�N�X��
		const void* indices = 0;    //�`��J�n�C���f�b�N�X�̃o�C�g�I�t�Z�b�g
		GLint baseVertex = 0;       //�C���f�b�N�X0�ƂȂ钸�_�z����̈ʒu
		int materialNo = -1;        // �}�e���A��
	};
	void Draw(const Primitive& prim);

	/**
	* �v���~�e�B�u���Ǘ�����N���X
	*/
	class PrimitiveBuffer
	{
	public:
		//�v���~�e�B�u�o�b�t�@���쐬����
		static PrimitiveBufferPtr Create(size_t vboCapacity, size_t iboCapacity)
		{
			return std::make_shared<PrimitiveBuffer>(vboCapacity, iboCapacity);
		}
		//�R���X�g���N�^�E�f�X�g���N�^
		PrimitiveBuffer(size_t vboCapacity, size_t iboCapacity);
		~PrimitiveBuffer();

		//�R�s�[�Ƒ�����֎~
		PrimitiveBuffer(const PrimitiveBuffer&) = delete;
		PrimitiveBuffer& operator=(const PrimitiveBuffer&) = delete;

		//�v���~�e�B�u�̒ǉ�
		void AddPrimitive(const Vertex* vertices, size_t vertexCount,
			const uint16_t* indices, size_t indexCount, GLenum mode = GL_TRIANGLES);

		//�v���~�e�B�u�̎擾
		const Primitive& GetPrimitive(size_t index) const { return primitives[index]; }

		//VAO�̎擾
		VertexArrayPtr GetVAO() const { return vao; }

		//���_�f�[�^�A�C���f�b�N�X�f�[�^�̒ǉ�
		GLint AddVertices(const Vertex* vertices, size_t count);
		const void* AddIndices(const uint16_t* indices, size_t count);

		//�S�Ẵv���~�e�B�u���폜
		void Clear();

	private:
		std::vector<Primitive> primitives;
		VertexArrayPtr vao;

		GLuint vbo = 0;          // VBO�Ǘ��ԍ�
		uint8_t* pVbo = nullptr; // VBO�̏������ݐ�A�h���X
		size_t vboCapacity = 0;  // VBO�̑S�e��(�o�C�g)
		size_t vboSize = 0;      // VBO�̎g�p�ςݗe��(�o�C�g)

		GLuint ibo = 0;          // IBO�Ǘ��ԍ�
		uint8_t* pIbo = nullptr; // IBO�̏������ݐ�A�h���X
		size_t iboCapacity = 0;  // IBO�̑S�e��(�o�C�g)
		size_t iboSize = 0;      // IBO�̎g�p�ςݗe��(�o�C�g)
	};

	/**
	* 3D���f��
	*/
	struct StaticMesh
	{
		std::string name;    //���b�V����
		std::vector<Primitive> primitives;
		std::vector<MaterialPtr> materials;
	};
	using StaticMeshPtr = std::shared_ptr<StaticMesh>;
	using StaticMeshList = std::vector<StaticMeshPtr>;

	void Draw(const StaticMesh& mesh,
		const std::vector<MaterialPtr>&materials, GLuint program = 0);

	/**
	* ���b�V�����Ǘ�����N���X
	*/
	class MeshBuffer
	{
	public:
		//���b�V���o�b�t�@���쐬����
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