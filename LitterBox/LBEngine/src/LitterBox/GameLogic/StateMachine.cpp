#include "StateMachine.h"

State::State(FiniteStateMachine& FSM, std::string ID) :
	mFSM{ FSM }, mStateID{ ID }
{

}

std::string State::GetStateID()
{
	return mStateID;
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

State& FiniteStateMachine::GetState(std::string stateID)
{
	return *mCurrentState;
}

void FiniteStateMachine::SetCurrentState(std::string stateID)
{
	mCurrentState = &GetState(stateID);
}