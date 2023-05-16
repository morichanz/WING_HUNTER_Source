#include"FactoryBase.h"
#include"IsState/IsState.h"

void FactoryBase::Initialize()
{

}
void FactoryBase::Execute(float deltaTime)
{

}
void FactoryBase::Finalize()
{
	state->Finalize();
}
void FactoryBase::StateExecute(float deltaTime)
{
	state->Execute(deltaTime);
}