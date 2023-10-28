#include "StateMachine.h"

// State Class member functions =======================================

template <typename T>
State<T>::State(FiniteStateMachine<T>& FSM, T ID) :
	mFSM{ FSM }, mStateID{ID}
{
	// Empty by default, initialized values
}

template <typename T>
T State<T>::GetStateID()
{
	return mStateID;
}

// Finite State Machine Class member function

template <typename T>
FiniteStateMachine<T>::FiniteStateMachine()
{
	mCurrentState = nullptr;
}


template <typename T>
State<T>& FiniteStateMachine<T>::AddState(T stateType) 
{
	std::unique_ptr<State<stateType>> stateClass;
	mStateMap[stateType] = stateClass;
	return mStateMap[stateType];
}

template <typename T>
State<T>& FiniteStateMachine<T>::GetState(T stateID) 
{
	return mStateMap[stateID];
}

template <typename T>
void FiniteStateMachine<T>::SetCurrentState(T stateID)
{
	mCurrentState = &GetState(stateID);
}