/**
* @file MainGameScene.cpp
*/
#include"MainGameScene.h"
#include"../Component/Move.h"
#include"../Component/Bullet/BulletMove.h"
#include"../Component/OutOfScreen.h"
#include"../Component/DamageSource.h"
#include"../Component/Health.h"
#include"../Component/Boss/BossHealth.h"
#include"../Component/Explosion.h"
#include"../Component/Boss/BossExplosion.h"
#include"../Component/Bullet/FireBullet.h"
#include"../Component/Bullet/CircleBullet.h"
#include"../Component/Enemy/LargeEnemyAction.h"
#include"../Component/Text.h"
#include"../Component/Score/ScoreManager.h"
#include"../Component/Score/Score.h"
#include"../Component/../Audio/AudioEvent.h"
#include"../Component/MeshRenderer.h"
#include"../Component/Boss/BossAction.h"
#include"../Component/Enemy/SmallEnemyAction.h"
#include"../Component/Enemy/MiddleEnemyAction.h"
#include"../Component/Color.h"
#include"../Component/ScaleManager.h"
#include"../Component/Rotation/Rotation.h"
#include"../Sprite/SpritePriority.h"
#include"../GameObject/GameObject.h"
#include"../Engine/Engine.h"
#include"../../Audio/EasyAudio.h"
#include"../../Audio/AudioSetting.h"
#include"../Easing.h"
#include"ClearScene.h"
#include"GameOverScene.h"
#include"TitleScene.h"
#include"Fade.h"
#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include"../../json/json.hpp"


#include "../../../ImGui/ImGui/imgui.h"
#include "../../../ImGui/ImGui/imgui_impl_opengl3.h"
#include "../../../ImGui/ImGui/imgui_impl_glfw.h"

using json = nlohmann::json;
const std::string pathToJSON("data/file.json");	

using namespace VecMath;
/**
* 背景スクロール用コンポーネント
*/
class BgScroll : public Component
 {
public:
	BgScroll() = default;
	virtual ~BgScroll() = default;
	
	virtual void Start(GameObject & go) override 
	{
	    //初期座標を記録
		initialPosition = vec3(go.x, go.y, go.z);
	}

	virtual void Update(GameObject & go, float deltaTime) override
	{
	    //座標を更新
		if (offset)
		{
			const vec3 v = initialPosition + *offset;
			go.x = v.x;
			go.y = v.y;
			go.z = v.z;
		}
	}
	vec3 initialPosition = vec3(0, 0, 0); // 初期座標
	std::shared_ptr<vec3> offset; // スクロール値
};

//敵の座標リスト
const Texcoord texcoordList[] = {
	//MakeTexcoord(画像全体のX座標(テクスチャの開始値),画像全体のY座標(テクスチャの開始値),テクスチャのX座標,テクスチャのY座標)
	MakeTexcoord(0,0,64,64),	//大型機
	MakeTexcoord(0,110,32,32),	//小型機
	MakeTexcoord(128,160,46,46),//隕石
	MakeTexcoord(130,67,27,45), //変な奴
	MakeTexcoord(256,0,256,128),//ボス
	MakeTexcoord(277,127,100,205),//ボス
	MakeTexcoord(1,882,177,142),//ボス
	MakeTexcoord(460,863,237,161),//ボス
	MakeTexcoord(608,663,200,173),//ボス
	MakeTexcoord(-2,631,307,227),//ボス
};

const Texcoord jettc[] = {
MakeTexcoord(32,488,7,7),
MakeTexcoord(32,496,7,7),
MakeTexcoord(32,504,7,7),
};

/**
* 座標(x, y)が画面外かどうか
*/
bool IsOutOfScreen(float x, float y)
{
	const float xMin = -32;     // 画面内とみなす最少X座標
	const float xMax = 18 * 32; // 画面内とみなす最大X座標
	const float yMin = -32;     // 画面内とみなす最少Y座標
	const float yMax = 23 * 32; // 画面内とみなす最大Y座標
	return
		x <= xMin || x >= xMax ||
		y <= yMin || y >= yMax;
}

