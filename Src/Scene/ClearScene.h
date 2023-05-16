/**
* @file ClearScene.h
*/
#ifndef CLEARSCENE_H_INCLUDE
#define CLEARSCENE_H_INCLUDE
#include"Scene.h"

class ScoreManager;
using ScoreManagerPtr = std::shared_ptr<ScoreManager>;

/**
* クリアシーン
*/
class ClearScene : public Scene
{
public:
	ClearScene() = default;
	virtual ~ClearScene() = default;
	virtual bool Initialize(Engine& engine)override;
	virtual void Update(Engine& engine, float deltaTime)override;
	void RankLogo(Engine& engine);
	void FadeOut(Engine& engine, float deltaTime);

	ScoreManagerPtr scoreManager;//スコア
	GameObjectPtr clear;
	GameObjectPtr fade;
	GameObjectPtr pressEnter;
	bool enterFlg = false;
	float timer = 0.05f;
	bool fadeFlg = false;
};
#endif // !CLEARSCENE_H_INCLUDE
