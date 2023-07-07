#include"BossState.h"
#include"../../GameObject/GameObject.h"
#include"../FactoryBase.h"
#include"../../Engine/Engine.h"
#include<time.h>

#pragma region Appearance

void Appearance::Initialize()
{
	BossPosition(gameObject);
	val = 0.0f;
}

void Appearance::Execute(float deltaTime)
{
	EaseBossMove(gameObject, deltaTime);

	if (gameObject->engine->stageCnt != 6)
	{
		BossMove(gameObject);
	}
	else
	{
		if (appearanceFlg)
		{
			BossMove(gameObject);
		}
	}

	if (val >= 1)
	{
		this->Finalize();
	}
}

void Appearance::Finalize()
{
	gameObject->collisionFlg = true;

	if (gameObject->engine->stageCnt == 1)
	{
		gameObject->AddCollision({ -120,-50,120,50 });
	}
	else if (gameObject->engine->stageCnt == 2)
	{
		gameObject->AddCollision({ -50,-85,50,85 });
	}
	else if (gameObject->engine->stageCnt == 3)
	{
		gameObject->AddCollision({ -87,-69,87,69 });
	}
	else if (gameObject->engine->stageCnt == 4)
	{
		gameObject->AddCollision({ -110,-50,110,50 });
	}
	else if (gameObject->engine->stageCnt == 5)
	{
		gameObject->AddCollision({ -96,-65,96,65 });
	}
	else if (gameObject->engine->stageCnt == 6)
	{
		gameObject->AddCollision({ -150,-98,150,98 });
	}
	base->SetState<MaxMove>();
}

void Appearance::GiveGameObject(GameObjectPtr gameObject)
{
	this->gameObject = gameObject;
}

void Appearance::GiveBaseFactory(FactoryBasePtr base)
{
	this->base = base;
}

void Appearance::BossPosition(GameObjectPtr gameObject)
{
	//ボスポジション
	if (gameObject->engine->stageCnt == 1)
	{
		gameObject->y = -400;
	}
	else if (gameObject->engine->stageCnt == 2)
	{
		gameObject->y = 100;
		gameObject->x = -300;
	}
	else if (gameObject->engine->stageCnt == 3)
	{
		gameObject->y = 800;
	}
	else if (gameObject->engine->stageCnt == 4)
	{
		gameObject->x = 700;
		gameObject->y = -400;
	}
	else if (gameObject->engine->stageCnt == 5)
	{
		gameObject->x = -300;
	}
	else if (gameObject->engine->stageCnt == 6)
	{
		gameObject->y = -400;
	}
}

void Appearance::BossMove(GameObjectPtr gameObject)
{
	if (!status.bossPos.disFlg)
	{
		status.bossPos.bossPosX = gameObject->x;
		status.bossPos.bossPosY = gameObject->y;

		status.bossPos.disX = (status.position.middle[0] - gameObject->x);
		status.bossPos.disY = (status.position.middle[1] - gameObject->y);
		status.bossPos.disFlg = true;
	}
	gameObject->x = status.bossPos.bossPosX + status.bossPos.disX * Easing::EaseOutBack(val);
	gameObject->y = status.bossPos.bossPosY + status.bossPos.disY * Easing::EaseOutBack(val);
}

void Appearance::EaseBossMove(GameObjectPtr gameObject, float deltaTime)
{
	if (gameObject->z > 0)gameObject->z -= deltaTime * 150.0f;

	//ボス本体の動き
	if (gameObject->engine->stageCnt != 6 && val < 1)val += deltaTime * 0.5f;
	else
	{
		//ボスの出現(ステージ６の場合)
		if (!appearanceFlg)
		{
			//一旦画面外まで飛ぶ
			if (gameObject->y < 1000)
			{
				gameObject->y += deltaTime * 700.0f;
			}
			else appearanceFlg = true;
		}
		else
		{
			if (val < 1) val += deltaTime * 0.5f;
		}
	}
}

#pragma endregion


#pragma region MaxMove

void MaxMove::Initialize()
{
	health = gameObject->health;
	playerPtr = gameObject->engine->FindGameObjectWithTag("player");
	val = 0.0f;
}