/**
* シーンを初期化する
*
* @retval true  初期化成功
* @retval false 初期化失敗
*/
bool MainGameScene::Initialize(Engine& engine)
{
	engine.efect = 1;
	engine.fadeFlg = false;
	GLFWwindow* window = engine.GetWindow();
	std::mt19937& rg = engine.GetRandomGenerator();
	const auto [bgSizeX, bgSizeY, bgBaseX, bgBaseY] = engine.GetBgSize(0, 0);

	//ゲームオブジェクトを削除
	engine.ClearGameObjectList();
	engine.ClearUILayers();

	//カメラパラメータを設定
	const vec3 viewSize = engine.GetViewSize();
	cameraObject = engine.GetMainCameraObject();
	cameraObject->x = viewSize.x / 2;
	cameraObject->y = viewSize.y / 2;
	cameraObject->z = 500;
	cameraObject->rotation[0] = 0;
	//cameraObject->rotation[0] = radians(90);

	//ビューポートを設定
	auto cameraComponent = engine.GetMainCamera();
	const vec2 windowSize = engine.GetWindowSize();
	cameraComponent->viewport = {0, 0, static_cast<int>(viewSize.x), static_cast<int>(viewSize.y) };

	//垂直視野角を計算
	const float tanTheta = (viewSize.y * 0.5f) / cameraObject->z;
	cameraComponent->fovY = VecMath::degrees(atan(tanTheta)) * 2;

	//アスペクト比を計算
	cameraComponent->aspect = viewSize.y / viewSize.x;

	// ブルームパラメータを設定
	engine.SetBloomThreshold(0.5f); //しきい値
	engine.SetBloomStrength(4.0f);  //ブルームの強さ

	//自機を追加
	player = engine.Create<GameObject>("player", static_cast<float>(bgSizeX) * 32 / 2, 64);

	//スプライトの追加
	playermesh = player->AddComponent<MeshRenderer>();
	playermesh->mesh = engine.LoadOBJ("Res/3Dobj/player_fighter_01.obj");
	playermesh->scale = vec3(16);
	playermesh->rotation.x = radians(90);
	playerMaterialColor = player->AddComponent<Color>();
	playerMaterialColor->MaterialColor(playermesh);

	//プレイヤーの当たり判定
	player->AddCollision({-3,-4, 3, 4});

	//スプライトの座標をランダムに決定
	CreateMap(engine);

	//ジェット
	jet = engine.Create<GameObject>("jet", player->x, player->y);
	jet->AddSprite(jettc[0]);
	auto jetMove = jet->AddComponent<Move>();
	jetMove->player = player;
	jetMove->jetFlg = true;
	jet->priority = prioPlayerShadow;
	auto anim = jet->AddComponent<Animator2D>();
	anim->loopTime = true;		//ループ再生をしない
	anim->killWhenEnd = false;	//再生終了時にゲームオブジェクトを削除
	anim->clip.resize(std::size(jettc));
	for (size_t frameNo = 0; frameNo < std::size(jettc); ++frameNo)
	{
		anim->AddSprite(frameNo, jettc[frameNo], -4.8f, -21.0f, 2.0f, 2.0f, 2.0f);
		anim->AddSprite(frameNo, jettc[frameNo], 4.8f, -21.0f, 2.0f, 2.0f, 2.0f);
	}

	//プレイヤーのHP
	auto health = player->AddComponent<Health>();
	health->health = engine.j["Player"]["hp"];
	player->health = health->health;

	//爆発の数
	auto explosion = player->AddComponent<Explosion>();
	explosion->count = 5;	//爆発の表示数を増やす

	//プレイヤーの拡大縮小
	auto playerScale = player->AddComponent<ScaleManager>();

	//優先順位
	player->priority = prioPlayer;

	//UIレイヤーを追加
	const size_t frameLayer = engine.AddUILayer("Res/Images/frame.tga", GL_LINEAR, 10);
	const size_t textLayer = engine.AddUILayer("Res/Images/Font/FontList/font_05_2.tga", GL_NEAREST, 10);

	//フレームの作成
	auto uiFrame = engine.CreateUI<GameObject>(frameLayer, "frame", 640, 360);
	uiFrame->AddSprite({ 0,0,1,1 });

	//スコアマネージャーを作成
	auto uiScore = engine.CreateUI<GameObject>(1, "text", 930, 500);
	scoreManager = uiScore->AddComponent<ScoreManager>();
	scoreManager->textComponent = uiScore->AddComponent<Text>();
	scoreManager->score = engine.score;

	//スコアFont
	float size = 2;
	const size_t uiScoreFont = engine.AddUILayer("Res/Images/Font/FontList/Score.tga", GL_LINEAR, 10);
	scoreFont = engine.CreateUI<GameObject>(uiScoreFont, "HP", 1095, 600);
	scoreFont->AddSprite({ 0, 0, 1, 1 }, 0, 0, size, size, size);

	//ステージロゴ画像
	CreateStageLogo(engine);

	//プレイヤーのHPバー
	HpBar(engine);

	//操作説明
	Operation(engine);
	
	//フェード画像
	const size_t alphaLayer2 = engine.AddUILayer("Res/Images/Warning.tga", GL_LINEAR, 10);
	warnings = engine.CreateUI<GameObject>(alphaLayer2, "Warning", 640, 360);
	warnings->AddSprite({ 0,0,1,1 });
	warnings->alpha = 0.0f;

	//フェード画像
	const size_t alphaLayer1 = engine.AddUILayer("Res/Images/Fade_White_bg.tga", GL_LINEAR, 10);
	fade1 = engine.CreateUI<GameObject>(alphaLayer1, "Fade_White_bg", 640, 360);
	fade1->AddSprite({ 0,0,1,1 });
	if (engine.stageCnt != 6) fade1->alpha = 0;
	else fade1->alpha = 1;

	//フェード画像
	const size_t alphaLayer = engine.AddUILayer("Res/Images/Fade_bg.tga", GL_LINEAR, 10);
	fade = engine.CreateUI<GameObject>(alphaLayer, "Fade_bg", 640, 360);
	fade->AddSprite({ 0,0,1,1 });
	if (engine.stageCnt != 6) fade->alpha = 1;
	else fade->alpha = 0;

	//タイトルに戻る画像
	const size_t alphaLayer3 = engine.AddUILayer("Res/Images/BackToTitle.tga", GL_LINEAR, 10);
	backToTitle = engine.CreateUI<GameObject>(alphaLayer3, "BackToTitle", 640, 360);
	backToTitle->AddSprite({ 0,0,1,1 });
	backToTitle->alpha = 0.0f;

	engine.scoreBack = engine.score;

	engine.volume = engine.j["Audio"]["standardVolume"];
	engine.selectVolume = engine.j["Audio"]["selectVolume"];
	Audio::SetMasterVolume(engine.volume * engine.selectVolume);
	getVolume = Audio::GetMasterVolume();
	StageBgm(engine);

	return true; // 初期化成功
}

/**
* メインゲーム画面の状態を更新する
*/
void MainGameScene::Update(Engine& engine, float deltaTime)
{
	FadeIn(engine, deltaTime);
	StageLogoAnim(engine, deltaTime);
	UpdatePlayer(engine, deltaTime);
	SpawnEnemy(engine, deltaTime);
	Imgui(engine);

	//背景モデルのスクロール座標を更新
	if (bgOffset) 
	{
		const float scrollSpeed = 32.0f; //スクロール速度(ピクセル毎秒)
		const float tileSize = 32.0f;    //タイルマップのタイルサイズ
	
	    //スクロール終端座標
		const float bgScrollYEnd = -static_cast<float>(engine.GetBgSize(0,0).sizeY) *
		tileSize + engine.GetViewSize().y;
		
		if (bgOffset->y > bgScrollYEnd) 
		{
			bgOffset->y -= scrollSpeed * deltaTime;
		}
		else 
		{
			bgOffset->y = bgScrollYEnd;
		}
		bgOffset->x = -player->x / 5.0f;
	} //bgOffset

	if (blurStrength > 0.0f)
	{
		blurStrength -= deltaTime / 5.0f;
	}
	engine.SetBlurStrength(blurStrength);
}

