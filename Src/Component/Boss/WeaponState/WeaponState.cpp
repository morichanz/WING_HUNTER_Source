#include"WeaponState.h"
#include"../../GameObject/GameObject.h"
#include"../../Engine/Engine.h"
#include"../../Audio/EasyAudio.h"
#include"../../Audio/AudioSetting.h"
#include"../FactoryBase.h"
#include"../Beam.h"
#include"../BeamDamageSource.h"
#include"../../Animator/Animator2D.h"
#include"../../Bullet/BoundBullet.h"
#include"../BossBulletExplosion.h"
#include"../../OutOfScreen.h"
#include"../../DamageSource.h"

#pragma region BeamCharge

//チャージアニメーション
const Texcoord chargeTc[] = {
	MakeTexcoord(1,  585,2,30),
	MakeTexcoord(4,  585,4,30),
	MakeTexcoord(9,  585,6,30),
	MakeTexcoord(16, 585,8,30),
	MakeTexcoord(25, 585,10,30),
	MakeTexcoord(36, 585,12,30),
	MakeTexcoord(49, 585,14,30),
	MakeTexcoord(64, 585,16,30),
	MakeTexcoord(81, 585,18,30),
	MakeTexcoord(100,585,20,30),
	MakeTexcoord(121,585,22,30),
	MakeTexcoord(144,585,24,30),
	MakeTexcoord(169,585,26,30),
	MakeTexcoord(196,585,28,30),
	MakeTexcoord(225,585,30,30),
	MakeTexcoord(256,585,32,30),
	MakeTexcoord(289,585,34,30),
	MakeTexcoord(324,585,36,30),
	MakeTexcoord(361,585,38,30),
	MakeTexcoord(400,585,40,30),
	MakeTexcoord(441,585,42,30),
	MakeTexcoord(484,585,44,30),
	MakeTexcoord(529,585,46,30),
	MakeTexcoord(576,585,48,30),
	MakeTexcoord(625,585,50,30),
	MakeTexcoord(676,585,52,30),
	MakeTexcoord(729,585,54,30),
	MakeTexcoord(784,585,56,30),
	MakeTexcoord(841,585,58,30),
	MakeTexcoord(900,585,60,30),
	MakeTexcoord(961,585,62,30),
};
void Charge::Initialize()
{
	this->boss = gameObject->engine->FindGameObjectWithTag("boss");
	shotFlg = false;
}
void Charge::Execute(float deltaTime)
{
	if (this->boss->shotFlg && !shotFlg)
	{
		this->boss->chargeFlg = true;
		Audio::PlayOneShot(SE::BeamC, 2);
		charge = gameObject->engine->Create<GameObject>("enemy bullet", gameObject->x, gameObject->y);
		auto move = charge->AddComponent<Beam>();
		move->weapon = gameObject;
		auto anim = charge->AddComponent<Animator2D>();
		anim->interval = 0.05f;
		anim->loopTime = false;		//ループ再生をしない
		anim->killWhenEnd = true;	//再生終了時にゲームオブジェクトを削除
		anim->clip.resize(std::size(chargeTc));
		for (size_t frameNo = 0; frameNo < std::size(chargeTc); ++frameNo)
		{
			anim->AddSprite(frameNo, chargeTc[frameNo], 0.0f, -45.0f);
			anim->AddSprite(frameNo, chargeTc[frameNo], 0.0f, -30.0f, 1.0f, -1.0f, 1.0f);
		}
		shotFlg = true;
	}

	this->Finalize();
}
void Charge::Finalize()
{
	if (shotFlg && !gameObject->isDead)
	{
		this->boss->chargeFlg = false;
		if(charge->isDead)base->SetState<Shot>();
	}
	else if (gameObject->isDead)
	{
		return;
	}
	
}
void Charge::GiveGameObject(GameObjectPtr gameObject)
{
	this->gameObject = gameObject;
}
void Charge::GiveBaseFactory(FactoryBasePtr base)
{
	this->base = base;
}
#pragma endregion

#pragma region BeamShot

const Texcoord shotTc = MakeTexcoord(1, 524, 62, 60);

