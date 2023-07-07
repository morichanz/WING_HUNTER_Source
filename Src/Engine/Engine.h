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

//先行宣言
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
* ゲームエンジン
*/
class Engine
 {
	public:
		json j;
		int stageCnt = 1;	//ステージ切り替えカウント
		bool debugMap = true;//ボスデバッグ用ステージ
		bool invincible = false;//無敵状態
		float interval = 0;	//シーン切り替えインターバル
		int playerHp = 0;	//プレイヤーの現在のHP
		int playerLife = 0;	//プレイヤーの残機
		int score = 0;		//現在のスコア
		int scoreBack = 0;  //記録用スコア
		bool fadeFlg = false;//フェードフラグ
		int fadeNum = 0;	//1ならタイトル2ならゲームオーバーに移動する
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

		// 背景スプライトの配置情報
		struct BgSize
		{
			size_t sizeX; // 横のタイル数
			size_t sizeY; // 縦のタイル数
			float baseX;  // 背景の配置開始X座標
			float baseY;  // 背景の配置開始Y座標
		};

		BgSize GetBgSize(int bgSizeNum, int bgBaseNum) const 
		{
		  return { bgSizeX, bgSizeY[bgSizeNum], bgBaseX[bgBaseNum], bgBaseY[bgBaseNum] };  
		}

		// 次のシーンを設定する
		template<typename T>
		void SetNextScene() { nextScene = std::make_shared<T>(); }

		/**
		* ゲームオブジェクトを作成する
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
		//ゲームオブジェクトを削除する
		void ClearGameObjectList() { gameObjectList.clear(); }

		//スプライト描画範囲
		VecMath::vec3 GetViewSize() const { return { 17 * 32, 22 * 32, 300 }; }
		
		//ウィンドウサイズ
		VecMath::vec2 GetWindowSize() const { return { 1280, 720 }; }

		
		// キーが押されていたらtrue, 押されてなければfalseを返す
		bool GetKey(int key) const 
		{
		  return glfwGetKey(window, key) == GLFW_PRESS;
		}

		// 配置データの読み込み・スプライト設定
		TileMap LoadTileMap(const char* filename, int bgSizeNum);
		void SetSpriteList(const TileMap & tileMap, int bgBaseNum, float alpha = 1.0f, float color = 1.0f);

		// 3Dモデル配置データの読み込み
		GameObjectList LoadGameObjectMap(const char* filename,
		const VecMath::vec3 & translate = VecMath::vec3(0),
		const VecMath::vec3 & scale = VecMath::vec3(1));

		size_t AddUILayer(const char* image, GLenum filterMode, size_t reserve);
		void ClearUILayers();

		void BgScroll(int bgSIzeNum, int bgBaseNum, float deltaTime, float speed = 32.0f);
		void ClearSpriteList(int num);

		/**
		* UI用ゲームオブジェクトを作成する
		* 
		* @param layerNo 追加先のレイヤー番号
		* @param name	 ゲームオブジェクトの名前
		* @param X		 ゲームオブジェクトを配置するX座標
		* @param Y		 ゲームオブジェクトを配置するY座標
		* 
		* @return 作成したUI用ゲームオブジェクト
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

		//メインカメラを取得
		GameObjectPtr GetMainCameraObject() const { return cameraObject; }
		std::shared_ptr<Camera> GetMainCamera() const
		{
			return std::static_pointer_cast<Camera>(cameraObject->componentList[0]);
		}

		// ブルームのしきい値の取得・設定
		float GetBloomThreshold() const { return bloomThreshold; }
		void SetBloomThreshold(float t) { bloomThreshold = t; }
		
		// ブルームの強さの取得・設定
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

		//ウインドウ
		GLFWwindow* window = nullptr;	//ウィンドウオブジェクト
		const std::string title = "WingHunter";	//ウィンドウタイトル
		
		//シェーダ
		GLuint vs = 0;	//頂点シェーダの管理番号
		GLuint fs = 0;	//フラグメントシェーダーの管理番号
		ProgramPipelinePtr progSprite; //プログラムオブジェクトの管理番号

		//頂点データ
		GLuint vbo = 0;	//頂点バッファの管理番号
		GLuint ibo = 0;	//インデックスバッファの管理番号
		GLuint vao = 0;	//構成情報の管理番号

		//テクスチャ
		TexturePtr tex[3]; // テクスチャの管理番号

		//スプライト
		const size_t maxSpriteCount = 100'000;	//スプライトの最大数
		std::vector<Sprite> spriteListBg[2];	//背景用
		std::vector<Sprite> spriteListObj;	//味方、敵、その他

		//SSBO
		size_t spriteSsboSize = 0;	//スプライト用SSBOのバイトサイズ
		GLuint ssboSprite = 0;		//スプライト用SSBOの管理番号
		size_t spriteSsboIndex = 0;		//コピー先バッファのインデックス
		uint8_t* pSpriteSsbo = nullptr;	//スプライトデータのコピー先アドレス

		//同期オブジェクト
		GLsync syncSpriteSsbo[2] = { 0,0 };	//スプライト用SSBOの同期オブジェクト

		//乱数
		std::mt19937 rg;		//疑似乱数を生成するオブジェクト(速い)

		//背景スプライト
		const size_t bgSizeX = 17;	//横のタイル数
		const size_t bgSizeY[2] = { 110, 220 }; //縦のタイル数

		float bgBaseX[2] = { 16, 16 };	//背景の配置開始X座標
		float bgBaseY[2] = { 16, 16 }; //背景の配置開始Y座標

		//3Dモデル用の変数
		ProgramPipelinePtr progStandard3D;
		Mesh::MeshBufferPtr meshBuffer;
		Mesh::PrimitiveBufferPtr primitiveBuffer;
		GameObjectPtr cameraObject;

		//3Dモデルの影描画用の変数
		FramebufferObjectPtr fboShadow;
		ProgramPipelinePtr progShadow;

		FramebufferObjectPtr fboGameWindow;  //ゲームウィンドウ用FBO

		// エフェクト用の変数
		ProgramPipelinePtr progGaussianBlur; // ぼかしシェーダ
		float blurStrength = 1; // ぼかし強度(0.0～1.0)

		ProgramPipelinePtr progHighPassFilter;
		ProgramPipelinePtr progDownSampling;
		ProgramPipelinePtr progUpSampling;
		FramebufferObjectPtr fboBloom[6];
		float bloomThreshold = 1; // 高輝度とみなす明るさ(0.0～上限なし)
		float bloomStrength = 8; // ブルームの強さ

		//シーン
		ScenePtr scene; //実行中のシーン
		ScenePtr nextScene; //次のシーン
		GameObjectList gameObjectList;	//ゲームオブジェクト配列

		//UI用ゲームオブジェクトのレイヤー構造
		struct UILayer
		{
			TexturePtr tex;			
			GameObjectList gameObjectList;
			std::vector<Sprite> spriteList;
		};
		std::vector<UILayer> uiLayerList;
};

#endif // ENGINE_H_INCLUDED