#include "CPPSChaser.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Debugging/Debug.h"

#include "LitterBox/Factory/GameObjectManager.h"

namespace LB {

	void CPPSChaser::Start()
	{
		//GameObj = FACTORY->SpawnGameObject({ C_CPRender, C_CPRigidBody, C_CPCollider });
		if (GameObj->HasComponent<CPRender>()) 
		{
			mRender = GameObj->GetComponent<CPRender>();
		}
		else
		{
			mRender = nullptr;
			return;
		}
		if (GameObj->HasComponent<CPRigidBody>())
		{
			mRigidBody = GameObj->GetComponent<CPRigidBody>();
		}
		else 
		{
			mRigidBody = nullptr;
			return;
		}
		if (GameObj->HasComponent<CPCollider>())
		{
			mCollider = GameObj->GetComponent<CPCollider>();
		}
		else 
		{
			mCollider = nullptr;
			return;
		}

		mRender->UpdateTexture(ASSETMANAGER->GetTextureIndex("chaser"));


		IdleState* IDLESTATE = DBG_NEW IdleState(this, mFSM, "Idle");
		ChaseState* CHASESTATE = DBG_NEW ChaseState(this, mFSM, "Chase");
		HurtState* HURTSTATE = DBG_NEW HurtState(this, mFSM, "Hurt");

		mFSM.AddState(IDLESTATE);
		mFSM.AddState(CHASESTATE);
		mFSM.AddState(HURTSTATE);

		mFSM.SetCurrentState("Idle");

		std::vector<GameObject*> GOs = GOMANAGER->GetGameObjects();
		for (GameObject* GO : GOs) {
			if (GO->GetName() == "MainChar") {
				mPlayer = GO;
				break;
			}
		}

		mHealth = 3;
		mSpeedMagnitude = 1000.f;

		mInitialised = true;
	}

	void CPPSChaser::Update()
	{
		//DebuggerLog("In ChaserUpdate\n");
		if (mInitialised == false)
		{
			return;
		}
		mFSM.Update();
	}

	void CPPSChaser::Destroy()
	{
		delete mFSM.GetState("Idle");
		delete mFSM.GetState("Chase");
		delete mFSM.GetState("Hurt");

	}

	CPRender* CPPSChaser::GetRender()
	{
		return mRender;
	}

	CPRigidBody* CPPSChaser::GetRigidBody()
	{
		return mRigidBody;
	}

	CPCollider* CPPSChaser::GetCollider()
	{
		return mCollider;
	}

	GameObject* CPPSChaser::GetHero()
	{
		return mPlayer;
	}

	float CPPSChaser::GetSpeedMag()
	{
		return mSpeedMagnitude;
	}

	// States ===================

	// IDLE STATE FUNCTIONS !!!

	IdleState::IdleState(CPPSChaser* enemy_ptr, FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
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
	ChaseState::ChaseState(CPPSChaser* enemy_ptr, FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
	}

	void ChaseState::Enter()
	{
		DebuggerLog("Entered ChaseState");
		this->Update();
	}

	void ChaseState::Update()
	{
		// Calculate direction of force from enemy to player
		Vec2<float> CurEnemyPos = mEnemy->GetRigidBody()->getPos();
		Vec2<float> CurHeroPos = mEnemy->GetHero()->GetComponent<CPRigidBody>()->getPos();

		Vec2<float> Direction = CurHeroPos - CurEnemyPos;
		Direction = Normalise(Direction);

		Direction = Direction * mEnemy->GetSpeedMag();
		DebuggerLogFormat("%f, %f", Direction.x, Direction.y);
		mEnemy->GetRigidBody()->addForce(Direction);
	}

	void ChaseState::Exit()
	{

	}

	// HURT STATE FUNCTIONS !!!
	HurtState::HurtState(CPPSChaser* enemy_ptr, FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
	}

	void HurtState::Enter()
	{
		this->Update();
	}

	void HurtState::Update()
	{

	}

	void HurtState::Exit()
	{

	}
}