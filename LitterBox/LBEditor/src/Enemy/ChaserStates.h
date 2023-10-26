#pragma once
#include "LitterBox/GameLogic/StateMachine.h"

enum class ChaserStateType 
{
	IDLE,
	CHASE,
	DAMAGED
};

class ChaserIdleState : public State<ChaserStateType> 
{
public:
	ChaserIdleState(FiniteStateMachine<ChaserStateType>& fsm) :
		State(fsm, ChaserStateType::IDLE) 
	{
		// State base class is initialized with
		// fsm and IDLE as its params
		// Empty by design
	}

	// Virtual function overrides
	virtual void Enter() override;

	virtual void Update() override;

	virtual void Exit() override;

	// Keep the functions virtual if we want to have
	// a derived class from ChaserIdleState

private:
};

class ChaserChaseState : public State<ChaserStateType>
{
public:
	ChaserChaseState(FiniteStateMachine<ChaserStateType>& fsm) :
		State(fsm, ChaserStateType::CHASE)
	{
		// State base class is initialized with
		// fsm and IDLE as its params
		// Empty by design
	}

	// Virtual function overrides
	virtual void Enter();

	virtual void Update();

	virtual void Exit();
};
