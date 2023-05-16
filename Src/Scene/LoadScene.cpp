/**
* @file LoadScene.cpp
*/
#include "LoadScene.h"
#include"MainGameScene.h"
#include"../Component/Text.h"
#include"../GameObject/GameObject.h"
#include"../Engine/Engine.h"
#include"../../Audio/EasyAudio.h"
#include"../../Audio/AudioSetting.h"
#include"ClearScene.h"
#include"Fade.h"
#include"../Component/MeshRenderer.h"
#include"../Component/Bullet/BulletMove.h"

void SetStatus(json j, Engine& engine);


const char* data[]
{
	"Res/3Dobj/Model/Large_Enemy/LargeEnemy.obj",	//0.��^�@
	"Res/3Dobj/enemy_s_01.obj",						//1.���^�@
	"Res/3Dobj/Model/Boss/Boss1/Boss1.obj",			//2	
	"Res/3Dobj/Model/Middle_Enemy/MiddleEnemy.obj", //3.���^�@
	"Res/3Dobj/Model/Boss/Boss1/Boss1.obj",			//4.�{�X�P
	"Res/3Dobj/Model/Boss/Boss2/Boss2.obj",			//5.�{�X2
	"Res/3Dobj/Model/Boss/Boss4/Boss4.obj",			//6.�{�X3
	"Res/3Dobj/Model/Boss/Boss3/Boss3.obj",			//7.�{�X4
	"Res/3Dobj/Model/Boss/Boss5/Boss5.obj",			//8.�{�X5
	"Res/3Dobj/Model/Boss/Boss6/Boss6.obj",			//9.�{�X6
	"Res/3Dobj/Model/Boss/Weapon/Weapon.obj",		//10.����i�t�@���l���j
	"Res/3Dobj/Model/Boss/Weapon/SingleWeapon/SingleWeapon1.obj"	//11.����
};

/**
* ���[�h��ʂ�����������
*
* @retval true	����������
* @retval false ���������s
*/
bool LoadScene::Initialize(Engine& engine)
{
	//�Q�[���I�u�W�F�N�g���폜
	engine.ClearGameObjectList();
	engine.ClearUILayers();

	//UI���C���[���쐬
	const size_t bgLayer = engine.AddUILayer("Res/Images/GameOver.tga", GL_LINEAR, 10);
	const size_t textLayer = engine.AddUILayer("Res/Images/Font/FontList/font_05_2.tga", GL_LINEAR, 10);
	const size_t dialLayer = engine.AddUILayer("Res/Images/LoadAnim/Dial2.tga", GL_LINEAR, 10);
	const size_t hour_handLayer = engine.AddUILayer("Res/Images/LoadAnim/Hour_Hand.tga", GL_LINEAR, 10);
	const size_t long_handLayer = engine.AddUILayer("Res/Images/LoadAnim/Long_Hand.tga", GL_LINEAR, 10);

	//�w�i�摜
	auto uiBackGround = engine.CreateUI<GameObject>(bgLayer, "Loading_bg", 640, 360);
	uiBackGround->AddSprite({ 0,0,1,1 });

	//�Q�[���̊J�n���@�������e�L�X�g
	const char strNowLoading[] = "now Loading";
	const float fontSizeX = 16;
	const float x = 740 - static_cast<float>(std::size(strNowLoading) - 1) * fontSizeX;
	auto uiNowLoading = engine.CreateUI<GameObject>(textLayer, "now Loading", x, 50);
	auto textNowLoading = uiNowLoading->AddComponent<Text>();
	textNowLoading->SetText(strNowLoading, 2);

	Clock(engine, dialLayer, hour_handLayer, long_handLayer);

	//�t�F�[�h�摜
	const size_t alphaLayer = engine.AddUILayer("Res/Images/Fade_bg.tga", GL_LINEAR, 10);
	fade = engine.CreateUI<GameObject>(alphaLayer, "Fade_bg", 640, 360);
	fade->AddSprite({ 0,0,1,1 });
	fade->alpha = 1;

	//�^�C�g��BGM�̍Đ�
	Audio::Stop(AudioPlayer::bgm);

	SetStatus(engine.j,engine);
	return true;	//����������
}

/**
* ���[�h��ʂ̏�Ԃ��X�V����
*/
void LoadScene::Update(Engine& engine, float deltaTime)
{
	FadeOut(engine, deltaTime);

	//���[�h���I���΃Q�[���J�n
	if (roadFlg)
	{
		engine.score = engine.scoreBack;
		fadeFlg = true;
	}

	if (!roadFlg && fadeInFlg)
	{
		//���b�V�����X�g�̗v�f�����ǂݍ��񂾓ǂݍ��ݍς݂̗v�f���Ɠ����Ȃ�
		if (engine.meshList.size() == listSize)
		{
			//�G���W���̃��b�V�����X�g�ɃI�u�W�F��ǂݍ���
			engine.meshList.push_back(engine.LoadOBJ(data[engine.meshList.size()]));

			//���ꂼ��̐j�̈ړ��̈���v�Z����
			long_hand_Back = (engine.meshList.size()+ 1) * (360.0f / std::size(data));
			long_hand_Back_back = long_hand_Back * -1;
			hour_hand_Back -= (30.0f / std::size(data));
		}
		else
		{
			//deltaTime�����ȉ��Ȃ�j�̏�Ԃ��X�V����
			if (deltaTime < 0.2f)
			{
				if (hour_hand_Back < hour_hand->rotate)hour_hand->rotate -= 5.0f * deltaTime;
				if (long_hand_Back_back < long_hand->rotate && long_hand->rotate > -360.0f)long_hand->rotate -= 30.0f * deltaTime;

				//�j�̈ړ��̍X�V���I������烊�X�g�̗v�f�����X�V����
				if (long_hand_Back_back >= long_hand->rotate && hour_hand_Back >= hour_hand->rotate)listSize = engine.meshList.size();
			}
		}
		if (std::size(data) == engine.meshList.size())roadFlg = true;
	}
}

void LoadScene::FadeOut(Engine& engine, float deltaTime)
{
	if (!fadeFlg)
	{
		if (fade->alpha > 0)fade->alpha -= deltaTime * 2;
		else fadeInFlg = true;
	}
	else
	{
		fade->alpha += deltaTime / 2;
		if (fade->alpha > 1)engine.SetNextScene<MainGameScene>();
	}
}

void LoadScene::Clock(Engine& engine, size_t dialLayer, size_t hour_handLayer, size_t long_handLayer)
{
	//�Q�[���̊J�n���@�������e�L�X�g�摜
	dial = engine.CreateUI<GameObject>(dialLayer, "nowLoading_logo", x, y);
	dial->AddSprite({ 0,0,1,1 }, sprite_x);

	//�Q�[���̊J�n���@�������e�L�X�g�摜
	hour_hand = engine.CreateUI<GameObject>(hour_handLayer, "nowLoading_logo", x, y);
	hour_hand->AddSprite({ 0,0,1,1 }, sprite_x);
	hour_hand->rotate = 30.0f;

	//�Q�[���̊J�n���@�������e�L�X�g�摜
	long_hand = engine.CreateUI<GameObject>(long_handLayer, "nowLoading_logo", x, y);
	long_hand->AddSprite({ 0,0,1,1 }, sprite_x);
}

void SetStatus(json j, Engine& engine)
{
	engine.score = 0;
	engine.stageCnt = j["Stage"]["count"];
	engine.playerHp = j["Player"]["hp"];
	engine.playerLife = j["Player"]["life"];
}