/**
* 自機の状態を更新
*/
void MainGameScene::UpdatePlayer(Engine& engine, float deltaTime)
{
	if (engine.GetKey(GLFW_KEY_C) && engine.GetKey(GLFW_KEY_O) && engine.j["ImguiSetFlg"]["MainGameScene"])
	{
		engine.j["Player"]["collision"] = playerCollision;

		if(engine.j["Player"]["collision"])player->AddCollision({ -3,-4, 3, 4 });
		else player->RemoveCollision();

		//jsonファイル書き出し
		std::ofstream ofs(pathToJSON.c_str());
		if (ofs.good())
		{
			ofs << engine.j.dump(3) << std::endl;
			ofs.close();
		}
	}

	Pause(engine);

	//ダメージを受けたとき
	if (player->damageFlg)
	{
		hpBarGreen->x = (static_cast<float>( player->health * 26.0f) - 78.0f);
		engine.playerHp = player->health;
		player->RemoveCollision();
		player->invincible = engine.j["Player"]["invincibleTime"];
		hpAnimFlg = true;
		player->collisionFlg = false;
		player->damageFlg = false;
	}

	//ダメージを受けたときのHPのアニメーション
	HpAnim(deltaTime);

	if (player->invincible > 0.0f)
	{
		player->invincible -= deltaTime;
		playerMaterialColor->SetDamageMaterialColor(playermesh, playerMaterialColor->objectColor.red, deltaTime);
	}
	else
	{
		if (!player->collisionFlg)
		{
			playerMaterialColor->SetMaterialColor(playermesh, playerMaterialColor->objectColor.baceColor);
			if(engine.j["Player"]["collision"])player->AddCollision({ -3,-4, 3, 4 });
			player->alpha = 1.0f;
			player->collisionFlg = true;
		}
	}

	//自機が破壊されたらリスタート
	if (player->isDead)
	{
		player->damageFlg = true;
		jet->isDead = true;
		restartTimer += deltaTime;
		if (restartTimer >= 2.0f)
		{
			//フェードアウト
			if (fade->alpha < 1.0f)
			{
				fade->alpha += deltaTime / 2.0f;
				if (engine.volume > 0.0f && Audio::GetMasterVolume() != 0)
				{
					engine.volume -= deltaTime * engine.volFadeSpeed;
					Audio::SetMasterVolume(engine.volume * engine.selectVolume);
				}
			}
			else
			{
				engine.fadeFlg = false;
				//残機が残っているとき
				if (engine.playerLife > 0)
				{
					engine.score = engine.scoreBack;
					Audio::Stop(AudioPlayer::se);
					engine.SetNextScene<MainGameScene>();
					engine.playerLife--;
					engine.playerHp = engine.j["Player"]["hp"];
				}
				else
				{
					engine.scoreBack = 0;
					Audio::Stop(AudioPlayer::se);
					engine.SetNextScene<GameOverScene>();
				}
			}
		}
		return;
	}

	//ボスが死んだときにクリア画面へ切り替え
	if (boss != nullptr)
	{
		if (boss->health <= 0)
		{
			player->RemoveCollision();

			if (efecttimer > 0.0f)
			{
				efecttimer -= deltaTime;
				engine.efect = 0.5f;
			}
			else engine.efect = 1.0f;
		}
		if (boss->isDead)
		{
			engine.score = scoreManager->score;
			engine.scoreBack = 0;
			playerMoveFlg = true;
			restartTimer += deltaTime;
			player->y += deltaTime * 200.0f;
			if (restartTimer >= 2.0f)
			{
				if (engine.stageCnt != 5)
				{
					if (fade->alpha < 1.0f)fade->alpha += deltaTime / 2.0f;
					else
					{
						if (engine.stageCnt < 6)
						{
							engine.fadeFlg = false;
							engine.stageCnt++;
							engine.SetNextScene<MainGameScene>();
							playerMoveFlg = false;
							engine.interval = 200.0f;
						}
						else
						{
							engine.fadeFlg = false;
							engine.stageCnt++;
							engine.SetNextScene<ClearScene>();
							playerMoveFlg = false;
						}
					}
				}
				else
				{
					if (fade1->alpha < 1.0f)fade1->alpha += deltaTime / 2.0f;
					else
					{
						engine.fadeFlg = false;
						engine.stageCnt++;
						engine.SetNextScene<MainGameScene>();
						playerMoveFlg = false;
						engine.interval = 200.0f;
					}
				}
			}
		}
	}


	if (!playerMoveFlg)
	{
		//キー入力を移動方向に変換
		const float vx = static_cast<float>(
			engine.GetKey(engine.j["Key"]["right"]) - engine.GetKey(engine.j["Key"]["left"]));
		const float vy = static_cast<float>(
			engine.GetKey(engine.j["Key"]["up"]) - engine.GetKey(engine.j["Key"]["down"]));

		if (!pauseFlg)
		{
			const bool dKey = engine.GetKey(engine.j["Key"]["right"]);
			const bool aKey = engine.GetKey(engine.j["Key"]["left"]);
			const bool wKey = engine.GetKey(engine.j["Key"]["up"]);
			const bool sKey = engine.GetKey(engine.j["Key"]["down"]);

			if (dKey)right->y = 295.0f;
			else right->y = 300.0f;
			if (aKey)left->y = 295.0f;
			else left->y = 300.0f;
			if (wKey)up->y = 345.0f;
			else up->y = 350.0f;
			if (sKey)down->y = 295.0f;
			else down->y = 300.0f;
		}

		//移動方向の長さが0以上なら入力があったとみなす(0除算を回避するため)
		const float v = std::sqrt(vx * vx + vy * vy); //移動方向の長さvを求める
		if (v > 0.0f)
		{
			//自機の座標を更新
			const float playerSpeed = 200.0f * deltaTime / v;
			player->x += vx * playerSpeed;
			player->y += vy * playerSpeed;
		}
		//自機を左右に傾ける
		player->rotation[1] = vx * radians(30.0f);
	}

	if (!playerMoveFlg)
	{
		//自機が画面外に行かないように、値の範囲を制限する
		const float viewSizeX = 17.0f * 32.0f;
		const float viewSizeY = 22.0f * 32.0f;
		player->x = std::clamp(player->x, 16.0f, viewSizeX - 16.0f);
		player->y = std::clamp(player->y, 24.0f, viewSizeY - 24.0f);

		//弾を発射
		const Texcoord tcBullet = MakeTexcoord(16.0f, 432.0f, 16.0f, 32.0f);
		const Texcoord tcSBullet = MakeTexcoord(528.0f, 432.0f, 16.0f, 32.0f);

		if (!pauseFlg)
		{
			const bool shotKey = engine.GetKey(engine.j["Key"]["shot"]);
			if (shotKey)
			{
				space->y = 415.0f;

				shotTimer -= deltaTime; //キーが押されている間タイマーを減らす
				if (shotTimer <= 0.0f)
				{
					//タイマーが0以下になったら弾を発射
					//弾の作成
					GameObjectPtr bullet = engine.Create<GameObject>(
						"bullet", player->x, player->y);
					GameObjectPtr Rbullet = engine.Create<GameObject>(
						"bullet", player->x, player->y);
					GameObjectPtr Lbullet = engine.Create<GameObject>(
						"bullet", player->x, player->y);

					//優先順位
					Lbullet->priority = Rbullet->priority = bullet->priority = prioPlayerBullet;

					//スプライトの追加
					bullet->AddSprite(tcBullet);
					Rbullet->AddSprite(tcBullet);
					Lbullet->AddSprite(tcBullet);

					//当たり判定の追加
					bullet->AddCollision({ -12,-24, 12, 16 });
					Rbullet->AddCollision({ -12,-24, 12, 16 });
					Lbullet->AddCollision({ -12,-24, 12, 16 });

					//弾に動きを追加
					auto c = bullet->AddComponent<Move>();
					auto rc = Rbullet->AddComponent<Move>();
					auto Lc = Lbullet->AddComponent<Move>();
					c->vy = 1000.0f;

					rc->vy = 1000.0f;
					rc->vx = 100.0f;

					Lc->vy = 1000.0f;
					Lc->vx = -100.0f;

					//画面外に出たときの処理の追加
					bullet->AddComponent<OutOfScreen>();
					Rbullet->AddComponent<OutOfScreen>();
					Lbullet->AddComponent<OutOfScreen>();

					//弾が敵に接触したとき
					auto damageSource = bullet->AddComponent<DamageSource>();
					damageSource->targetNameList = { "enemy","boss","weaponRight","weaponLeft" };
					auto RdamageSource = Rbullet->AddComponent<DamageSource>();
					RdamageSource->targetNameList = { "enemy","boss","weaponRight","weaponLeft" };
					auto LdamageSource = Lbullet->AddComponent<DamageSource>();
					LdamageSource->targetNameList = { "enemy","boss","weaponRight","weaponLeft" };
					shotTimer += 0.1f; //弾の発射間隔を設定(秒)
					Audio::PlayOneShot(SE::playerShot, Audio::GetMasterVolume()); //効果音を再生
				}
			}
			else
			{
				//次にキーが押されたら、すぐ弾が発射されるようにする
				shotTimer = 0.0f;
				space->y = 420.0f;
			}
		}
	}
	else
	{
		if (engine.stageCnt == 5)
		{
			player->scale += deltaTime * 4.0f;
		}
	}
}

