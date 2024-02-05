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
		if (currentBallUpgrades & BIGBALL) {
			std::cout << "EMBIGGEN\n";
			mSize = 2.0f;
		}
		else mSize = 1.0f;

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
				if (currentBallUpgrades & BOMB) Explode();
				CPPSPlayer* player = (CPPSPlayer*)mPlayer->GetComponent<CPScriptCPP>()->GetInstance();
				--player->m_currentBalls;
				GOMANAGER->RemoveGameObject(this->GameObj);
			}
		}
	}

	/*!***********************************************************************
	\brief
	Checking on collision with other colliders
	*************************************************************************/
	void CPPSPlayerGolfBall::OnCollisionEnter(CollisionData colData)
	{
		
		if (currentBallUpgrades & BOMB) {

			//Renderer::GRAPHICS->shaker_camera();
			Explode();
			CPPSPlayer* player = (CPPSPlayer*)mPlayer->GetComponent<CPScriptCPP>()->GetInstance();
			--player->m_currentBalls;
			GOMANAGER->RemoveGameObject(this->GameObj);
			return;
		}
		if (colData.colliderOther->m_gameobj->GetName() == "Mage" ||
			colData.colliderOther->m_gameobj->GetName() == "EnemyChaser1")
		{
			int Channel = AUDIOMANAGER->PlaySound("Smoke Poof by sushiman2000 Id - 643876");

			AUDIOMANAGER->SetChannelVolume(Channel, 0.5f);
		}

	}

	void CPPSPlayerGolfBall::SetBallUpgrade(int upgradeType)
	{	
		//std::cout << "Upgrade type : " << upgradeType << '\n';
		//std::cout << "Bitshifted Upgrade type : " << (1 << upgradeType) << '\n';
		//std::cout << "curr upgrade : " << static_cast<int>(currentBallUpgrades) << '\n';
		//std::cout << "result : " << (currentBallUpgrades & BOMB) << '\n';
		//In order to set the upgrade type, we have to bit shift it.
		currentBallUpgrades |= static_cast<BallUpgrades>(1 << upgradeType);
		//currentBallUpgrades = static_cast<BallUpgrades>(static_cast<int>(currentBallUpgrades) | (1 << upgradeType));
	}


	//Function to handle when the ball explodes
	void CPPSPlayerGolfBall::Explode()
	{
		std::vector<CPCollider*> explosionColliders = COLLIDERS->OverlapCircle(this->GameObj->GetComponent<CPTransform>()->GetPosition(), 100.f);
		//We loop through all the colliders that were in the radius
		for (CPCollider* col : explosionColliders) {
				if (col->gameObj->GetName() == "MainChar") continue;
				Vec2<float> explosionForce = col->m_pos - this->GameObj->GetComponent<CPTransform>()->GetPosition();
				explosionForce = explosionForce.Normalise() * explosionForceMag;
				if (col->HasRB()) {
					col->gameObj->GetComponent<CPRigidBody>()->addImpulse(explosionForce);
					/*if (col->gameObj->HasComponent<CPPSBaseEnemy>()) {
					col->gameObj->GetComponent<CPPSBaseEnemy>()->Hurt();
					std::cout << "enemy hurt\n";
					}*/
				}
		}
	}

	void CPPSPlayerGolfBall::Split()
	{
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