void MaxMove::Execute(float deltaTime)
{
	if (val < 1 && !gameObject->chargeFlg)val += deltaTime * 0.5f;
	if(!halfFlg) MoveAiPattern();
	HalfMoveChange();
}

void MaxMove::Finalize()
{
	status.bossPos.disFlg = false;
	base->SetState<HalfMove>();
}

void MaxMove::GiveGameObject(GameObjectPtr gameObject)
{
	this->gameObject = gameObject;
}

void MaxMove::GiveBaseFactory(FactoryBasePtr base)
{
	this->base = base;
}

void MaxMove::MoveAiPattern()
{
	//ボスの動きパターンの実行
	switch (bossMove)
	{
	case 0:
		status.posSet = BossStatus::PositionSet::EASEINOUTQUART;
		BossPosAI(0);
		BossMoveAI();
		BossMoveChange(1);
		break;
	case 1:
		status.posSet = BossStatus::PositionSet::UPDOWN;
		BossPosAI(2);
		BossMoveAI();
		gameObject->shotFlg = true;
		BossMoveChange(2);
		break;
	case 2:
		status.posSet = BossStatus::PositionSet::RIGHTLEFT;
		BossPosAI(1);
		BossMoveAI();
		BossMoveChange(3);
		break;
	case 3:
		status.posSet = BossStatus::PositionSet::EASEINOUTQUART;
		BossPosAI(3);
		BossMoveAI();
		gameObject->shotFlg = false;
		BossMoveChange(0);
		break;
	}
}

void MaxMove::HalfMoveChange()
{
	//半分になったらHalfMoveに変更
	if (gameObject->health < health / 2)
	{
		gameObject->collisionFlg = false;
		gameObject->shotFlg = false;

		if (!halfFlg)
		{
			gameObject->RemoveCollision();
			val = 0.0f;
			status.bossPos.disFlg = false;
			halfFlg = true;
		}
		else
		{
			status.posSet = BossStatus::PositionSet::EASEINOUTQUART;
			this->Destination(status.position.middle[0], status.position.middle[1], 0);
			BossMoveAI();
			if (val > 1)
			{
				this->Finalize();
			}
		}
	}
}

void MaxMove::BossMoveAI()
{
	if (!status.bossPos.disFlg && halfFlg)
	{
		status.bossPos.bossPosX = gameObject->x;
		status.bossPos.bossPosY = gameObject->y;

		status.bossPos.disX = (status.bossPos.disx - gameObject->x);
		status.bossPos.disY = (status.bossPos.disy - gameObject->y);
		status.bossPos.disFlg = true;
	}
	if (!AiFlg)
	{
		status.bossPos.bossPosX = gameObject->x;
		status.bossPos.bossPosY = gameObject->y;

		status.bossPos.disX = (status.bossPos.Destination_KeepX - gameObject->x);
		status.bossPos.disY = (status.bossPos.Destination_KeepY - gameObject->y);

		status.bossPos.disAix = (status.bossPos.Distination_KeepRightLeft - gameObject->x);
		status.bossPos.disAiy = (status.bossPos.Distination_KeepUpDown - gameObject->y);

		AiFlg = true;
	}

	if (status.posSet == BossStatus::PositionSet::EASEINOUTQUART)
	{	
		gameObject->x = status.bossPos.bossPosX + status.bossPos.disX * Easing::EaseInOutQuart(val);
		gameObject->y = status.bossPos.bossPosY + status.bossPos.disY * Easing::EaseInOutQuart(val);
	}
	else if (status.posSet == BossStatus::PositionSet::EASEOUTBACK)
	{
		gameObject->x = status.bossPos.bossPosX + status.bossPos.disX * Easing::EaseOutBack(val);
		gameObject->y = status.bossPos.bossPosY + status.bossPos.disY * Easing::EaseOutBack(val);
	}
	else if (status.posSet == BossStatus::PositionSet::EASEOUTEXPO)
	{
		gameObject->x = status.bossPos.bossPosX + status.bossPos.disX * Easing::EaseOutExpo(val);
		gameObject->y = status.bossPos.bossPosY + status.bossPos.disY * Easing::EaseOutExpo(val);
	}
	else if (status.posSet == BossStatus::PositionSet::UPDOWN)
	{
		gameObject->y = status.bossPos.bossPosY + status.bossPos.disAiy * val;
	}
	else if (status.posSet == BossStatus::PositionSet::RIGHTLEFT)
	{
		gameObject->x = status.bossPos.bossPosX + status.bossPos.disAix * val;
	}
}

