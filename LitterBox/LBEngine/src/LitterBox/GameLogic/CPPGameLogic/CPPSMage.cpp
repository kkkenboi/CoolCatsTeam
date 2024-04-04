/*!************************************************************************
 \file				CPPSMage.cpp
 \author(s)			Vanessa Chua Siew Jin, Ryan Tan Jian Hao
 \par DP email(s):	vanessasiewjin@digipen.edu, ryanjianhao.tan@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the CPPSMage class and all its functionalities,
it handles the logic for the Mage enemy

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPSMage.h"
#include "CPPSProjectileBall.h"
#include "CPPGameManager.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Debugging/Debug.h"
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
		CPPSBaseEnemy::Start();
		// Cache the render and animator
		m_trans = GetComponent<CPTransform>();
		mRender = m_trans->GetChild()->GetChild()->GetComponent<CPRender>();
		mAnimator = m_trans->GetChild()->GetChild()->GetComponent<CPAnimator>();
		m_handTrans = m_trans->GetChild()->GetChild()->GetChild()->GetComponent<CPTransform>();
		m_wandTipTrans = m_trans->GetChild()->GetChild()->GetChild()->GetChild()->GetComponent<CPTransform>();
		mAudioManager = GOMANAGER->FindGameObjectWithName("AudioManager")->GetComponent<CPPSAudioManager>();
		//initialse the state of the mage
		//STATES : IDLE, CHASING, BACKOFF, HURT, SHOOTING
		MageIdleState* IDLESTATE = DBG_NEW MageIdleState(this, mFSM, "Idle");
		MageChaseState* CHASESTATE = DBG_NEW MageChaseState(this, mFSM, "Chase");
		MageBackOffState* BACKOFFSTATE = DBG_NEW MageBackOffState(this, mFSM, "BackOff");
		MageHurtState* HURTSTATE = DBG_NEW MageHurtState(this, mFSM, "Hurt");
		MageShootingState* SHOOTINGSTATE = DBG_NEW MageShootingState(this, mFSM, "Shooting");
		//MageShootingState* DEATHSTATE = DBG_NEW MageShootingState(this, mFSM, "Death");

		//STATES : IDLE, CHASING, BACKOFF, HURT, SHOOTING
		mFSM.AddState(IDLESTATE);
		mFSM.AddState(CHASESTATE);
		mFSM.AddState(BACKOFFSTATE);
		mFSM.AddState(HURTSTATE);
		mFSM.AddState(SHOOTINGSTATE);
		//mFSM.AddState(DEATHSTATE);

		//set current state of Mage to be on idle
		mFSM.SetCurrentState("Idle");

		//initialise the variables for the Mage
		GetHealth() = 3; //health {inherited from base class}
		GetSpeedMag() = 40000.f; //speed of movement {inherited from base class}
		//mBackOffSpeed = 50000.f; //speed of movement
		mDetectionRange = 800.f;

		//------------------CHASE STATE------------------
		//a little hardcoding for now, min and max distance between enemy and player
		//mAttackCooldown = 2.0f;
		//mAttackCooldownCurrent = 0.0f;

		//mMinDistance = 400.0f;
		//mBackOffDistance = 800.0f;
		//mMaxDistance = 1500.0f;

		//------------------SHOOTING STATE------------------
		//timer set for shooting of projectils
		//mNumOfProjectile = 3; //set to 3 projectiles
		//mNumOfProjectileCurrent = 0;
		//mProjCooldown = 0.35f;
		//mProjCooldownCurrent = 0.0f;
		//mProjSpeed = 1000.0f; //pojectile speed

		// 0.5 seconds of invincibility
		//mGotAttacked = 0.5f;

		//it has fully initialised
		mInitialised = true;
	}

	/*!***********************************************************************
	\brief
	FixedUpdate behaviour for Mage, doing the physics in when it calculates the player pos
	*************************************************************************/
	void CPPSMage::FixedUpdate()
	{
		// Update the hand position
		Vec2<float> playerToMageDir = m_trans->GetPosition() - GetHero()->GetComponent<CPTransform>()->GetPosition();
		playerToMageDir = playerToMageDir.Normalise();

		float angle = atan2f(playerToMageDir.y, playerToMageDir.x);
		m_handTrans->SetRotation(RadToDeg(angle) + 90.0f);

		if (mShouldDestroy)
		{
			GOMANAGER->RemoveGameObject(this->GameObj);
			return;
		}
		mFSM.FixedUpdate();
	}

	/*!***********************************************************************
	\brief
	Update behaviour for Mage, checks if everything has been initialised to start FSM
	*************************************************************************/
	void CPPSMage::Update()
	{
		if (mGameManager->GetComponent<CPPSGameManager>()->isGameOver) return;

		CPPSBaseEnemy::Update();
		if (mInitialised == false)
		{
			return;
		}

		if (mGotAttackedCooldown > 0.0f) {
			mGotAttackedCooldown -= static_cast<float>(TIME->GetFixedDeltaTime());
		}

		mFSM.Update();
	}

	/*!***********************************************************************
	\brief
	Destroy function for Mage, clear the states
	*************************************************************************/
	void CPPSMage::Destroy()
	{
		//STATES : IDLE, CHASING, BACKOFF, HURT, SHOOTING
		delete mFSM.GetState("Idle");
		delete mFSM.GetState("Chase");
		delete mFSM.GetState("BackOff");
		delete mFSM.GetState("Hurt");
		delete mFSM.GetState("Shooting");
		//delete mFSM.GetState("Death");
	}

	/*!***********************************************************************
	\brief
	Hurt function for Mage, Call base enemy hurt function, aggro to true
	*************************************************************************/
	void CPPSMage::Hurt()
	{
		isAggro = true;
		CPPSBaseEnemy::Hurt();
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
		CPPSBaseEnemy::OnCollisionEnter(colData);
		if (colData.colliderOther->m_gameobj->GetName() == "ball") {

			if (PHY_MATH::Length(colData.colliderOther->GetRigidBody()->mVelocity) > 200.f)
			{
				if (mGotAttackedCooldown > 0.0f) {
					return;
				}
				mAudioManager->Play3DSound(mAudioManager->MageHurtSounds, GetComponent<CPTransform>()->GetPosition(), 0.2);
				//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->MageHurtSounds, 0.2f);
				mGotAttackedCooldown = mGotAttacked;

				
				mFSM.ChangeState("Hurt");
			}
		}
		if (colData.colliderOther->m_gameobj->GetName() == "Bramble")
		{
			if (mGotAttackedCooldown > 0.0f) {
				return;
			}
			mGotAttackedCooldown = mGotAttacked;
			mFSM.ChangeState("Hurt");
			Hurt();	//This is here to play the anim
		}
	}

	/*!***********************************************************************
	\brief
	Die function call base enemy die
	*************************************************************************/
	void CPPSMage::Die()
	{
		//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->MageDeathSounds,0.2f);
		mAudioManager->Play3DSound(mAudioManager->MageDeathSounds, GetComponent<CPTransform>()->GetPosition(), 0.2);

		CPPSBaseEnemy::Die(); //We call this because the base class will have some logic to reduce enemy count
	}

	/*!***********************************************************************
	\brief
	Spawn function to spawn projectile Game Object
	*************************************************************************/
	void CPPSMage::SpawnProjectile()
	{
		Vec2<float> CurHeroPos = GetHero()->GetComponent<CPRigidBody>()->getPos(); //Getting the Player Position
		Vec2<float> CurEnemyPos = m_handTrans->GetPosition(); //Getting the current Mage Position

		//having offset where it will shoot at the side of the mage
		// float offset = 50.0f; NOTREFERENCED
		Vec2<float> Direction = (CurHeroPos - CurEnemyPos).Normalise();
		Vec2<float> ShootingForce = Direction * mProjSpeed;
		CurEnemyPos += Direction * m_handOffset;

		Vec2<float> PosToSpawn{ CurEnemyPos.x, CurEnemyPos.y};
		/*int Channel = AUDIOMANAGER->PlaySound("Fire, Whoosh, Flame, Fireball, Fast x4 SND11948 1");
		AUDIOMANAGER->SetChannelVolume(Channel, 0.3f);*/

		mAudioManager->Play3DSound("Fire, Whoosh, Flame, Fireball, Fast x4 SND11948 1", PosToSpawn, false, 0.3f);

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

	/*!***********************************************************************
	\brief
	Enter the state of Idle where it will initialise the values
	*************************************************************************/
	void MageIdleState::Enter()
	{
		//stopping the animation and playing idle
		//mEnemy->mRender->stop_anim();
		//mEnemy->mRender->play_repeat("mage_idle");
		this->Update();
	}

	/*!***********************************************************************
	\brief
	FixedUpdate the state of Idle 
	*************************************************************************/
	void MageIdleState::FixedUpdate(){}

	/*!***********************************************************************
	\brief
	Update the state of Idle where it changes the state to chasing when it reaches on idle state
	(May change in future)
	*************************************************************************/
	void MageIdleState::Update()
	{
		if (mEnemy->GetDistToPlayer() <= mEnemy->mDetectionRange) mEnemy->isAggro = true;
		if (mEnemy->isAggro)
		{
		GetFSM().ChangeState("Chase");
		mEnemy->mAudioManager->Play3DSound(mEnemy->mAudioManager->MageAttackSounds, mEnemy->GameObj->GetComponent<CPTransform>()->GetPosition(), false, 0.2f);

		//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->MageAttackSounds, 0.2f);
		}

	}

	/*!***********************************************************************
	\brief
	Exit the state
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	Enter the state of chasing where it will initialise the values
	*************************************************************************/
	void MageChaseState::Enter()
	{
		this->Update();
	}

	/*!***********************************************************************
	\brief
	FixedUpdate the state of chasing where it caculates the distance and movement
	*************************************************************************/
	void MageChaseState::FixedUpdate()
	{
		Vec2<float> CurEnemyPos = mEnemy->GetRigidBody()->getPos(); //Getting the current Mage Position
		Vec2<float> CurHeroPos = mEnemy->GetHero()->GetComponent<CPRigidBody>()->getPos(); //Getting the Player Position
		mEnemy->mDistInBwn = Vec2<float>::Distance(CurEnemyPos, CurHeroPos);

		////Getting the direction of the player and the enemy
		Vec2<float> Direction = (CurHeroPos - CurEnemyPos).Normalise();
		Vec2<float> NormalForce = Direction * mEnemy->GetSpeedMag();

		mEnemy->GetRigidBody()->addForce(NormalForce * static_cast<float>(TIME->GetFixedDeltaTime())); //add force to move
	}

	/*!***********************************************************************
	\brief
	Update the state of chasing where it will be chasing the enemy when its nearby,
	changes states when it reaches a condition
	*************************************************************************/
	void MageChaseState::Update()
	{
		if (mEnemy->mDistInBwn < mEnemy->mMinDistance) //checking the distance if its too close
		{
			GetFSM().ChangeState("BackOff");//it will change the state to back off
		}
		else if (mEnemy->mDistInBwn >= mEnemy->mMinDistance && mEnemy->mDistInBwn <= mEnemy->mMaxDistance) //if distance bwn not too close and its on the attack range
		{
			mEnemy->mAttackCooldownCurrent += static_cast<float>(TIME->GetFixedDeltaTime());
			if (mEnemy->mAttackCooldownCurrent > mEnemy->mAttackCooldown)
			{
				mEnemy->mAttackCooldownCurrent = 0.0f;
				GetFSM().ChangeState("Shooting"); //it will change the state to shooting
			}
		}
	}

	/*!***********************************************************************
	\brief
	Exit the state
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	Enter the state of Backing off where it will initialise the values
	*************************************************************************/
	void MageBackOffState::Enter()
	{
		this->Update();
	}


	/*!***********************************************************************
	\brief
	FixedUpdate the state of Backing off where it caculates the distance and movement
	*************************************************************************/
	void MageBackOffState::FixedUpdate()
	{
		Vec2<float> CurEnemyPos = mEnemy->GetRigidBody()->getPos(); //Getting the current Mage Position
		Vec2<float> CurHeroPos = mEnemy->GetHero()->GetComponent<CPRigidBody>()->getPos(); //Getting the Player Position
		mEnemy->mDistInBwn = Vec2<float>::Distance(CurEnemyPos, CurHeroPos); //Dist in between

		//Getting the direction of the player and the enemy
		Vec2<float> Direction = (CurHeroPos - CurEnemyPos).Normalise();
		Vec2<float> BackOffForce = (-Direction) * mEnemy->mBackOffSpeed; //opposite direction

		mEnemy->GetRigidBody()->addForce(BackOffForce * static_cast<float>(TIME->GetFixedDeltaTime())); //Adding force with back off force
	}

	/*!***********************************************************************
	\brief
	Update the state of Backing off where it will be Back off where the enemy when its too nearby,
	changes states when it reaches a condition
	*************************************************************************/
	void MageBackOffState::Update()
	{
		if (mEnemy->mDistInBwn >= mEnemy->mBackOffDistance) //if dist is far
		{
			GetFSM().ChangeState("Chase"); //goes back to chasing state
		}
		else if (mEnemy->mDistInBwn >= mEnemy->mMinDistance && mEnemy->mDistInBwn <= mEnemy->mMaxDistance) //if its in between
		{
			mEnemy->mAttackCooldownCurrent += static_cast<float>(TIME->GetFixedDeltaTime());
			if (mEnemy->mAttackCooldownCurrent > mEnemy->mAttackCooldown)
			{
				mEnemy->mAttackCooldownCurrent = 0.0f;
				GetFSM().ChangeState("Shooting"); //it will change the state to shooting
			}
		}
	}

	/*!***********************************************************************
	\brief
	Exit the state
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	Enter the state of Hurt where it will initialise the values
	*************************************************************************/
	void MageHurtState::Enter()
	{
		mEnemy->GetAnimator()->PlayAndReset("Mage_Hurt");
		mEnemy->Hurt();
		this->Update();
	}

	/*!***********************************************************************
	\brief
	Update the state of Hurt where it will change back the state to idle
	*************************************************************************/
	void MageHurtState::FixedUpdate() {}

	/*!***********************************************************************
	\brief
	Update the state of Hurt where it will change back the state to idle
	*************************************************************************/
	void MageHurtState::Update()
	{
		GetFSM().ChangeState("Idle");
	}

	/*!***********************************************************************
	\brief
	Exit the state
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	Enter the state of Shooting where it will initialise the values
	*************************************************************************/
	void MageShootingState::Enter()
	{
		mEnemy->GetAnimator()->PlayAndReset("Mage_Attack");

		mEnemy->mNumOfProjectileCurrent = 0;
		mEnemy->mProjCooldownCurrent = 0.0f;
		mEnemy->mAudioManager->Play3DSound(mEnemy->mAudioManager->MageAttackSounds, mEnemy->GameObj->GetComponent<CPTransform>()->GetPosition(), false, 0.3f);
		//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->MageAttackSounds, .3f);
		this->Update();
	}

	/*!***********************************************************************
	\brief
	FixedUpdate the state of Shooting
	*************************************************************************/
	void MageShootingState::FixedUpdate() {}

	/*!***********************************************************************
	\brief
	Update the state of Shooting where it will be shooting the player when player is nearby
	*************************************************************************/
	void MageShootingState::Update()
	{
		mEnemy->mProjCooldownCurrent += static_cast<float>(TIME->GetFixedDeltaTime());
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

	/*!***********************************************************************
	\brief
	Exit the state
	*************************************************************************/
	void MageShootingState::Exit() { }
}