#pragma once
#include "LitterBox/Engine/Input.h"
#include <map>
#include <memory>

// Forward declaration of FiniteStateMachine class
template <typename T>
class FiniteStateMachine;

// T here would be like a
// scoped enum class type such as T = CHASERENEMY
// which would have enums like IDLE, CHASE, etc...
template <typename T>
class State
{
public:
	// Stores the FSM that it is located in, allowing
	// for the change of states within the FSM
	State(FiniteStateMachine<T>& FSM, T ID);

	T GetStateID();

	// Virtual functions
	//virtual ~State();

	virtual void Enter();

	virtual void Update();

	virtual void Exit();



private:
	FiniteStateMachine<T>& mFSM;
	T mStateID;
};

// Consists of a set of states
// Only can be in one state at a time
template <typename T>
class FiniteStateMachine 
{
public:

	FiniteStateMachine();

	// Add a state
	State<T>& AddState(std::unique_ptr<State<T>> stateClass);

	// Get a state
	State<T>& GetState(T stateID);

	// Sets a state as the current state
	// Returns true if successful
	// False if unsuccessful
	void SetCurrentState(T stateID);

private:
	// Map that contains all the states within this given FiniteStateMachine
	std::map<T, std::unique_ptr<State<T>>> mStateMap;

	State<T>* mCurrentState;
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