/**
* 敵を出現させる
*/
void MainGameScene::SpawnEnemy(Engine& engine, float deltaTime)
{
	if (warningsFlg)
	{
		if (!warningsAlphaFlg && warningsCnt < 3)
		{
			if (warnings->alpha < 1.0f) warnings->alpha += deltaTime * 2.0f;
			else warningsAlphaFlg = true;
		}

		if (warningsAlphaFlg)
		{
			if (warnings->alpha > 0.0f) warnings->alpha -= deltaTime * 2.0f;
			else
			{
				warningsCnt++;
				warningsAlphaFlg = false;
			}
		}
	}

	//スクロール値を取得
	auto [sx, sy, bgBaseX, bgBaseY] = engine.GetBgSize(0, 0);
	if (bgBaseY >= 0)
	{
		return; //配置データの範囲外なので何もしない
	}

	//画面上部(天井)ラインの位置を計算
	const int startY = static_cast<int>(enemyMap.sizeY) - 23.0f * 2.0f;
	int ceilY = startY + static_cast<int>(bgBaseY / 32.0f * 2.0f);
	if (ceilY < 0)
	{
		ceilY = 0; //範囲外アクセスを避けるために0を最小値とする
	}

	//敵出現ライン～天井ライン間の配置データに従って敵を出現させる
	for (; spawningY > ceilY; --spawningY)
	{
		//横一列分の配置データをチェック
		for (size_t x = 0; x < enemyMap.sizeX; ++x)
		{
			//配置データから配置する敵の番号を取得
			const size_t i = spawningY * enemyMap.sizeX + x;
			const int enemyId = enemyMap.data[i];

			//敵番号がテクスチャ座標配列の範囲内の場合、敵を出現させる
			if (enemyId >= 0 && enemyId < std::size(texcoordList))
			{
				//実際の出現位置に敵を配置(配置データは画像の右下を指している)
				//画像サイズ=512x512, タイルサイズ=32x32
				const Texcoord& tc = texcoordList[enemyId];
				//const Texcoord & tc = texcoordSList[enemyId];
				if (enemyId == 4 + (engine.stageCnt - 1))
				{
					enemy = engine.Create<GameObject>(
						"boss",
						static_cast<float>(x * 32.0f + tc.sx * 1024.0f / 4.0f),
						static_cast<float>(21.0f * 32.0f + tc.sy * 1024.0f / 4.0f));
				}
				else
				{
					enemy = engine.Create<GameObject>(
						"enemy",
						static_cast<float>(x * 32.0f + tc.sx * 1024.0f / 4.0f),
						static_cast<float>(21.0f * 32.0f + tc.sy * 1024.0f / 4.0f));
				}

				//敵の動きを取得(影も同じ)
				auto move = enemy->AddComponent<Move>();
				auto explosion = enemy->AddComponent<BossExprosion>();
				explosion->count = 3;

				//スコアコンポーネントを設定
				auto score = enemy->AddComponent<Score>();
				score->scoreManager = scoreManager;
				engine.score = score->scoreManager->score;

				//大型機
				if (enemyId == 0)
				{
					enemy->meshRenderer = enemy->AddComponent<MeshRenderer>();
					enemy->meshRenderer->mesh = engine.meshList[0];
					enemy->meshRenderer->scale = vec3(16.0f);
					enemy->meshRenderer->rotation.x = radians(90.0f);
					enemy->meshRenderer->rotation.y = radians(0.0f);
					auto enemyColor = enemy->AddComponent<Color>();
					enemyColor->MaterialColor(enemy->meshRenderer);

					auto damageSource = enemy->AddComponent<DamageSource>();
					damageSource->targetName = "player";
					damageSource->isOnce = false;

					auto largeMove = enemy->AddComponent<LargeEnemyAction>();
					largeMove->stageCnt = engine.stageCnt;
					largeMove->enemyColor = enemyColor;

					auto largTransform = enemy->AddComponent<ScaleManager>();
					largTransform->scale = 5.0f;
					explosion->count = 5;
					enemy->priority = prioLargeEnemyMachine;

					auto fireBulletL = enemy->AddComponent<CircleBullet>();
					fireBulletL->enemy = enemy;
					fireBulletL->timerA = 0.9f;	//最初の弾を出現後に発射させる
					fireBulletL->target = player;
					fireBulletL->x = 10.0f;
					fireBulletL->y = -10.0f;
					score->score = engine.j["LargeEnemy"]["score"];
				}
				//中型機
				else if (enemyId == 3)
				{
					auto middle = enemy->AddComponent<MiddleEnemyAction>();
					auto renderer = enemy->AddComponent<MeshRenderer>();
					renderer->mesh = engine.meshList[3];
					renderer->scale = vec3(16.0f);
					renderer->rotation.x = radians(90.0f);
					renderer->rotation.y = radians(180.0f);

					auto damageSource = enemy->AddComponent<DamageSource>();
					damageSource->targetName = "player";

					//当たり判定のサイズを設定
					const float collisionSize = 1024.0f * 0.7f;
					enemy->AddCollision({ -tc.sx * collisionSize, -tc.sy * collisionSize,
										   tc.sx * collisionSize, tc.sy * collisionSize });

					//敵と敵の影が画面外に出たときの処理を追加
					enemy->AddComponent<OutOfScreen>();

					//敵のHP情報　プレイヤー参照
					auto health = enemy->AddComponent<Health>();
					health->health = engine.j["MiddleEnemy"]["hp"];
					score->score = engine.j["MiddleEnemy"]["score"];
				}
				//ボス
				else if (enemyId == 4 + (engine.stageCnt - 1))
				{
					int count = 4 + (engine.stageCnt - 1);
					enemy->meshRenderer = enemy->AddComponent<MeshRenderer>();
					enemy->meshRenderer->mesh = engine.meshList[count];
					enemy->meshRenderer->scale = vec3(16.0f);
					enemy->meshRenderer->rotation.x = radians(90.0f);
					enemy->meshRenderer->rotation.y = radians(0.0f);

					auto enemyColor = enemy->AddComponent<Color>();
					enemyColor->MaterialColor(enemy->meshRenderer);

					auto damageSource = enemy->AddComponent<DamageSource>();
					damageSource->targetName = "player";
					damageSource->isOnce = false;
					warningsFlg = true;
					enemy->priority = prioLargeEnemyMachine;

					auto bossMove = enemy->AddComponent<BossAction>();
					bossMove->stageCnt = engine.stageCnt;
					bossMove->enemyColor = enemyColor;
					bossMove->target = player;
					bossMove->scoreManager = scoreManager;

					auto bossBullet = enemy->AddComponent<FireBullet>();
					bossBullet->enemy = enemy;
					bossBullet->timerA = 0.2f;	//最初の弾を出現後に発射させる
					bossBullet->target = player;
					if (engine.stageCnt != 6)bossBullet->intervalA = 0.2f;
					else bossBullet->intervalA = 0.9f;
					bossBullet->boundFlg = true;
					bossBullet->speed = 400.0f;
					score->score = engine.j["Boss"]["score"];

					BossBgm(engine);
					boss = enemy;
				}
				//小型機
				else
				{
					enemy->meshRenderer = enemy->AddComponent<MeshRenderer>();
					enemy->meshRenderer->mesh = engine.meshList[1];
					enemy->meshRenderer->scale = vec3(16);
					enemy->meshRenderer->rotation.x = radians(270.0f);
					enemy->meshRenderer->rotation.y = radians(180.0f);
					enemy->meshRenderer->translate.z = 10.0f;

					auto enemyColor = enemy->AddComponent<Color>();
					enemyColor->MaterialColor(enemy->meshRenderer);

					auto smallEnemy = enemy->AddComponent<SmallEnemyAction>();
					smallEnemy->player = player;
					smallEnemy->enemyColor = enemyColor;

					auto smallBullet = enemy->AddComponent<FireBullet>();
					smallBullet->enemy = enemy;
					smallBullet->timerA = 0.9f;	//最初の弾を出現後に発射させる
					smallBullet->target = player;
					smallBullet->speed = engine.j["SmallEnemy"]["bulletSpeed"];

					auto damageSource = enemy->AddComponent<DamageSource>();
					damageSource->targetName = "player";

					auto smallEnemyTransform = enemy->AddComponent<ScaleManager>();
					smallEnemyTransform->scale = 0.3f;
					enemy->y = 0.0f;

					auto health = enemy->AddComponent<Health>();
					health->health = engine.j["SmallEnemy"]["hp"];

					//敵と敵の影に画面外に出たときの処理を追加
					enemy->AddComponent<OutOfScreen>();

					//敵と影の優先順位
					smallEnemy->vy = 100.0f;
					score->score = engine.j["SmallEnemy"]["score"];
				}
			}
		} // for x
	} // for spawningY
}

