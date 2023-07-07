/**
* @file TitleScene.h
*/
#ifndef TITLESCENE_H_INCLUDE
#define TITLESCENE_H_INCLUDE
#include"Scene.h"
#include<string>

/**
* タイトルシーン
*/
class TitleScene : public Scene
{
public:
	TitleScene() = default;
	virtual ~TitleScene() = default;
	virtual bool Initialize(Engine& engine)override;
	virtual void Update(Engine& engine, float deltaTime)override;
	void TitleLogo(Engine& engine);
	void TitleAnim(Engine& engine, float deltaTime);
	void FadeOut(Engine& engine, float deltaTime);
	void Dis(GameObjectPtr& gameObject, float disx,int fontPosNum);
	void BossRushSelect(Engine& engine);
	void SetSelectAnim(GameObjectPtr gameObject, float deltaTime);
	void Imgui(Engine& engine);
	GameObjectPtr fade;
	GameObjectPtr player;
	GameObjectPtr pressEnter;
	GameObjectPtr bossRush;
	GameObjectPtr config;

	GameObjectPtr uiW, uiI, uiN, uiG, uiH, uiU, uiN1, uiT, uiE, uiR;
	bool enterFlg = false;  //PressEnterのフェード管理
	bool fadeFlg = false;	//フェード開始フラグ
	bool sceneChangeFlg = false;
	bool titleAnim = false; //フェードインが終わってからタイトルのアニメーションを開始する
	bool SceneChange = false;
	bool key = false;
	float timer = 0.0f;		//PressEnterをチカチカさせる残り時間
	float maxTimer = 0.05f;	//PressEnterをチカチカさせる時間
	float pressEnter_alphaSpeed = 2;	//pressEnterの表示速度
	float fade_alphaSpeed = 0.5f; //フェードの速度
	float size = 100.0f;
	
	//UI周り
	bool disFlg[10] = {false};
	float uiPosX = 1500.0f;//UIの初期ポジション
	float fontPosX[10]; //タイトルUIのフォントのCX座標
	float uiX = 150.0f;//タイトルUIの最終到達目標
	float uiY = 440.0f;//タイトルUIのY座標
	float disX[10];
	float val = 0.0f;
	float mastarVol = 0.05f;
	float volTimer = 0.0f;

	int select = 1;
};
#endif // !TITLESCENE_H_INCLUDE
