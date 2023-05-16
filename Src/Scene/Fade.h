/**
* @file Fade.h
*/
#ifndef FADE_H_INCLUDE
#define FADE_H_INCLUDE
#include"Scene.h"

/**
* タイトルシーン
*/
class Fade : public Scene
{
public:
	Fade() = default;
	virtual ~Fade() = default;
	virtual bool Initialize(Engine& engine)override;
	virtual void Update(Engine& engine, float deltaTime)override;


	GameObjectPtr fade;
	GameObjectPtr fade1;
};
#endif // !TITLESCENE_H_INCLUDE
