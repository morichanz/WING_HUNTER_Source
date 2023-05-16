#include"../IsState/IsState.h"
#include<memory>


#pragma region BossStatus
class BossStatus
{
private:
	//ボスの移動先位置情報
	struct BossPos
	{
		float bossPosX = 0.0f;
		float bossPosY = 0.0f;
		float disX = 0.0f;
		float disY = 0.0f;
		float disx = 0.0f;
		float disy = 0.0f;
		bool disFlg = false;

		float disAix = 0.0f;
		float disAiy = 0.0f;

		float Destination_KeepX = 0.0f;
		float Destination_KeepY = 0.0f;
		float Distination_KeepUpDown = 0.0f;
		float Distination_KeepRightLeft = 0.0f;
	};

	//目的地
	struct Position
	{
		const float viewSizeX = 17.0f * 32.0f;
		const float viewSizeY = 22.0f * 32.0f;
		const float right = viewSizeX - 128.0f;
		const float left = 128.0f;
		const float up = viewSizeY - 24.0f;
		const float down = viewSizeY / 2.0f;
		const float middle[2] = { 280.0f ,540.0f };
	};
	
public:
	//方向ベクトルを決める
	enum class PositionSet
	{
		EASEINOUTQUART,
		EASEOUTBACK,
		EASEOUTEXPO,
		UPDOWN,
		RIGHTLEFT,
	};

	PositionSet posSet;
	BossPos bossPos;
	Position position;
};
#pragma endregion

#pragma region Appearance
class Appearance : public STATE::IsState
{
public:
	virtual void Initialize()override;
	virtual void Execute(float deltaTime)override;
	virtual void Finalize()override;
	virtual void GiveGameObject(GameObjectPtr gameObject)override;
	virtual void GiveBaseFactory(FactoryBasePtr base)override;

private:
	//ボスの初期位置
	void BossPosition(GameObjectPtr gameObject);
	//ボスの初期移動
	void BossMove(GameObjectPtr gameObject);
	//イージングの移動
	void EaseBossMove(GameObjectPtr gameObject, float deltaTime);
	GameObjectPtr gameObject;
	FactoryBasePtr base;
	BossStatus status;
	bool appearanceFlg = false;
};
#pragma endregion

#pragma region MaxMove
class MaxMove : public STATE::IsState
{
public:
	virtual void Initialize()override;
	virtual void Execute(float deltaTime)override;
	virtual void Finalize()override;
	virtual void GiveGameObject(GameObjectPtr gameObject)override;
	virtual void GiveBaseFactory(FactoryBasePtr base)override;

private:
	//体力が半分になった時に切り替える
	void HalfMoveChange();

	void BossMoveAI();
	void BossPosAI(int num);
	void MoveAiPattern();

	//ボスの移動変更
	void BossMoveChange(int _bossMove)
	{
		if (val >= 1)
		{
			bossMove = _bossMove;
			val = 0.0f;
			AiFlg = false;
			status.bossPos.disFlg = false;
		}
	}
	
	//移動目的地
	void Destination(float disx = 0, float disy = 0, int num = 1)
	{
		if (num == 0)
		{
			status.bossPos.disx = disx;
			status.bossPos.disy = disy;
		}
		else if (num == 1)
		{
			status.bossPos.disx = disx;
		}
		else
		{
			status.bossPos.disy = disy;
		}
	}

	GameObjectPtr gameObject;
	FactoryBasePtr base;
	BossStatus status;
	GameObjectPtr playerPtr;
	const float viewSizeX = 17.0f * 32.0f; //画面外に出ないように設定
	const float viewSizeY = 22.0f * 32.0f; //画面外に出ないように設定

	int moveRand = 0;
	int bossMove = 0;
	int num = 1;
	int health = 0;
	bool halfFlg = false;
	bool AiFlg = false;
};
#pragma endregion

#pragma region HalfMove
class HalfMove : public STATE::IsState
{
public:
	virtual void Initialize()override;
	virtual void Execute(float deltaTime)override;
	virtual void Finalize()override;
	virtual void GiveGameObject(GameObjectPtr gameObject)override;
	virtual void GiveBaseFactory(FactoryBasePtr base)override;

private:
	GameObjectPtr gameObject;
	GameObjectPtr playerPtr;
	FactoryBasePtr base;
	BossStatus status;

	bool AiFlg = false;

	//次のパターンに変更
	void RandChange();
	//ボスの移動パターン
	void MoveAiPattern();

	void BossPosAI(int num);

	void BossMoveAI();

	//ボスの移動変更
	void BossMoveChange(int _bossMove)
	{
		if (val >= 1)
		{
			bossMove = _bossMove;
			val = 0.0f;
			AiFlg = false;
		}
	}

	//移動目的地
	void Destination(float disx = 0, float disy = 0, int num = 1)
	{
		if (num == 0)
		{
			status.bossPos.disx = disx;
			status.bossPos.disy = disy;
		}
		else if (num == 1)
		{
			status.bossPos.disx = disx;
		}
		else
		{
			status.bossPos.disy = disy;
		}
	}
};
#pragma endregion

#pragma region DeadMove
class DeadMove : public STATE::IsState
{
public:
	virtual void Initialize()override;
	virtual void Execute(float deltaTime)override;
	virtual void Finalize()override;
	virtual void GiveGameObject(GameObjectPtr gameObject)override;
	virtual void GiveBaseFactory(FactoryBasePtr base)override;

private:
	GameObjectPtr gameObject;
	FactoryBasePtr base;
	BossStatus status;
	float bossDead_Zpos = 100.0f;   //ボス破壊後のz軸の移動時間
};
#pragma endregion