//ステージ管理
void MainGameScene::CreateMap(Engine& engine)
{
	if (engine.stageCnt == 1)
	{
		//敵配置データを読み込む
		if (engine.debugMap)enemyMap = engine.LoadTileMap("Res/DebugBoss/DebugBoss_obj.csv", 0);
		else enemyMap = engine.LoadTileMap("Res/Stage/stage1_obj.csv", 0);
		spawningY = enemyMap.sizeY - 1; //敵出現ラインを初期化

	    //3Dモデル配置ファイルを読み込む
		auto gameObjectList = engine.LoadGameObjectMap("Res/3Dobj/Model/StageMap/Stage1/Stage1.json", vec3(0.0f, 0.0f, -300.0f), vec3(3.2f));

		//背景スクロールの設定
		bgOffset = std::make_shared<vec3>(0.0f);
		for (auto& e : gameObjectList)
		{
			auto c = e->AddComponent<BgScroll>();
			c->offset = bgOffset;
		}

	    //背景スプライトを作成(タイトルサイズ=32×32)
		TileMap tileMap = engine.LoadTileMap("Res/Stage/stage1_bg.csv", 0);
		engine.SetSpriteList(tileMap, 0);
	}
	else if (engine.stageCnt == 2)
	{
		//敵配置データを読み込む
		if (engine.debugMap)enemyMap = engine.LoadTileMap("Res/DebugBoss/DebugBoss2_obj.csv", 0);
		else enemyMap = engine.LoadTileMap("Res/Stage/stage2_obj.csv", 0);
		spawningY = enemyMap.sizeY - 1; //敵出現ラインを初期化

		//3Dモデル配置ファイルを読み込む
		auto gameObjectList = engine.LoadGameObjectMap("Res/3Dobj/Model/StageMap/Stage2/Stage2.json", vec3(0, 0, -300), vec3(3.2f));

		//背景スクロールの設定
		bgOffset = std::make_shared<vec3>(0.0f);
		for (auto& e : gameObjectList)
		{
			auto c = e->AddComponent<BgScroll>();
			c->offset = bgOffset;
		}

		//背景スプライトを作成(タイトルサイズ=32×32)
		TileMap tileMap = engine.LoadTileMap("Res/Stage/stage2_bg.csv", 0);
		engine.SetSpriteList(tileMap, 0);
	}
	else if (engine.stageCnt == 3)
	{
		//敵配置データを読み込む
		if (engine.debugMap)enemyMap = engine.LoadTileMap("Res/DebugBoss/DebugBoss3_obj.csv", 0);
		else enemyMap = engine.LoadTileMap("Res/Stage/stage3_obj.csv", 0);
		spawningY = enemyMap.sizeY - 1; //敵出現ラインを初期化

		//3Dモデル配置ファイルを読み込む
		auto gameObjectList = engine.LoadGameObjectMap("Res/3Dobj/Model/StageMap/Stage3/Stage3.json", vec3(0, 0, -300), vec3(3.2f));

		//背景スクロールの設定
		bgOffset = std::make_shared<vec3>(0.0f);
		for (auto& e : gameObjectList)
		{
			auto c = e->AddComponent<BgScroll>();
			c->offset = bgOffset;
		}

		//背景スプライトを作成(タイトルサイズ=32×32)
		TileMap tileMap = engine.LoadTileMap("Res/Stage/stage3_bg.csv", 0);
		engine.SetSpriteList(tileMap, 0);
	}
	else if (engine.stageCnt == 4)
	{
		//敵配置データを読み込む
		if (engine.debugMap)enemyMap = engine.LoadTileMap("Res/DebugBoss/DebugBoss4_obj.csv", 0);
		else enemyMap = engine.LoadTileMap("Res/Stage/stage4_obj.csv", 0);
		spawningY = enemyMap.sizeY - 1; //敵出現ラインを初期化

		//3Dモデル配置ファイルを読み込む
		auto gameObjectList = engine.LoadGameObjectMap("Res/3Dobj/Model/StageMap/Stage4/Stage4.json", vec3(0.0f, 0.0f, -300.0f), vec3(3.2f));

		//背景スクロールの設定
		bgOffset = std::make_shared<vec3>(0.0f);
		for (auto& e : gameObjectList)
		{
			auto c = e->AddComponent<BgScroll>();
			c->offset = bgOffset;
		}

		//背景スプライトを作成(タイトルサイズ=32×32)
		TileMap tileMap = engine.LoadTileMap("Res/stage/stage4_bg.csv", 0);
		engine.SetSpriteList(tileMap, 0);
	}
	else if (engine.stageCnt == 5)
	{
		//敵配置データを読み込む
		if (engine.debugMap)enemyMap = engine.LoadTileMap("Res/DebugBoss/DebugBoss5_obj.csv", 0);
		else enemyMap = engine.LoadTileMap("Res/Stage/stage5_obj.csv", 0);
		spawningY = enemyMap.sizeY - 1; //敵出現ラインを初期化

		//3Dモデル配置ファイルを読み込む
		auto gameObjectList = engine.LoadGameObjectMap("Res/3Dobj/Model/StageMap/Stage5/Stage5.json", vec3(0.0f, 0.0f, -300.0f), vec3(3.2f));

		//背景スクロールの設定
		bgOffset = std::make_shared<vec3>(0.0f);
		for (auto& e : gameObjectList)
		{
			auto c = e->AddComponent<BgScroll>();
			c->offset = bgOffset;
		}

		//背景スプライトを作成(タイトルサイズ=32×32)
		TileMap tileMap = engine.LoadTileMap("Res/Stage/stage5_bg.csv", 0);
		engine.SetSpriteList(tileMap, 0);
		TileMap tileMap1 = engine.LoadTileMap("Res/Stage/stage5_bg1.csv", 1);
		engine.SetSpriteList(tileMap1, 1, 0.5f);
	}
	else if (engine.stageCnt == 6)
	{
		engine.ClearSpriteList(1);

		//敵配置データを読み込む
		if (engine.debugMap)enemyMap = engine.LoadTileMap("Res/DebugBoss/DebugBoss6_obj.csv", 0);
		else enemyMap = engine.LoadTileMap("Res/Stage/stage6_obj.csv", 0);
		spawningY = enemyMap.sizeY - 1; //敵出現ラインを初期化

		//3Dモデル配置ファイルを読み込む
		auto gameObjectList = engine.LoadGameObjectMap("Res/3Dobj/Model/StageMap/Stage6/Stage6.json", vec3(0.0f, 0.0f, -300.0f), vec3(3.2f));

		//背景スクロールの設定
		bgOffset = std::make_shared<vec3>(0.0f);
		for (auto& e : gameObjectList)
		{
			auto c = e->AddComponent<BgScroll>();
			c->offset = bgOffset;
		}

		//背景スプライトを作成(タイトルサイズ=32×32)
		TileMap tileMap = engine.LoadTileMap("Res/Stage/stage6_bg.csv", 0);
		engine.SetSpriteList(tileMap, 0, 1);
		TileMap tileMap1 = engine.LoadTileMap("Res/Stage/stage6_bg1.csv", 1);
		tileMap1.priority = prioCloud;
		engine.SetSpriteList(tileMap1, 1, 0.7f);
	}
}

