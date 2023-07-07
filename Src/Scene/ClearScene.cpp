/**
* @file ClearScene.cpp
*/
#include "ClearScene.h"
#include"Fade.h"
#include"../Component/Text.h"
#include"../GameObject/GameObject.h"
#include"../Engine/Engine.h"
#include"../Component/Score/ScoreManager.h"
#include"../../Audio/EasyAudio.h"
#include"../../Audio/AudioSetting.h"
#include"TitleScene.h"
#include<fstream>

const std::string pathToJSON("data/file.json");

/**
* タイトル画面を初期化する
*
* @retval true	初期化成功
* @retval false 初期化失敗
*/
bool ClearScene::Initialize(Engine& engine)
{
	//ゲームオブジェクトを削除
	engine.ClearGameObjectList();
	engine.ClearUILayers();

	//UIレイヤーを作成
	const size_t bgLayer = engine.AddUILayer("Res/Images/Clear_bg.tga", GL_LINEAR, 10);
	const size_t logoLayer = engine.AddUILayer("Res/Images/Clear_logo.tga", GL_LINEAR, 10);
	const size_t textLayer = engine.AddUILayer("Res/Images/Font/FontList/font_05_2.tga", GL_LINEAR, 10);
	const size_t pressEnterLayer = engine.AddUILayer("Res/Images/TitleAnim/pressEnter.tga", GL_LINEAR, 10);

	//背景画像
	auto uiBackGround = engine.CreateUI<GameObject>(bgLayer, "Clear_bg", 640.0f, 360.0f);
	uiBackGround->AddSprite({ 0,0,1,1 });

	//タイトルロゴ画像
	clear = engine.CreateUI<GameObject>(logoLayer, "Clear_logo", 640.0f, 440.0f);
	clear->AddSprite({ 0,0,1,1 }, 0.0f, 0.0f, 1.25f);

	RankLogo(engine);

	//ゲームの開始方法を示すテキスト画像
	pressEnter = engine.CreateUI<GameObject>(pressEnterLayer, "pressEnter_logo", 640.0f, 140.0f);
	pressEnter->AddSprite({ 0,0,1,1 });
	pressEnter->alpha = 0;

	const char strPressEnter1[] = "Score";
	const float fontSizeX = 16.0f;
	const float x = 645.0f - static_cast<float>(std::size(strPressEnter1) - 1.0f) * fontSizeX;
	auto uiPressEnter1 = engine.CreateUI<GameObject>(textLayer, "SCORE", x, 270.0f);
	auto textPressEnter1 = uiPressEnter1->AddComponent<Text>();
	textPressEnter1->SetText(strPressEnter1, 2.0f);

	//スコアマネージャーを作成
	auto uiScore = engine.CreateUI<GameObject>(textLayer, "text", 484.0f, 220.0f);
	scoreManager = uiScore->AddComponent<ScoreManager>();
	scoreManager->textComponent = uiScore->AddComponent<Text>();
	scoreManager->score = engine.score;

	//フェード画像
	const size_t alphaLayer = engine.AddUILayer("Res/Images/Fade_bg.tga", GL_LINEAR, 10);
	fade = engine.CreateUI<GameObject>(alphaLayer, "Fade_bg", 640.0f, 360.0f);
	fade->AddSprite({ 0,0,1,1 });
	fade->alpha = 1.0f;

	engine.volume = engine.j["Audio"]["standardVolume"];
	engine.selectVolume = engine.j["Audio"]["selectVolume"];
	Audio::SetMasterVolume(engine.volume * engine.selectVolume);
	Audio::Play(AudioPlayer::bgm, BGM::gameClear, 1, true);

	return true;	//初期化成功
}

/**
* クリア画面の状態を更新する
*/
void ClearScene::Update(Engine& engine, float deltaTime)
{
	FadeOut(engine, deltaTime);

	//Enterキーが押されたらタイトルに戻る
	if (engine.GetKey(GLFW_KEY_ENTER) && fade->alpha <= 0 && !fadeFlg)
	{
		Audio::PlayOneShot(SE::decision, Audio::GetMasterVolume());
		engine.ClearSpriteList(1);
		pressEnter->alpha = 1.0f;
		engine.stageCnt = engine.j["Stage"]["count"];

		//jsonファイル書き出し
		std::ofstream ofs(pathToJSON.c_str());
		if (ofs.good())
		{
			engine.j["Stage"]["rushFlg"] = true;
			ofs << engine.j.dump(3) << std::endl;
			ofs.close();
		}

		fadeFlg = true;
	}
}

void ClearScene::RankLogo(Engine& engine)
{
	const float x = 394.0f;
	const float y = 250.0f;

	if (engine.score > 120000)
	{
		const size_t logoSLayer = engine.AddUILayer("Res/Images/Rank/S.tga", GL_LINEAR, 10);
		auto S = engine.CreateUI<GameObject>(logoSLayer, "S_logo", x, y);
		S->AddSprite({ 0,0,1,1 });
	}
	else if (engine.score > 100000)
	{
		const size_t logoALayer = engine.AddUILayer("Res/Images/Rank/A.tga", GL_LINEAR, 10);
		auto A = engine.CreateUI<GameObject>(logoALayer, "A_logo", x, y);
		A->AddSprite({ 0,0,1,1 });
	}
	else if (engine.score > 80000)
	{
		const size_t logoBLayer = engine.AddUILayer("Res/Images/Rank/B.tga", GL_LINEAR, 10);
		auto B = engine.CreateUI<GameObject>(logoBLayer, "B_logo", x, y);
		B->AddSprite({ 0,0,1,1 });
	}
	else
	{
		const size_t logoLayerC = engine.AddUILayer("Res/Images/Rank/C.tga", GL_LINEAR, 10);
		auto C = engine.CreateUI<GameObject>(logoLayerC, "C_logo", x, y);
		C->AddSprite({ 0,0,1,1 });
	}

}

void ClearScene::FadeOut(Engine& engine, float deltaTime)
{
	if (!fadeFlg)
	{
		if (fade->alpha > 0.0f)
		{
			fade->alpha -= deltaTime * 0.5f;
		}
		else
		{
			if (pressEnter->alpha >= 1.0f)enterFlg = true;
			else if (pressEnter->alpha <= 0.0f)enterFlg = false;
			if (pressEnter->alpha < 1.0f && !enterFlg)	pressEnter->alpha += deltaTime;
			else if (pressEnter->alpha > 0.0f && enterFlg)pressEnter->alpha -= deltaTime * 2.0f;
		}
	}

	if (fadeFlg)
	{
		if (engine.volume > 0.0f)
		{
			engine.volume -= deltaTime * engine.volFadeSpeed;
			Audio::SetMasterVolume(engine.volume);
		}
		if (timer > 0.0f && pressEnter->alpha == 1.0f)
		{
			timer -= deltaTime;
			if (timer <= 0.0f)
			{
				pressEnter->alpha = 0.0f;
				timer = 0.05f;
			}
		}
		if (timer > 0.0f && pressEnter->alpha == 0.0f)
		{
			timer -= deltaTime;
			if (timer <= 0.0f)
			{
				pressEnter->alpha = 1.0f;
				timer = 0.05f;
			}
		}
		fade->alpha += deltaTime * 0.5f;
		if (fade->alpha > 1.0f)engine.SetNextScene<TitleScene>();
	}
}
