/**
* FactoryBase.h
*/
#include"IsState/IsState.h"
#include<memory>
#include<string>

class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>;

class FactoryBase : public std::enable_shared_from_this<FactoryBase>
{
public:
	FactoryBase(GameObjectPtr _gameObject):gameObject(_gameObject){}
	virtual ~FactoryBase() = default;

	template<typename T>
	void SetState()
	{
		state.reset();
		state = std::make_shared<T>();
		state->GiveBaseFactory(shared_from_this());
		state->GiveGameObject(gameObject);
		state->Initialize();
	}

	virtual void Initialize();
	virtual void Execute(float deltaTime);
	virtual void Finalize();
	void StateExecute(float deltaTime);

private:
	std::shared_ptr<STATE::IsState> state;
	GameObjectPtr gameObject;
	std::string name;
};