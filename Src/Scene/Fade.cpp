/*
* @file Fade.cpp
*/
#include"Fade.h"
#include"../GameObject/GameObject.h"
#include"ClearScene.h"
#include"GameOverScene.h"
#include"../Engine/Engine.h"
#include"MainGameScene.h"
#include"ClearScene.h"
#include"TitleScene.h"

bool Fade::Initialize(Engine& engine)
{
	//UI���C���[���쐬
	const size_t bgLayer = engine.AddUILayer("Res/Images/Fade_bg.tga", GL_LINEAR, 10);

	//�w�i�摜
	fade = engine.CreateUI<GameObject>(bgLayer, "Fade_bg", 640.0f, 360.0f);
	fade->AddSprite({ 0,0,1,1 });
	fade->alpha = 0.0f;

	//UI���C���[���쐬
	const size_t bgLayer1 = engine.AddUILayer("Res/Images/Fade_White_bg.tga", GL_LINEAR, 10);

	//�w�i�摜
	fade1 = engine.CreateUI<GameObject>(bgLayer1, "Fade_White_bg", 640.0f, 360.0f);
	fade1->AddSprite({ 0,0,1,1 });
	fade1->alpha = 0.0f;

	return true; // ����������
}

void Fade::Update(Engine& engine, float deltaTime)
{
		if (engine.interval <= 0.0f) fade->alpha += deltaTime / 2.0f;
		else engine.interval -= deltaTime;

		if (fade->alpha >= 1.0f)
		{
			engine.interval = 200.0f;
			if (engine.fadeNum == 0) engine.SetNextScene<MainGameScene>();
			else if(engine.fadeNum == 1)
			{
				engine.stageCnt = 1;
				engine.SetNextScene<TitleScene>();
			}
			else if(engine.fadeNum == 2)
			{
				engine.stageCnt = 1;
				engine.SetNextScene<GameOverScene>();
			}
		}
}