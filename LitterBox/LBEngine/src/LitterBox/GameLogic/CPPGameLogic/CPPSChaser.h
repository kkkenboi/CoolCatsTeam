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

		CPRender* GetRender();

		CPRigidBody* GetRigidBody();

		CPCollider* GetCollider();

		GameObject* GetHero();

		float GetSpeedMag();

	private:
		CPRender* mRender{ nullptr };
		CPRigidBody* mRigidBody{ nullptr };
		CPCollider* mCollider{ nullptr };

		GameObject* mPlayer{ nullptr };

		// Holds the different state and the current state
		// that the Chaser enemy is in
		FiniteStateMachine mFSM{};

		int mHealth{};
		float mSpeedMagnitude{};

		bool mInitialised{false};
	};

	// States
	class IdleState : public State
	{
	public:
		IdleState(CPPSChaser* enemy_ptr ,FiniteStateMachine& fsm, std::string name);

		void Enter() override;

		void Update() override;

		void Exit() override;
	private:
		CPPSChaser* mEnemy;
	};

	class ChaseState : public State
	{
	public:
		ChaseState(CPPSChaser* enemy_ptr, FiniteStateMachine& fsm, std::string name);

		void Enter() override;

		void Update() override;

		void Exit() override;
	private:
		CPPSChaser* mEnemy;
	};

	class HurtState : public State
	{
	public:
		HurtState(CPPSChaser* enemy_ptr, FiniteStateMachine& fsm, std::string name);

		void Enter() override;

		void Update() override;

		void Exit() override;
	private:
		CPPSChaser* mEnemy;
	};

}