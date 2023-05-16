/**
* @file ConfigScene.cpp
*/
#include "ConfigScene.h"
#include"MainGameScene.h"
#include"../Component/Text.h"
#include"../GameObject/GameObject.h"
#include"../Engine/Engine.h"
#include"../../Audio/EasyAudio.h"
#include"../../Audio/AudioSetting.h"
#include "../Component/MeshRenderer.h"
#include"ClearScene.h"
#include"Fade.h"
#include"../VecMath/VecMath.h"
#include"TitleScene.h"
#include"../Easing.h"
#include<fstream>
#include "../../../ImGui/ImGui/imgui.h"
#include "../../../ImGui/ImGui/imgui_impl_opengl3.h"
#include "../../../ImGui/ImGui/imgui_impl_glfw.h"
#include "../Debug/Debug.h"

const std::string pathToJSON("data/file.json");

const char* number[]
{
	"0",	//0.大型機
	"1",						//1.小型機
	"2",			//2	
	"3", //3.中型機
	"4",			//4.ボス１
	"5",			//5.ボス2
	"6",			//6.ボス3
	"7",			//7.ボス4
	"8",			//8.ボス5
	"9",			//9.ボス6
	"10",		//10.武器（ファンネル）
};

const char* keyName[]
{
	"up",
	"down",
	"right",
	"left",
	"shot",
};


enum SettingName
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
	SHOT,
	RESET,
	VOLUME,
	TITLEBACK
};

bool ConfigScene::Initialize(Engine& engine)
{
	//UIレイヤーを作成
	const size_t bgLayer = engine.AddUILayer("Res/Images/Title_bg.tga", GL_LINEAR, 10);
	const size_t backLayer = engine.AddUILayer("Res/Images/Fade_bg.tga", GL_LINEAR, 10);
	const size_t textLayer = engine.AddUILayer("Res/Images/Font/FontList/font_05_2.tga", GL_LINEAR, 10);
	const size_t selectLayer = engine.AddUILayer("Res/Images/Config/Select.tga", GL_LINEAR, 10);
	const size_t configLayer = engine.AddUILayer("Res/Images/TitleAnim/Config_logo.tga", GL_LINEAR, 10);
	const size_t leftArrowLayer = engine.AddUILayer("Res/Images/Config/leftArrow.tga", GL_LINEAR, 10);
	const size_t enterNewKeyLayer = engine.AddUILayer("Res/Images/Config/EnterNewKey.tga", GL_LINEAR, 10);
	const size_t resetLayer = engine.AddUILayer("Res/Images/Config/Reset.tga", GL_LINEAR, 10);
	const size_t upArrowLayer = engine.AddUILayer("Res/Images/Config/upArrow.tga", GL_LINEAR, 10);
	const size_t downArrowLayer = engine.AddUILayer("Res/Images/Config/downArrow.tga", GL_LINEAR, 10);
	SetTextLayer = textLayer;

	ConfigText(engine, textLayer);
	VolumeNumImage(engine, textLayer);

	//ゲームの開始方法を示すテキスト画像
	enterNewKey = engine.CreateUI<GameObject>(enterNewKeyLayer, "enterNewKey_logo", 900.0f + selectBox_x, 160.0f);
	enterNewKey->AddSprite({ 0,0,1,1 });
	enterNewKey->alpha = 0;

	//キー設定リセットのロゴ
	reset = engine.CreateUI<GameObject>(resetLayer, "reset_logo", 900.0f, 550.0f - (selectBox_y * RESET));
	reset->AddSprite({ 0,0,1,1 });
	reset->alpha = 0;

	//ゲームの開始方法を示すテキスト画像
	auto config = engine.CreateUI<GameObject>(configLayer, "config_logo", 640.0f, 650.0f);
	config->AddSprite({ 0,0,1,1 }, 0, 0, 2, 2, 2);


	//背景(黒)
	auto back = engine.CreateUI<GameObject>(backLayer, "back_logo", 640.0f, 360.0f);
	back->AddSprite({ 0,0,1,1 });
	back->alpha = 0.5f;

	//背景画像
	auto uiBackGround = engine.CreateUI<GameObject>(bgLayer, "Title_bg", 640.0f, 360.0f);
	uiBackGround->AddSprite({ 0,0,1,1 });

	//選択ボックス
	selectBox = engine.CreateUI<GameObject>(selectLayer, "SelectBox", 900.0f, 550.0f);
	selectBox->AddSprite({ 0,0,1,1 });

	//タイトル選択矢印
	leftArrow = engine.CreateUI<GameObject>(leftArrowLayer, "backToTile", 900.0f, 60.0f);
	leftArrow->AddSprite({ 0,0,1,1 });
	leftArrow->alpha = 0;

	//上矢印
	upArrow = engine.CreateUI<GameObject>(upArrowLayer, "upArrow", 1100.0f, 550.0f - (selectBox_y * VOLUME));
	upArrow->AddSprite({ 0,0,1,1 });
	upArrow->alpha = 0;

	//下矢印
	downArrow = engine.CreateUI<GameObject>(downArrowLayer, "downArrow", 1100.0f, 550.0f - (selectBox_y * VOLUME));
	downArrow->AddSprite({ 0,0,1,1 });
	downArrow->alpha = 0;

	selectTimer = defaultTimer;

	//キーリストにそれぞれのキーを割り当てる
	keylist[0] = engine.j["Key"]["up"];
	keylist[1] = engine.j["Key"]["down"];
	keylist[2] = engine.j["Key"]["right"];
	keylist[3] = engine.j["Key"]["left"];
	keylist[4] = engine.j["Key"]["shot"];

	ConfigKeyImage(engine);

	//保存されているvolumeを設定する
	engine.selectVolume = engine.j["Audio"]["selectVolume"];
	Audio::SetMasterVolume(engine.volume * engine.selectVolume);

	getVolume = Audio::GetMasterVolume();

	return true;
}