void Shot::Initialize()
{
	this->boss = gameObject->engine->FindGameObjectWithTag("boss");
	Audio::Play(AudioPlayer::se, SE::BeamS, 2, true);
	shot = gameObject->engine->Create<GameObject>("enemy bullet", gameObject->x, gameObject->y);
	shot->AddSprite(shotTc, 0.0f, -360.0f, 1.0f, 11.0f);
	shot->AddCollision({ -31,-700,31,700 });
	auto move = shot->AddComponent<Beam>();
	move->weapon = gameObject;
	auto damageSource = shot->AddComponent<BeamDamageSource>();
	damageSource->targetName = "player";
}
void Shot::Execute(float deltaTime)
{
	this->Finalize();
}
void Shot::Finalize()
{
	if (boss->isDead || gameObject->isDead)
	{
		shot->isDead = true;
		base->SetState<End>();
	}
	else if (!boss->shotFlg)
	{
		shot->isDead = true;
		base->SetState<End>();
	}
}
void Shot::GiveGameObject(GameObjectPtr gameObject)
{
	this->gameObject = gameObject;
}
void Shot::GiveBaseFactory(FactoryBasePtr base)
{
	this->base = base;
}
#pragma endregion

#pragma region BeamEnd
//ビームアニメーション
const Texcoord beamEndtc[] = {
	MakeTexcoord(1,  524,62,60),
	MakeTexcoord(64, 524,60,60),
	MakeTexcoord(125,524,58,60),
	MakeTexcoord(184,524,56,60),
	MakeTexcoord(241,524,54,60),
	MakeTexcoord(296,524,52,60),
	MakeTexcoord(349,524,50,60),
	MakeTexcoord(400,524,48,60),
	MakeTexcoord(449,524,46,60),
	MakeTexcoord(496,524,44,60),
	MakeTexcoord(542,524,42,60),
	MakeTexcoord(585,524,40,60),
	MakeTexcoord(626,524,38,60),
	MakeTexcoord(665,524,36,60),
	MakeTexcoord(702,524,34,60),
	MakeTexcoord(737,524,32,60),
	MakeTexcoord(770,524,30,60),
	MakeTexcoord(801,524,28,60),
	MakeTexcoord(830,524,26,60),
	MakeTexcoord(857,524,24,60),
	MakeTexcoord(882,524,22,60),
	MakeTexcoord(905,524,20,60),
	MakeTexcoord(926,524,18,60),
	MakeTexcoord(945,524,16,60),
	MakeTexcoord(962,524,14,60),
	MakeTexcoord(977,524,12,60),
	MakeTexcoord(990,524,10,60),
	MakeTexcoord(1001,524,8,60),
	MakeTexcoord(1010,524,6,60),
	MakeTexcoord(1017,524,4,60),
	MakeTexcoord(1022,524,2,60),
};
void End::Initialize()
{
	end = gameObject->engine->Create<GameObject>("enemy bullet", gameObject->x, gameObject->y);
	end->AddSprite(beamEndtc[0], 0.0f, -360.0f, 1.0f, 11.0f);
	end->AddCollision({ -31,-700,31,700 });
	auto anim = end->AddComponent<Animator2D>();
	anim->interval = 0.05f;
	anim->loopTime = false;		//ループ再生をしない
	anim->killWhenEnd = true;	//再生終了時にゲームオブジェクトを削除
	anim->clip.resize(std::size(beamEndtc));
	for (size_t frameNo = 0; frameNo < std::size(beamEndtc); ++frameNo)
	{
		anim->AddSprite(frameNo, beamEndtc[frameNo], 0.0f, -360.0f, 1.0f, 11.0f, 1.0f);
	}
}
void End::Execute(float deltaTime)
{
	//ビームが小さくなると同時に当たり判定も小さくする
	auto shape = end->GetAabb();
	shape.min[0] += deltaTime * 10.0f;
	shape.max[0] -= deltaTime * 10.0f;
	end->SetAabb(shape);
	this->Finalize();
}
void End::Finalize()
{
	if (end->isDead)
	{
		base->SetState<Charge>();
	}
}
void End::GiveGameObject(GameObjectPtr gameObject)
{
	this->gameObject = gameObject;
}
void End::GiveBaseFactory(FactoryBasePtr base)
{
	this->base = base;
}

#pragma endregion

