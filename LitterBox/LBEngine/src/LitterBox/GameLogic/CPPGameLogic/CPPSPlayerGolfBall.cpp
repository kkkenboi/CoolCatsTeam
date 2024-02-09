/*!************************************************************************
 \file				CPPSPlayerGolfBall.cpp
 \author(s)			Vanessa Chua Siew Jin, Kenji Brannon Chong
 \par DP email(s):	vanessasiewjin@digipen.edu, kenjibrannon.c@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the Golf Ball behaviour the player will hold

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPSPlayerGolfBall.h"

#include "CPPSPlayer.h"
#include "LitterBox/Debugging/Debug.h"
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Physics/ColliderManager.h"

#include "LitterBox/Physics/PhysicsMath.h"
#include "LitterBox/Audio/AudioManager.h"
#include "LitterBox/Engine/Time.h"
#include "CPPSPlayerHUD.h"
#include "LitterBox/Renderer/Renderer.h"
#include "CPPSBaseEnemy.h"
#include "CPPSMage.h"
#include "CPPSChaser.h"


namespace LB
{
	/*!***********************************************************************
	\brief
	Starting behaviour for Ball where variables are initialized
	*************************************************************************/
	void CPPSPlayerGolfBall::Start()
	{
		mRender = GameObj->GetComponent<CPRender>();
		mRigidBody = GameObj->GetComponent<CPRigidBody>();
		mCollider = GameObj->GetComponent<CPCollider>();

		mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");
		/*std::vector<GameObject*> const& GOs = GOMANAGER->GetGameObjects();
		for (GameObject* GO : GOs) {
			if (GO->GetName() == "MainChar")
			{
				mPlayer = GO;
				break;
			}
		}*/

		mSpeedMagnitude = 1000.0f;
		mVelocity = 1000.0f; //with direction
	/*	if (currentBallUpgrades & BIGBALL) {
			std::cout << "EMBIGGEN\n";
			mSize = 2.0f;
		}
		else mSize = 1.0f;*/

		mCurrentLifetime = mLifetime = 1.0f;
		onBallDisappear.Subscribe(IncreaseBalls);

	}

	/*!***********************************************************************
	\brief
	Update behaviour for Ball where it has physics and lifetime
	*************************************************************************/
	void CPPSPlayerGolfBall::Update()
	{
		if (!mRigidBody)
		{
			mRigidBody = GameObj->GetComponent<CPRigidBody>();
			return;
		}

		if (mRigidBody->mVelocity.LengthSquared() < 50.0f)
		{
			mCurrentLifetime -= static_cast<float>(TIME->GetDeltaTime());
			if (mCurrentLifetime <= 0.0f)
			{
				//if (currentBallUpgrades & BOMB) Explode();
				mPlayer->GetComponent<CPPSPlayer>()->m_currentBalls--;
				//CPPSPlayer* player = (CPPSPlayer*)mPlayer->GetComponent<CPScriptCPP>()->GetInstance();
				//--player->m_currentBalls;
				canDestroy = true;
				//GOMANAGER->RemoveGameObject(this->GameObj);
				if (currentBallUpgrades & BOMB) {
					Explode();
				}
			}
		}
		if (canDestroy) GOMANAGER->RemoveGameObject(this->GameObj);
	}

	/*!***********************************************************************
	\brief
	Checking on collision with other colliders
	*************************************************************************/
	void CPPSPlayerGolfBall::OnCollisionEnter(CollisionData colData)
	{
		if ((currentBallUpgrades & BOMB) && colData.colliderOther->gameObj->GetName() != "ball") {

			//Renderer::GRAPHICS->shaker_camera();
			Explode();
			mPlayer->GetComponent<CPPSPlayer>()->m_currentBalls--;
			
			//CPPSPlayer* player = mPlayer->GetComponent<CPPSPlayer>();
			//CPPSPlayer* player = (CPPSPlayer*)mPlayer->GetComponent<CPScriptCPP>()->GetInstance();
			//--player->m_currentBalls;
			//GOMANAGER->RemoveGameObject(this->GameObj);
			return;
		}
		if (colData.colliderOther->m_gameobj->GetName() == "Mage" ||
			colData.colliderOther->m_gameobj->GetName() == "EnemyChaser1")
		{
			int Channel = AUDIOMANAGER->PlaySound("Smoke Poof by sushiman2000 Id - 643876");
			AUDIOMANAGER->SetChannelVolume(Channel, 0.5f);
		}
		else if (colData.colliderOther->m_gameobj->GetName() != "MainChar")
		{
			//play ball knocking sound
			AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->BallCollisionSounds, 0.4f);
		}
	}

	//Function to set the current upgrade for the ball
	//the ball will check against this value to know what upgrades it has
	void CPPSPlayerGolfBall::SetBallUpgrade(int upgradeType)
	{	
		//std::cout << "Upgrade type : " << upgradeType << '\n';
		//std::cout << "Bitshifted Upgrade type : " << (1 << upgradeType) << '\n';
		//std::cout << "result : " << (currentBallUpgrades & BOMB) << '\n';
		//In order to set the upgrade type, we have to bit shift it.
		currentBallUpgrades = static_cast<BallUpgrades>(upgradeType);
		//currentBallUpgrades |= static_cast<BallUpgrades>(1 << upgradeType);
		//currentBallUpgrades = static_cast<BallUpgrades>(static_cast<int>(currentBallUpgrades) | (1 << upgradeType));
	}

	void CPPSPlayerGolfBall::DestroyBall()
	{
		//We do this instead of calling can destroy because we still want the logic in the
		//update loop of decreasing the player's current ball count.
		//Probably refactor this in the future
		mCurrentLifetime = 0;
		mRigidBody->mVelocity = Vec2<float>().Zero();
	}


	//Function to handle when the ball explodes
	void CPPSPlayerGolfBall::Explode()
	{
		Renderer::GRAPHICS->shake_camera(80.f,0.3f);
		int channel = AUDIOMANAGER->PlaySound("EXPLOSION");
		AUDIOMANAGER->SetChannelVolume(channel, 0.3f);
		std::vector<CPCollider*> explosionColliders = COLLIDERS->OverlapCircle(this->GameObj->GetComponent<CPTransform>()->GetPosition(), 100.f);
		//We loop through all the colliders that were in the radius
		for (CPCollider* col : explosionColliders) {
				if (col->gameObj->GetName() == "MainChar") continue;	
				if (col->gameObj->GetName() == "Projectile") continue;	//we don't want it to affect the enemy projectiles
				Vec2<float> explosionForce = col->m_pos - this->GameObj->GetComponent<CPTransform>()->GetPosition();
				explosionForce = explosionForce.Normalise() * explosionForceMag;
				if (col->HasRB()) {
					col->gameObj->GetComponent<CPRigidBody>()->addImpulse(explosionForce);
					if (col->gameObj->GetName() == "Mage") {
						//std::cout << "mage hurt by explosion\n";
						col->gameObj->GetComponent<CPPSMage>()->Hurt();
					}
					if (col->gameObj->GetName() == "EnemyChaser1") {
						//std::cout << "chaser hurt by explosion\n";
						col->gameObj->GetComponent<CPPSChaser>()->Hurt();
					}
				}
		}
		canDestroy = true;
	}

	void CPPSPlayerGolfBall::Split()
	{
		if (currentBallUpgrades & SPLIT) {
			if (!hasSplit)
			{
				hasSplit = true;
				//Doesn't work for now
				//Spawn Game Object
			/*	GameObject* ballObject = FACTORY->SpawnGameObject(this->GameObj);
				ballObject->GetComponent<CPTransform>()->SetPosition({
					ballObject->GetComponent<CPTransform>()->GetPosition().x + 1,
					ballObject->GetComponent<CPTransform>()->GetPosition().y + 1
					}
				);*/
			}
		}
	}

	/*!***********************************************************************
	\brief
	Destroy
	*************************************************************************/
	void CPPSPlayerGolfBall::Destroy() 
	{
		onBallDisappear.Invoke();
	}

	//Getter functions
	/*!***********************************************************************
	\brief
	Getter for the render component
	*************************************************************************/
	CPRender* CPPSPlayerGolfBall::GetRender()
	{
		return mRender;
	}
	/*!***********************************************************************
	\brief
	Getter for the rigidbody component
	*************************************************************************/
	CPRigidBody* CPPSPlayerGolfBall::GetRigidBody()
	{
		return mRigidBody;
	}

	/*!***********************************************************************
	\brief
	Getter for the collider component
	*************************************************************************/
	CPCollider* CPPSPlayerGolfBall::GetCollider()
	{
		return mCollider;
	}

	/*!***********************************************************************
	\brief
	Getter for the player object
	*************************************************************************/
	GameObject* CPPSPlayerGolfBall::GetHero()
	{
		return mPlayer;
	}
}

