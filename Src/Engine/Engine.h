/**
* @file Engine.h
*/
#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED
#include "glad/glad.h"
#include"../Component/Camera.h"
#include"../Sprite/Sprite.h"
#include "../Scene/Scene.h"
#include "../TileMap.h"
#include"../GameObject/GameObject.h"
#include "../VecMath/VecMath.h"
#include"../FramebufferObject/FramebufferObject.h"
#include"../Component/MeshRenderer.h"
#include <GLFW/glfw3.h>
#include <string>
#include <random>
#include"../../json/json.hpp"

//��s�錾
class ProgramPipeline;
using ProgramPipelinePtr = std::shared_ptr<ProgramPipeline>;

class Texture;
using TexturePtr = std::shared_ptr<Texture>;

using json = nlohmann::json;

namespace Mesh 
{
	class PrimitiveBuffer;
	using PrimitiveBufferPtr = std::shared_ptr<PrimitiveBuffer>;
	class MeshBuffer;
	using MeshBufferPtr = std::shared_ptr<MeshBuffer>;
	struct StaticMesh;
	using StaticMeshPtr = std::shared_ptr<StaticMesh>;
	using StaticMeshList = std::vector<StaticMeshPtr>;
} // namespace Mesh

/**
* �Q�[���G���W��
*/
class Engine
 {
	public:
		json j;
		int stageCnt = 1;	//�X�e�[�W�؂�ւ��J�E���g
		bool debugMap = true;//�{�X�f�o�b�O�p�X�e�[�W
		bool invincible = false;//���G���
		float interval = 0;	//�V�[���؂�ւ��C���^�[�o��
		int playerHp = 0;	//�v���C���[�̌��݂�HP
		int playerLife = 0;	//�v���C���[�̎c�@
		int score = 0;		//���݂̃X�R�A
		int scoreBack = 0;  //�L�^�p�X�R�A
		bool fadeFlg = false;//�t�F�[�h�t���O
		int fadeNum = 0;	//1�Ȃ�^�C�g��2�Ȃ�Q�[���I�[�o�[�Ɉړ�����
		float efect = 1.0f;
		float selectVolume = 10.0f;
		float volume = 0.05f;
		float volFadeSpeed = 0.03f;
		Mesh::StaticMeshList meshList;

		int Run();

		void SetBlurStrength(float _blurStrength)
		{
			blurStrength = _blurStrength;
		}

		GLFWwindow* GetWindow() { return window; }
		std::mt19937 & GetRandomGenerator() { return rg; }
		SpriteList & GetSpriteListBg(int num) { return spriteListBg[num]; }
		SpriteList & GetSpriteListObj() { return spriteListObj; }

		// �w�i�X�v���C�g�̔z�u���
		struct BgSize
		{
			size_t sizeX; // ���̃^�C����
			size_t sizeY; // �c�̃^�C����
			float baseX;  // �w�i�̔z�u�J�nX���W
			float baseY;  // �w�i�̔z�u�J�nY���W
		};

		BgSize GetBgSize(int bgSizeNum, int bgBaseNum) const 
		{
		  return { bgSizeX, bgSizeY[bgSizeNum], bgBaseX[bgBaseNum], bgBaseY[bgBaseNum] };  
		}

		// ���̃V�[����ݒ肷��
		template<typename T>
		void SetNextScene() { nextScene = std::make_shared<T>(); }

		/**
		* �Q�[���I�u�W�F�N�g���쐬����
		*/
		template<typename T>
		std::shared_ptr<T> Create(const std::string& name, float x = 0, float y = 0)
		{
			std::shared_ptr<T> p = std::make_shared<T>();
			p->engine = this;
			p->name = name;
			p->x = x;
			p->y = y;
			gameObjectList.push_back(p);
			return p;
		}
		//�Q�[���I�u�W�F�N�g���폜����
		void ClearGameObjectList() { gameObjectList.clear(); }

		//�X�v���C�g�`��͈�
		VecMath::vec3 GetViewSize() const { return { 17 * 32, 22 * 32, 300 }; }
		
		//�E�B���h�E�T�C�Y
		VecMath::vec2 GetWindowSize() const { return { 1280, 720 }; }

		
		// �L�[��������Ă�����true, ������ĂȂ����false��Ԃ�
		bool GetKey(int key) const 
		{
		  return glfwGetKey(window, key) == GLFW_PRESS;
		}

		// �z�u�f�[�^�̓ǂݍ��݁E�X�v���C�g�ݒ�
		TileMap LoadTileMap(const char* filename, int bgSizeNum);
		void SetSpriteList(const TileMap & tileMap, int bgBaseNum, float alpha = 1.0f, float color = 1.0f);

		// 3D���f���z�u�f�[�^�̓ǂݍ���
		GameObjectList LoadGameObjectMap(const char* filename,
		const VecMath::vec3 & translate = VecMath::vec3(0),
		const VecMath::vec3 & scale = VecMath::vec3(1));

		size_t AddUILayer(const char* image, GLenum filterMode, size_t reserve);
		void ClearUILayers();

		void BgScroll(int bgSIzeNum, int bgBaseNum, float deltaTime, float speed = 32.0f);
		void ClearSpriteList(int num);

		/**
		* UI�p�Q�[���I�u�W�F�N�g���쐬����
		* 
		* @param layerNo �ǉ���̃��C���[�ԍ�
		* @param name	 �Q�[���I�u�W�F�N�g�̖��O
		* @param X		 �Q�[���I�u�W�F�N�g��z�u����X���W
		* @param Y		 �Q�[���I�u�W�F�N�g��z�u����Y���W
		* 
		* @return �쐬����UI�p�Q�[���I�u�W�F�N�g
		*/
		template<typename T>
		std::shared_ptr<T> CreateUI(size_t layerNo, const std::string& name, float x = 0, float y = 0)
		{
			if (layerNo >= uiLayerList.size())
			{
				return nullptr;
			}
			std::shared_ptr<T> p = std::make_shared<T>();
			p->engine = this;
			p->name = name;
			p->x = x;
			p->y = y;
			uiLayerList[layerNo].gameObjectList.push_back(p);
			return p;
		}

		Mesh::StaticMeshPtr LoadOBJ(const char* filename);

		//���C���J�������擾
		GameObjectPtr GetMainCameraObject() const { return cameraObject; }
		std::shared_ptr<Camera> GetMainCamera() const
		{
			return std::static_pointer_cast<Camera>(cameraObject->componentList[0]);
		}

		// �u���[���̂������l�̎擾�E�ݒ�
		float GetBloomThreshold() const { return bloomThreshold; }
		void SetBloomThreshold(float t) { bloomThreshold = t; }
		
		// �u���[���̋����̎擾�E�ݒ�
		float GetBloomStrength() const { return bloomStrength; }
		void SetBloomStrength(float s) { bloomStrength = s; }

		GameObjectPtr FindGameObjectWithTag(std::string name);

		GameObjectList FindGameObjectsWithTag(std::string name);


	private:
		int Initialize();
		int MainLoop();
		int Finalize();
		
		void UpdateGameObject(float deltaTime);
		void CollideGameObject();
		void RemoveGameObject(GameObjectList& gameObjectList);
		void MakeSpriteList(GameObjectList& gameObjectList, SpriteList& spriteList);
		void DrawSpriteList(const SpriteList& spriteList, GLuint tex, size_t& spriteSsboOffset);
		void DrawStaticMesh(GameObjectList& gameObjectList);

		VecMath::mat4 CreateDepthMap(GameObjectList& gameObjectList,
			const VecMath::vec3 & eye, const VecMath::vec3& target,
			float range, float zNear, float zFar);
		void DrawBloomEffect();

		bool guiFlg = false;

		//�E�C���h�E
		GLFWwindow* window = nullptr;	//�E�B���h�E�I�u�W�F�N�g
		const std::string title = "WingHunter";	//�E�B���h�E�^�C�g��
		
		//�V�F�[�_
		GLuint vs = 0;	//���_�V�F�[�_�̊Ǘ��ԍ�
		GLuint fs = 0;	//�t���O�����g�V�F�[�_�[�̊Ǘ��ԍ�
		ProgramPipelinePtr progSprite; //�v���O�����I�u�W�F�N�g�̊Ǘ��ԍ�

		//���_�f�[�^
		GLuint vbo = 0;	//���_�o�b�t�@�̊Ǘ��ԍ�
		GLuint ibo = 0;	//�C���f�b�N�X�o�b�t�@�̊Ǘ��ԍ�
		GLuint vao = 0;	//�\�����̊Ǘ��ԍ�

		//�e�N�X�`��
		TexturePtr tex[3]; // �e�N�X�`���̊Ǘ��ԍ�

		//�X�v���C�g
		const size_t maxSpriteCount = 100'000;	//�X�v���C�g�̍ő吔
		std::vector<Sprite> spriteListBg[2];	//�w�i�p
		std::vector<Sprite> spriteListObj;	//�����A�G�A���̑�

		//SSBO
		size_t spriteSsboSize = 0;	//�X�v���C�g�pSSBO�̃o�C�g�T�C�Y
		GLuint ssboSprite = 0;		//�X�v���C�g�pSSBO�̊Ǘ��ԍ�
		size_t spriteSsboIndex = 0;		//�R�s�[��o�b�t�@�̃C���f�b�N�X
		uint8_t* pSpriteSsbo = nullptr;	//�X�v���C�g�f�[�^�̃R�s�[��A�h���X

		//�����I�u�W�F�N�g
		GLsync syncSpriteSsbo[2] = { 0,0 };	//�X�v���C�g�pSSBO�̓����I�u�W�F�N�g

		//����
		std::mt19937 rg;		//�^�������𐶐�����I�u�W�F�N�g(����)

		//�w�i�X�v���C�g
		const size_t bgSizeX = 17;	//���̃^�C����
		const size_t bgSizeY[2] = { 110, 220 }; //�c�̃^�C����

		float bgBaseX[2] = { 16, 16 };	//�w�i�̔z�u�J�nX���W
		float bgBaseY[2] = { 16, 16 }; //�w�i�̔z�u�J�nY���W

		//3D���f���p�̕ϐ�
		ProgramPipelinePtr progStandard3D;
		Mesh::MeshBufferPtr meshBuffer;
		Mesh::PrimitiveBufferPtr primitiveBuffer;
		GameObjectPtr cameraObject;

		//3D���f���̉e�`��p�̕ϐ�
		FramebufferObjectPtr fboShadow;
		ProgramPipelinePtr progShadow;

		FramebufferObjectPtr fboGameWindow;  //�Q�[���E�B���h�E�pFBO

		// �G�t�F�N�g�p�̕ϐ�
		ProgramPipelinePtr progGaussianBlur; // �ڂ����V�F�[�_
		float blurStrength = 1; // �ڂ������x(0.0�`1.0)

		ProgramPipelinePtr progHighPassFilter;
		ProgramPipelinePtr progDownSampling;
		ProgramPipelinePtr progUpSampling;
		FramebufferObjectPtr fboBloom[6];
		float bloomThreshold = 1; // ���P�x�Ƃ݂Ȃ����邳(0.0�`����Ȃ�)
		float bloomStrength = 8; // �u���[���̋���

		//�V�[��
		ScenePtr scene; //���s���̃V�[��
		ScenePtr nextScene; //���̃V�[��
		GameObjectList gameObjectList;	//�Q�[���I�u�W�F�N�g�z��

		//UI�p�Q�[���I�u�W�F�N�g�̃��C���[�\��
		struct UILayer
		{
			TexturePtr tex;			
			GameObjectList gameObjectList;
			std::vector<Sprite> spriteList;
		};
		std::vector<UILayer> uiLayerList;
};

#endif // ENGINE_H_INCLUDED