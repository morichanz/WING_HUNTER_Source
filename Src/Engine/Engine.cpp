/**
* @file Engine.cpp
*/
#define _CRT_SECURE_NO_WARNINGS
#include "Engine.h"
#include "../../Scene/MainGameScene.h"
#include"../../Scene/TitleScene.h"
#include"../../Scene/ClearScene.h"
#include "../ProgramPipeline/ProgramPipeline.h"
#include "../Texture/Texture.h"
#include "../Mesh/Mesh.h"
#include "../VertexArray.h"
#include "../Component/Camera.h"
#include "../Component/MeshRenderer.h"
#include "../Debug/Debug.h"
#include"../../Audio/EasyAudio.h"
#include<Windows.h>
#include<filesystem>
#include<fstream>
#include<iostream>
#include<vector>
//#include"../json/json.hpp"

using json = nlohmann::json;
const std::string pathToJSON("data/file.json");

#include "../../ImGui/ImGui/imgui.h"
#include "../../ImGui/ImGui/imgui_impl_opengl3.h"
#include "../../ImGui/ImGui/imgui_impl_glfw.h"

using namespace VecMath;

void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* userParam)
{
	std::string s;
	if (length < 0)
	{
		s = message;
	}
	else
	{
		s.assign(message, message + length);
	}
	s += '\n';
	OutputDebugString(s.c_str());
}

/**
* SSBOのサイズを計算する
*/
size_t CalcSsboSize(size_t n)
{
	const size_t alignment = 256;	//アライメント(データの境界)
	return ((n + alignment - 1) / alignment) * alignment;
};


/**
* 全てのUIレイヤーを削除する
*/
void Engine::ClearUILayers()
{
	uiLayerList.clear();
}

/**
* OBJファイルを読み込む
*
* @param filename 読み込むOBJファイル名
*
* @return 作成したメッシュのアドレス
*/
Mesh::StaticMeshPtr Engine::LoadOBJ(const char* filename)
{
	return meshBuffer->LoadOBJ(filename);
}

/**
* ゲームエンジンを実行する
*
* @retval 0     正常に実行が完了した
* @retval 0以外 エラーが発生した
*/

int Engine::Run()
{
	int ret = Initialize();
	if (ret == 0) {
		ret = MainLoop();
	}
	Finalize();
	return ret;
}

/**
* 配置データを読み込む
*/
TileMap Engine::LoadTileMap(const char* filename, int bgSizeNum)
{
	// ファイルを開く
	std::ifstream file;
	file.open(filename);
	if (!file) {
		return {}; // 失敗(ファイル名が違う？)

	}

	// 読み込み先となる変数を準備
	TileMap tileMap;
	tileMap.data.reserve(bgSizeX * bgSizeY[bgSizeNum]); // 適当な容量を予約

	// ファイルを読み込む
	while (!file.eof()) {
		// 1行読み込む
		std::string line;
		std::getline(file, line);
		if (line.size() <= 0) {
			continue; // データのない行は無視

		}
		line.push_back(','); // 終端を追加

		// カンマ区切り文字列を数値に変換
		tileMap.sizeX = 0;
		const char* start = line.data();
		const char* end = start + line.size();
		for (const char* p = start; p != end; ++p) {
			if (*p == ',') {
				const int n = atoi(start);
				tileMap.data.push_back(n);
				start = p + 1; // 次のカンマ区切りを指すように更新
				++tileMap.sizeX; // 横のタイル数を更新
			}
		}
		++tileMap.sizeY; // 縦のタイル数を更新
	}
	// 読み込んだ配置データを返す
	return tileMap;
}

/**
* 配置データに従って背景スプライトを設定する
*/
void Engine::SetSpriteList(const TileMap & tileMap, int bgBaseNum, float alpha, float color)
{
	// 背景スプライトを削除
	spriteListBg[bgBaseNum].clear();
	// 配置開始座標を初期化
	bgBaseX[bgBaseNum] = 16;
	bgBaseY[bgBaseNum] = 16;
	
	// 配置データを参照して背景スプライトを設定
	// 画像サイズ=512x512, タイルサイズ=32x32
	for (size_t y = tileMap.sizeY; y > 0;) 
	{
		--y; // マイナスにならないように、ここで減算

		for (size_t x = 0; x < tileMap.sizeX; ++x)
		{
		      // タイル番号を取得
			const int tileNo = tileMap.data[y * tileMap.sizeX + x];
		
			      // タイル番号に対応するテクスチャ座標を求める
			const float u = static_cast<float>(tileNo % 16) * 32;
			const float v = static_cast<float>(tileNo / 16) * 32;
		
			      // 背景スプライトを設定
			spriteListBg[bgBaseNum].push_back({
			static_cast<float>(x * 32 + bgBaseX[bgBaseNum]),
			static_cast<float>(y * 32 + bgBaseY[bgBaseNum]),
			0, alpha, 1,1,1,1,color,color,color,color,MakeTexcoord(u, v, 32, 32) });
		} // for x
	} // for y
}

