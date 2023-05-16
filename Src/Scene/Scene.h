/**
* @file Scene.h
*/
#ifndef SCENE_H_INCLUDED
 #define SCENE_H_INCLUDED
 #include <memory>

//先行宣言
class Engine;
class GameObject;
class MeshRendarer;
using GameObjectPtr = std::shared_ptr<GameObject>;

/**
* シーンの基底クラス
*/
class Scene
 {
	public:
		Scene() = default;	
		virtual ~Scene() = default;
		virtual bool Initialize(Engine & engine) { return true; }
		virtual void Update(Engine & engine, float deltaTime) = 0;
		virtual void Finalize(Engine & engine) {}
};

using ScenePtr = std::shared_ptr<Scene>; //シーンポインタ型

#endif //SCENE_H_INCLUDED