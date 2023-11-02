#include "CPPSChaser.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Serialization/AssetManager.h"


namespace LB {
	void CPPSChaser::Start()
	{
		GameObj = FACTORY->SpawnGameObject({ C_CPRender, C_CPRigidBody, C_CPCollider });
		mRender = GameObj->GetComponent<CPRender>();
		
		mRender->UpdateTexture(ASSETMANAGER->GetTextureIndex("chaser"));

		IdleState* IDLESTATE = DBG_NEW IdleState(mFSM, "Idle");
		ChaseState* CHASESTATE = DBG_NEW ChaseState(mFSM, "Chase");
		HurtState* HURTSTATE = DBG_NEW HurtState(mFSM, "Hurt");

		mFSM.AddState(IDLESTATE);
		mFSM.AddState(CHASESTATE);
		mFSM.AddState(HURTSTATE);

		mFSM.SetCurrentState("Idle");
	}

	void CPPSChaser::Update()
	{
		mFSM.Update();
	}

	void CPPSChaser::Destroy()
	{
		delete mFSM.GetState("Idle");
		delete mFSM.GetState("Chase");
		delete mFSM.GetState("Hurt");

	}

	// States ===================

	// IDLE STATE FUNCTIONS !!!

	IdleState::IdleState(FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		// Empty by design, initialied state
	}

	void IdleState::Enter()
	{
		this->Update();
	}

	void IdleState::Update()
	{
		if (INPUT->IsKeyPressed(KeyCode::KEY_R)) 
		{
			// Change the state to Chase
			GetFSM().ChangeState("Chase");
		}
	}

	void IdleState::Exit()
	{

	}

	// CHASE STATE FUNCTIONS !!!

	ChaseState::ChaseState(FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		// Empty by design, initialised state
	}

	void ChaseState::Enter()
	{

	}

	void ChaseState::Update()
	{

	}

	void ChaseState::Exit()
	{

	}

	// HURT STATE FUNCTIONS !!!

	HurtState::HurtState(FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		// Empty by design, initialised state
	}

	void HurtState::Enter()
	{

	}

	void HurtState::Update()
	{

	}

	void HurtState::Exit()
	{

	}
}