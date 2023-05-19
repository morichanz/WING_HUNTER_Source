/**
* @file TitleScene.h
*/
#ifndef CONFIGSCENE_H_INCLUDE
#define CONFIGSCENE_H_INCLUDE
#include"Scene.h"
#include"../Component/Text.h"
#include<string>
#include<vector>

struct KeyConnection
{
	int keylist[5];
	bool key;		//一瞬でselectの値が変わらないように(１ずつ変更されるように)する
	bool keySet;	//キーの変更を許可する
	int keyup;		//keyの番号を使ってkey画像を描画
	int keydown;	//keyの番号を使ってkey画像を描画
	int keyright;	//keyの番号を使ってkey画像を描画
	int keyleft;	//keyの番号を使ってkey画像を描画
	int keyshot;	//keyの番号を使ってkey画像を描画
	int keyMemory;	//既に存在するkeyを記憶しておく
};

struct VolumeConnection
{
	size_t SetTextLayer;
	std::shared_ptr<Text> textnum;
	GameObjectPtr uiNum;
};

/**
* 設定シーン
*/
class ConfigScene : public Scene
{
public:
	ConfigScene() = default;
	virtual ~ConfigScene() = default;
	virtual bool Initialize(Engine& engine)override;
	virtual void Update(Engine& engine, float deltaTime)override;
	int GetKeyCoad(Engine& engine);
	bool GetAnyKeyDown(Engine& engine);
	void Imgui(Engine& engine);
	void Select(Engine& engine);
	void KeySetting(Engine& engine, float deltaTime);
	void VolumeSetting(Engine& engine, float deltaTime);
	void BackToTitle(Engine& engine,float deltaTime);
	void ConfigText(Engine& engine, const size_t textLayer);
	void ConfigKeyImage(Engine& engine);
	void ConfigKeyImage_Reset(Engine& engine);
	void ResetAnimImage(Engine& engine, float deltaTime);
	void VolumeNumImage(Engine& engine, const size_t textLayer);
	void VolumeArrowImage(Engine& engine, float deltaTime);

	GameObjectPtr fade;
	GameObjectPtr player;
	GameObjectPtr pressEnter;
	GameObjectPtr bossRush;

	GameObjectPtr uiW, uiI, uiN, uiG, uiH, uiU, uiN1, uiT, uiE, uiR;

	GameObjectPtr selectBox;
	GameObjectPtr leftArrow, upArrow, downArrow;
	GameObjectPtr enterNewKey;
	GameObjectPtr reset;
	GameObjectPtr upImage,downImage,rightImage,leftImage,shotImage,resetImage;

	bool imageSetFlg = false;

	bool enterFlg = false;  //PressEnterのフェード管理
	bool fadeFlg = false;	//フェード開始フラグ
	bool titleAnim = false; //フェードインが終わってからタイトルのアニメーションを開始する
	bool SceneChange = false;
	bool disFlg[10] = { false };
	int select = 0;
	float getVolume = 0.0f;
	bool isEnable = false;
 	float timer = 0.0f;		//PressEnterをチカチカさせる残り時間
	float maxTimer = 0.05f;	//PressEnterをチカチカさせる時間
	float pressEnter_alphaSpeed = 2;	//pressEnterの表示速度

	float selectTimer = 0.0f; //タイトルからシーンが移動したときに勝手にkeySetがtrueにならないようにする
	float defaultTimer = 0.1f;

	float selectBox_x = 200.0f; //キーを押したときにselectBoxを移動させる
	float selectBox_y = 70.0f; //キーを押したときにselectBoxを移動させる

	int castKeyNum = 0;

	bool backToTitleAnim = false;
	float resetAnimTimer = 0.0f;

	bool resetAnimFlg = false;

	KeyConnection key;
	VolumeConnection volume;
};
#endif // !CONFIGSCENE_H_INCLUDE
