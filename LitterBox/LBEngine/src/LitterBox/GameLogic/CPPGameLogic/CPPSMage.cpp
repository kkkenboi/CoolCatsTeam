/*!************************************************************************
 \file				CPPSMage.cpp
 \author(s)			Vanessa Chua Siew Jin, Ryan Tan Jian Hao
 \par DP email(s):	vanessasiewjin@digipen.edu, ryanjianhao.tan\@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the CPPSMage class and all its functionalities,
it handls the logic for the Mage enemy

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPSMage.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Debugging/Debug.h"

#include "CPPSBaseGolfBall.h"

#include "LitterBox/Engine/Time.h"

#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Physics/PhysicsMath.h"

namespace LB
{
	//animation array
	std::array<std::array<LB::Vec2<float>, 4>, 33> mage_anim_frams;
	/*!***********************************************************************
	\brief
	Starting behaviour for Mage where variables are initialized
	*************************************************************************/
	void CPPSMage::Start()
	{
		//initialising the components of the mage, basically same as chase where I am getting the components
		mRender = GameObj->GetComponent<CPRender>();
		mRigidBody = GameObj->GetComponent<CPRigidBody>();
		mCollider = GameObj->GetComponent<CPCollider>();

		//--------------------------------get the mage animation--------------------------------
		if (ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap["sheet"]) != LB::ASSETMANAGER->Textures.end()) {
			int img_width{ LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap["sheet"])->second.first->width };
			int img_height{ LB::ASSETMANAGER->Textures.find(ASSETMANAGER->assetMap["sheet"])->second.first->height };
		
			float x_inc{ 1.f / 17.f };
			float y_inc{ 110.f / (float)img_height };

			//y increments 110 pixels at a time for 4 times
			for (int x{ 0 }; x < 17; ++x) {
				mage_anim_frams[x].at(0) = { x * x_inc, 0.f };//bottom left
				mage_anim_frams[x].at(1) = { (x + 1) * x_inc, 0.f };//bottom right
				mage_anim_frams[x].at(2) = { (x + 1) * x_inc, 1.f };//top right
				mage_anim_frams[x].at(3) = { x * x_inc, 1.f };//top left
			}

			Renderer::GRAPHICS->init_anim("mage_float", mage_anim_frams.data(), 0.5f, 17);
			Renderer::GRAPHICS->init_anim("mage_idle", mage_anim_frams.data(), 1.f, 1);

			mRender->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("sheet"), mRender->w, mRender->h);
			mRender->play_repeat("mage_idle");
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
		mHealth = 3; //health
		mSpeedMagnitude = 1000.f; //speed of movement
		mBackOffSpeed = 500.f; //speed of movement

		//------------------CHASE STATE------------------
		//a little hardcoding for now, min and max distance between enemy and player
		mAttackCooldown = 2.0f;
		mAttackCooldownCurrent = 0.0f;

		mMinDistance = 400.0f;
		mBackOffDistance = 800.0f;
		mMaxDistance = 1500.0f;

		//------------------SHOOTING STATE------------------
		//timer set for shooting of projectils
		mNumOfProjectile = 3; //set to 3 projectiles
		mNumOfProjectileCurrent = 0;
		mProjCooldown = 0.35f;
		mProjCooldownCurrent = 0.0f;
		mProjSpeed = 1000.0f; //pojectile speed

		// 0.5 seconds of invincibility
		mGotAttacked = 0.5f;

		//it has fully initialised
		mInitialised = true;
	}

	/*!***********************************************************************
	\brief
	Update behaviour for Mage, checks if everything has been initialised to start FSM
	*************************************************************************/
	void CPPSMage::Update()
	{
		if (mInitialised == false)
		{
			return;
		}
		if (mShouldDestroy)
		{
			GOMANAGER->RemoveGameObject(this->GameObj);
			return;
		}
		if (mGotAttackedCooldown > 0.0f) {
			mGotAttackedCooldown -= TIME->GetDeltaTime();
		}

		mFSM.Update();
	}

	/*!***********************************************************************
	\brief
	Desroy function fpr Mage, clear the states
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	Getter for the Projectile Object
	*************************************************************************/
	GameObject* CPPSMage::GetProjectile()
	{
		return mProjectile;
	}

	/*!***********************************************************************
	\brief
	On collision to check who it is colliding with and what will happen
	*************************************************************************/
	void CPPSMage::OnCollisionEnter(CollisionData colData)
	{
		if (colData.colliderOther->m_gameobj->GetName() == "ball") {
			int Channel = AUDIOMANAGER->PlaySound("Enemy hurt");
			AUDIOMANAGER->SetChannelVolume(Channel, 0.7f);

			if (PHY_MATH::Length(colData.colliderOther->GetRigidBody()->mVelocity) > 500.f)
			{
				if (mGotAttackedCooldown > 0.0f) {
					return;
				}
				mGotAttackedCooldown = mGotAttacked;

				--mHealth;
				mFSM.ChangeState("Hurt");

				if (mHealth < 0)
				{
					GameObj->GetComponent<CPTransform>()->SetPosition(Vec2<float>{0.0f, 10000.0f});
					GOMANAGER->RemoveGameObject(this->GameObj);
				}
			}
		}
	}

	/*!***********************************************************************
	\brief
	Spawn function to spawn projectile Game Object
	*************************************************************************/
	void CPPSMage::SpawnProjectile()
	{
		Vec2<float> CurHeroPos = GetHero()->GetComponent<CPRigidBody>()->getPos(); //Getting the Player Position
		Vec2<float> CurEnemyPos = GetRigidBody()->getPos(); //Getting the current Mage Position

		float offset = 100.0f;
		Vec2<float> Direction = (CurHeroPos - (Vec2<float>{ CurEnemyPos.x + offset, CurEnemyPos.y + offset })).Normalise();
		Vec2<float> ShootingForce = Direction * mProjSpeed;


		Vec2<float> PosToSpawn{ CurEnemyPos.x + offset, CurEnemyPos.y + offset };
		//ASSETMANAGER->SpawnGameObject("Projectile", PosToSpawn);
		int Channel = AUDIOMANAGER->PlaySound("Fire, Whoosh, Flame, Fireball, Fast x4 SND11948 1");
		AUDIOMANAGER->SetChannelVolume(Channel, 0.3f);

		//Spawn Game Object
		GameObject* mageProjectileObject = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("Projectile", *mageProjectileObject);
		if (!(PosToSpawn == Vec2<float>{0, 0}))
		{
			mageProjectileObject->GetComponent<CPTransform>()->SetPosition(PosToSpawn);
		}

		mageProjectileObject->GetComponent<CPRigidBody>()->addImpulse(ShootingForce);
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
		mEnemy->mRender->stop_anim();
		mEnemy->mRender->play_repeat("mage_idle");
		this->Update();
	}
	void MageIdleState::Update()
	{
		//DebuggerLog("Entered MageIdleState");
		//if (INPUT->IsKeyPressed(KeyCode::KEY_M)) //MAGE WILL CHASE WHEN THIS IS PRESSED
		GetFSM().ChangeState("Chase");
	}

	void MageIdleState::Exit() {	}

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
		this->Update();
	}
	void MageChaseState::Update()
	{
		//Calculating the distance between the Enemy and the player
		Vec2<float> CurEnemyPos = mEnemy->GetRigidBody()->getPos(); //Getting the current Mage Position
		Vec2<float> CurHeroPos = mEnemy->GetHero()->GetComponent<CPRigidBody>()->getPos(); //Getting the Player Position
		float DistInBwn = Vec2<float>::Distance(CurEnemyPos, CurHeroPos);

		////Getting the direction of the player and the enemy
		Vec2<float> Direction = (CurHeroPos - CurEnemyPos).Normalise();
		Vec2<float> NormalForce = Direction * mEnemy->mSpeedMagnitude;
		
		mEnemy->GetRigidBody()->addForce(NormalForce); //add force to move

		if (DistInBwn < mEnemy->mMinDistance) //checking the distance if its too close
		{
			GetFSM().ChangeState("BackOff");//it will change the state to back off
		}
		else if (DistInBwn >= mEnemy->mMinDistance && DistInBwn <= mEnemy->mMaxDistance) //if distance bwn not too close and its on the attack range
		{
			mEnemy->mAttackCooldownCurrent += TIME->GetDeltaTime();
			if (mEnemy->mAttackCooldownCurrent > mEnemy->mAttackCooldown)
			{
				mEnemy->mAttackCooldownCurrent = 0.0f;
				GetFSM().ChangeState("Shooting"); //it will change the state to shooting
			}
		}
	}
	void MageChaseState::Exit() { }

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
		this->Update();
	}
	void MageBackOffState::Update()
	{
		//Calculating the distance between the Enemy and the player
		Vec2<float> CurEnemyPos = mEnemy->GetRigidBody()->getPos(); //Getting the current Mage Position
		Vec2<float> CurHeroPos = mEnemy->GetHero()->GetComponent<CPRigidBody>()->getPos(); //Getting the Player Position
		float DistInBwn = Vec2<float>::Distance(CurEnemyPos, CurHeroPos); //Dist in between

		//Getting the direction of the player and the enemy
		Vec2<float> Direction = (CurHeroPos - CurEnemyPos).Normalise();
		Vec2<float> BackOffForce = (-Direction) * mEnemy->mBackOffSpeed; //opposite direction

		mEnemy->GetRigidBody()->addForce(BackOffForce); //Adding force with back off force

		if (DistInBwn >= mEnemy->mBackOffDistance) //if dist is far
		{
			GetFSM().ChangeState("Chase"); //goes back to chasing state
		}
		else if (DistInBwn >= mEnemy->mMinDistance && DistInBwn <= mEnemy->mMaxDistance) //if its in between
		{
			mEnemy->mAttackCooldownCurrent += TIME->GetDeltaTime();
			if (mEnemy->mAttackCooldownCurrent > mEnemy->mAttackCooldown)
			{
				mEnemy->mAttackCooldownCurrent = 0.0f;
				GetFSM().ChangeState("Shooting"); //it will change the state to shooting
			}
		}
	}
	void MageBackOffState::Exit() { }

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
		this->Update();
	}

	void MageHurtState::Update()
	{
		GetFSM().ChangeState("Idle");
	}

	void MageHurtState::Exit() { }

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
		mEnemy->mRender->stop_anim();
		mEnemy->mRender->play_repeat("mage_float");

		mEnemy->mNumOfProjectileCurrent = 0;
		mEnemy->mProjCooldownCurrent = 0.0f;
		this->Update();
	}

	void MageShootingState::Update()
	{
		mEnemy->mProjCooldownCurrent += TIME->GetDeltaTime();
		if (mEnemy->mProjCooldownCurrent > mEnemy->mProjCooldown)
		{
			mEnemy->mProjCooldownCurrent = 0.0f;

			++mEnemy->mNumOfProjectileCurrent;
			mEnemy->SpawnProjectile();

			if (mEnemy->mNumOfProjectileCurrent >= mEnemy->mNumOfProjectile)
			{
				GetFSM().ChangeState("Idle");	
			}
		}
	}

	void MageShootingState::Exit() { }
}