/**
* @file LoadScene.h
*/
#ifndef LOADSCENE_H_INCLUDE
#define LOADSCENE_H_INCLUDE
#include"Scene.h"
#include<string>

/**
* タイトルシーン
*/
class LoadScene : public Scene
{
public:
	LoadScene() = default;
	virtual ~LoadScene() = default;
	virtual bool Initialize(Engine& engine)override;
	virtual void Update(Engine& engine, float deltaTime)override;
	void FadeOut(Engine& engine, float deltaTime);
	void Clock(Engine& engine, size_t dialLayer,size_t hour_handLayer,size_t long_handLayer);

	GameObjectPtr fade;
	GameObjectPtr dial;
	GameObjectPtr long_hand;
	GameObjectPtr hour_hand;

	bool enterFlg = false;
	bool fadeFlg = false;
	bool disFlg = false;
	float fontPosX = 0;
	float disX = 0;
	float val = 0;
	float loadSpeed = 10;
	float x = 640;
	float y = 140;
	float sprite_x = 450;
	bool roadFlg = false;
	float fadeInFlg = false;
	
	std::size_t listSize;
	float long_hand_Back = 0;
	float long_hand_Back_back = 0;
	float hour_hand_Back = 30;
};
#endif // !LOADSCENE_H_INCLUDE