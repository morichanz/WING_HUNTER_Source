/**
* @file GameOverScene.cpp
*/
#include "GameOverScene.h"
#include"Fade.h"
#include"../GameObject/GameObject.h"
#include"../Engine/Engine.h"
#include"../Component/Score/ScoreManager.h"
#include"../Component/Text.h"
#include"../../Audio/EasyAudio.h"
#include"../../Audio/AudioSetting.h"
#include"TitleScene.h"

/**
* タイトル画面を初期化する
*
* @retval true	初期化成功
* @retval false 初期化失敗
*/
bool GameOverScene::Initialize(Engine& engine)
{
	//ゲームオブジェクトを削除
	engine.ClearGameObjectList();
	engine.ClearUILayers();

	//UIレイヤーを作成
	const size_t bgLayer = engine.AddUILayer("Res/Images/GameOver.tga", GL_LINEAR, 10);
	const size_t logoLayer = engine.AddUILayer("Res/Images/Font/FontList/GameOver_logo.tga", GL_LINEAR, 10);
	const size_t textLayer = engine.AddUILayer("Res/Images/Font/FontList/font_05_2.tga", GL_LINEAR, 10);
	const size_t pressEnterLayer = engine.AddUILayer("Res/Images/TitleAnim/pressEnter.tga", GL_LINEAR, 10);

	//背景画像
	auto uiBackGround = engine.CreateUI<GameObject>(bgLayer, "GameOver_bg", 640.0f, 360.0f);
	uiBackGround->AddSprite({ 0,0,1,1 });

	RankLogo(engine);

	//ゲームの開始方法を示すテキスト画像
	pressEnter = engine.CreateUI<GameObject>(pressEnterLayer, "pressEnter_logo", 640.0f, 140.0f);
	pressEnter->AddSprite({ 0,0,1,1 }, 0.0f, 0.0f, 2.0f, 2.0f, 2.0f);
	pressEnter->alpha = 0;

	//ゲームオーバーロゴ画像
	logo = engine.CreateUI<GameObject>(logoLayer, "GameOver_Logo", 640.0f, 540.0f);
	logo->AddSprite({ 0,0,1,1 }, 0.0f, 0.0f,3.0f, 3.0f, 3.0f);
	logo->alpha = 0.0f;

	const char strPressEnter1[] = "Score";
	const float fontSizeX1 = 16.0f;
	const float x1 = 645.0f - static_cast<float>(std::size(strPressEnter1) - 1.0f) * fontSizeX1;
	auto uiPressEnter1 = engine.CreateUI<GameObject>(textLayer, "SCORE", x1, 280.0f);
	auto textPressEnter1 = uiPressEnter1->AddComponent<Text>();
	textPressEnter1->SetText(strPressEnter1, 2);

	//スコアマネージャーを作成
	auto uiScore = engine.CreateUI<GameObject>(textLayer, "text", 484.0f, 230.0f);
	scoreManager = uiScore->AddComponent<ScoreManager>();
	scoreManager->textComponent = uiScore->AddComponent<Text>();
	scoreManager->score = engine.score;

	//フェード画像
	const size_t alphaLayer = engine.AddUILayer("Res/Images/Fade_bg.tga", GL_LINEAR, 10);
	fade = engine.CreateUI<GameObject>(alphaLayer, "Fade_bg", 640.0f, 360.0f);
	fade->AddSprite({ 0,0,1,1 });
	fade->alpha = 1.0f;
	timer = maxTimer;

	//ゲームオーバー
	engine.volume = engine.j["Audio"]["standardVolume"];
	Audio::SetMasterVolume(engine.volume * engine.selectVolume);
	Audio::Play(AudioPlayer::bgm, BGM::gameOver, 1.0f, true);

	return true;	//初期化成功
}

/**
* クリア画面の状態を更新する
*/
void GameOverScene::Update(Engine& engine, float deltaTime)
{
	FadeOut(engine, deltaTime);
	//Enterキーが押されたらタイトルに戻る
	if (engine.GetKey(GLFW_KEY_ENTER) && fade->alpha <= 0.0f)
	{
		Audio::PlayOneShot(SE::decision, Audio::GetMasterVolume());
		engine.ClearSpriteList(1);
		pressEnter->alpha = 1.0f;
		engine.stageCnt = 1.0f;
		fadeFlg = true;
	}
}

void GameOverScene::RankLogo(Engine& engine)
{
	if (engine.score > 100000)
	{
		const size_t logoLayerS = engine.AddUILayer("Res/Images/Rank/S.tga", GL_LINEAR, 10);
		auto S = engine.CreateUI<GameObject>(logoLayerS, "S_logo", 394.0f, 250.0f);
		S->AddSprite({ 0,0,1,1 });
	}
	else if (engine.score > 80000)
	{
		const size_t logoLayerA = engine.AddUILayer("Res/Images/Rank/A.tga", GL_LINEAR, 10);
		auto A = engine.CreateUI<GameObject>(logoLayerA, "A_logo", 394.0f, 250.0f);
		A->AddSprite({ 0,0,1,1 });
	}
	else if (engine.score > 60000)
	{
		const size_t logoLayerB = engine.AddUILayer("Res/Images/Rank/B.tga", GL_LINEAR, 10);
		auto B = engine.CreateUI<GameObject>(logoLayerB, "B_logo", 394.0f, 250.0f);
		B->AddSprite({ 0,0,1,1 });
	}
	else
	{
		const size_t logoLayerC = engine.AddUILayer("Res/Images/Rank/C.tga", GL_LINEAR, 10);
		auto C = engine.CreateUI<GameObject>(logoLayerC, "C_logo", 394.0f, 250.0f);
		C->AddSprite({ 0,0,1,1 });
	}
}

void GameOverScene::FadeOut(Engine& engine, float deltaTime)
{
	if (!fadeFlg)
	{
		if (fade->alpha > 0.0f)
		{
			fade->alpha -= deltaTime * fadeSpeed;
		}
		else
		{
			if(logo->alpha < 1.0f)logo->alpha += deltaTime;
			if (pressEnter->alpha >= 1.0f)enterFlg = true;
			else if (pressEnter->alpha <= 0.0f)enterFlg = false;
			if (pressEnter->alpha < 1.0f && !enterFlg)	pressEnter->alpha += deltaTime;
			else if (pressEnter->alpha > 0.0f && enterFlg)pressEnter->alpha -= deltaTime * 2.0f;
		}
	}

	//pressEnterのチカチカ
	if (fadeFlg)
	{
		if (timer > 0.0f && pressEnter->alpha == 1.0f)
		{
			timer -= deltaTime;
			if (timer <= 0.0f)
			{
				pressEnter->alpha = 0.0f;
				timer = maxTimer;
			}
		}
		if (timer > 0.0f && pressEnter->alpha == 0.0f)
		{
			timer -= deltaTime;
			if (timer <= 0.0f)
			{
				pressEnter->alpha = 1.0f;
				timer = maxTimer;
			}
		}
		if (engine.volume > 0.0f)
		{
			engine.volume -= deltaTime * engine.volFadeSpeed;
			Audio::SetMasterVolume(engine.volume);
		}
		fade->alpha += deltaTime * fadeSpeed;
		if (fade->alpha > 1)engine.SetNextScene<TitleScene>();
	}
}