//ステージ情報ロゴの管理
void MainGameScene::CreateStageLogo(Engine& engine)
{
	const size_t logoLayer = engine.AddUILayer("Res/Images/Font/StageFont/stageFont.tga", GL_LINEAR, 10);
	stage_logo = engine.CreateUI<GameObject>(logoLayer, "stage_logo", 650.0f, 440.0f);
	stage_logo->AddSprite({ 0,0,1,1 });
	if (engine.stageCnt == 1)
	{
		const size_t numLayer = engine.AddUILayer("Res/Images/Font/StageFont/stage1.tga", GL_LINEAR, 10);
		stage_Num = engine.CreateUI<GameObject>(numLayer, "stage_logo", 820.0f, 800.0f);
		stage_Num->AddSprite({ 0,0,1,1 });
	}
	else if (engine.stageCnt == 2)
	{
		const size_t backNumLayer = engine.AddUILayer("Res/Images/Font/StageFont/stage1.tga", GL_LINEAR, 10);
		stage_BackNum = engine.CreateUI<GameObject>(backNumLayer, "stage_logo", 820.0f, 440.0f);
		stage_BackNum->AddSprite({ 0,0,1,1 });
		const size_t numLayer = engine.AddUILayer("Res/Images/Font/StageFont/stage2.tga", GL_LINEAR, 10);
		stage_Num = engine.CreateUI<GameObject>(numLayer, "stage_logo", 820.0f, 800.0f);
		stage_Num->AddSprite({ 0,0,1,1 });
	}
	else if (engine.stageCnt == 3)
	{
		const size_t backNumLayer = engine.AddUILayer("Res/Images/Font/StageFont/stage2.tga", GL_LINEAR, 10);
		stage_BackNum = engine.CreateUI<GameObject>(backNumLayer, "stage_logo", 820, 440);
		stage_BackNum->AddSprite({ 0,0,1,1 });

		const size_t numLayer = engine.AddUILayer("Res/Images/Font/StageFont/stage3.tga", GL_LINEAR, 10);
		stage_Num = engine.CreateUI<GameObject>(numLayer, "stage_logo", 820, 800);
		stage_Num->AddSprite({ 0,0,1,1 });
	}
	else if (engine.stageCnt == 4)
	{
		const size_t backNumLayer = engine.AddUILayer("Res/Images/Font/StageFont/stage3.tga", GL_LINEAR, 10);
		stage_BackNum = engine.CreateUI<GameObject>(backNumLayer, "stage_logo", 820, 440);
		stage_BackNum->AddSprite({ 0,0,1,1 });

		const size_t numLayer = engine.AddUILayer("Res/Images/Font/StageFont/stage4.tga", GL_LINEAR, 10);
		stage_Num = engine.CreateUI<GameObject>(numLayer, "stage_logo", 820, 800);
		stage_Num->AddSprite({ 0,0,1,1 });
	}
	else if (engine.stageCnt == 5)
	{
		const size_t backNumLayer = engine.AddUILayer("Res/Images/Font/StageFont/stage4.tga", GL_LINEAR, 10);
		stage_BackNum = engine.CreateUI<GameObject>(backNumLayer, "stage_logo", 820, 440);
		stage_BackNum->AddSprite({ 0,0,1,1 });

		const size_t numLayer = engine.AddUILayer("Res/Images/Font/StageFont/stage5.tga", GL_LINEAR, 10);
		stage_Num = engine.CreateUI<GameObject>(numLayer, "stage_logo", 820, 800);
		stage_Num->AddSprite({ 0,0,1,1 });
	}
	else if (engine.stageCnt == 6)
	{
		const size_t backNumLayer = engine.AddUILayer("Res/Images/Font/StageFont/stage5.tga", GL_LINEAR, 10);
		stage_BackNum = engine.CreateUI<GameObject>(backNumLayer, "stage_logo", 820, 440);
		stage_BackNum->AddSprite({ 0,0,1,1 });

		const size_t numLayer = engine.AddUILayer("Res/Images/Font/StageFont/stage6.tga", GL_LINEAR, 10);
		stage_Num = engine.CreateUI<GameObject>(numLayer, "stage_logo", 820, 800);
		stage_Num->AddSprite({ 0,0,1,1 });
	}
}

//ステージロゴのイージング
void MainGameScene::LogoEase_CurrentStage(GameObjectPtr& gameObject, float disy)
{
	if (!disFlg)
	{
		fontPosY = gameObject->y;

		disY = (disy - gameObject->y);
		disFlg = true;
	}
	gameObject->y = fontPosY + disY * Easing::EaseOutExpo(val);
}

