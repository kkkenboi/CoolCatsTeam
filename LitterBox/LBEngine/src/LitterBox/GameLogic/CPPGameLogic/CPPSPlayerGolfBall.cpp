/*!************************************************************************
 \file				CPPSPlayerGolfBall.cpp
 \author(s)			Vanessa Chua Siew Jin, Kenji Brannon Chong, Amadeus Chia
 \par DP email(s):	vanessasiewjin@digipen.edu, kenjibrannon.c@digipen.edu, amadeusjinhan.chia@digipen.edu
 \par Course:		CSD2401A
 \date				09-02-2024
 \brief
 This file contains the Golf Ball behaviour the player will hold

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
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
#include "CPPGameManager.h"
#include "CPPVFXManager.h"
#include "LitterBox/Physics/RigidBodyManager.h"


#include "CPPSUpgradeManager.h"
#include "LitterBox/Utils/Matrix3x3.h"
//#include <random>
namespace LB
{
	/*!***********************************************************************
	\brief
	Starting behaviour for Ball where variables are initialized
	*************************************************************************/
	void CPPSPlayerGolfBall::Start()
	{
		mTransform = GameObj->GetComponent<CPTransform>();
		mRender = GameObj->GetComponent<CPRender>();
		mRigidBody = GameObj->GetComponent<CPRigidBody>();
		mCollider = GameObj->GetComponent<CPCollider>();

		mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");
		mGameManager = GOMANAGER->FindGameObjectWithName("GameManager");
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
		mRotation = 0.f;
		
		mCurrentLifetime = mLifetime = 2.5f;
		onBallDisappear.Subscribe(DecreaseBalls);

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
				//CPPSPlayer* player = (CPPSPlayer*)mPlayer->GetComponent<CPScriptCPP>()->GetInstance();
				//--player->m_currentBalls;
				canDestroy = true;
				//GOMANAGER->RemoveGameObject(this->GameObj);
				//If the ball's lifetime runs out and it has the explosion upgrade, go kaboom
				if (currentBallUpgrades & BOMB) {
					Explode();
				}
			}
		}
		// Rotation
		if (mRigidBody->mVelocity.x >= 0.f) 
		{
			//360 * 1.f - (speed curr/ speed max)
			float rotAdd = 360.f * (1.f - (mRigidBody->mVelocity.x/PHYSICS->GetMaxVelocity().x));
			mRotation += rotAdd;
			if (mRotation >= 360.f) 
			{
				float toAdd = mRotation - 360.f;
				mRotation = toAdd;
			}
			
			// Right rotation
			mTransform->SetRotation(mRotation);
		}
		else 
		{
			//360 * 1.f - (speed curr/ speed max)
			float rotAdd = 360.f * ((mRigidBody->mVelocity.x/PHYSICS->GetMaxVelocity().x));
			mRotation -= rotAdd;
			if (mRotation <= -(360.f))
			{
				float toAdd = mRotation - 360.f;
				mRotation = toAdd;
			}

			// Left rotation
			mTransform->SetRotation(mRotation);
		}

		if (canDestroy) 
		{ 
			GOMANAGER->FindGameObjectWithName("VFXManager")->GetComponent<CPPSVFXManager>()->SpawnPoofAnim(GetComponent<CPTransform>()->GetPosition(), 1);
			GOMANAGER->RemoveGameObject(this->GameObj); 
		}
		//if (PHY_MATH::Length(GetRigidBody()->mVelocity) > 300.f) std::cout << PHY_MATH::Length(GetRigidBody()->mVelocity) << '\n';
	}

	/*!***********************************************************************
	\brief
	Checking on collision with other colliders
	*************************************************************************/
	void CPPSPlayerGolfBall::OnCollisionEnter(CollisionData colData)
	{
		if ((currentBallUpgrades & BOMB) && colData.colliderOther->gameObj->GetName() != "ball") {
			if (colData.colliderOther->gameObj->GetName() == "Mage" ||
				colData.colliderOther->gameObj->GetName() == "EnemyChaser1" ||
				colData.colliderOther->gameObj->GetName() == "Charger_Shield")
			{
				//Renderer::GRAPHICS->shaker_camera();
				Explode();
				canDestroy = true;
				//CPPSPlayer* player = mPlayer->GetComponent<CPPSPlayer>();
				//CPPSPlayer* player = (CPPSPlayer*)mPlayer->GetComponent<CPScriptCPP>()->GetInstance();
				//--player->m_currentBalls;
				//GOMANAGER->RemoveGameObject(this->GameObj);
			}
			return;
		}
		if (colData.colliderOther->m_gameobj->GetName() == "Mage" ||
			colData.colliderOther->m_gameobj->GetName() == "EnemyChaser1" ||
			colData.colliderOther->m_gameobj->GetName() == "Charger" ||
			colData.colliderOther->m_gameobj->GetName() == "Bramble" ||
			colData.colliderOther->m_gameobj->GetName() == "Mushroom")
		{
			int Channel = AUDIOMANAGER->PlaySound("Thud");
			AUDIOMANAGER->SetChannelVolume(Channel, 0.2f);
		}
		else if (colData.colliderOther->m_gameobj->GetName() != "MainChar" && colData.colliderOther->m_gameobj->GetName()!= "Sandpit" && 
			colData.colliderOther->m_gameobj->GetName()!= "MouseWorld")
		{
			//play ball knocking sound
			AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->BallCollisionSounds, 0.4f);
			GOMANAGER->FindGameObjectWithName("VFXManager")->GetComponent<CPPSVFXManager>()->SpawnHitAnim(GetComponent<CPTransform>()->GetPosition());
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
		//This applies the COMBINED upgrade values that have been OR'd together
		currentBallUpgrades = static_cast<BallUpgrades>(upgradeType);
		//At the same time, we want to apply the effects of some of the upgrades
		if (currentBallUpgrades & BIGBALL)
		{
			//This doubles the size of the ball
			mSize = 1.5f;
			Vec2<float> newSize = GetComponent<CPTransform>()->GetScale() * mSize;
			//We set the ball sprite to the big ball
			GetComponent<CPRender>()->SetSpriteTexture(GetComponent<CPRender>()->spriteSheetName, 52);
			GetComponent<CPTransform>()->SetScale(newSize);
		}
	}

	/*!************************************************************************
	* \brief Helper function to remove the ball by setting it's lifetime to zero
	* 
	**************************************************************************/
	void CPPSPlayerGolfBall::DestroyBall()
	{
		//We do this instead of calling can destroy because we still want the logic in the
		//update loop of decreasing the player's current ball count.
		//Probably refactor this in the future
		mCurrentLifetime = 0.f;
		mRigidBody->mVelocity = Vec2<float>().Zero();
	}


	//Function to handle when the ball explodes
	/*!************************************************************************
	* \brief Function for handling the ball explosion
	* 
	**************************************************************************/
	void CPPSPlayerGolfBall::Explode()
	{
		Renderer::GRAPHICS->shake_camera(80.f,0.3f);
		//Play sound
		int channel = AUDIOMANAGER->PlaySound("EXPLOSION");
		AUDIOMANAGER->SetChannelVolume(channel, 0.3f);
		//Spawn explosion on pos
		GOMANAGER->FindGameObjectWithName("VFXManager")->GetComponent<CPPSVFXManager>()->SpawnExplosion(GetComponent<CPTransform>()->GetPosition());
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



	/*!************************************************************************
	* \brief Function to handle the ball splitting (empty for now)
	* This function gets called when the player right-clicks 
	**************************************************************************/
	void CPPSPlayerGolfBall::Split(Vec2<float> forceToApply)
	{	//First we verify that we have the upgrade
		if (currentBallUpgrades & SPLIT) {
			if (!hasSplit)	//then we see if the ball has been split before
			{	//if it hasn't been split then we want to split it
				hasSplit = true;
				//Spawn Game Object at a position
				GameObject* ballClone1 = FACTORY->SpawnGameObject();
				JSONSerializer::DeserializeFromFile("ball", *ballClone1);
				Vec2<float> playerPos = GetHero()->GetComponent<CPTransform>()->GetPosition();
				playerPos.x += GetHero()->GetComponent<CPPSPlayer>()->m_isFacingLeft ? -51.0f : 51.0f;
				ballClone1->GetComponent<CPTransform>()->SetPosition(playerPos);
				//we don't want the ball clones to split
				ballClone1->GetComponent<CPPSPlayerGolfBall>()->hasSplit = true;
				ballClone1->GetComponent<CPPSPlayerGolfBall>()->isClone = true;
				//but we want them to have the upgrades
				ballClone1->GetComponent<CPPSPlayerGolfBall>()->SetBallUpgrade(GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->GetBallUpgrades());
				//Personally I think we should slightly rotate the force by a small angle to make it look better
				Matrix3x3<float> randomRotation = Matrix3x3<float>().Identity();
				randomRotation.SetDegRotate(std::rand() % 25 + (-12));
				forceToApply = randomRotation*forceToApply;
				ballClone1->GetComponent<CPRigidBody>()->addImpulse(forceToApply);
			}
		}
	}

	/*!***********************************************************************
	\brief
	Destroy
	*************************************************************************/
	void CPPSPlayerGolfBall::Destroy() 
	{
		if(!isClone)
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

