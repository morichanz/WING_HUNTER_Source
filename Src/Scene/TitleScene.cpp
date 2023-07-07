/**
* @file TitleScene.cpp
*/
#include "TitleScene.h"
#include"MainGameScene.h"
#include"../Component/Text.h"
#include"../GameObject/GameObject.h"
#include"../Engine/Engine.h"
#include"../../Audio/EasyAudio.h"
#include"../../Audio/AudioSetting.h"
#include "../Component/MeshRenderer.h"
#include"ClearScene.h"
#include"ConfigScene.h"
#include"Fade.h"
#include"../VecMath/VecMath.h"
#include"LoadScene.h"
#include"../Easing.h"

#include "../../../ImGui/ImGui/imgui.h"
#include "../../../ImGui/ImGui/imgui_impl_opengl3.h"
#include "../../../ImGui/ImGui/imgui_impl_glfw.h"


using namespace VecMath;
/**
* タイトル画面を初期化する
* 
* @retval true	初期化成功
* @retval false 初期化失敗
*/
bool TitleScene::Initialize(Engine& engine)
{
	//ゲームオブジェクトを削除
	engine.ClearGameObjectList();
	engine.ClearUILayers();

	//UIレイヤーを作成
	const size_t bgLayer = engine.AddUILayer("Res/Images/Title_bg.tga", GL_LINEAR, 10);
	const size_t textLayer = engine.AddUILayer("Res/Images/Font/FontList/font_05_2.tga", GL_LINEAR, 10);
	const size_t pressEnterLayer = engine.AddUILayer("Res/Images/TitleAnim/PressEnter.tga", GL_LINEAR, 10);
	const size_t bossRushLayer = engine.AddUILayer("Res/Images/TitleAnim/BossRush.tga", GL_LINEAR, 10);
	const size_t configLayer = engine.AddUILayer("Res/Images/TitleAnim/Config_logo.tga", GL_LINEAR, 10);

	//背景画像
	auto uiBackGround = engine.CreateUI<GameObject>(bgLayer, "Title_bg", 640.0f, 360.0f);
	uiBackGround->AddSprite({ 0,0,1,1 });

	TitleLogo(engine);

	//ゲームの開始方法を示すテキスト画像
	pressEnter = engine.CreateUI<GameObject>(pressEnterLayer, "pressEnter_logo", 640.0f, 140.0f);
	pressEnter->AddSprite({ 0,0,1,1 },0.0f,40.0f);
	pressEnter->alpha = 0.0f;

	//ゲームの開始方法を示すテキスト画像
	bossRush = engine.CreateUI<GameObject>(bossRushLayer, "bossLush_logo", 640.0f, 140.0f);
	if (engine.j["Stage"]["rushFlg"])
	{
		bossRush->AddSprite({ 0,0,1,1 }, 0.0f, -60.0f);
		bossRush->alpha = 0.0f;
	}

	//ゲームの開始方法を示すテキスト画像
	config = engine.CreateUI<GameObject>(configLayer, "config_logo_logo", 640.0f, 140.0f);
	config->AddSprite({ 0,0,1,1 }, 0.0f, 140.0f);
	config->alpha = 0.0f;

	//フェード画像
	const size_t alphaLayer = engine.AddUILayer("Res/Images/Fade_bg.tga", GL_LINEAR, 10);
	fade = engine.CreateUI<GameObject>(alphaLayer, "Fade_bg", 640.0f, 360.0f);
	fade->AddSprite({ 0,0,1,1 });
	fade->alpha = 1.0f;
	timer = maxTimer;

	//タイトルBGMの再生
	engine.volume = engine.j["Audio"]["standardVolume"];
	engine.selectVolume = engine.j["Audio"]["selectVolume"];
	Audio::SetMasterVolume(engine.volume * engine.selectVolume);
	Audio::Play(AudioPlayer::bgm, BGM::title, Audio::GetMasterVolume(), true);

	return true;	//初期化成功
}

/**
* タイトル画面の状態を更新する
*/
void TitleScene::Update(Engine& engine, float deltaTime)
{
	FadeOut(engine, deltaTime);
	BossRushSelect(engine);
	Imgui(engine);
	
	//Enterキーが押されたらゲーム開始
	if (engine.GetKey(GLFW_KEY_ENTER) && uiW->x != uiPosX)
	{
		Audio::PlayOneShot(SE::decision, Audio::GetMasterVolume());
		pressEnter->alpha = 1.0f;
		bossRush->alpha = 1.0f;
		config->alpha = 1.0f;
		sceneChangeFlg = true;
		if(select != 2) fadeFlg = true;
		if (select == 1) engine.debugMap = false;
		else engine.debugMap = true;
	}
}

void TitleScene::Dis(GameObjectPtr& gameObject, float disx, int fontPosNum)
{
	if (!disFlg[fontPosNum])
	{
		fontPosX[fontPosNum] = gameObject->x;
		disX[fontPosNum] = (disx - gameObject->x);
		disFlg[fontPosNum] = true;
	}
}