//ロゴのイージング
void MainGameScene::LogoEase_PreviousStage(GameObjectPtr& gameObject, float disy)
{
	if (!disFlg1)
	{
		fontPosY1 = gameObject->y;

		disY1 = (disy - gameObject->y);
		disFlg1 = true;
	}
	gameObject->y = fontPosY1 + disY1 * Easing::EaseOutExpo(val);
}
//ステージロゴのアニメーション
void MainGameScene::StageLogoAnim(Engine& engine, float deltaTime)
{
	if (val < 1.0f)
	{
		if (engine.stageCnt != 6)
		{
			if (fade->alpha <= 0.0f) val += deltaTime / 2.0f;
		}
		else
		{
			if (fade1->alpha <= 0.0f) val += deltaTime / 2.0f;
		}
		LogoEase_CurrentStage(stage_Num, 440.0f);
	}
	else
	{
		stage_logo->alpha -= deltaTime;
		stage_Num->alpha -= deltaTime;
	}

	if (engine.stageCnt != 1)
	{
		if (val < 1.0f)
		{
			LogoEase_PreviousStage(stage_BackNum, -50.0f);
		}
		else
		{
			stage_BackNum->alpha -= deltaTime;
		}
	}
}

void MainGameScene::FadeIn(Engine& engine, float deltaTime)
{
	if (engine.stageCnt != 6)
	{
		if (fade->alpha > 0.0f && !engine.fadeFlg)fade->alpha -= deltaTime / 2.0f;
		else
		{
			engine.fadeFlg = true;
		}
	}
	else
	{
		if (fade1->alpha > 0.0f && !engine.fadeFlg)fade1->alpha -= deltaTime / 2.0f;
		else
		{
			engine.fadeFlg = true;
		}
	}
}