void MaxMove::BossPosAI(int num)
{
	if (num == 0)
	{
		if (playerPtr->x > 250) status.bossPos.Destination_KeepX = status.position.left;
		else status.bossPos.Destination_KeepX = status.position.right;
		
		if (playerPtr->y > 250) status.bossPos.Destination_KeepY = status.position.up;
		else status.bossPos.Destination_KeepY = status.position.down;
	}
	else if (num == 1)
	{
		if (playerPtr->x > 250)
		{
			status.bossPos.Destination_KeepX = status.position.left;
			status.bossPos.Distination_KeepRightLeft = status.position.right;
		}
		else
		{
			status.bossPos.Destination_KeepX = status.position.right;
			status.bossPos.Distination_KeepRightLeft = status.position.left;
		}
	}
	else if(num == 2)
	{
		if (playerPtr->y > 250)
		{
			status.bossPos.Destination_KeepY = status.position.up;
			status.bossPos.Distination_KeepUpDown = status.position.down;
		}
		else
		{
			status.bossPos.Destination_KeepY = status.position.down;
			status.bossPos.Distination_KeepUpDown = status.position.up;
		}
	}
	else if (num == 3)
	{
		status.bossPos.Destination_KeepX = status.position.middle[0];
		status.bossPos.Destination_KeepY = status.position.middle[1];
	}
}
#pragma endregion


#pragma region HalfMove
void HalfMove::Initialize()
{
	playerPtr = gameObject->engine->FindGameObjectWithTag("player");
	gameObject->collisionFlg = true;

	if (gameObject->engine->stageCnt == 1)
	{
		gameObject->AddCollision({ -120,-50,120,50 });
	}
	else if (gameObject->engine->stageCnt == 2)
	{
		gameObject->AddCollision({ -50,-85,50,85 });
	}
	else if (gameObject->engine->stageCnt == 3)
	{
		gameObject->AddCollision({ -87,-69,87,69 });
	}
	else if (gameObject->engine->stageCnt == 4)
	{
		gameObject->AddCollision({ -110,-50,110,50 });
	}
	else if (gameObject->engine->stageCnt == 5)
	{
		gameObject->AddCollision({ -96,-65,96,65 });
	}
	else if (gameObject->engine->stageCnt == 6)
	{
		gameObject->AddCollision({ -150,-98,150,98 });
	}
	val = 0.0f;
	moveRand = 0;
	bossMove = 0;
}
void HalfMove::Execute(float deltaTime)
{
	if (val < 1)val += deltaTime * 0.5f;

	MoveAiPattern();
	Finalize();
}
void HalfMove::Finalize()
{
	if (gameObject->health <= 0)
	{
		base->SetState<DeadMove>();
	}
}

void HalfMove::GiveGameObject(GameObjectPtr gameObject)
{
	this->gameObject = gameObject;
}

void HalfMove::GiveBaseFactory(FactoryBasePtr base)
{
	this->base = base;
}

void HalfMove::RandChange()
{
	//一度だけボスの移動パターンを決める
	if (val > 1)
	{
		std::random_device rnd;     // 非決定的な乱数生成器を生成
		std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
		std::uniform_int_distribution<> rand3(0, 2);        // [0, 300] 範囲の一様乱数
		moveRand = rand3(mt);
		status.bossPos.disFlg = false;
		val = 0.0f;
	}
}

void HalfMove::MoveAiPattern()
{
	//ボスの動きパターンの実行
	switch (bossMove)
	{
	case 0:
		status.posSet = BossStatus::PositionSet::EASEINOUTQUART;
		BossPosAI(0);
		BossMoveAI();
		BossMoveChange(1);

		break;
	case 1:
		status.posSet = BossStatus::PositionSet::UPDOWN;
		BossPosAI(2);
		BossMoveAI();
		gameObject->shotFlg = true;
		BossMoveChange(2);
		break;
	case 2:
		status.posSet = BossStatus::PositionSet::RIGHTLEFT;
		BossPosAI(1);
		BossMoveAI();
		BossMoveChange(3);
		break;
	case 3:
		status.posSet = BossStatus::PositionSet::EASEINOUTQUART;
		BossPosAI(3);
		BossMoveAI();
		gameObject->shotFlg = false;
		BossMoveChange(0);
		break;
	}
}