void TitleScene::BossRushSelect(Engine& engine)
{
	const bool upKey = engine.GetKey(GLFW_KEY_UP);
	const bool downKey = engine.GetKey(GLFW_KEY_DOWN);
	if (!fadeFlg && engine.j["Stage"]["rushFlg"])
	{
		if (select < 2 && upKey && !key)
		{
			select += 1;
			key = true;
		}
		else if (select > 0 && downKey && !key)
		{
			select--;
			key = true;
		}

		if (!upKey && !downKey)key = false;
	}
}

//タイトルロゴのアニメーション
void TitleScene::TitleAnim(Engine& engine, float deltaTime)
{
	if (val < 1.0f)val += deltaTime / 2.0f;

	Dis(uiW, uiX, 0);
	uiW->x = fontPosX[0] + disX[0] * Easing::EaseOutExpo(val);
	Dis(uiI, uiX + size, 1);
	uiI->x = fontPosX[1] + disX[1] * Easing::EaseOutExpo(val);
	Dis(uiN, uiX + (size * 2), 2);
	uiN->x = fontPosX[2] + disX[2] * Easing::EaseOutExpo(val);
	Dis(uiG, uiX + (size * 3), 3);
	uiG->x = fontPosX[3] + disX[3] * Easing::EaseOutExpo(val);
	Dis(uiH, uiX + (size * 4), 4);
	uiH->x = fontPosX[4] + disX[4] * Easing::EaseOutExpo(val);
	Dis(uiU, uiX + (size * 5), 5);
	uiU->x = fontPosX[5] + disX[5] * Easing::EaseOutExpo(val);
	Dis(uiN1, uiX + (size * 6), 6);
	uiN1->x = fontPosX[6] + disX[6] * Easing::EaseOutExpo(val);
	Dis(uiT, uiX + (size * 7), 7);
	uiT->x = fontPosX[7] + disX[7] * Easing::EaseOutExpo(val);
	Dis(uiE, uiX + (size * 8), 8);
	uiE->x = fontPosX[8] + disX[8] * Easing::EaseOutExpo(val);
	Dis(uiR, uiX + (size * 9), 9);
	uiR->x = fontPosX[9] + disX[9] * Easing::EaseOutExpo(val);
}

//フェードアウト
void TitleScene::FadeOut(Engine& engine, float deltaTime)
{
	if (!fadeFlg)
	{
		if (fade->alpha > 0.0f)fade->alpha -= deltaTime * fade_alphaSpeed;
		else
		{
			titleAnim = true;
			//ボスラッシュ選択
			if (select == 0)
			{
				pressEnter->alpha = 1.0f;
				if (bossRush->alpha >= 1.0f)enterFlg = true;
				else if (bossRush->alpha <= 0.0f)enterFlg = false;
				if (bossRush->alpha < 1.0f && !enterFlg)	bossRush->alpha += deltaTime;
				else if (bossRush->alpha > 0.0f && enterFlg)bossRush->alpha -= deltaTime * pressEnter_alphaSpeed;
			}
			//本編スタート選択
			else if (select == 1)
			{
				bossRush->alpha = 1.0f;
				config->alpha = 1.0f;
				if (pressEnter->alpha >= 1.0f)enterFlg = true;
				else if (pressEnter->alpha <= 0.0f)enterFlg = false;
				if (pressEnter->alpha < 1.0f && !enterFlg)	pressEnter->alpha += deltaTime;
				else if (pressEnter->alpha > 0.0f && enterFlg)pressEnter->alpha -= deltaTime * pressEnter_alphaSpeed;
			}
			//設定画面選択
			else if (select == 2)
			{
				pressEnter->alpha = 1.0f;
				if (config->alpha >= 1.0f)enterFlg = true;
				else if (config->alpha <= 0.0f)enterFlg = false;
				if (config->alpha < 1.0f && !enterFlg)	config->alpha += deltaTime;
				else if (config->alpha > 0.0f && enterFlg)config->alpha -= deltaTime * pressEnter_alphaSpeed;
			}
		}
	}
	else
	{
		fade->alpha += deltaTime / 2.0f;

		//ゲーム開始時にボリュームを時間で下げる
		if (engine.volume > 0.0f && engine.selectVolume != 0)
		{
			engine.volume -= deltaTime * engine.volFadeSpeed;
			Audio::SetMasterVolume(engine.volume);
		}

		//エンターが押されたらbossRushをチカチカさせる
		if (select == 0)
		{
			SetSelectAnim(bossRush, deltaTime);
		}
		//エンターが押されたらpressEnterをチカチカさせる
		else if (select == 1)
		{
			SetSelectAnim(pressEnter, deltaTime);
		}
		else if (select == 2)
		{
			SetSelectAnim(config, deltaTime);
		}

		//フェードアウトが終わったらシーンを変える
		if (fade->alpha > 1)
		{
			if (engine.meshList.size() == NULL && select == 1|| engine.meshList.size() == NULL && select == 0)engine.SetNextScene<LoadScene>();
			else if(engine.meshList.size() != NULL && select == 1 || engine.meshList.size() != NULL && select == 0) engine.SetNextScene<MainGameScene>();
			else if(select == 2)engine.SetNextScene<ConfigScene>();
		}
	}

	if (sceneChangeFlg && select == 2)
	{
		engine.SetNextScene<ConfigScene>();
	}

	if (titleAnim) TitleAnim(engine, deltaTime);
}