//プレイヤーのHP関連
void MainGameScene::HpBar(Engine& engine)
{
	float scale = 1.0f;
	float x = 182.0f;
	float y = 50.0f;
	float hpScale = 1.5f;

	//プレイヤーの残機
	if (engine.playerLife == 3)
	{
		const size_t playerLife = engine.AddUILayer("Res/Images/PlayerHp/Life3.tga", GL_LINEAR, 10);
		life = engine.CreateUI<GameObject>(playerLife, "Life", lifeX, lifeY);
		life->AddSprite({ 0, 0, 1, 1 }, 0, 0, hpScale, hpScale, hpScale);
	}
	else if (engine.playerLife == 2)
	{
		const size_t playerLife = engine.AddUILayer("Res/Images/PlayerHp/Life2.tga", GL_LINEAR, 10);
		life = engine.CreateUI<GameObject>(playerLife, "Life", lifeX, lifeY);
		life->AddSprite({ 0, 0, 1, 1 }, 0, 0, hpScale, hpScale, hpScale);
	}
	else if (engine.playerLife == 1)
	{
		const size_t playerLife = engine.AddUILayer("Res/Images/PlayerHp/Life1.tga", GL_LINEAR, 10);
		life = engine.CreateUI<GameObject>(playerLife, "Life", lifeX, lifeY);
		life->AddSprite({ 0, 0, 1, 1 }, 0, 0, hpScale, hpScale, hpScale);
	}
	else if (engine.playerLife == 0)
	{
		const size_t playerLife = engine.AddUILayer("Res/Images/PlayerHp/Life0.tga", GL_LINEAR, 10);
		life = engine.CreateUI<GameObject>(playerLife, "Life", lifeX, lifeY);
		life->AddSprite({ 0, 0, 1, 1 }, 0, 0, hpScale, hpScale, hpScale);
	}

	//HPバー
	const size_t playerHPGray = engine.AddUILayer("Res/Images/PlayerHp/HPGray.tga", GL_LINEAR, 10);
	hpBarGray = engine.CreateUI<GameObject>(playerHPGray, "HP", x, y);
	hpBarGray->AddSprite({ 0, 0, 1, 1 }, 0, 0, scale, scale, scale);

	const size_t playerHPRed = engine.AddUILayer("Res/Images/PlayerHp/HPRed.tga", GL_LINEAR, 10);
	hpBarRed = engine.CreateUI<GameObject>(playerHPRed, "HP", x, y);
	hpBarRed->AddSprite({ 0, 0, 1, 1 }, 0, 0, scale, scale, scale);

	const size_t playerHPGreen = engine.AddUILayer("Res/Images/PlayerHp/HPGreen.tga", GL_LINEAR, 10);
	hpBarGreen = engine.CreateUI<GameObject>(playerHPGreen, "HP", 0, y);
	hpBarGreen->AddSprite({ 0, 0, 1, 1 }, 0, 0, scale, scale, scale);

	hpBarGreen->x = static_cast<float>((player->health * 26) - 78);
	hpBarRed->x = hpBarGreen->x;

	const size_t playerHPGage = engine.AddUILayer("Res/Images/PlayerHp/HPGage.tga", GL_LINEAR, 10);
	hpBar = engine.CreateUI<GameObject>(playerHPGage, "HP", x, y);
	hpBar->AddSprite({ 0, 0, 1, 1 }, 0, 0, scale, scale, scale);
}
//HPのアニメーション
void MainGameScene::HpAnim(float deltaTime)
{
	const float x = 182.0f;
	const float y = 50.0f;
	const float amount = 5.0f;

	if (hpBarGreen->x < hpBarRed->x)
	{
		hpBarRed->x -= deltaTime * 20.0f;
	}

	if (hpAnimFlg)
	{
		switch (hpAnimNum)
		{
		case 0:
			Audio::PlayOneShot(SE::bossShot, Audio::GetMasterVolume());
			hpBar->x = x + amount;
			hpBar->y = y - amount;
			hpAnimNum = 1;
			break;
		case 1:
			hpBar->x = x - amount;
			hpBar->y = y + amount;
			hpAnimNum = 2;
			break;
		case 2:
			hpBar->x = x - amount;
			hpBar->y = y - amount;
			hpAnimNum = 3;
			break;
		case 3:
			hpBar->x = x + amount;
			hpBar->y = y + amount;
			hpAnimNum = 4;
			break;
		case 4:
			hpBar->x = x;
			hpBar->y = y;
			if(!player->isDead)hpAnimNum = 0;
			hpAnimFlg = false;
			break;
		}
	}
}
//ボスのBGM
void MainGameScene::BossBgm(Engine& engine)
{
	Audio::PlayOneShot(SE::warning, Audio::GetMasterVolume());
	if (engine.stageCnt == 1)
	{
		Audio::Play(AudioPlayer::bgm, BGM::Boss, Audio::GetMasterVolume(), true);
	}
	else if (engine.stageCnt == 2)
	{
		Audio::Play(AudioPlayer::bgm, BGM::Boss2, Audio::GetMasterVolume(), true);
	}
	else if (engine.stageCnt == 3)
	{
		Audio::Play(AudioPlayer::bgm, BGM::Boss3, Audio::GetMasterVolume(), true);
	}
	else if (engine.stageCnt == 4)
	{
		Audio::Play(AudioPlayer::bgm, BGM::Boss4, Audio::GetMasterVolume(), true);
	}
	else if (engine.stageCnt == 5)
	{
		Audio::Play(AudioPlayer::bgm, BGM::Boss5, Audio::GetMasterVolume(), true);
	}
	else if (engine.stageCnt == 6)
	{
		Audio::Play(AudioPlayer::bgm, BGM::Boss6, Audio::GetMasterVolume(), true);
	}

}
//ステージのBGM
void MainGameScene::StageBgm(Engine& engine)
{
	if (engine.stageCnt == 1)
	{
		Audio::Play(AudioPlayer::bgm, BGM::stage01, Audio::GetMasterVolume(), true);
	}
	else if (engine.stageCnt == 2)
	{
		Audio::Play(AudioPlayer::bgm, BGM::stage02, Audio::GetMasterVolume(), true);
	}
	else if (engine.stageCnt == 3)
	{
		Audio::Play(AudioPlayer::bgm, BGM::stage03, Audio::GetMasterVolume(), true);
	}
	else if (engine.stageCnt == 4)
	{
		Audio::Play(AudioPlayer::bgm, BGM::stage04, Audio::GetMasterVolume(), true);
	}
	else if (engine.stageCnt == 5)
	{
		Audio::Play(AudioPlayer::bgm, BGM::stage05, Audio::GetMasterVolume(), true);
	}
	else if (engine.stageCnt == 6)
	{
		Audio::Play(AudioPlayer::bgm, BGM::stage06, Audio::GetMasterVolume(), true);
	}
}
//操作説明
void MainGameScene::Operation(Engine& engine)
{
    const int keyup = engine.j["Key"]["up"];
	const int keydown = engine.j["Key"]["down"];
	const int keyright = engine.j["Key"]["right"];
	const int keyleft = engine.j["Key"]["left"];
	const int keyshot = engine.j["Key"]["shot"];

	const std::string strup = std::string("Res/Images/Operation/") + static_cast<char>(keyup) +".tga";
	const std::string strdown = std::string("Res/Images/Operation/") + static_cast<char>(keydown) + ".tga";
	const std::string strright = std::string("Res/Images/Operation/") + static_cast<char>(keyright) + ".tga";
	const std::string strleft = std::string("Res/Images/Operation/") + static_cast<char>(keyleft) + ".tga";
	const std::string strshot = std::string("Res/Images/Operation/") + static_cast<char>(keyshot) + ".tga";
	

	upup = strup;
	downdown = strdown;	
	rightright = strright;
	leftleft = strleft;
	shotshot = strshot;

	intup = keyup;


	const size_t operation_Down = engine.AddUILayer(strdown.c_str(), GL_LINEAR, 10);
	down = engine.CreateUI<GameObject>(operation_Down, "Operation", 250.0f);
	down->AddSprite({ 0, 0, 1, 1 });

	const size_t operation_Up = engine.AddUILayer(strup.c_str(), GL_LINEAR, 10);
	up = engine.CreateUI<GameObject>(operation_Up, "Operation", 250.0f);
	up->AddSprite({ 0, 0, 1, 1 });

	const size_t operation_Right = engine.AddUILayer(strright.c_str(), GL_LINEAR, 10);
	right = engine.CreateUI<GameObject>(operation_Right, "Operation", 305.0f);
	right->AddSprite({ 0, 0, 1, 1 });

	const size_t operation_Left = engine.AddUILayer(strleft.c_str(), GL_LINEAR, 10);
	left = engine.CreateUI<GameObject>(operation_Left, "Operation", 195.0f);
	left->AddSprite({ 0, 0, 1, 1 });

	if (keyshot == 32)
	{
		const size_t operation_Space = engine.AddUILayer("Res/Images/Operation/Space.tga", GL_LINEAR, 10);
		space = engine.CreateUI<GameObject>(operation_Space, "Operation", 250.0f);
		space->AddSprite({ 0, 0, 1, 1 });
	}
	else
	{
		const size_t operation_Space = engine.AddUILayer(strshot.c_str(), GL_LINEAR, 10);
		space = engine.CreateUI<GameObject>(operation_Space, "Operation", 250.0f);
		space->AddSprite({ 0, 0, 1, 1 });
	}

	const size_t textLayer = engine.AddUILayer("Res/Images/Font/FontList/font_05_2.tga", GL_NEAREST, 10);
	const char strOperation[] = "Operation";
	const float fontSizeX = 16.0f;
	const float operationX = 170.0f - static_cast<float>(std::size(strOperation) - 1.0f) * fontSizeX;
	auto uiOperation = engine.CreateUI<GameObject>(textLayer, "OPERATION", operationX, 550.0f);
	auto textOperation = uiOperation->AddComponent<Text>();
	textOperation->SetText(strOperation, 2.0f);

	const char strShot[] = "Shot";
	const float shotX = 80.0f - static_cast<float>(std::size(strShot) - 1.0f) * fontSizeX;
	auto uiShot = engine.CreateUI<GameObject>(textLayer, "SHOT", shotX, 380.0f);
	auto textShot = uiShot->AddComponent<Text>();
	textShot->SetText(strShot, 2.0f);

	const char strMove[] = "Move";
	const float moveX = 80.0f - static_cast<float>(std::size(strMove) - 1.0f) * fontSizeX;
	auto uiMove = engine.CreateUI<GameObject>(textLayer, "MOVE", moveX, 290.0f);
	auto textMove = uiMove->AddComponent<Text>();
	textMove->SetText(strMove, 2.0f);
}
//ポーズ画面
void MainGameScene::Pause(Engine& engine)
{
	//Escキーでポーズ画面
	if (engine.GetKey(GLFW_KEY_ESCAPE) && engine.efect == 1 && !pauseFlg)
	{
		engine.efect = 0.0f;
		fade->alpha = 0.3f;
		backToTitle->alpha = 1.0f;
	}
	if (engine.GetKey(GLFW_KEY_ESCAPE) && engine.efect == 0 && pauseFlg)
	{
		engine.efect = 1.0f;
		fade->alpha = 0.0f;
		backToTitle->alpha = 0.0f;
	}
	if (!engine.GetKey(GLFW_KEY_ESCAPE) && engine.efect == 1.0f)pauseFlg = false;
	if (!engine.GetKey(GLFW_KEY_ESCAPE) && engine.efect == 0.0f) pauseFlg = true;

	if (pauseFlg && engine.GetKey(GLFW_KEY_ENTER))
	{
		engine.efect = 1.0f;
		engine.SetNextScene<TitleScene>();
	}
}
//Imguiの表示
void MainGameScene::Imgui(Engine& engine)
{
	if (engine.j["ImguiSetFlg"]["MainGameScene"])
	{
		ImGui::Begin("Fade");
		if (ImGui::CollapsingHeader("fade"))
		{
			ImGui::Checkbox("fadeFlg", &engine.fadeFlg);
		}
		if (ImGui::CollapsingHeader("config"))
		{
			ImGui::Text(upup.c_str());
			ImGui::Text(downdown.c_str());
			ImGui::Text(rightright.c_str());
			ImGui::Text(leftleft.c_str());
			ImGui::Text(shotshot.c_str());
			ImGui::InputInt("up", &intup);
		}
		if (ImGui::CollapsingHeader("volume"))
		{
			ImGui::InputFloat("selectVolume", &engine.selectVolume);
			ImGui::InputFloat("standardVolume", &engine.volume);
			ImGui::DragFloat("getVolume", &getVolume);
		}
		if (ImGui::CollapsingHeader("playerCollision"))
		{
			ImGui::Checkbox("Collision = Checkbox + C_key + O_key", &playerCollision);
		}
		ImGui::End();
	}
}
