#pragma once
#include "CPPBehaviour.h"
#include "CPPSBaseGolfBall.h"
#include "Litterbox/Factory/Components.h"
#include "LitterBox/GameLogic/StateMachine.h"

namespace LB
{
	// Use GameObj to access the gameobj of the class
	class CPPSMage : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		Inherited Functions from CPPBehaviour that the user must implement
		*************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;

		//getter functions
		CPRender* GetRender();
		CPRigidBody* GetRigidBody();
		CPCollider* GetCollider();
		GameObject* GetHero();
		GameObject* GetProjectile();

		void SpawnProjectile();

		float GetSpeedMag();	//Getter function for the speed when chasing
		float GetBackOffSpeedMag(); //Getter function for the speed when backingOff
		float TooCloseDistance(); //Getter function if they get too close
		float RangeAttackDistance(); //Getter function if the in range to shoot
		float GetProjSpeed();

		double& NextShot();
		double& SetTime();
		double& FireRate();
		int& Count();

		int& NumOfProj();


		bool& CheckHasShot();
		void OnCollisionEnter(CollisionData colData);
		
		//void Timer(double setTimer, int fireRate, int counter, FiniteStateMachine& fsm);

	private:
		CPRender* mRender; //animation purpose
		CPRigidBody* mRigidBody; //Getting the RB of the enemy
		CPCollider* mCollider; //Getting the collider of the enemy

		GameObject* mPlayer{ nullptr }; //Getting the player
		GameObject* mProjectile{ nullptr };

		//stats of what the mage will have
		int mHealth; //health of the mage
		float mSpeedMagnitude{}; //normal speed when approaching towards the player
		float mBackOffSpeed{}; //When player is nearby, it backs off
		float mTooClose{};

		//shooting
		int mNumOfProjectile{}; //the number of projectile the player will shoot
		float mAttackRange{};
		float mProjSpeed{};
		bool mHasShot{};

		double mnextTimeToShoot;
		int mcount;
		double msetTimer;
		double mfireRate;

		//Boundaries between enemy and player
		float mMaxDistance{};
		float mMinDistance{};
		float rangeDistance{};

		// Holds the different state and the current state
		// that the Chaser enemy is in
		FiniteStateMachine mFSM{}; //state machine of the mage enemy, I will have to add into it

		bool mInitialised{ false }; //setting to false, later after everything has been initialised in the cpp, it will set to true
	};

	// States
	//STATES : IDLE, CHASING, BACKOFF, HURT, SHOOTING
	/*!***********************************************************************
	\brief
	Idle state of the Mage enemy
	*************************************************************************/
	class MageIdleState : public State
	{
	public:
		MageIdleState(CPPSMage* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSMage* mEnemy;
	};

	/*!***********************************************************************
	\brief
	Chase state of the Mage enemy
	*************************************************************************/
	class MageChaseState : public State
	{
	public:
		MageChaseState(CPPSMage* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSMage* mEnemy;
	};

	/*!***********************************************************************
	\brief
	BackOff state of the Mage enemy
	*************************************************************************/
	class MageBackOffState : public State
	{
	public:
		MageBackOffState(CPPSMage* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSMage* mEnemy;
	};

	/*!***********************************************************************
	\brief
	Hurt state of the Mage enemy
	*************************************************************************/
	class MageHurtState : public State
	{
	public:
		MageHurtState(CPPSMage* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSMage* mEnemy;
	};

	/*!***********************************************************************
	\brief
	Shooting state of the Mage enemy
	*************************************************************************/
	class MageShootingState : public State
	{
	public:
		MageShootingState(CPPSMage* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSMage* mEnemy;
	};
}