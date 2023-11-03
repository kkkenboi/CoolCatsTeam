#pragma once
//#include "LitterBox/Engine/Input.h"
#include <map>
#include <memory>
#include <string>

// Forward declaration of FiniteStateMachine class
class FiniteStateMachine;

// T here would be like a
// scoped enum class type such as T = CHASERENEMY
// which would have enums like IDLE, CHASE, etc...
class State
{
public:
	// Stores the FSM that it is located in, allowing
	// for the change of states within the FSM
	State(FiniteStateMachine& FSM, std::string ID);

	std::string GetStateID();

	FiniteStateMachine& GetFSM();

	// Virtual functions
	//virtual ~State();
	
	virtual void Enter() = 0;

	virtual void Update() = 0;

	virtual void Exit() = 0;



private:
	FiniteStateMachine& mFSM;
	std::string mStateID;
};

// Consists of a set of states
// Only can be in one state at a time
class FiniteStateMachine 
{
public:

	FiniteStateMachine();

	// Add a state
	void AddState(State* stateType);

	// Get a state
	State* GetState(std::string stateID);

	// Sets a state as the current state
	// Returns true if successful
	// False if unsuccessful
	void SetCurrentState(std::string stateID);

	void ChangeState(std::string stateID);
	
	void Update();

private:
	// Map that contains all the states within this given FiniteStateMachine
	std::map<std::string, State*> mStateMap;

	State* mCurrentState;
};


/*
class BaseState
{
public:
	BaseState(FiniteStateMachine FSM);

	// Virtual functions
	virtual ~BaseState();
	virtual void Enter();
	virtual void Update();
	virtual void Exit();

private:
	FiniteStateMachine& mFSM;
};

class FiniteStateMachine
{
public:
	
private:
	BaseState* mCurrentState;
};
*/



/*
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
State<T>& FiniteStateMachine<T>::AddState(State<T> stateType)
{
	mStateMap[stateType] = stateType;
    return *mStateMap[stateType];
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
*/