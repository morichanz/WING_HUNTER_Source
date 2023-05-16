#include"../IsState/IsState.h"
#include<memory>

#pragma region BeamCharge

class Charge : public STATE::IsState
{
public:
	virtual void Initialize()override;
	virtual void Execute(float deltaTime)override;
	virtual void Finalize()override;
	virtual void GiveGameObject(GameObjectPtr gameObject)override;
	virtual void GiveBaseFactory(FactoryBasePtr base)override;

private:
	GameObjectPtr gameObject;
	GameObjectPtr charge;
	GameObjectPtr boss;
	FactoryBasePtr base;
	bool shotFlg = false;
};
#pragma endregion

#pragma region BeamShot

class Shot : public STATE::IsState
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
	GameObjectPtr boss;
	GameObjectPtr shot;
};
#pragma endregion

#pragma region BeamShotEnd

class End : public STATE::IsState
{
public:
	virtual void Initialize()override;
	virtual void Execute(float deltaTime)override;
	virtual void Finalize()override;
	virtual void GiveGameObject(GameObjectPtr gameObject)override;
	virtual void GiveBaseFactory(FactoryBasePtr base)override;

private:
	GameObjectPtr gameObject;
	GameObjectPtr end;
	GameObjectPtr boss;
	FactoryBasePtr base;
};
#pragma endregion

#pragma region FireShot

class FireShot : public STATE::IsState
{
public:
	virtual void Initialize()override;
	virtual void Execute(float deltaTime)override;
	virtual void Finalize()override;
	virtual void GiveGameObject(GameObjectPtr gameObject)override;
	virtual void GiveBaseFactory(FactoryBasePtr base)override;

private:
	GameObjectPtr gameObject;
	GameObjectPtr end;
	GameObjectPtr boss;
	FactoryBasePtr base;
	GameObjectPtr player;

	float numberOfFire = 1.0f;

	float vx = 0.0f;
	float vy = 0.0f;
	float speed = 400.0f;

	float weaponFireCounter = 0.0f;
	float shotSize = 1.0f;

	float timerA = 0.0f;
	float timerB = 0.0f;
	float intervalA = 0.4f;
	float intervalB = 0.2f;
	bool isFiring = false;
	bool vyFlg = false;
	bool isDead = false;

};
#pragma endregion