/**
* 3Dモデル配置ファイルを読み込む
*
* @param filename  3Dモデル配置ファイル名
* @param translate 全ての3Dモデル座標に加算する平行移動量
* @param scale     全ての3Dモデルに掛ける拡大率
*
* @return ファイルから読み込んだゲームオブジェクトの配列
*/
GameObjectList Engine::LoadGameObjectMap(const char* filename,const vec3 & translate, const vec3 & scale)
{
	//ファイルを開く
	std::ifstream ifs(filename);
	if (!ifs)
	{
		LOG_WARNING("%sを開けません", filename);
		return {};
	}
	
	GameObjectList gameObjectList;
	gameObjectList.reserve(1000); // 適当な数を予約
	
	//ファイルを読み込む
	while (!ifs.eof())
	{
		std::string line;
		std::getline(ifs, line);
		const char* p = line.c_str();
		int readByte = 0;

		//データ行の開始判定
		char name[256];
		if (sscanf(p, R"( { "name" : "%255[^"]" %n)", name, &readByte) != 1)
		{
			continue; //データ行ではない
		}
		name[255] = '\0';
		p += readByte; //読み取り位置を更新

		//名前以外の要素を読み込む
		char baseMeshName[256];
		if (sscanf(p, R"(, "mesh" : "%255[^"]" %n)", baseMeshName, &readByte) != 1)
		{
			continue;
		}
		baseMeshName[255] = '\0';
		p += readByte;

		vec3 t(0);
		if (sscanf(p, R"(, "translate" : [ %f, %f, %f ] %n)",
			&t.x, &t.y, &t.z, &readByte) != 3)
		{
			continue;
		}
		p += readByte;

		vec3 r(0);
		if (sscanf(p, R"(, "rotate" : [ %f, %f, %f ] %n)",
			&r.x, &r.y, &r.z, &readByte) != 3)
		{
			continue;
		}
		p += readByte;

		vec3 s(1);
		if (sscanf(p, R"(, "scale" : [ %f, %f, %f ] %n)",
			&s.x, &s.y, &s.z, &readByte) != 3) {
			continue;
		}
		p += readByte;

		//メッシュを読み込む
		const std::string meshName = std::string("Res/3Dobj/Model/StageMap/") + baseMeshName + "/" + baseMeshName + ".obj";
		auto mesh = LoadOBJ(meshName.c_str());
		if (!mesh)
		{
			LOG_WARNING("メッシュファイル%sの読み込みに失敗", meshName.c_str());
			continue;
		}
		
		//座標の拡大縮小と平行移動
		t = t * scale + translate;
		s *= scale;
		
		//ゲームオブジェクトを作成
		auto gameObject = Create<GameObject>(meshName);
		gameObject->x = t.x;
		gameObject->y = t.y;
		gameObject->z = t.z;
		
		//Unityは左手座標系なので、回転方向を右手座標系に変換する
		gameObject->rotation[0] = radians(-r.x);
		gameObject->rotation[1] = radians(-r.y);
		gameObject->rotation[2] = radians(r.z);
		auto renderer = gameObject->AddComponent<MeshRenderer>();
		renderer->mesh = mesh;
		renderer->scale = s;
		
		//UnityからエクスポートしたOBJはX軸が反転しているので、Y軸で180度回転させる
		renderer->rotation.y = radians(180);
	
		//ゲームオブジェクトを追加
		gameObjectList.push_back(gameObject);
	}

	//作成したゲームオブジェクト配列を返す
	LOG("3Dモデル配置ファイル%sを読み込みました", filename);
	return gameObjectList;
}

/**
* UIレイヤーを追加する
* 
* @param image		レイヤーで使用するテクスチャ画像
* @param filterMode テクスチャに設定する拡大縮小フィルタ
* @param reserve	レイヤー用に予約するゲームオブジェクト数
* 
* @return 追加したレイヤーの番号
*/
size_t Engine::AddUILayer(const char* image, GLenum filterMode, size_t reserve)
{
	const TexturePtr tex = Texture::Create(image, filterMode);
	uiLayerList.push_back({ tex });
	uiLayerList.back().gameObjectList.reserve(reserve);
	uiLayerList.back().spriteList.reserve(reserve);
	return uiLayerList.size() - 1;
}

GameObjectPtr Engine::FindGameObjectWithTag(std::string name)
{
	for (auto itr : gameObjectList)
	{
		if (itr->name == name)
		{
			return itr->shared_from_this();
		}
	}
	return nullptr;
}

GameObjectList Engine::FindGameObjectsWithTag(std::string name)
{
	GameObjectList tmp;
	tmp.reserve(gameObjectList.size());
	for (auto itr : gameObjectList)
	{
		if (itr->name == name)
		{
			tmp.push_back(itr->shared_from_this());
		}
	}
	return tmp;
}


/**
* ゲームエンジンを初期化する
*
* @retval 0     正常に初期化された
* @retval 0以外 エラーが発生した
*/
int Engine::Initialize()
{
	//jsonファイル読み込み
	std::ifstream ifs(pathToJSON.c_str());
	if (ifs.good())
	{
		j;
		ifs >> j;
		ifs.close();
	}

	//GLFWの初期化
	if (glfwInit() != GLFW_TRUE)
	{
		return 1;
	}

	//描画ウインドウの作成
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	window = glfwCreateWindow(1280, 720, title.c_str(), nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return 1;
	}

	//GLFWimage (32,32);
	//glfwSetWindowIcon(window, 1, );

	//OpenGLコンテキストの作成
	glfwMakeContextCurrent(window);

	//OpenGL関数のアドレスを取得する
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		glfwTerminate();
		return 1;
	}

	//メッセージコールバックの設定
	glDebugMessageCallback(DebugCallback, nullptr);

	progSprite = ProgramPipeline::Create("Res/Shader/standard_2D.vert", "Res/Shader/standard_2D.frag");
	progStandard3D = ProgramPipeline::Create("Res/Shader/standard_3D.vert", "Res/Shader/standard_3D.frag");

	//影用オブジェクトを作成
	fboShadow = FramebufferObject::Create(1024, 1024, FboType::depth);
	progShadow = ProgramPipeline::Create("Res/Shader/shadow.vert", "Res/Shader/shadow.frag");

	//ゲームウィンドウ用FBOを作成
	const vec3 viewSize = GetViewSize();
	fboGameWindow = FramebufferObject::Create(static_cast<int>(viewSize.x), static_cast<int>(viewSize.y), FboType::colorDepth);
	
	//ぼかしシェーダを読み込む
	progGaussianBlur = ProgramPipeline::Create("Res/Shader/standard_2D.vert", "Res/Shader/gaussian_blur.frag");

	// ブルーム用シェーダを読み込む
	progHighPassFilter = ProgramPipeline::Create("Res/Shader/simple_2D.vert", "Res/Shader/high_pass_filter.frag");
	progDownSampling = ProgramPipeline::Create("Res/Shader/simple_2D.vert", "Res/Shader/down_sampling.frag");
	progUpSampling = ProgramPipeline::Create("Res/Shader/simple_2D.vert", "Res/Shader/up_sampling.frag");

	// 座標変換行列をユニフォーム変数にコピー
	// 使用する頂点座標が-0.5～0.5のサイズなので2倍する。
	// スプライトとFBOのテクスチャ座標は逆なのでY軸をマイナスにする。
	const mat4 matBloomM = {
		vec4(2, 0, 0, 0),
		vec4(0,-2, 0, 0),
		vec4(0, 0, 2, 0),
		vec4(0, 0, 0, 1),
	};

	const mat4 matBloomVP = mat4(1);
	const GLuint bloomProgramIdList[] = {*progHighPassFilter, *progDownSampling, *progUpSampling };
	for (auto prog : bloomProgramIdList) 
	{
		glProgramUniformMatrix4fv(prog, 0, 1, GL_FALSE, &matBloomM[0][0]);
		glProgramUniformMatrix4fv(prog, 1, 1, GL_FALSE, &matBloomVP[0][0]);
	}

	// メイン画面のブルームエフェクト用FBOを作成
	int bloomX = static_cast<int>(viewSize.x);
	int bloomY = static_cast<int>(viewSize.y);
	for (auto& e : fboBloom)
	{
		bloomX /= 2;
		bloomY /= 2;
		e = FramebufferObject::Create(bloomX, bloomY, FboType::color);
	}

	//頂点データをGPUメモリにコピー
	struct Vertex {
		float x, y, z;	//頂点座標
		float u, v;		//テクスチャ座標
	};

	float vData = 1;

	const Vertex vertexData[] = 
	{
		{-0.5,-0.5,0,    0,vData},
		{ 0.5,-0.5,0,vData,vData},
		{ 0.5, 0.5,0,vData,	   0},
		{-0.5, 0.5,0,    0,	   0},
	};

	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, sizeof(vertexData), vertexData, 0);

	//インデックスデータをGPUメモリにコピー
	const GLushort indexData[] = {
		0,1,2,2,3,0,
	};

	glCreateBuffers(1, &ibo);
	glNamedBufferStorage(ibo, sizeof(indexData), indexData, 0);

	//頂点データの構成情報を設定
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, u));

	//テクスチャを作成する
	const char* filenameImageList[] = { "Res/Images/bg3.tga","Res/Images/bg3.tga" ,"Res/Images/objects.tga"};
	for (size_t i = 0; i < std::size(tex); ++i)
	{
		tex[i] = Texture::Create(filenameImageList[i], GL_NEAREST);
	}


	//スプライトの配列を作成
	//std::vector<Sprite> spriteList(maxSpriteCount);
	spriteListBg[0].reserve(10'000);
	spriteListBg[1].reserve(10'000);
	spriteListObj.reserve(10'000);

	//スプライト用のSSBOを作成
	spriteSsboSize = CalcSsboSize(maxSpriteCount * sizeof(Sprite));
	glCreateBuffers(1, &ssboSprite);
	glNamedBufferStorage(ssboSprite,
		spriteSsboSize * 2,	//ダブルバッファのためにサイズを２倍にする
		nullptr,
		//コピー先のフラグを取得するためのフラグを指定
		GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_WRITE_BIT);

	//スプライトデータのコピー先アドレスを取得(マッピング)
	pSpriteSsbo = static_cast<uint8_t*>(glMapNamedBuffer(ssboSprite, GL_WRITE_ONLY));

	//3Dモデル用のバッファを作成
	primitiveBuffer = Mesh::PrimitiveBuffer::Create(sizeof(Mesh::Vertex) * 1'000'000, sizeof(uint16_t) * 3'000'000);
	meshBuffer = Mesh::MeshBuffer::Create(sizeof(Mesh::Vertex) * 1'000'000, sizeof(uint16_t) * 10'000'000);

	//メインカメラを作成
	cameraObject = Create<GameObject>("Main Camera");
	cameraObject->y = 15;
	cameraObject->rotation[0] = -60.0f * acos(-1.0f) / 180.0f; //カメラを斜め下に向ける
	cameraObject->AddComponent<Camera>();

	//乱数を初期化
	std::random_device rd;	//真の乱数を生成するオブジェクト(遅い)
	rg.seed(rd());			//疑似乱数を「真の乱数」で初期化

	//ゲームオブジェクト配列の容量を予約
	gameObjectList.reserve(1000);	

	//音声ライブラリを初期化
	if (!Audio::Initialize())
	{
		return 1;	//初期化失敗
	}

	// 最初のシーンを作成して初期化
	scene = std::make_shared<TitleScene>();
	if (scene)
	{
		scene->Initialize(*this);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");

	return 0; // 正常に初期化された
}

/**
* メインループを実行する
*
* @retval 0     正常に実行された
* @retval 0以外 エラーが発生した
*/
int Engine::MainLoop()
{
	//FPS計測用の変数
	double fpsTime = glfwGetTime();
	double fpsCount = 0;
	double prevTime = glfwGetTime();	//前回のループで計測した時間

	//メインループ
	while (!glfwWindowShouldClose(window))
	{
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();


		//FPSを計測
		const double curTime = glfwGetTime();
		const double diffTime = curTime - fpsTime;
		++fpsCount;

		//1秒ごとにFPSを表示
		if (diffTime >= 1)
		{
			const std::string fps = std::to_string(fpsCount / diffTime);
			const std::string newTitle = title + " - FPS:" + fps;
			glfwSetWindowTitle(window, newTitle.c_str());

			//計測用変数をリセット
			fpsTime = curTime;
			fpsCount = 0;
		}

		//経過時間(デルタタイム)を計算
		float deltaTime  = static_cast<float>(curTime - prevTime) * efect;
		prevTime = curTime;

		if (j["ImguiSetFlg"]["Engine"])
		{
			ImGui::Begin("TimeDebug");
			if (ImGui::CollapsingHeader("Time"))
			{
				ImGui::DragFloat("deltaTime", &deltaTime);
			}
			ImGui::End();
		}

		if (GetKey(GLFW_KEY_G) && GetKey(GLFW_KEY_U) && GetKey(GLFW_KEY_I) && !guiFlg)
		{
			if (j["ImguiSetFlg"]["TitleScene"] == false)
			{
				j["ImguiSetFlg"]["TitleScene"] = true;
				j["ImguiSetFlg"]["ConfigScene"] = true;
				j["ImguiSetFlg"]["MainGameScene"] = true;
				j["ImguiSetFlg"]["Engine"] = true;
			}
			else
			{
				j["ImguiSetFlg"]["TitleScene"] = false;
				j["ImguiSetFlg"]["ConfigScene"] = false;
				j["ImguiSetFlg"]["MainGameScene"] = false;
				j["ImguiSetFlg"]["Engine"] = false;
			}

			std::ofstream ofs(pathToJSON.c_str());
			if (ofs.good())
			{
				ofs << j.dump(3) << std::endl;
				ofs.close();
			}
			guiFlg = true;
		}
		else if(!GetKey(GLFW_KEY_G) && !GetKey(GLFW_KEY_U) && !GetKey(GLFW_KEY_I))
		{
			if (guiFlg)guiFlg = false;
		}


		// シーンの切り替え
		if (nextScene) 
		{
			if (scene) 
			{
				scene->Finalize(*this);
			}
			nextScene->Initialize(*this);
			scene = std::move(nextScene);
		}
	
		    // シーンを更新
		if (scene) 
		{
			scene->Update(*this, deltaTime);
		}

		//ゲームオブジェクトを更新
		UpdateGameObject(deltaTime);
		CollideGameObject();
		RemoveGameObject(gameObjectList);
		for (auto& layer : uiLayerList)
		{
			RemoveGameObject(layer.gameObjectList);
		}

		MakeSpriteList(gameObjectList, spriteListObj);
		for (auto& layer : uiLayerList)
		{
			MakeSpriteList(layer.gameObjectList, layer.spriteList);
		}

		BgScroll(0, 0, deltaTime);
		BgScroll(1, 1, deltaTime, 48);

		//コピー先バッファを切り替える
		spriteSsboIndex = (spriteSsboIndex + 1) % 2;

		//GPUがバッファを使い終わるのを待つ
		for (GLsync& sync = syncSpriteSsbo[spriteSsboIndex]; sync;)
		{
			const GLuint64 timeout = 16'700'000;	//16.7ミリ秒(約1/60秒)
			const GLenum status = glClientWaitSync(sync, 0, timeout);

			if (status != GL_TIMEOUT_EXPIRED)
			{
				//使い終わった同期オブジェクトは削除しなければならない
				glDeleteSync(sync);
				sync = 0;
				break;
			}
		}

		//スプライトデータをSSBOにコピー
		const std::vector<Sprite>* drawList[] = { &spriteListBg[0],&spriteListBg[1],&spriteListObj };
		uint8_t* p = pSpriteSsbo + spriteSsboIndex * spriteSsboSize;
		for (size_t i = 0; i < std::size(drawList); ++i)
		{
			memcpy(p, drawList[i]->data(), drawList[i]->size() * sizeof(Sprite));
			p += CalcSsboSize(drawList[i]->size() * sizeof(Sprite));
		}

		//ゲームウィンドウ用のスプライトデータをSSBOにコピー
		SpriteList spriteListPostProcess;
		if (!gameObjectList.empty()) 
		{
			//ポストプロセス用スプライトを追加
			const vec2 gameWindowPos = GetWindowSize() * 0.5f;
			spriteListPostProcess.push_back(Sprite{ gameWindowPos.x, gameWindowPos.y, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, Texcoord{ 0, 1, 1, -1 } });
			
			const size_t size = spriteListPostProcess.size() * sizeof(Sprite);
			memcpy(p, spriteListPostProcess.data(), size);
			p += CalcSsboSize(size);
		}

		//UIプレイヤーのスプライトデータをSSBOにコピー
		for (const auto& layer : uiLayerList)
		{
			memcpy(p, layer.spriteList.data(), layer.spriteList.size() * sizeof(Sprite));
			p += CalcSsboSize(layer.spriteList.size() * sizeof(Sprite));
		}

		//描画先をゲームウィンドウに変更
		glBindFramebuffer(GL_FRAMEBUFFER, *fboGameWindow);
		const vec3 viewSize = GetViewSize();
		glViewport(0, 0, static_cast<int>(viewSize.x), static_cast<int>(viewSize.y));
		glBindVertexArray(vao);

		//バックバッファをクリア
		glClearColor(0.5f, 1.0f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//描画に使うシェーダーを指定
		glUseProgram(*progSprite);
		glDisable(GL_DEPTH_TEST); //深度テストを無効化
		glDisable(GL_CULL_FACE);  // 裏面カリングを無効化

		//アルファブレンドを有効にする
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if 0
		//ユニフォーム変数にデータをコピーする
		const float timer = static_cast<float>(glfwGetTime());
		glProgramUniform1f(progSprite, 0, timer);
#endif

		//ユニフォーム変数に座標変換用パラメータをコピーする
		//int w, h;
		//glfwGetWindowSize(window, &w, &h);
		glProgramUniform4f(*progSprite, 2, 2.0f / viewSize.x, 2.0f / viewSize.y, -1, -1);

		//スプライト描画用のVAOを設定
		glBindVertexArray(vao);

		//2D背景を描画
		size_t spriteSsboOffset = spriteSsboIndex * spriteSsboSize;
		DrawSpriteList(*drawList[0], *tex[0], spriteSsboOffset);
		DrawSpriteList(*drawList[1], *tex[1], spriteSsboOffset);

		//3Dメッシュを描画
		DrawStaticMesh(gameObjectList);
		
		//2Dオブジェクトを描画
		glBindVertexArray(vao);    //スプライト描画用VAOを設定
		glUseProgram(*progSprite); //描画に使うシェーダを指定
		glDisable(GL_DEPTH_TEST);  //深度テストを無効化
		glDisable(GL_CULL_FACE);   //裏面カリングを無効化
		DrawSpriteList(*drawList[2], *tex[2], spriteSsboOffset);

		
		// ブルームを描画
		DrawBloomEffect();

		//デフォルトフレームバッファに戻す
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 1280, 720);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		//ゲームウィンドウを描画
		glUseProgram(*progSprite);
		glProgramUniform4f(*progSprite, 2, 2.0f / 1280, 2.0f / 720, -1, -1);
		if (!gameObjectList.empty()) 
		{
			//ぼかし強度が0より大きければぼかしシェーダを使う
			if (blurStrength > 0) 
			{
				glUseProgram(*progGaussianBlur);
				const vec2 windowSize = GetWindowSize();
				glProgramUniform4f(*progGaussianBlur, 2, 2.0f / windowSize.x, 2.0f / windowSize.y, -1, -1);
				glProgramUniform1f(*progGaussianBlur, 100, blurStrength);
				DrawSpriteList(spriteListPostProcess, *fboGameWindow->GetColorTexture(), spriteSsboOffset);
				glUseProgram(*progSprite);
			}
			else 
			{
				DrawSpriteList(spriteListPostProcess,*fboGameWindow->GetColorTexture(), spriteSsboOffset);
			} //blurStrength
		}
		
		//UIレイヤーのスプライト配列を描画
		glProgramUniform4f(*progSprite, 2, 2.0f / 1280, 2.0f / 720, -1, -1);
		for (const auto& layer : uiLayerList)
		{
			DrawSpriteList(layer.spriteList, *layer.tex, spriteSsboOffset);
		}
		
		//スプライト用SSBOの割り当てを解除する
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 0, 0, 0);

		//スプライト描画の直後にフェンスを作成
		syncSpriteSsbo[spriteSsboIndex] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

		//音声ライブラリを更新
		Audio::Update();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	return 0; //正常に実行された
}
/**
* ゲームエンジンを終了する
*
* @retval 0     正常に終了した
* @retval 0以外 エラーが発生した
*/
int Engine::Finalize()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//音声ライブラリを終了
	Audio::Finalize();

	//GLFWの終了
	glfwTerminate();
	return 0;
}

/**
* ゲームオブジェクトの状態を更新
*/
void Engine::UpdateGameObject(float deltaTime)
{
	//処理中に変化しないようにコピーを作成
	std::vector<GameObject*> list(gameObjectList.size());
	std::transform(gameObjectList.begin(), gameObjectList.end(), list.begin(), 
		[](const GameObjectPtr& e) {return e.get(); });

	//UIレイヤーのゲームオブジェクトのコピーを作成
	for (auto& layer : uiLayerList)
	{
		const size_t prevListSize = list.size();
		list.resize(prevListSize + layer.gameObjectList.size());
		std::transform(layer.gameObjectList.begin(), layer.gameObjectList.end(),
			list.begin() + prevListSize, [](const GameObjectPtr& e) {return e.get(); });
	}

	//カメラオブジェクトを追加
	if (cameraObject)
	{
		list.push_back(cameraObject.get());
	}

	//Startイベント
	for (auto& e : list)
	{
		e->Start();
	}

	//Updateイベント
	for (auto& e : list)
	{
		if (!e->isDead)
		{
			e->Update(deltaTime);
		}
	}//for list
}

/**
* ゲームオブジェクトの衝突判定
*/
void Engine::CollideGameObject()
{
	struct WorldCollider
	{
		GameObject* gameObject;
		Rect aabb;
		std::vector<Rect> collision;
	};

	//ワールド座標のコライダーを計算
	std::vector<WorldCollider> tmp;
	tmp.reserve(gameObjectList.size());
	for (auto& e : gameObjectList)
	{
		//スタートしていないか死んでいるゲームオブジェクトは衝突しない
		if (!e->IsStarted() || e->isDead)
		{
			continue;
		}

		//ワールド座標　＝　ローカル座標　＋　ゲームオブジェクトの座標
		const RectList& local = e->GetCollision();
		WorldCollider world;
		world.gameObject = e.get();
		world.aabb = Add(e->GetAabb(), e->x, e->y);
		world.collision.resize(local.size());
		for (size_t i = 0; i < local.size(); i++)
		{
			world.collision[i] = Add(local[i], e->x, e->y);
		}
		tmp.push_back(world);
	}

	//コライダーの衝突判定
	for (auto pa = tmp.begin(); pa != tmp.end(); ++pa)
	{
		for (auto pb = pa + 1; pb != tmp.end(); ++pb)
		{
			if (pb->gameObject->isDead)
			{
				continue;	//pbが死んでいるなら衝突しない
			}

			if (!Overlap(pa->aabb, pb->aabb))
			{
				continue;	//衝突していない
			}

			if (!Overlap(pa->collision, pb->collision))
			{
				continue;	//衝突していない
			}

			//衝突しているので、衝突イベント関数を実行
			pa->gameObject->OnCollision(*pb->gameObject);
			pb->gameObject->OnCollision(*pa->gameObject);

			if (pa->gameObject->isDead)
			{
				break;	//paが死んでいるならループを打ち切る
			}//for pb
		}//for pa
	}
}

/**
* 死亡状態のゲームオブジェクトを削除
*/
void Engine::RemoveGameObject(GameObjectList& gameObjectList)
{
	//生きているオブジェクトと死んでいるオブジェクトに分ける
	const auto i = std::stable_partition(gameObjectList.begin(),
		gameObjectList.end(), [](const GameObjectPtr& e) {return !e->isDead; });
	
	//死んでいるオブジェクトを別の配列に移動
	GameObjectList deadList(std::make_move_iterator(i), std::make_move_iterator(gameObjectList.end()));

	//配列から移動済みオブジェクトを削除
	gameObjectList.erase(i, gameObjectList.end());

	//死んでいるオブジェクトに対してDestoryイベントを実行
	for (auto& e : deadList)
	{
		e->OnDestroy();
	}
	//ここで死んでいるオブジェクトが削除される(tmpの寿命が終わるため)
}

/**
* ゲームオブジェクトからスプライト配列を作成
*/
void Engine::MakeSpriteList(GameObjectList& gameObjectList, SpriteList& spriteList)
{
	//既存の動く物体用のスプライト配列を空にする
	spriteList.clear();

	//優先順位が大きい順番に並び変える
	std::stable_sort(gameObjectList.begin(), gameObjectList.end(), [](const GameObjectPtr& a, const GameObjectPtr& b)
	{
		return a->priority > b->priority;
	});

	//ゲームオブジェクトのスプライトデータをスプライト配列に追加
	//同時にワールド座標へ変換する
	for (const auto& e : gameObjectList)
	{
		for (const auto& sprite : e->spriteList)
		{
			spriteList.push_back(sprite);
			Sprite& s = spriteList.back();
			s.x += e->x;
			s.y += e->y;
			s.z += e->rotate;
			s.w = e->alpha;
		}
	}//for gameObjectList
}

/**
* スプライト配列を描画する
* @param spriteList			描画するスプライト配列
* @param tex				描画に使用するテクスチャ
* @param spriteSsboOffset	SSBO内のスプライトデータの開始位置
*/
void Engine::DrawSpriteList(const SpriteList& spriteList, GLuint tex, size_t& spriteSsboOffset)
{
	//描画データがなければスキップ
	const size_t size = CalcSsboSize(spriteList.size() * sizeof(Sprite));
	if (size == 0)
	{
		return;
	}

	//テクスチャを指定する
	glBindTextures(0, 1, &tex);

	//スプライト用SSBOを割り当てる
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, ssboSprite, spriteSsboOffset, static_cast<GLintptr>(size));
	spriteSsboOffset += size;

	//図形を描画
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, static_cast<GLsizei>(spriteList.size()));
}

/**
* 3Dメッシュを描画する
*
* @param gameObjectList 描画するゲームオブジェクトの配列
*/
void Engine::DrawStaticMesh(GameObjectList& gameObjectList)
{
	//平行光源
	struct DirectionalLight
	{
		vec3 color;
		float intensity;
		vec3 direction;
	};
	DirectionalLight directionalLight = {vec3(1, 0.9f, 0.8f),4, vec3(0.57735f, -0.57735f, -0.57735f)};

	//深度マップ作成用の注視点座標
	const vec3 viewSize = GetViewSize();
	const vec3 shadowTarget =
	vec3(viewSize.x * 0.5f, viewSize.y * 0.5f, 0);
	
	//深度マップ作成用の視点座標
	const vec3 shadowEye = shadowTarget - directionalLight.direction * 1000;
	
	//深度マップを作成
	const mat4 matShadow = CreateDepthMap(gameObjectList, shadowEye, shadowTarget, 750, 100, 2000);

	//ゲームウィンドウ用FBOに描画
	glBindFramebuffer(GL_FRAMEBUFFER, *fboGameWindow);
	glUseProgram(*progStandard3D);
	glBindVertexArray(*meshBuffer->GetVAO());

	glEnable(GL_DEPTH_TEST); // 深度テストを有効化
	glEnable(GL_CULL_FACE);  // 裏面カリングを有効化
	glEnable(GL_BLEND);      // アルファブレンドを有効化
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//深度マップをテクスチャとして割り当てる
	const GLuint texDepth = *fboShadow->GetDepthTexture();
	glBindTextures(1, 1, &texDepth);
	
	//影座標からテクスチャ座標に変換する行列
	const mat4 matTexture = {
		vec4(0.5f, 0.0f, 0.0f, 0.0f),
		vec4(0.0f, 0.5f, 0.0f, 0.0f),
		vec4(0.0f, 0.0f, 0.5f, 0.0f),
		vec4(0.5f, 0.5f, 0.5f, 1.0f),
	};

	const mat4 matShadowTexture = matTexture * matShadow;
	glProgramUniformMatrix4fv(*progStandard3D, 1, 1, GL_FALSE, &matShadowTexture[0][0]);

	//平行光源の情報を設定
	const vec3 color = directionalLight.color * directionalLight.intensity;
	glProgramUniform3f(*progStandard3D, 100, color.x, color.y, color.z);
	glProgramUniform3f(*progStandard3D, 101,
		directionalLight.direction.x,
		directionalLight.direction.y,
		directionalLight.direction.z);

	//環境光の情報を設定
	const vec3 ambientLight = vec3(0.2f, 0.2f, 0.3f);
	glProgramUniform3f(*progStandard3D, 104,ambientLight.x, ambientLight.y, ambientLight.z);

	//3D表示用のカメラ情報を設定
	Camera& camera = static_cast<Camera&>(*cameraObject->componentList[0]);
	glProgramUniform4f(*progStandard3D, 3, camera.fovScaleX, camera.fovScaleY, camera.A, camera.B);
	glProgramUniform3f(*progStandard3D, 4, cameraObject->x, cameraObject->y, cameraObject->z);
	glProgramUniform2f(*progStandard3D, 5, sin(cameraObject->rotation[0]), cos(cameraObject->rotation[0]));

	//ビューポートを設定
	glViewport(camera.viewport.x, camera.viewport.y, camera.viewport.width, camera.viewport.height);

	//メッシュレンダラを持つ全てのゲームオブジェクトを描画
	for (const auto& gameObject : gameObjectList) 
	{
		gameObject->DrawStaticMesh(*progStandard3D);
	}

	glBindTextures(0, 2, nullptr);
	glBindVertexArray(0);
	glUseProgram(0);
}

/**
* 深度マップを作成する
*
* @param gameObjectList 描画するゲームオブジェクトの配列
* @param eye    影描画用の視点座標
* @param target 影描画用の注視点の座標
* @param range  eyeから影描画範囲の上端、下端、左端、右端までの距離
* @param zNear  eyeから影描画範囲の手前端までの距離
* @param zFar   eyeから影描画範囲の奥端までの距離
*
* @return 影描画用ビュープロジェクション行列
*/
mat4 Engine::CreateDepthMap(GameObjectList & gameObjectList,const vec3 & eye, const vec3 & target, float range, float zNear, float zFar)
{
	//描画先を影用FBOに変更
	glBindFramebuffer(GL_FRAMEBUFFER, *fboShadow);
	glClear(GL_DEPTH_BUFFER_BIT);

	//ビューポートをFBOのサイズに合わせる
	const GLuint texDepth = *fboShadow->GetDepthTexture();
	GLint w, h;
	glGetTextureLevelParameteriv(texDepth, 0, GL_TEXTURE_WIDTH, &w);
	glGetTextureLevelParameteriv(texDepth, 0, GL_TEXTURE_HEIGHT, &h);
	glViewport(0, 0, w, h);

	//描画パラメータを設定
	glBindVertexArray(*meshBuffer->GetVAO()); //3Dモデル用VAOをバインド
	glUseProgram(*progShadow); //影描画用シェーダを指定
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND); //アルファブレンドを無効化

	//影用ビュー行列を作成
	const mat4 matShadowView = mat4::LookAt(eye, target, vec3(0, 1, 0));
	
	//影用プロジェクション行列を作成
	const mat4 matShadowProj = mat4::Orthogonal(-range, range, -range, range, zNear, zFar);
	
	//影用ビュープロジェクション行列を作成
	const mat4 matShadow = matShadowProj * matShadowView;
	
	//matShadowをユニフォーム変数にコピー
	glProgramUniformMatrix4fv(*progShadow, 1, 1, GL_FALSE, &matShadow[0][0]);

	//メッシュレンダラを持つ全てのゲームオブジェクトを描画
	for (const auto& gameObject : gameObjectList) 
	{
		gameObject->DrawStaticMesh(*progShadow);
	}
	
	return matShadow;
}

