/**
* @file MainGameScene.h
*/
#ifndef MAINGAMESCENE_H_INCLUDED
#define MAINGAMESCENE_H_INCLUDED
#include "Scene.h"
#include"../TileMap.h"
#include "../VecMath/VecMath.h"
#include"../../Component/MeshRenderer.h"
#include"../Component/Color.h"

//先行宣言
class ScoreManager;
using ScoreManagerPtr = std::shared_ptr<ScoreManager>;

/**
* メインゲーム画面
*/
class MainGameScene : public Scene
 {
public:	
	MainGameScene() = default;
	virtual ~MainGameScene() = default;
	virtual bool Initialize(Engine & engine) override;
	virtual void Update(Engine & engine, float deltaTime) override;	

 protected:
	 void UpdatePlayer(Engine& engine, float deltaTime);
	 void SpawnEnemy(Engine& engine,float deltaTime);
	 void CreateMap(Engine& engine);
	 void CreateStageLogo(Engine& engine);
	 void LogoEase_CurrentStage(GameObjectPtr& gameObject,float disy);
	 void LogoEase_PreviousStage(GameObjectPtr& gameObject, float disy);
	 void StageLogoAnim(Engine& engine, float deltaTime);
	 void FadeIn(Engine& engine, float deltaTime);
	 void HpBar(Engine& engine);
	 void BossBgm(Engine& engine);
	 void StageBgm(Engine& engine);
	 void Operation(Engine& engine);
	 void HpAnim(float deltaTime);
	 void Pause(Engine& engine);
	 void Imgui(Engine& engine);

	 int keyNum = 68;
	 bool pauseFlg = false;

	 bool playerCollision = false;

	 float restartTimer = 0.0f;	//再スタートまでの時間をはかるタイマー
	 float shotTimer = 0.0f; //弾の発射間隔タイマー
	 bool playerMoveFlg = false;
	 float effecttimer = 1.0f;
	 int bossCnt = 0;
	 float color = 0.0f;
	 float efecttimer = 0.8f;	//ボスのhealthが０の時時間を遅くする
	 bool playerDeadSe = false;

	 bool disFlg = false;
	 float fontPosY = 0.0f;
	 float disY = 0.0f;
	 float val = 0.0f;

	 bool disFlg1 = false;
	 float fontPosY1 = 0.0f;
	 float disY1 = 0.0f;

	 bool disFlg2 = false;
	 float fontPosY2 = 0.0f;
	 float disY2 = 0.0f;

	 bool warningsFlg = false;
	 int warningsCnt = 0;
	 bool warningsAlphaFlg = false;
	 float blurStrength = 1.0f;

	 //HP関係
	 float lifeX = 100.0f;
	 float lifeY = 100.0f;
	 bool hpAnimFlg = false;//ダメージを受けたときにHPバーを揺らす
	 int hpAnimNum = 0;

	 //volume設定
	 float getVolume = 0.0f;

	 MeshRendererPtr playermesh;
	 std::shared_ptr<Color> playerMaterialColor;

	 TileMap enemyMap;     //敵出現データ
	 size_t spawningY = 0; //敵出現ライン
	 std::shared_ptr<VecMath::vec3> bgOffset; // 背景スクロール座標
	 GameObjectPtr playerShadow;//プレイヤーの影
	 GameObjectPtr player;		//プレイヤー
	 GameObjectPtr boss;		//ボス
	 GameObjectPtr smallEnemyPtr;  //小型機
	 GameObjectPtr scoreFont;
	 ScoreManagerPtr scoreManager;//スコア
	 GameObjectPtr rank;
	 GameObjectPtr enemy;
	 GameObjectPtr fade;
	 GameObjectPtr fade1;
	 GameObjectPtr jet;
	 GameObjectPtr backToTitle;
	 GameObjectPtr cameraObject;

	 GameObjectPtr life,hpBarGreen, hpBarRed, hpBarGray, hpBar;
	 GameObjectPtr warnings;
	 GameObjectPtr up, down, right, left, space;
	 GameObjectPtr stage_logo, stage_Num, stage_BackNum;


	 std::string upup;
	 std::string downdown;
	 std::string rightright;
	 std::string leftleft;
	 std::string shotshot;
	 int intup = 0;
};
#endif // MAINGAMESCENE_H_INCLUDED