#pragma region FireShot
const Texcoord weaponFireTc[] =
{
	MakeTexcoord(96, 465, 14, 14),
	MakeTexcoord(113, 465, 14, 14),
	MakeTexcoord(129, 465, 14, 14),
	MakeTexcoord(145, 465, 14, 14),
};
void FireShot::Initialize()
{
	player = gameObject->engine->FindGameObjectWithTag("player");
	boss = gameObject->engine->FindGameObjectWithTag("boss");
}
void FireShot::Execute(float deltaTime)
{
	if (!boss->shotFlg)return;

	//発射用タイマーが発射間隔を超えたら、発射用変数を初期化
	timerA += deltaTime;
	if (timerA >= intervalA)
	{
		isFiring = true;
		timerA -= intervalA;
		timerB = intervalB;	//1発目が即座に発射されるようにする
		weaponFireCounter = numberOfFire;

		//ターゲット指定があれば、発射ベクトルをターゲットに向ける
		if (player)
		{
			vx = player->x - gameObject->x;
			vy = player->y - gameObject->y;
			const float v = sqrt(vx * vx + vy * vy);
			if (vy > 0)
			{
				vyFlg = true;
			}
			else
			{
				vyFlg = false;
				vx = vx / v * speed;
				vy = vy / v * speed;
			}
		}
		else
		{
			vx = 0;
			vy = -speed;
		}
	}

	//発射モードでなければ何もしない
	if (!isFiring)
	{
		return;
	}

	//連射用タイマーが連射間隔を超えたら、弾を発射
	timerB += deltaTime;
	if (timerB < intervalB)
	{
		return;
	}
	timerB -= intervalB;

	if (!vyFlg)
	{
		if (isFiring)
		{
			Audio::PlayOneShot(SE::enemyShot, 2);
			auto bullet = gameObject->engine->Create<GameObject>("enemy bullet", gameObject->x + 10, gameObject->y - 30);
			bullet->AddSprite(weaponFireTc[0]);
			//bullet->priority = prioWaaponBullet;
			bullet->AddCollision({ -4,-4,4,4 });
			auto move = bullet->AddComponent<BoundBullet>();
			move->boundCounter = 2;
			move->vx = vx;
			move->vy = vy;
			auto exploaion = bullet->AddComponent<BossBulletExprosion>();

			bullet->AddComponent<OutOfScreen>();
			auto damageSource = bullet->AddComponent<DamageSource>();
			damageSource->targetName = "player";

			auto anim = bullet->AddComponent<Animator2D>();
			anim->loopTime = true;		//ループ再生をしない
			anim->killWhenEnd = false;	//再生終了時にゲームオブジェクトを削除
			anim->clip.resize(std::size(weaponFireTc));
			for (size_t frameNo = 0; frameNo < std::size(weaponFireTc); ++frameNo)
			{
				anim->AddSprite(frameNo, weaponFireTc[frameNo], 1, 1, shotSize, shotSize, shotSize);
			}

			auto bullet1 = gameObject->engine->Create<GameObject>("enemy bullet", gameObject->x - 10, gameObject->y - 30);
			bullet1->AddSprite(weaponFireTc[0]);
			bullet1->priority = prioWaaponBullet;
			bullet1->AddCollision({ -4,-4,4,4 });
			auto move1 = bullet1->AddComponent<BoundBullet>();
			move1->boundCounter = 2;
			move1->vx = vx;
			move1->vy = vy;
			auto exploaion1 = bullet1->AddComponent<BossBulletExprosion>();
			bullet1->AddComponent<OutOfScreen>();
			auto damageSource1 = bullet1->AddComponent<DamageSource>();
			damageSource1->targetName = "player";

			auto anim1 = bullet1->AddComponent<Animator2D>();
			anim1->loopTime = true;		//ループ再生をしない
			anim1->killWhenEnd = false;	//再生終了時にゲームオブジェクトを削除
			anim1->clip.resize(std::size(weaponFireTc));
			for (size_t frameNo = 0; frameNo < std::size(weaponFireTc); ++frameNo)
			{
				anim1->AddSprite(frameNo, weaponFireTc[frameNo], 1, 1, shotSize, shotSize, shotSize);
			}
		}
	}

	//連射カウンターを減らし、0以下になったら発射モードを解除
	--weaponFireCounter;
	if (weaponFireCounter <= 0)
	{
		isFiring = false;
	}
}
void FireShot::Finalize()
{
}
void FireShot::GiveGameObject(GameObjectPtr gameObject)
{
	this->gameObject = gameObject;
}
void FireShot::GiveBaseFactory(FactoryBasePtr base)
{
	this->base = base;
}

#pragma endregion