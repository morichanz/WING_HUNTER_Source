/**
* @file MiddleEnemyAction.h
*/
#ifndef COMPONENT_MIDDLENEMYACTION_H_INCLUDE
#define COMPONENT_MIDDLENEMYACTION_H_INCLUDE
#include"../Component.h"
#include"../../GameObject/GameObject.h"
#include"../Bullet/FireBullet.h"
#include"../../Easing.h"
#include"../MeshRenderer.h"
#include<time.h>

/**
* 中型機を移動する
*/
class MiddleEnemyAction :public Component
{
public:
	MiddleEnemyAction() = default;
	virtual ~MiddleEnemyAction() = default;

	virtual void Start(GameObject& gameObject)override
	{
		srand(static_cast<unsigned int>(time(NULL)));
		_rand = rand() % 2;

		std::random_device rnd;     // 非決定的な乱数生成器を生成
		std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
		std::uniform_int_distribution<> rand3(0, 300);        // [0, 300] 範囲の一様乱数
		_randPosition = rand3(mt);

		if (rand == 0)
		{
			gameObject.x = -32;
			gameObject.y = 550;
		}
		else
		{
			gameObject.x = 500;
			gameObject.y = 550;
		}
	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		EnemyMove(gameObject, deltaTime);
		Shot(gameObject, deltaTime);
	}

	//ダメージを受けたとき
	virtual void OnTakeDamage(GameObject& gameObject, GameObject& other, const Damage& damage) override
	{
		health -= damage.amount;
		if (health <= 0)
		{
			Audio::PlayOneShot(SE::enemyExplosionM);
			gameObject.scoreFlg = true;
			gameObject.isDead = true;
		}
	}

	//距離
	void Dis(GameObject& gameObject, float disx, float disy)
	{
		if (!disFlg)
		{
			enemyPosX = gameObject.x;
			enemyPosY = gameObject.y;
			disX = (disx - gameObject.x);
			disY = (disy - gameObject.y);
		}
		disFlg = true;
	}
	
	//敵の弾
	void Shot(GameObject& gameObject, float deltaTime)
	{
		if (!shadowFlg)
		{
			//発射用タイマーが発射間隔を超えたら、発射用変数を初期化
			timerA += deltaTime;
			if (timerA >= intervalA)
			{
				isFiring = true;
				timerA -= intervalA;
				timerB = intervalB;	//1発目が即座に発射されるようにする
				fireCounter = numberOfFire;

				//ターゲット指定があれば、発射ベクトルをターゲットに向ける
				if (player)
				{
					shotvx = player->x - gameObject.x;
					shotvy = player->y - gameObject.y;
					const float v = sqrt(shotvx * shotvx + shotvy * shotvy);

					shotvx = shotvx / v * speed;
					shotvy = shotvy / v * speed;
				}
				else
				{
					shotvx = 0;
					shotvy = -speed;
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

			//弾を発射
			if (shotFlg)
			{
				auto bullet = gameObject.engine->Create<GameObject>("enemy bullet", gameObject.x + x, gameObject.y + y);
				bullet->AddSprite(tc);
				bullet->priority = prioEnemyBullet;
				bullet->AddCollision({ -4,-4,4,4 });
				auto move = bullet->AddComponent<Move>();
				move->vx = shotvx;
				move->vy = shotvy;
				bullet->AddComponent<OutOfScreen>();
				auto damageSource = bullet->AddComponent<DamageSource>();
				damageSource->targetName = "player";
			}

			//連射カウンターを減らし、0以下になったら発射モードを解除
			--fireCounter;
			if (fireCounter <= 0)
			{
				isFiring = false;
			}
		}
	}

	//敵の動き
	void EnemyMove(GameObject& gameObject, float deltaTime)
	{
		//本体の動き
		if (_rand == 0)
		{
			switch (moveCnt)
			{
			case 0:
				Dis(gameObject, static_cast<float>(_randPosition), 450);
				gameObject.x = enemyPosX + disX * Easing::EaseOutExpo(val);
				gameObject.y = enemyPosY + disY * Easing::EaseOutExpo(val);
				val += deltaTime;
				if (val > 1)
				{
					disFlg = false;
					val = 0.0f;
					moveCnt = 1;
				}
				break;

			case 1:
				Dis(gameObject, static_cast<float>(_randPosition), -150);
				gameObject.x = enemyPosX + disX * Easing::EaseOutExpo(val);
				gameObject.y = enemyPosY + disY * Easing::EaseOutExpo(val);
				val += deltaTime / 2;
				if (val > 1)
				{
					disFlg = false;
					val = 0.0f;
				}
				break;
			}
		}
		else
		{
			switch (moveCnt)
			{
			case 0:
				Dis(gameObject, static_cast<float>(_randPosition), 450);
				gameObject.x = enemyPosX + disX * Easing::EaseOutExpo(val);
				gameObject.y = enemyPosY + disY * Easing::EaseOutExpo(val);
				val += deltaTime / 2;
				if (val > 1)
				{
					disFlg = false;
					val = 0.0f;
					moveCnt = 1;
				}
				break;

			case 1:
				Dis(gameObject, static_cast<float>(_randPosition), -150);
				gameObject.x = enemyPosX + disX * Easing::EaseOutExpo(val);
				gameObject.y = enemyPosY + disY * Easing::EaseOutExpo(val);
				val += deltaTime / 2;

				if (val > 1)
				{
					disFlg = false;
					val = 0.0f;
				}
				break;
			}
		}
	}
	GameObjectPtr target;
	GameObjectPtr player;

	bool shotFlg = false;
	float vy = 0.0f;
	float vx = 0.0f;
	float shotvy = 100.0f;
	float shotvx = 100.0f;
	float val = 0.0f;
	bool DownMoveTimer = false;
	bool addBulletFlg = false;
	bool shadowFlg = false;

	Texcoord tc = MakeTexcoord(336, 448, 16, 16);	//テクスチャ座標
	float intervalA = 1.0f;	//発射間隔
	float intervalB = 0.1f;	//連射間隔
	float numberOfFire = 1.0f;	//連射数
	float speed = 200.0f;		//弾の速度
	bool isFiring = false;		//発射フラグ
	float timerA = 0.0f;	//発射用タイマー
	float timerB = 0.0f;	//連射用タイマー
	float fireCounter = 1.0f;//連射数カウンター
	float x = 0.0f;			//発射位置のX座標
	float y = 0.0f;			//発射位置のY座標

private:
	int health = 2;	//耐久値
	float disX = 0.0f;
	float disY = 0.0f;
	float enemyPosX = 0.0f;
	float enemyPosY = 0.0f;
	bool disFlg = false;
	bool enemyUpFlg = false;
	float scale = 0.3f;

	float shadowX = 32.0f;
	float shadowY = -16.0f;

	bool side = false;
	bool right = false;
	bool left = false;

	int moveCnt = 0;
	int _rand = 0;
	int _randPosition = 0;
};
#endif //!COMPONENT_MIDDLENEMYACTION_H_INCLUDE