void HalfMove::BossPosAI(int num)
{
	if (num == 0)
	{
		if (playerPtr->x > 250) status.bossPos.Destination_KeepX = status.position.left;
		else status.bossPos.Destination_KeepX = status.position.right;

		if (playerPtr->y > 250) status.bossPos.Destination_KeepY = status.position.up;
		else status.bossPos.Destination_KeepY = status.position.down;
	}
	else if (num == 1)
	{
		if (playerPtr->x > 250)
		{
			status.bossPos.Destination_KeepX = status.position.left;
			status.bossPos.Distination_KeepRightLeft = status.position.right;
		}
		else
		{
			status.bossPos.Destination_KeepX = status.position.right;
			status.bossPos.Distination_KeepRightLeft = status.position.left;
		}
	}
	else if (num == 2)
	{
		if (playerPtr->y > 250)
		{
			status.bossPos.Destination_KeepY = status.position.up;
			status.bossPos.Distination_KeepUpDown = status.position.down;
		}
		else
		{
			status.bossPos.Destination_KeepY = status.position.down;
			status.bossPos.Distination_KeepUpDown = status.position.up;
		}
	}
	else if (num == 3)
	{
		status.bossPos.Destination_KeepX = status.position.middle[0];
		status.bossPos.Destination_KeepY = status.position.middle[1];
	}
}

void HalfMove::BossMoveAI()
{
	if (!AiFlg)
	{
		status.bossPos.bossPosX = gameObject->x;
		status.bossPos.bossPosY = gameObject->y;

		status.bossPos.disX = (status.bossPos.Destination_KeepX - gameObject->x);
		status.bossPos.disY = (status.bossPos.Destination_KeepY - gameObject->y);

		status.bossPos.disAix = (status.bossPos.Distination_KeepRightLeft - gameObject->x);
		status.bossPos.disAiy = (status.bossPos.Distination_KeepUpDown - gameObject->y);

		AiFlg = true;
	}

	if (status.posSet == BossStatus::PositionSet::EASEINOUTQUART)
	{
		gameObject->x = status.bossPos.bossPosX + status.bossPos.disX * Easing::EaseInOutQuart(val);
		gameObject->y = status.bossPos.bossPosY + status.bossPos.disY * Easing::EaseInOutQuart(val);
	}
	else if (status.posSet == BossStatus::PositionSet::EASEOUTBACK)
	{
		gameObject->x = status.bossPos.bossPosX + status.bossPos.disX * Easing::EaseOutBack(val);
		gameObject->y = status.bossPos.bossPosY + status.bossPos.disY * Easing::EaseOutBack(val);
	}
	else if (status.posSet == BossStatus::PositionSet::EASEOUTEXPO)
	{
		gameObject->x = status.bossPos.bossPosX + status.bossPos.disX * Easing::EaseOutExpo(val);
		gameObject->y = status.bossPos.bossPosY + status.bossPos.disY * Easing::EaseOutExpo(val);
	}
	else if (status.posSet == BossStatus::PositionSet::UPDOWN)
	{
		gameObject->y = status.bossPos.bossPosY + status.bossPos.disAiy * val;
	}
	else if (status.posSet == BossStatus::PositionSet::RIGHTLEFT)
	{
		gameObject->x = status.bossPos.bossPosX + status.bossPos.disAix * val;
	}
}

#pragma endregion


#pragma region DeadMove
void DeadMove::Initialize()
{
	gameObject->shotFlg = false;
	gameObject->collisionFlg = false;
	gameObject->RemoveCollision();
}

void DeadMove::Execute(float deltaTime)
{
	if(gameObject->z > -280) gameObject->z -= deltaTime * bossDead_Zpos;
	else Finalize();
}

void DeadMove::Finalize()
{
	gameObject->isDead = true;
}

void DeadMove::GiveGameObject(GameObjectPtr gameObject)
{
	this->gameObject = gameObject;
}

void DeadMove::GiveBaseFactory(FactoryBasePtr base)
{
	this->base = base;
}
#pragma endregion