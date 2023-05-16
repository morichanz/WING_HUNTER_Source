/**
* @file LoadScene.cpp
*/
#include "LoadScene.h"
#include"MainGameScene.h"
#include"../Component/Text.h"
#include"../GameObject/GameObject.h"
#include"../Engine/Engine.h"
#include"../../Audio/EasyAudio.h"
#include"../../Audio/AudioSetting.h"
#include"ClearScene.h"
#include"Fade.h"
#include"../Component/MeshRenderer.h"
#include"../Component/Bullet/BulletMove.h"

void SetStatus(json j, Engine& engine);


const char* data[]
{
	"Res/3Dobj/Model/Large_Enemy/LargeEnemy.obj",	//0.大型機
	"Res/3Dobj/enemy_s_01.obj",						//1.小型機
	"Res/3Dobj/Model/Boss/Boss1/Boss1.obj",			//2	
	"Res/3Dobj/Model/Middle_Enemy/MiddleEnemy.obj", //3.中型機
	"Res/3Dobj/Model/Boss/Boss1/Boss1.obj",			//4.ボス１
	"Res/3Dobj/Model/Boss/Boss2/Boss2.obj",			//5.ボス2
	"Res/3Dobj/Model/Boss/Boss4/Boss4.obj",			//6.ボス3
	"Res/3Dobj/Model/Boss/Boss3/Boss3.obj",			//7.ボス4
	"Res/3Dobj/Model/Boss/Boss5/Boss5.obj",			//8.ボス5
	"Res/3Dobj/Model/Boss/Boss6/Boss6.obj",			//9.ボス6
	"Res/3Dobj/Model/Boss/Weapon/Weapon.obj",		//10.武器（ファンネル）
	"Res/3Dobj/Model/Boss/Weapon/SingleWeapon/SingleWeapon1.obj"	//11.武器
};

/**
* ロード画面を初期化する
*
* @retval true	初期化成功
* @retval false 初期化失敗
*/
bool LoadScene::Initialize(Engine& engine)
{
	//ゲームオブジェクトを削除
	engine.ClearGameObjectList();
	engine.ClearUILayers();

	//UIレイヤーを作成
	const size_t bgLayer = engine.AddUILayer("Res/Images/GameOver.tga", GL_LINEAR, 10);
	const size_t textLayer = engine.AddUILayer("Res/Images/Font/FontList/font_05_2.tga", GL_LINEAR, 10);
	const size_t dialLayer = engine.AddUILayer("Res/Images/LoadAnim/Dial2.tga", GL_LINEAR, 10);
	const size_t hour_handLayer = engine.AddUILayer("Res/Images/LoadAnim/Hour_Hand.tga", GL_LINEAR, 10);
	const size_t long_handLayer = engine.AddUILayer("Res/Images/LoadAnim/Long_Hand.tga", GL_LINEAR, 10);

	//背景画像
	auto uiBackGround = engine.CreateUI<GameObject>(bgLayer, "Loading_bg", 640, 360);
	uiBackGround->AddSprite({ 0,0,1,1 });

	//ゲームの開始方法を示すテキスト
	const char strNowLoading[] = "now Loading";
	const float fontSizeX = 16;
	const float x = 740 - static_cast<float>(std::size(strNowLoading) - 1) * fontSizeX;
	auto uiNowLoading = engine.CreateUI<GameObject>(textLayer, "now Loading", x, 50);
	auto textNowLoading = uiNowLoading->AddComponent<Text>();
	textNowLoading->SetText(strNowLoading, 2);

	Clock(engine, dialLayer, hour_handLayer, long_handLayer);

	//フェード画像
	const size_t alphaLayer = engine.AddUILayer("Res/Images/Fade_bg.tga", GL_LINEAR, 10);
	fade = engine.CreateUI<GameObject>(alphaLayer, "Fade_bg", 640, 360);
	fade->AddSprite({ 0,0,1,1 });
	fade->alpha = 1;

	//タイトルBGMの再生
	Audio::Stop(AudioPlayer::bgm);

	SetStatus(engine.j,engine);
	return true;	//初期化成功
}

/**
* ロード画面の状態を更新する
*/
void LoadScene::Update(Engine& engine, float deltaTime)
{
	FadeOut(engine, deltaTime);

	//ロードが終わればゲーム開始
	if (roadFlg)
	{
		engine.score = engine.scoreBack;
		fadeFlg = true;
	}

	if (!roadFlg && fadeInFlg)
	{
		//メッシュリストの要素数が読み込んだ読み込み済みの要素数と同じなら
		if (engine.meshList.size() == listSize)
		{
			//エンジンのメッシュリストにオブジェを読み込む
			engine.meshList.push_back(engine.LoadOBJ(data[engine.meshList.size()]));

			//それぞれの針の移動領域を計算する
			long_hand_Back = (engine.meshList.size()+ 1) * (360.0f / std::size(data));
			long_hand_Back_back = long_hand_Back * -1;
			hour_hand_Back -= (30.0f / std::size(data));
		}
		else
		{
			//deltaTimeが一定以下なら針の状態を更新する
			if (deltaTime < 0.2f)
			{
				if (hour_hand_Back < hour_hand->rotate)hour_hand->rotate -= 5.0f * deltaTime;
				if (long_hand_Back_back < long_hand->rotate && long_hand->rotate > -360.0f)long_hand->rotate -= 30.0f * deltaTime;

				//針の移動の更新が終わったらリストの要素数を更新する
				if (long_hand_Back_back >= long_hand->rotate && hour_hand_Back >= hour_hand->rotate)listSize = engine.meshList.size();
			}
		}
		if (std::size(data) == engine.meshList.size())roadFlg = true;
	}
}

void LoadScene::FadeOut(Engine& engine, float deltaTime)
{
	if (!fadeFlg)
	{
		if (fade->alpha > 0)fade->alpha -= deltaTime * 2;
		else fadeInFlg = true;
	}
	else
	{
		fade->alpha += deltaTime / 2;
		if (fade->alpha > 1)engine.SetNextScene<MainGameScene>();
	}
}

void LoadScene::Clock(Engine& engine, size_t dialLayer, size_t hour_handLayer, size_t long_handLayer)
{
	//ゲームの開始方法を示すテキスト画像
	dial = engine.CreateUI<GameObject>(dialLayer, "nowLoading_logo", x, y);
	dial->AddSprite({ 0,0,1,1 }, sprite_x);

	//ゲームの開始方法を示すテキスト画像
	hour_hand = engine.CreateUI<GameObject>(hour_handLayer, "nowLoading_logo", x, y);
	hour_hand->AddSprite({ 0,0,1,1 }, sprite_x);
	hour_hand->rotate = 30.0f;

	//ゲームの開始方法を示すテキスト画像
	long_hand = engine.CreateUI<GameObject>(long_handLayer, "nowLoading_logo", x, y);
	long_hand->AddSprite({ 0,0,1,1 }, sprite_x);
}

void SetStatus(json j, Engine& engine)
{
	engine.score = 0;
	engine.stageCnt = j["Stage"]["count"];
	engine.playerHp = j["Player"]["hp"];
	engine.playerLife = j["Player"]["life"];
}