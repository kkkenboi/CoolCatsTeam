#include "CPPSMage.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Debugging/Debug.h"

#include "CPPSBaseGolfBall.h"

#include "LitterBox/Engine/Time.h"

#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Physics/PhysicsMath.h"
//namespace test
//{
//	double nextTimeToShoot = 0.0;
//	int counter = 0;
//	double setTimer = 0.0;
//	double fireRate = 2.0f;
//}

namespace LB
{
	std::array<std::array<LB::Vec2<float>, 4>, 33> mage_anim_frams;
	void CPPSMage::Start()
	{
		//initialising the components of the mage, basically same as chase where I am getting the components
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

		//get the mage animation
		if (ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap["sheet"]) != LB::ASSETMANAGER->Textures.end()) {
			int img_width{ LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap["sheet"])->second.first->width };
			int img_height{ LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap["sheet"])->second.first->height };
		
			float x_inc{ 1.f / 17.f };
			float y_inc{ 110.f / (float)img_height };

			//y increments 110 pixels at a time for 4 times
			//for(int y{0}; y < 3; ++y)
				for (int x{ 0 }; x < 17; ++x) {
					mage_anim_frams[x].at(0) = { x * x_inc, 0.f };//bottom left
					mage_anim_frams[x].at(1) = { (x + 1) * x_inc, 0.f };//bottom right
					mage_anim_frams[x].at(2) = { (x + 1) * x_inc, 1.f };//top right
					mage_anim_frams[x].at(3) = { x * x_inc, 1.f };//top left
				}


			Renderer::GRAPHICS->init_anim("mage_float", mage_anim_frams.data(), 0.5f, 17);

			mRender->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("sheet"), LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap["sheet"])->second.first->width, LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap["sheet"])->second.first->height);
			mRender->play_repeat("mage_float");
		}

		
		//initialse the state of the mage
		//STATES : IDLE, CHASING, BACKOFF, HURT, SHOOTING
		MageIdleState* IDLESTATE = DBG_NEW MageIdleState(this, mFSM, "Idle");
		MageChaseState* CHASESTATE = DBG_NEW MageChaseState(this, mFSM, "Chase");
		MageBackOffState* BACKOFFSTATE = DBG_NEW MageBackOffState(this, mFSM, "BackOff");
		MageHurtState* HURTSTATE = DBG_NEW MageHurtState(this, mFSM, "Hurt");
		MageShootingState* SHOOTINGSTATE = DBG_NEW MageShootingState(this, mFSM, "Shooting");

		//STATES : IDLE, CHASING, BACKOFF, HURT, SHOOTING
		mFSM.AddState(IDLESTATE);
		mFSM.AddState(CHASESTATE);
		mFSM.AddState(BACKOFFSTATE);
		mFSM.AddState(HURTSTATE);
		mFSM.AddState(SHOOTINGSTATE);

		//set current state of Mage to be on idle
		mFSM.SetCurrentState("Idle");

		std::vector<GameObject*> const& GOs = GOMANAGER->GetGameObjects();
		for (GameObject* GO : GOs) {
			if (GO->GetName() == "MainChar") {
				mPlayer = GO;
				break;
			}
		}

		//initialise the variables for the Mage
		mHealth = 3;
		mSpeedMagnitude = 1000.f;
		mProjSpeed = 1500.0f;
		mNumOfProjectile = 3; //set to 3 projectiles

		mHasShot = false;

		//a little hardcoding for now
		mMinDistance = 120.0f;
		mMaxDistance = 1330.0f;
		rangeDistance = mMaxDistance - mMinDistance;

		mnextTimeToShoot = 0.0;
		mcount = 0;
		msetTimer = 0.0;
		mfireRate = 2.0;

		//timer
		//mtimerInterval = 3.0f; //3 second interval
		//mtimerDone = false;

		//it has fully initialised
		mInitialised = true;
		
	}

	void CPPSMage::Update()
	{
		if (mInitialised == false)
		{
			return;
		}
		mFSM.Update();
	}

	void CPPSMage::Destroy()
	{
		//STATES : IDLE, CHASING, BACKOFF, HURT, SHOOTING
		delete mFSM.GetState("Idle");
		delete mFSM.GetState("Chase");
		delete mFSM.GetState("BackOff");
		delete mFSM.GetState("Hurt");
		delete mFSM.GetState("Shooting");
	}

	//Getter functions
	/*!***********************************************************************
	\brief
	Getter for the render component
	*************************************************************************/
	CPRender* CPPSMage::GetRender()
	{
		return mRender;
	}

	/*!***********************************************************************
	\brief
	Getter for the rigidbody component
	*************************************************************************/
	CPRigidBody* CPPSMage::GetRigidBody()
	{
		return mRigidBody;
	}

	/*!***********************************************************************
	\brief
	Getter for the collider component
	*************************************************************************/
	CPCollider* CPPSMage::GetCollider()
	{
		return mCollider;
	}

	/*!***********************************************************************
	\brief
	Getter for the player object
	*************************************************************************/
	GameObject* CPPSMage::GetHero()
	{
		return mPlayer;
	}

	GameObject* CPPSMage::GetProjectile()
	{
		return mProjectile;
	}

	float CPPSMage::GetProjSpeed()
	{
		return mProjSpeed;
	}

	/*!***********************************************************************
	\brief
	Getter for speed magnitude
	*************************************************************************/
	float CPPSMage::GetSpeedMag()
	{
		return mSpeedMagnitude;
	}

	/*!***********************************************************************
	\brief
	Getter for speed magnitude
	*************************************************************************/
	float CPPSMage::GetBackOffSpeedMag()
	{
		return mBackOffSpeed = mSpeedMagnitude / 2.0f;
	}

	/*!***********************************************************************
	\brief
	Getter for tooClose
	*************************************************************************/
	float CPPSMage::TooCloseDistance()
	{
		mTooClose = rangeDistance / 3.0f;
		return mTooClose;
	}

	/*!***********************************************************************
	\brief
	Getter for range attack
	*************************************************************************/
	float CPPSMage::RangeAttackDistance()
	{
		mAttackRange = rangeDistance / 2.0f;
		return mAttackRange;
	}

	void CPPSMage::OnCollisionEnter(CollisionData colData)
	{
		if (this->mFSM.GetCurrentState()->GetStateID() == "Chase")
		{
			if (colData.colliderOther->m_gameobj->GetName() == "ball") {
				if (PHY_MATH::Length(colData.colliderOther->GetRigidBody()->mVelocity) > 500.f)
				{
					--mHealth;
					mFSM.ChangeState("Hurt");
				}
			}
		}

	}

	///*!***********************************************************************
	//\brief
	//Getter for Timer
	//*************************************************************************/
	//float& CPPSMage::GetTimer()
	//{
	//	return mtimerInterval;
	//}

	//bool& CPPSMage::TimerCheck()
	//{
	//	return mtimerDone;
	//}
	/*!***********************************************************************
	\brief
	Getter for shooting intervals timer
	*************************************************************************/
	//void CPPSMage::Timer(double setTimer, int fireRate, int counter, FiniteStateMachine& fsm)
	//{
	//	if (TIME->GetTime() >= NextShot())
	//	{
	//		NextShot() = TIME->GetTime() + (double)(1 / FireRate());
	//		++Count();
	//		if (Count() == NumOfProj())
	//		{
	//			Count() = 0;
	//			fsm.ChangeState("Idle");
	//		}
	//	}
	//}

	bool& CPPSMage::CheckHasShot()
	{
		return mHasShot;
	}

	void CPPSMage::SpawnProjectile()
	{
		Vec2<float> CurHeroPos = GetHero()->GetComponent<CPRigidBody>()->getPos(); //Getting the Player Position
		Vec2<float> CurEnemyPos = GetRigidBody()->getPos(); //Getting the current Mage Position

		float offset = 100.0f;
		Vec2<float> Direction = (CurHeroPos - (Vec2<float>{ CurEnemyPos.x + offset, CurEnemyPos.y + offset })).Normalise();
		Vec2<float> ShootingForce = Direction * GetSpeedMag() * 3.0f;


		Vec2<float> PosToSpawn{ CurEnemyPos.x + offset, CurEnemyPos.y + offset };
		//ASSETMANAGER->SpawnGameObject("Projectile", PosToSpawn);
		int Channel = AUDIOMANAGER->PlaySound("Fire, Whoosh, Flame, Fireball, Fast x4 SND11948 1");
		AUDIOMANAGER->SetChannelVolume(Channel, 0.3f);

		GameObject* mageProjectileObject = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("Projectile", *mageProjectileObject);
		if (!(PosToSpawn == Vec2<float>{0, 0}))
		{
			mageProjectileObject->GetComponent<CPTransform>()->SetPosition(PosToSpawn);
		}

		mageProjectileObject->GetComponent<CPRigidBody>()->addImpulse(ShootingForce);
	}

	int& CPPSMage::NumOfProj()
	{
		return mNumOfProjectile;
	}
	
	double& CPPSMage::NextShot()
	{
		return mnextTimeToShoot;
	}

	double& CPPSMage::SetTime()
	{
		return msetTimer;
	}

	double& CPPSMage::FireRate()
	{
		return mfireRate;
	}

	int& CPPSMage::Count()
	{
		return mcount;
	}
	// States ===================

	/*!***********************************************************************
	\brief
	IDLE STATE FUNCTIONS (Ctor, Enter,Update,Exit)
	*************************************************************************/
	MageIdleState::MageIdleState(CPPSMage* enemy_ptr, FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
	}

	void MageIdleState::Enter()
	{
		this->Update();
	}
	void MageIdleState::Update()
	{
		DebuggerLog("Entered MageIdleState");
		if (INPUT->IsKeyPressed(KeyCode::KEY_M)) //MAGE WILL CHASE WHEN THIS IS PRESSED
		{
			// Change the state to Chase
			GetFSM().ChangeState("Chase");
		}
	}
	void MageIdleState::Exit()
	{
	}

	/*!***********************************************************************
	\brief
	CHASE STATE FUNCTIONS (Ctor, Enter,Update,Exit)
	*************************************************************************/
	MageChaseState::MageChaseState(CPPSMage* enemy_ptr, FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
	}

	void MageChaseState::Enter()
	{
		DebuggerLog("Entered MageChaseState");
		this->Update();
	}
	void MageChaseState::Update()
	{
		DebuggerLog("Entered MageChaseState");
		//Calculating the distance between the Enemy and the player
		Vec2<float> CurEnemyPos = mEnemy->GetRigidBody()->getPos(); //Getting the current Mage Position
		Vec2<float> CurHeroPos = mEnemy->GetHero()->GetComponent<CPRigidBody>()->getPos(); //Getting the Player Position
		float DistInBwn = Vec2<float>::Distance(CurEnemyPos, CurHeroPos);

		////Getting the direction of the player and the enemy
		Vec2<float> Direction = (CurHeroPos - CurEnemyPos).Normalise();
		Vec2<float> NormalForce = Direction * mEnemy->GetSpeedMag();
		
		mEnemy->GetRigidBody()->addForce(NormalForce);

		if (DistInBwn < mEnemy->TooCloseDistance())
		{
			GetFSM().ChangeState("BackOff");
		}
		else if (DistInBwn >= mEnemy->TooCloseDistance() && DistInBwn <= mEnemy->RangeAttackDistance())
		{
			GetFSM().ChangeState("Shooting");
		}
		
		//I need to check if its too close or not
		//if so it will backoff to the opposite direction from where its heading
		
		//else, it will just go towards the player

	}
	void MageChaseState::Exit()
	{
	}

	/*!***********************************************************************
	\brief
	BACKOFF STATE FUNCTIONS (Ctor, Enter,Update,Exit)
	*************************************************************************/
	MageBackOffState::MageBackOffState(CPPSMage* enemy_ptr, FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
	}

	void MageBackOffState::Enter()
	{
		DebuggerLog("Entered MageBackOffState");
		this->Update();
	}
	void MageBackOffState::Update()
	{
		DebuggerLog("Entered MageBackOffState");
		//Calculating the distance between the Enemy and the player
		Vec2<float> CurEnemyPos = mEnemy->GetRigidBody()->getPos(); //Getting the current Mage Position
		Vec2<float> CurHeroPos = mEnemy->GetHero()->GetComponent<CPRigidBody>()->getPos(); //Getting the Player Position
		float DistInBwn = Vec2<float>::Distance(CurEnemyPos, CurHeroPos);

		//Getting the direction of the player and the enemy
		Vec2<float> Direction = (CurHeroPos - CurEnemyPos).Normalise();
		Vec2<float> BackOffForce = (-Direction) * mEnemy->GetBackOffSpeedMag();

		mEnemy->GetRigidBody()->addForce(BackOffForce);

		if (DistInBwn >= mEnemy->TooCloseDistance())
		{
			GetFSM().ChangeState("Chase");
		}
		else if (DistInBwn >= mEnemy->TooCloseDistance() && DistInBwn <= mEnemy->RangeAttackDistance())
		{
			GetFSM().ChangeState("Shooting");
		}
	}
	void MageBackOffState::Exit()
	{
	}

	/*!***********************************************************************
	\brief
	HURT STATE FUNCTIONS (Ctor, Enter,Update,Exit)
	*************************************************************************/
	MageHurtState::MageHurtState(CPPSMage* enemy_ptr, FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
	}

	void MageHurtState::Enter()
	{
		DebuggerLog("Entered MageHurtState");
		this->Update();
	}
	void MageHurtState::Update()
	{
		DebuggerLog("Entered MageHurtState");
	}
	void MageHurtState::Exit()
	{
	}

	/*!***********************************************************************
	\brief
	SHOOTING STATE FUNCTIONS (Ctor, Enter,Update,Exit)
	*************************************************************************/
	MageShootingState::MageShootingState(CPPSMage* enemy_ptr, FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
	}

	void MageShootingState::Enter()
	{
		DebuggerLog("Entered MageShootingState");
		
		this->Update();
	}
	void MageShootingState::Update()
	{
		Vec2<float> CurEnemyPos = mEnemy->GetRigidBody()->getPos(); //Getting the current Mage Position
		Vec2<float> CurHeroPos = mEnemy->GetHero()->GetComponent<CPRigidBody>()->getPos(); //Getting the Player Position
		//float DistInBwn = Vec2<float>::Distance(CurEnemyPos, CurHeroPos);

		////Getting the direction of the player and the enemy
		Vec2<float> Direction = (CurHeroPos - CurEnemyPos).Normalise();
		Vec2<float> ShootingForce = Direction * (mEnemy->GetSpeedMag() * 10.0f);

		//if (INPUT->IsKeyPressed(KeyCode::KEY_5))
		//{
			if (TIME->GetTime() >= mEnemy->NextShot())
			{
				mEnemy->SpawnProjectile();
				//current time + 1/firerate
				mEnemy->NextShot() = TIME->GetTime() + (double)(1 / mEnemy->FireRate());
				//++mEnemy->FireRate();
				DebuggerLog("SHOOOOTTTINGGG");
				DebuggerLogFormat("Entered MageShootingState: %d", mEnemy->SetTime());
				++mEnemy->Count();
				if (mEnemy->Count() == mEnemy->NumOfProj())
				{
					mEnemy->Count() = 0;
					GetFSM().ChangeState("Idle");	
				}
			}
		//}
		
		//if (INPUT->IsKeyPressed(KeyCode::KEY_5) && mEnemy->CheckHasShot() == false)
		//{
			//if (mEnemy->NumOfProj() >= 0) //if got bullet
			//{
			//	mEnemy->SpawnProjectile(); //it spawns for now
			//	mEnemy->CheckHasShot() = true; //meaning it shot
			//	//run the timer
			//	if (mEnemy->TimerCheck() == false) //timer is not done
			//	{
			//		DebuggerLog("Entered");
			//		mEnemy->GetTimer() -= TIME->GetDeltaTime(); //time is decreasing
			//		if (mEnemy->GetTimer() <= 0.0) //if hit 0
			//		{
			//			mEnemy->TimerCheck() = true; //timer is done
			//			mEnemy->CheckHasShot() = false; //and it went to have not shoot yet
			//		}
			//	}
			//	mEnemy->NumOfProj() -= 1; //decrease the num of projectile
			//	DebuggerLogFormat("Num Of Projectile: %i", mEnemy->NumOfProj());
			//}
		//}
		//test->StartComponents();
		
	}

	void MageShootingState::Exit()
	{
	}
}