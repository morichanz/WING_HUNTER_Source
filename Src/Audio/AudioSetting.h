/**
* @file AudioSetting.h
*/
#ifndef SUDIOSETTING_H_INCLUDE
#define SUDIOSETTING_H_INCLUDE

/**
* 音声再生プレイヤー
*/
namespace AudioPlayer
{
	inline constexpr int bgm = 0;	//BGMの再生に使うプレイヤー番号
	inline constexpr int se = 1;	//SEの再生に使うプレイヤー番号
}//namespace Player

/**
* BGM設定
*/
namespace BGM
{
	inline constexpr char title[] = "Res/Audio/title.mp3";	//タイトル画面のBGM
	inline constexpr char gameOver[] = "Res/Audio/gmaeOver.mp3";	//ゲームオーバー画面のBGM
	inline constexpr char gameClear[] = "Res/Audio/gameClear.mp3";	//ゲームクリア画面のBGM
	inline constexpr char stage01[] = "Res/Audio/play.mp3";	//ステージ1のBGM
	inline constexpr char stage02[] = "Res/Audio/stage02.mp3";	//ステージ2のBGM
	inline constexpr char stage03[] = "Res/Audio/stage03.mp3";	//ステージ3のBGM
	inline constexpr char stage04[] = "Res/Audio/stage04.mp3";	//ステージ4のBGM
	inline constexpr char stage05[] = "Res/Audio/stage05.mp3";	//ステージ5のBGM
	inline constexpr char stage06[] = "Res/Audio/stage06.mp3";	//ステージ6のBGM
	inline constexpr char Boss[] = "Res/Audio/BossBattle.mp3";	//ボスステージ1のBGM
	inline constexpr char Boss2[] = "Res/Audio/BossBattle2.mp3";	//ボスステージ2のBGM
	inline constexpr char Boss3[] = "Res/Audio/BossBattle3.mp3";	//ボスステージ3のBGM
	inline constexpr char Boss4[] = "Res/Audio/BossBattle4.mp3";	//ボスステージ4のBGM
	inline constexpr char Boss5[] = "Res/Audio/BossBattle5.mp3";	//ボスステージ5のBGM
	inline constexpr char Boss6[] = "Res/Audio/BossBattle6.mp3";	//ボスステージ6のBGM
}//namespace BGM

/**
* 効果音設定
*/
namespace SE
{
	inline constexpr char playerShot[] = "Res/Audio/shoot.mp3";	//弾の発射音
	inline constexpr char decision[] = "Res/Audio/decision.mp3";	//決定音
	inline constexpr char enemyShot[] = "Res/Audio/enemyShot.mp3";	//発射音
	inline constexpr char bossShot[] = "Res/Audio/bossShot1.mp3";	//着弾音
	inline constexpr char bossDamage[] = "Res/Audio/break.mp3";	//着弾音
	inline constexpr char weaponDamage[] = "Res/Audio/weaponDamage.mp3";	//武器ダメージ音
	inline constexpr char Beam[] = "Res/Audio/beam.mp3";	//ボスのレーザー音
	inline constexpr char BeamC[] = "Res/Audio/beamC.mp3";	//ボスのレーザー音
	inline constexpr char BeamS[] = "Res/Audio/beamS.mp3";	//ボスのレーザー音
	inline constexpr char playerExplosion[] = "Res/Audio/Explosion.mp3"; // 爆発(自機)
	inline constexpr char enemyExplosionS[] = "Res/Audio/Explosion.mp3"; // 爆発(小)
	inline constexpr char enemyExplosionM[] = "Res/Audio/Explosion.mp3"; // 爆発(中)
	inline constexpr char explosion[] = "Res/Audio/Explosion.mp3"; // 爆発(大)
	inline constexpr char warning[] = "Res/Audio/Warning.mp3"; //爆発(大)
}//namespace SE

#endif // !SUDIOSETTING_H_INCLUDE
