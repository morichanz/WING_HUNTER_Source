/**
* @file GameOverScene.h
*/
#ifndef GAMEOVERSCENE_H_INCLUDE
#define GAMEOVERSCENE_H_INCLUDE
#include"Scene.h"
#include"../Component/Score/ScoreManager.h"

/**
* クリアシーン
*/
class GameOverScene : public Scene
{
public:
	GameOverScene() = default;
	virtual ~GameOverScene() = default;
	virtual bool Initialize(Engine& engine)override;
	virtual void Update(Engine& engine, float deltaTime)override;
	void FadeOut(Engine& engine, float deltaTime);
	void RankLogo(Engine& engine);
	ScoreManagerPtr scoreManager;//スコア
	GameObjectPtr logo;

	GameObjectPtr fade;
	GameObjectPtr pressEnter;
	bool enterFlg = false;
	float maxTimer = 0.05f;
	float timer = 0;
	float fadeSpeed = 0.5f;
	bool fadeFlg = false;
};
#endif // !GAMEOVERSCENE_H_INCLUDE