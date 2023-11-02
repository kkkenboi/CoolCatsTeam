#include "StateMachine.h"

State::State(FiniteStateMachine& FSM, std::string ID) :
	mFSM{ FSM }, mStateID{ ID }
{

}

std::string State::GetStateID()
{
	return mStateID;
}

FiniteStateMachine& State::GetFSM()
{
	return mFSM;
}

// =====

FiniteStateMachine::FiniteStateMachine()
{
	mCurrentState = nullptr;
}

void FiniteStateMachine::AddState(State* stateType)
{
	if (!(mStateMap.find(stateType->GetStateID()) == mStateMap.end()))
	{
		// Found the key already, so do not make a new key
		return;
	}
	mStateMap[stateType->GetStateID()] = stateType;
}

State* FiniteStateMachine::GetState(std::string stateID)
{
	return mCurrentState;
}

void FiniteStateMachine::SetCurrentState(std::string stateID)
{
	mCurrentState = GetState(stateID);
}

void FiniteStateMachine::ChangeState(std::string stateID)
{
	mCurrentState->Exit();

	mCurrentState = GetState(stateID);

	mCurrentState->Enter();
}

void FiniteStateMachine::Update()
{
	if (mCurrentState != nullptr)
	{
		mCurrentState->Update();
	}
}