void TitleScene::TitleLogo(Engine& engine)
{
	const size_t W = engine.AddUILayer("Res/Images/TitleAnim/W.tga", GL_LINEAR, 10);
	const size_t I = engine.AddUILayer("Res/Images/TitleAnim/I.tga", GL_LINEAR, 10);
	const size_t N = engine.AddUILayer("Res/Images/TitleAnim/N.tga", GL_LINEAR, 10);
	const size_t G = engine.AddUILayer("Res/Images/TitleAnim/G.tga", GL_LINEAR, 10);
	const size_t H = engine.AddUILayer("Res/Images/TitleAnim/H.tga", GL_LINEAR, 10);
	const size_t U = engine.AddUILayer("Res/Images/TitleAnim/U.tga", GL_LINEAR, 10);
	const size_t T = engine.AddUILayer("Res/Images/TitleAnim/T.tga", GL_LINEAR, 10);
	const size_t E = engine.AddUILayer("Res/Images/TitleAnim/E.tga", GL_LINEAR, 10);
	const size_t R = engine.AddUILayer("Res/Images/TitleAnim/R.tga", GL_LINEAR, 10);

	//タイトルロゴ画像
	uiW = engine.CreateUI<GameObject>(W, "title_logo", uiX, 440.0f);
	uiW->AddSprite({ 0,0,1,1 });
	uiW->x = uiPosX;

	//タイトルロゴ画像
	uiI = engine.CreateUI<GameObject>(I, "title_logo", uiX + size, uiY);
	uiI->AddSprite({ 0,0,1,1 });
	uiI->x = uiPosX;

	//タイトルロゴ画像
	uiN = engine.CreateUI<GameObject>(N, "title_logo", uiX + (size * 2), uiY);
	uiN->AddSprite({ 0,0,1,1 });
	uiN->x = uiPosX;

	//タイトルロゴ画像
	uiG = engine.CreateUI<GameObject>(G, "title_logo", uiX + (size * 3), uiY);
	uiG->AddSprite({ 0,0,1,1 });
	uiG->x = uiPosX;

	//タイトルロゴ画像
	uiH = engine.CreateUI<GameObject>(H, "title_logo", uiX + (size * 4), uiY);
	uiH->AddSprite({ 0,0,1,1 });
	uiH->x = uiPosX;

	//タイトルロゴ画像
	uiU = engine.CreateUI<GameObject>(U, "title_logo", uiX + (size * 5), uiY);
	uiU->AddSprite({ 0,0,1,1 });
	uiU->x = uiPosX;

	//タイトルロゴ画像
	uiN1 = engine.CreateUI<GameObject>(N, "title_logo", uiX + (size * 6), uiY);
	uiN1->AddSprite({ 0,0,1,1 });
	uiN1->x = uiPosX;

	//タイトルロゴ画像
	uiT = engine.CreateUI<GameObject>(T, "title_logo", uiX + (size * 7), uiY);
	uiT->AddSprite({ 0,0,1,1 });
	uiT->x = uiPosX;

	//タイトルロゴ画像
	uiE = engine.CreateUI<GameObject>(E, "title_logo", uiX + (size * 8), uiY);
	uiE->AddSprite({ 0,0,1,1 });
	uiE->x = uiPosX;

	//タイトルロゴ画像
	uiR = engine.CreateUI<GameObject>(R, "title_logo", uiX + (size * 9), uiY);
	uiR->AddSprite({ 0,0,1,1 });
	uiR->x = uiPosX;
}

void TitleScene::SetSelectAnim(GameObjectPtr gameObject, float deltaTime)
{
	if (timer > 0 && gameObject->alpha == 1)
	{
		timer -= deltaTime;
		if (timer <= 0)
		{
			gameObject->alpha = 0;
			timer = maxTimer;
		}
	}
	if (timer > 0 && gameObject->alpha == 0)
	{
		timer -= deltaTime;
		if (timer <= 0)
		{
			gameObject->alpha = 1;
			timer = maxTimer;
		}
	}
}

void TitleScene::Imgui(Engine& engine)
{
	if (engine.j["ImguiSetFlg"]["TitleScene"])
	{
		ImGui::Begin("StageSelect");
		if (ImGui::CollapsingHeader("select"))
		{
			ImGui::DragInt("select", &select);
		}
		if (ImGui::CollapsingHeader("volume"))
		{
			ImGui::DragFloat("volume", &engine.volume);
			ImGui::DragFloat("selectVolume", &engine.selectVolume);
		}
		ImGui::End();
	}
}
