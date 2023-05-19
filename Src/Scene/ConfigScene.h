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
	bool key;		//��u��select�̒l���ς��Ȃ��悤��(�P���ύX�����悤��)����
	bool keySet;	//�L�[�̕ύX��������
	int keyup;		//key�̔ԍ����g����key�摜��`��
	int keydown;	//key�̔ԍ����g����key�摜��`��
	int keyright;	//key�̔ԍ����g����key�摜��`��
	int keyleft;	//key�̔ԍ����g����key�摜��`��
	int keyshot;	//key�̔ԍ����g����key�摜��`��
	int keyMemory;	//���ɑ��݂���key���L�����Ă���
};

struct VolumeConnection
{
	size_t SetTextLayer;
	std::shared_ptr<Text> textnum;
	GameObjectPtr uiNum;
};

/**
* �ݒ�V�[��
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

	bool enterFlg = false;  //PressEnter�̃t�F�[�h�Ǘ�
	bool fadeFlg = false;	//�t�F�[�h�J�n�t���O
	bool titleAnim = false; //�t�F�[�h�C�����I����Ă���^�C�g���̃A�j���[�V�������J�n����
	bool SceneChange = false;
	bool disFlg[10] = { false };
	int select = 0;
	float getVolume = 0.0f;
	bool isEnable = false;
 	float timer = 0.0f;		//PressEnter���`�J�`�J������c�莞��
	float maxTimer = 0.05f;	//PressEnter���`�J�`�J�����鎞��
	float pressEnter_alphaSpeed = 2;	//pressEnter�̕\�����x

	float selectTimer = 0.0f; //�^�C�g������V�[�����ړ������Ƃ��ɏ����keySet��true�ɂȂ�Ȃ��悤�ɂ���
	float defaultTimer = 0.1f;

	float selectBox_x = 200.0f; //�L�[���������Ƃ���selectBox���ړ�������
	float selectBox_y = 70.0f; //�L�[���������Ƃ���selectBox���ړ�������

	int castKeyNum = 0;

	bool backToTitleAnim = false;
	float resetAnimTimer = 0.0f;

	bool resetAnimFlg = false;

	KeyConnection key;
	VolumeConnection volume;
};
#endif // !CONFIGSCENE_H_INCLUDE
