#pragma once
#include "CPPBehaviour.h"
#include "Litterbox/Factory/Components.h"
#include "LitterBox/GameLogic/StateMachine.h"

namespace LB 
{
	class CPPSChaser : public CPPBehaviour 
	{
	public:
		// Use GameObj to access the gameobj of the class

		void Start() override;

		void Update() override;

		void Destroy() override;



	private:
		CPRender* mRender;
		CPRigidBody* mRigidBody;
		CPCollider* mCollider;

		// Holds the different state and the current state
		// that the Chaser enemy is in
		FiniteStateMachine mFSM;

		int mHealth;
		int mSpeed;
	};

	// States
	class IdleState : public State
	{
	public:
		IdleState(FiniteStateMachine& fsm, std::string name);

		void Enter() override;

		void Update() override;

		void Exit() override;
	private:
	};

	class ChaseState : public State
	{
	public:
		ChaseState(FiniteStateMachine& fsm, std::string name);

		void Enter() override;

		void Update() override;

		void Exit() override;
	private:
	};

	class HurtState : public State
	{
	public:
		HurtState(FiniteStateMachine& fsm, std::string name);

		void Enter() override;

		void Update() override;

		void Exit() override;
	private:
	};

}