/**
* カラーテクスチャをFBOに描画する
*
* @param fbo      描画先FBO
* @param texture  FBOに描画するテクスチャ
* @param wrapMode テクスチャに設定するラップモード
*/
void DrawTextureToFbo(FramebufferObject& fbo,Texture& texture, GLenum wrapMode)
{
	const Texture& texFbo = *fbo.GetColorTexture();
	glViewport(0, 0, texFbo.GetWidth(), texFbo.GetHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	texture.SetWrapMode(wrapMode);
	glBindTextureUnit(0, texture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

/**
* ブルームを描画
*/
void Engine::DrawBloomEffect()
 {
	// ブルームエフェクト用にGLコンテキストを設定
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	// 1. ゲーム画面用FBOから高輝度成分を抽出
	glUseProgram(*progHighPassFilter);
	glDisable(GL_BLEND); // 合成なし
	{
		// 高輝度成分はFBOの枚数だけ加算されるので、その影響を打ち消すために枚数で割る
		const float strength = bloomStrength / static_cast<float>(std::size(fboBloom));
		glProgramUniform2f(*progHighPassFilter, 100, bloomThreshold, strength);
		
		DrawTextureToFbo(*fboBloom[0],*fboGameWindow->GetColorTexture(), GL_CLAMP_TO_BORDER);
	}
	// 2. 縮小ぼかしを行い、1段小さいFBOにコピー
	glUseProgram(*progDownSampling);
	for (size_t i = 1; i < std::size(fboBloom); ++i) 
	{
		DrawTextureToFbo(*fboBloom[i],*fboBloom[i - 1]->GetColorTexture(), GL_CLAMP_TO_BORDER);
	}
	// 3. 拡大ぼかしを行い、1段大きいFBOに加算合成
	glUseProgram(*progUpSampling);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE); // 加算合成
	for (size_t i = std::size(fboBloom) - 1; i > 0;)
	{
		--i;
		DrawTextureToFbo(*fboBloom[i], *fboBloom[i + 1]->GetColorTexture(), GL_CLAMP_TO_EDGE);
	}
	// 4. 最後の拡大ぼかしを行い、ゲーム画面用FBOに加算合成
	{
		DrawTextureToFbo(*fboGameWindow, *fboBloom[0]->GetColorTexture(), GL_CLAMP_TO_EDGE);
	}
}

void Engine::BgScroll(int bgSizeNum, int bgBaseNum, float deltaTime, float speed)
{
	//背景をスクロールさせる
	const int viewSizeY = 22 * 32;	//描画範囲の高さ
	const float scrollSpeed = speed;	//スクロール速度(ピクセル毎秒)
	if (spriteListBg[bgBaseNum].size() >= bgSizeX * bgSizeY[bgSizeNum])
	{
		const float bgBaseYEnd = -static_cast<float>(bgSizeY[bgSizeNum] * 32) + viewSizeY + 16;	//スクロール終端座標
		bgBaseY[bgBaseNum] -= scrollSpeed * deltaTime;
		if (bgBaseY[bgBaseNum] < bgBaseYEnd)
		{
			bgBaseY[bgBaseNum] = bgBaseYEnd;	//終端座標でスクロールを止める
		}
		for (size_t y = 0; y < bgSizeY[bgSizeNum]; ++y)
		{
			const float sy = y * 32 + bgBaseY[bgBaseNum];
			for (size_t x = 0; x < bgSizeX; ++x)
			{
				spriteListBg[bgBaseNum][y * bgSizeX + x].y = sy;
			}
		}
	}//if size
}

void Engine::ClearSpriteList(int num)
{
	spriteListBg[num].clear();
}