void ConfigScene::Update(Engine& engine, float deltaTime)
{
	if (selectTimer > 0 && !engine.GetKey(GLFW_KEY_ENTER)) selectTimer -= deltaTime;
	Imgui(engine);
	Select(engine);
	KeySetting(engine,deltaTime);
	VolumeSetting(engine,deltaTime);
	BackToTitle(engine, deltaTime);
	ResetAnimImage(engine, deltaTime);
	VolumeArrowImage(engine, deltaTime);
}

//GetAnyKeyDownで押されたキーは何なのかを番号で返す
int ConfigScene::GetKeyCoad(Engine& engine)
{
	for (auto itr : engine.j["KeyList"])
	{
		if (engine.GetKey(itr))
		{
			castKeyNum = itr;
			return itr;
		}
	}
}
//何かのキーを押したときにtrueを返す
bool ConfigScene::GetAnyKeyDown(Engine& engine)
{
	for (auto& itr : engine.j["KeyList"])
	{
		if(engine.GetKey(itr))return true;
	}
	return false;
}
//Imguiの表示
void ConfigScene::Imgui(Engine& engine)
{
	ImGui::Begin("key");
	if (ImGui::CollapsingHeader("key"))
	{
		ImGui::DragInt("key", &castKeyNum);
		ImGui::DragFloat("selectTimer", &selectTimer);
		ImGui::DragInt("select", &select);
		ImGui::Checkbox("isEnable", &isEnable);
		ImGui::Checkbox("keySet", &keySet);
	}
	if (ImGui::CollapsingHeader("keylist"))
	{
		ImGui::DragInt("keybindList0 up", &keylist[0]);
		ImGui::DragInt("keybindList1 down", &keylist[1]);
		ImGui::DragInt("keybindList2 right", &keylist[2]);
		ImGui::DragInt("keybindList3 left", &keylist[3]);
		ImGui::DragInt("keybindList4 shot", &keylist[4]);
		ImGui::Text("text", static_cast<char>(castKeyNum));
	}
	if (ImGui::CollapsingHeader("SetVolume"))
	{
		ImGui::DragInt("selectVolume", &engine.selectVolume);
		ImGui::DragFloat("selectVolume", &getVolume);
	}
	if (ImGui::CollapsingHeader("Select"))
	{
		ImGui::DragFloat("select", &selectBox->y);
	}
	ImGui::End();
}
//設定の選択
void ConfigScene::Select(Engine& engine)
{
	const bool upKey = engine.GetKey(GLFW_KEY_UP);
	const bool downKey = engine.GetKey(GLFW_KEY_DOWN);
	if (!fadeFlg && !keySet)
	{
		if (select < TITLEBACK && downKey && !key)
		{
			select++;
			selectBox->y -= selectBox_y;
			key = true;
		}
		else if (select > UP && upKey && !key)
		{
			select--;
			selectBox->y += selectBox_y;
			key = true;
		}

		if (!upKey && !downKey)key = false;
	}
}
//キーの変更を設定
void ConfigScene::KeySetting(Engine& engine, float deltaTime)
{
	if (engine.GetKey(GLFW_KEY_ENTER) && selectTimer <= 0.0f && select < TITLEBACK && !keySet)
	{
		selectTimer = defaultTimer;
		resetAnimTimer = 1.0f;
		if (select != RESET && select != TITLEBACK)
		{
			keyMemory = keylist[select];
			selectBox->x += selectBox_x;
			if(select != VOLUME)enterNewKey->alpha = 1.0f;
			enterNewKey->y = selectBox->y;
			keySet = true;
		}
	}

	if (keySet && !engine.GetKey(GLFW_KEY_ENTER) && GetAnyKeyDown(engine) && select != VOLUME)
	{
		if (!engine.GetKey(GLFW_KEY_UP) && !engine.GetKey(GLFW_KEY_DOWN))
		{
			if(GetKeyCoad(engine) < 100)
			{
				//キー設定したときに同じキーが他で設定されていたら
				//キーを入れ替える
				for (int i = 0; i < 5; i++)
				{
					if (GetKeyCoad(engine) == keylist[i] && GetKeyCoad(engine) != 32)
					{
						keylist[i] = keyMemory;
						engine.j["Key"][keyName[i]] = keylist[i];
					}
				}

				if (GetKeyCoad(engine) != 32 && isalpha(GetKeyCoad(engine)))
				{
					if (select == UP)
					{
						engine.j["Key"]["up"] = GetKeyCoad(engine);
						keylist[select] = GetKeyCoad(engine);
					}
					else if (select == DOWN)
					{
						engine.j["Key"]["down"] = GetKeyCoad(engine);
						keylist[select] = GetKeyCoad(engine);
					}
					else if (select == RIGHT)
					{
						engine.j["Key"]["right"] = GetKeyCoad(engine);
						keylist[select] = GetKeyCoad(engine);
					}
					else if (select == LEFT)
					{
						engine.j["Key"]["left"] = GetKeyCoad(engine);
						keylist[select] = GetKeyCoad(engine);
					}
				}
				if(select == SHOT)
				{
					//スペースなら代入
					if (GetKeyCoad(engine) == 32)
					{
						engine.j["Key"]["shot"] = GetKeyCoad(engine);
						keylist[select] = GetKeyCoad(engine);
					}
					//スペース以外でかつ文字じゃない場合は代入
					else if (isalpha(GetKeyCoad(engine)))
					{
						engine.j["Key"]["shot"] = GetKeyCoad(engine);
						keylist[select] = GetKeyCoad(engine);
					}
				}
				ConfigKeyImage(engine);
				selectBox->x -= selectBox_x;
				enterNewKey->alpha = 0.0f;
				keySet = false;
			}
		}
	}
	if (engine.GetKey(GLFW_KEY_ENTER) && select == RESET)
	{
		resetAnimFlg = true;
		reset->alpha = 1;
		engine.j["Key"]["up"] = engine.j["KeyList"]["GLFW_KEY_ALPHABET_W"];
		engine.j["Key"]["down"] = engine.j["KeyList"]["GLFW_KEY_ALPHABET_S"];
		engine.j["Key"]["right"] = engine.j["KeyList"]["GLFW_KEY_ALPHABET_D"];
		engine.j["Key"]["left"] = engine.j["KeyList"]["GLFW_KEY_ALPHABET_A"];
		engine.j["Key"]["shot"] = engine.j["KeyList"]["GLFW_KEY_SPACE"];

		keylist[0] = engine.j["Key"]["up"];
		keylist[1] = engine.j["Key"]["down"];
		keylist[2] = engine.j["Key"]["right"];
		keylist[3] = engine.j["Key"]["left"];
		keylist[4] = engine.j["Key"]["shot"];
		ConfigKeyImage_Reset(engine);
	}
}
//音量設定
void ConfigScene::VolumeSetting(Engine& engine, float deltaTime)
{
	//十字キーでvolumeを設定
	if (select == VOLUME && keySet)
	{
		const bool upKey = engine.GetKey(GLFW_KEY_UP);
		const bool downKey = engine.GetKey(GLFW_KEY_DOWN);
		if (engine.selectVolume < 10 && downKey && !key)
		{
			upArrow->alpha = 0.0f;
			downArrow->alpha = 1.0f;
			engine.selectVolume++;
			VolumeNumImage(engine, SetTextLayer);
			key = true;
		}
		else if (engine.selectVolume > 0 && upKey && !key)
		{
			downArrow->alpha = 0.0f;
			upArrow->alpha = 1.0f;
			engine.selectVolume--;
			VolumeNumImage(engine, SetTextLayer);
			key = true;
		}
		if (!upKey && !downKey)key = false;
		
		Audio::SetMasterVolume(engine.volume * engine.selectVolume);
	}
	
	//エンターキーで設定を保存
	if (engine.GetKey(GLFW_KEY_ENTER) && selectTimer < 0.0f)
	{
		engine.j["Audio"]["selectVolume"] = engine.selectVolume;
		getVolume = Audio::GetMasterVolume();
		selectTimer = defaultTimer;
		if (select != TITLEBACK)
		{
			selectBox->x -= selectBox_x;
		}
		enterNewKey->alpha = 0.0f;
		keySet = false;
	}
}
//タイトルに戻る
void ConfigScene::BackToTitle(Engine& engine, float deltaTime)
{
	//選択カーソルがTitleBackになっているとき
	if (select == TITLEBACK)
	{
		leftArrow->alpha = 1.0f;
		//矢印を移動させる
		if (leftArrow->x < 895.0f)backToTitleAnim = true;
		else if (leftArrow->x > 905.0f) backToTitleAnim = false;
		if (!backToTitleAnim)leftArrow->x -= deltaTime * 20.0f;
		else leftArrow->x += deltaTime * 20.0f;
	}
	else leftArrow->alpha = 0.0f;

	//タイトルに戻る場合
	if (engine.GetKey(GLFW_KEY_ENTER) && select == TITLEBACK)
	{
		const int target = 0;
		bool exists = std::find(std::begin(keylist), std::end(keylist), target) != std::end(keylist);
		if (!exists)
		{
			//設定を保存してタイトルに戻る
			//jsonファイル書き出し
			std::ofstream ofs(pathToJSON.c_str());
			if (ofs.good())
			{
				ofs << engine.j.dump(3) << std::endl;
				ofs.close();
			}
			engine.SetNextScene<TitleScene>();
		}
	}
}
//設定テキスト
void ConfigScene::ConfigText(Engine& engine, const size_t textLayer)
{
	//設定項目を示すテキスト
	const float fontSizeX = 16.0f;
	const float fontposY = 520.0f;
	const float x = 640.0f;

	const char strup[] = "up";
	const float upX = x - static_cast<float>(std::size(strup) - 1.0f) * fontSizeX;
	auto uiup = engine.CreateUI<GameObject>(textLayer, "up", upX, fontposY);
	auto textup = uiup->AddComponent<Text>();
	textup->SetText(strup, 2.0f);

	const char strdown[] = "down";
	const float downX = x - static_cast<float>(std::size(strdown) - 1.0f) * fontSizeX;
	auto uidown = engine.CreateUI<GameObject>(textLayer, "down", downX, fontposY - selectBox_y);
	auto textdown = uidown->AddComponent<Text>();
	textdown->SetText(strdown, 2.0f);

	const char strright[] = "right";
	const float rightX = x - static_cast<float>(std::size(strright) - 1.0f) * fontSizeX;
	auto uiright = engine.CreateUI<GameObject>(textLayer, "right", rightX, fontposY - (selectBox_y * 2));
	auto textright = uiright->AddComponent<Text>();
	textright->SetText(strright, 2.0f);

	const char strleft[] = "left";
	const float leftX = x - static_cast<float>(std::size(strleft) - 1.0f) * fontSizeX;
	auto uileft = engine.CreateUI<GameObject>(textLayer, "left", leftX, fontposY - (selectBox_y * 3));
	auto textleft = uileft->AddComponent<Text>();
	textleft->SetText(strleft, 2.0f);

	const char strshot[] = "shot";
	const float shotX = x - static_cast<float>(std::size(strshot) - 1.0f) * fontSizeX;
	auto uishot = engine.CreateUI<GameObject>(textLayer, "shot", shotX, fontposY - (selectBox_y * 4));
	auto textshot = uishot->AddComponent<Text>();
	textshot->SetText(strshot, 2.0f);

	const char strreset[] = "reset";
	const float resetX = x - static_cast<float>(std::size(strreset) - 1.0f) * fontSizeX;
	auto uireset = engine.CreateUI<GameObject>(textLayer, "reset", resetX, fontposY - (selectBox_y * 5));
	auto textreset = uireset->AddComponent<Text>();
	textreset->SetText(strreset, 2.0f);

	const char strvolume[] = "volume";
	const float volumeX = x - static_cast<float>(std::size(strvolume) - 1.0f) * fontSizeX;
	auto uivolume = engine.CreateUI<GameObject>(textLayer, "volume", volumeX, fontposY - (selectBox_y * 6));
	auto textvolume = uivolume->AddComponent<Text>();
	textvolume->SetText(strvolume, 2.0f);

	const char strtitleBack[] = "titleBack";
	const float titleBackX = x - static_cast<float>(std::size(strtitleBack) - 1.0f) * fontSizeX;
	auto uititleBack = engine.CreateUI<GameObject>(textLayer, "titleBack", titleBackX, fontposY - (selectBox_y * 7));
	auto texttitleBack = uititleBack->AddComponent<Text>();
	texttitleBack->SetText(strtitleBack, 2.0f);
}
//キー設定の画像
void ConfigScene::ConfigKeyImage(Engine& engine)
{
	keyup = engine.j["Key"]["up"];
	keydown = engine.j["Key"]["down"];
	keyright = engine.j["Key"]["right"];
	keyleft = engine.j["Key"]["left"];
	keyshot = engine.j["Key"]["shot"];

	const std::string strup = std::string("Res/Images/Operation/") + static_cast<char>(keyup) + ".tga";
	const std::string strdown = std::string("Res/Images/Operation/") + static_cast<char>(keydown) + ".tga";
	const std::string strright = std::string("Res/Images/Operation/") + static_cast<char>(keyright) + ".tga";
	const std::string strleft = std::string("Res/Images/Operation/") + static_cast<char>(keyleft) + ".tga";
	const std::string strshot = std::string("Res/Images/Operation/") + static_cast<char>(keyshot) + ".tga";

	//上
	if (upImage != nullptr)
	{
		upImage->ClearSprite();
		const size_t operation_Up = engine.AddUILayer(strup.c_str(), GL_LINEAR, 10);
		upImage = engine.CreateUI<GameObject>(operation_Up, "Operation", 900.0f, 550.0f);
		upImage->AddSprite({ 0, 0, 1, 1 });
	}
	else
	{
		const size_t operation_Up = engine.AddUILayer(strup.c_str(), GL_LINEAR, 10);
		upImage = engine.CreateUI<GameObject>(operation_Up, "Operation", 900.0f, 550.0f);
		upImage->AddSprite({ 0, 0, 1, 1 });
	}
	//下
	if (downImage != nullptr)
	{
		downImage->ClearSprite();
		const size_t operation_Down = engine.AddUILayer(strdown.c_str(), GL_LINEAR, 10);
		downImage = engine.CreateUI<GameObject>(operation_Down, "Operation", 900.0f, 550.0f - selectBox_y);
		downImage->AddSprite({ 0, 0, 1, 1 });
	}
	else
	{
		const size_t operation_Down = engine.AddUILayer(strdown.c_str(), GL_LINEAR, 10);
		downImage = engine.CreateUI<GameObject>(operation_Down, "Operation", 900.0f, 550.0f - selectBox_y);
		downImage->AddSprite({ 0, 0, 1, 1 });
	}
	//右
	if (rightImage != nullptr)
	{
		rightImage->ClearSprite();
		const size_t operation_Right = engine.AddUILayer(strright.c_str(), GL_LINEAR, 10);
		rightImage = engine.CreateUI<GameObject>(operation_Right, "Operation", 900.0f, 550.0f - (selectBox_y * 2.0f));
		rightImage->AddSprite({ 0, 0, 1, 1 });
	}
	else
	{
		const size_t operation_Right = engine.AddUILayer(strright.c_str(), GL_LINEAR, 10);
		rightImage = engine.CreateUI<GameObject>(operation_Right, "Operation", 900.0f, 550.0f - (selectBox_y * 2.0f));
		rightImage->AddSprite({ 0, 0, 1, 1 });
	}
	//左
	if (leftImage != nullptr)
	{
		leftImage->ClearSprite();
		const size_t operation_Left = engine.AddUILayer(strleft.c_str(), GL_LINEAR, 10);
		leftImage = engine.CreateUI<GameObject>(operation_Left, "Operation", 900.0f, 550.0f - (selectBox_y * 3.0f));
		leftImage->AddSprite({ 0, 0, 1, 1 });
	}
	else
	{
		const size_t operation_Left = engine.AddUILayer(strleft.c_str(), GL_LINEAR, 10);
		leftImage = engine.CreateUI<GameObject>(operation_Left, "Operation", 900.0f, 550.0f - (selectBox_y * 3.0f));
		leftImage->AddSprite({ 0, 0, 1, 1 });
	}
	//ショット
	if (shotImage != nullptr)
	{
		shotImage->ClearSprite();
		if (keyshot == 32)
		{
			const size_t operation_Shot = engine.AddUILayer("Res/Images/Operation/Space.tga", GL_LINEAR, 10);
			shotImage = engine.CreateUI<GameObject>(operation_Shot, "Operation", 900.0f, 550.0f - (selectBox_y * 4.0f));
			shotImage->AddSprite({ 0, 0, 1, 1 });
		}
		else
		{
			const size_t operation_Shot = engine.AddUILayer(strshot.c_str(), GL_LINEAR, 10);
			shotImage = engine.CreateUI<GameObject>(operation_Shot, "Operation", 900.0f, 550.0f - (selectBox_y * 4.0f));
			shotImage->AddSprite({ 0, 0, 1, 1 });
		}

	}
	else
	{
		if (keyshot == 32)
		{
			const size_t operation_Shot = engine.AddUILayer("Res/Images/Operation/Space.tga", GL_LINEAR, 10);
			shotImage = engine.CreateUI<GameObject>(operation_Shot, "Operation", 900.0f, 550.0f - (selectBox_y * 4.0f));
			shotImage->AddSprite({ 0, 0, 1, 1 });
		}
		else
		{
			const size_t operation_Shot = engine.AddUILayer(strshot.c_str(), GL_LINEAR, 10);
			shotImage = engine.CreateUI<GameObject>(operation_Shot, "Operation", 900.0f, 550.0f - (selectBox_y * 4.0f));
			shotImage->AddSprite({ 0, 0, 1, 1 });
		}
	}

	if(keylist[0] == 0)upImage->ClearSprite();
	if(keylist[1] == 0)downImage->ClearSprite();
	if(keylist[2] == 0)rightImage->ClearSprite();
	if(keylist[3] == 0)leftImage->ClearSprite();
	if(keylist[4] == 0)shotImage->ClearSprite();
}
//画像の初期化(W,A,S,D,Space)
void ConfigScene::ConfigKeyImage_Reset(Engine& engine)
{
	upImage->ClearSprite();
	const size_t operation_Up = engine.AddUILayer("Res/Images/Operation/W.tga", GL_LINEAR, 10);
	upImage = engine.CreateUI<GameObject>(operation_Up, "Operation", 900.0f, 550.0f);
	upImage->AddSprite({ 0, 0, 1, 1 });

	downImage->ClearSprite();
	const size_t operation_Down = engine.AddUILayer("Res/Images/Operation/S.tga", GL_LINEAR, 10);
	downImage = engine.CreateUI<GameObject>(operation_Down, "Operation", 900.0f, 550.0f - selectBox_y);
	downImage->AddSprite({ 0, 0, 1, 1 });

	rightImage->ClearSprite();
	const size_t operation_Right = engine.AddUILayer("Res/Images/Operation/D.tga", GL_LINEAR, 10);
	rightImage = engine.CreateUI<GameObject>(operation_Right, "Operation", 900.0f, 550.0f - (selectBox_y * 2.0f));
	rightImage->AddSprite({ 0, 0, 1, 1 });

	leftImage->ClearSprite();
	const size_t operation_Left = engine.AddUILayer("Res/Images/Operation/A.tga", GL_LINEAR, 10);
	leftImage = engine.CreateUI<GameObject>(operation_Left, "Operation", 900.0f, 550.0f - (selectBox_y * 3.0f));
	leftImage->AddSprite({ 0, 0, 1, 1 });

	shotImage->ClearSprite();
	const size_t operation_Shot = engine.AddUILayer("Res/Images/Operation/Space.tga", GL_LINEAR, 10);
	shotImage = engine.CreateUI<GameObject>(operation_Shot, "Operation", 900.0f, 550.0f - (selectBox_y * 4.0f));
	shotImage->AddSprite({ 0, 0, 1, 1 });
}
//リセットのアニメーション
void ConfigScene::ResetAnimImage(Engine& engine, float deltaTime)
{
	if (resetAnimFlg)
	{
		if (reset->alpha > 0.0f)reset->alpha -= deltaTime;
		else resetAnimFlg = false;
	}
}
//volume画像
void ConfigScene::VolumeNumImage(Engine& engine, const size_t textLayer)
{
	if (uiNum == nullptr)
	{
		//設定項目を示すテキスト
		const float fontSizeX = 16.0f;
		const float fontposY = 520.0f - (selectBox_y * VOLUME) ;
		const float x = 920.0f;
		const char strnum[] = "nm";
		const float numX = x - static_cast<float>(std::size(strnum) - 1.0f) * fontSizeX;
		uiNum = engine.CreateUI<GameObject>(textLayer, "number", numX, fontposY);
		textnum = uiNum->AddComponent<Text>();
		textnum->SetText(number[engine.selectVolume],2.0f);
	}
	else
	{
		textnum->SetText(number[engine.selectVolume], 2.0f);
	}
}
//volumeの選択キー(上、下矢印)が押されたらimageを表示させる
void ConfigScene::VolumeArrowImage(Engine& engine, float deltaTime)
{
	//上キーが押されたとき
	if (upArrow->alpha > 0.0f)
	{
		upArrow->alpha -= deltaTime;
	}
	//下キーが押されたとき
	if (downArrow->alpha > 0.0f)
	{
		downArrow->alpha -= deltaTime;
	}
}