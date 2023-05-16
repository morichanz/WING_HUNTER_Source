/**
* @file GameObject.h
*/

#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include"../Sprite/Sprite.h"
#include"../Component/Component.h"

#include"../Rect.h"
#include<string>
#include<vector>
#include<memory>
//#include <type_traits>

//先行宣言
class Engine;
class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>;	//コンポーネントのポインタ
using GameObjectList = std::vector<GameObjectPtr>;	//コンポーネントポインタの配列
class MeshRenderer;
using MeshRendererPtr = std::shared_ptr<MeshRenderer>;
using MeshRendererList = std::vector<MeshRendererPtr>;
class ProgramPipeline;


/**
* ゲーム内に登場する様々なオブジェクトを表す基本クラス
*/
class GameObject:public std::enable_shared_from_this<GameObject>
{
public:
	GameObject() = default;
	virtual ~GameObject() = default;

	//イベントの操作
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void OnCollision(GameObject& object);
	virtual void OnDestroy();
	virtual void TakeDamage(GameObject& other, const Damage& damage);
	bool IsStarted() const { return isStarted; }

	//スプライトの操作
	void AddSprite(const Texcoord& tc, float x = 0, float y = 0, float scalex = 1, float scaley = 1, float scale = 1, float rotation = 0, float alpha = 1,float color = 1);
	void ClearSprite();
	const SpriteList& GetSprite() const { return spriteList; }

	//コリジョンの操作
	void AddCollision(const Rect& r);
	void RemoveCollision();

	const RectList& GetCollision() const { return collisionList; }
	const Rect& GetAabb() const { return aabb; }
	void SetAabb(Rect aabb) {
		this->aabb = aabb;
	}

	//コンポーネントの操作
	template<typename T>
	std::shared_ptr<T> AddComponent()
	{
		std::shared_ptr<T> p = std::make_shared<T>();
		componentList.push_back(p);
		//メッシュレンダラの場合は専用リストに追加
		if constexpr(std::is_base_of<MeshRenderer, T>::value)
		{
			meshRendererList.push_back(p);
		}
		return p;
	}
	
	template <typename T>
	std::shared_ptr<T> GetComponent()
	{
		auto component = std::make_shared<T>();
		for (auto itr : componentList)
		{
			if (component->name == itr->name)
			{
				return std::dynamic_pointer_cast<T>(itr) ;
			}
		}
		return nullptr;
	}

	//描画
	void DrawStaticMesh(const ProgramPipeline & program) const;
public:

	Engine* engine = nullptr;	//エンジンのアドレス
	std::string name;			//オブジェクト名
	float x = 0.0f;				//x座標
	float y = 0.0f;				//y座標
	float z = 0.0f;                 //Z座標
	float rotation[3] = { 0, 0, 0 }; //回転
	int priority = 0;			//表示優先順位
	SpriteList spriteList;		//スプライト配列
	ComponentList componentList;//コンポーネント配列
	MeshRendererPtr meshRenderer;
	bool isDead = false;		//死亡フラグ
	int health = 10;			//体力
	float scale = 0.0f;			//サイズ
	bool clearFlg = false;		//クリアフラグ
	bool scoreFlg = false;		//スコアフラグ(画面外に出たときはスコアが加算されないようにする)
	float clearTimer = 1;		//クリアに行くタイムラグ
	bool bossFlg = false;	//ボス出現フラグ
	int AI = 0;
	bool deleteFlg = false;
	float alpha = 1.0f;
	bool shotFlg = false;
	float size = 1.0f;
	bool collisionFlg = false;
	float invincible = 0.0f; //無敵時間
	float rotate = 0.0f;
	bool damageFlg = false;
	bool beamEndFlg = false;
	bool push = false;
	float translate = 0.0f;
	bool chargeFlg = false;
private:
	bool isStarted = false;		//スターと実行済みフラグ
	RectList collisionList;		//コリジョン配列
	Rect aabb = { 0,0,0,0 };	//コリジョン境界ボックス
	std::vector<MeshRendererPtr> meshRendererList;
};
#endif //GAMEOBJECT_H_INCLUDED
