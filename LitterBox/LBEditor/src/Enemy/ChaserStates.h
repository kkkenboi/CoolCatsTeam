#pragma once
#include "LitterBox/GameLogic/StateMachine.h"
/*
enum class ChaserStateType 
{
	IDLE,
	CHASE,
	DAMAGED
};
*/

class ChaserIdleState : public State
{
public:
	ChaserIdleState(FiniteStateMachine& fsm, std::string name);

	// Virtual function overrides
	void Enter() override;

	void Update() override;

	void Exit() override;

	// Keep the functions virtual if we want to have
	// a derived class from ChaserIdleState

private:
};

class ChaserChaseState : public State
{
public:
	ChaserChaseState(FiniteStateMachine& fsm, std::string name);

	// Virtual function overrides
	void Enter() override;

	void Update() override;

	void Exit() override;
};

