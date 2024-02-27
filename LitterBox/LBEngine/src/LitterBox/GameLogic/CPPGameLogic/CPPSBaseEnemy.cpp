/*!************************************************************************
 * \file				CPPSBaseEnemy.cpp
 * \author(s)			Amadeus Chia 
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2450A
 * \date				09/02/2024
 * \brief 				This source file contains the function declarations
 * 						for the BaseEnemy class. All enemies can and SHOULD
 * 						inherit from this class to implement the base 
 * 						functions like hurt etc.
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "pch.h"
#include "CPPSBaseEnemy.h"
#include "CPPGameManager.h"
#include "LitterBox/Engine/Input.h"

namespace LB
{
	/*!************************************************************************
	 * \brief Start function for the base enemy that handles the initialising
	 * of values for the base enemy like setting it's right face/left face
	 * calculations
	**************************************************************************/
	void CPPSBaseEnemy::Start()
	{
		//We just cache some values
		mRender = GameObj->GetComponent<CPRender>();
		mRigidBody = GameObj->GetComponent<CPRigidBody>();
		mCollider = GameObj->GetComponent<CPCollider>();
		mAnimator = GameObj->GetComponent<CPAnimator>();

		//Grabbing the player GO reference
		mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");
		//Grabbing GameManager reference
		mGameManager = GOMANAGER->FindGameObjectWithName("GameManager");
		//This is just so we can make ALL enemies face left or right
		rightFace = GameObj->GetComponent<CPTransform>()->GetScale();
		leftFace = GameObj->GetComponent<CPTransform>()->GetScale();
		leftFace.x = -leftFace.x;	//forgive me lord for I have sinned
	}

	/*!************************************************************************
	 * \brief Update function for the base enemy class. We mainly update the 
	 * stuff like the transform component of the enemy to make the sprite
	 * face the correct direction
	 * (Side note, if you press K, enemies should die)
	**************************************************************************/
	void CPPSBaseEnemy::Update()
	{
		//All enemies must always face player (?) Might not work for charger who knows
		DirToPlayer = mPlayer->GetComponent<CPTransform>()->GetPosition() - GameObj->GetComponent<CPTransform>()->GetPosition();
		if (DotProduct(DirToPlayer.Normalise(), TransformRight) < 0.0f)
		{
			GameObj->GetComponent<CPTransform>()->SetScale(leftFace);
		}
		else GameObj->GetComponent<CPTransform>()->SetScale(rightFace);

		//All enemies will sepuku if you press K
		if (INPUT->IsKeyTriggered(KeyCode::KEY_K))
		{
			Die();
		}
		
	}

	/*!************************************************************************
	 * \brief Handles the destroy function for the base enemy. This should
	 * be empty for now for all the derived enemy classes to inherit
	**************************************************************************/
	void CPPSBaseEnemy::Destroy()
	{
		//Should be empty?
	}

	/*!************************************************************************
	 * \brief Handles the collision with the enemy. 
	 * 
	 * \param col Collision Data of the collision
	**************************************************************************/
	void CPPSBaseEnemy::OnCollisionEnter(CollisionData col)
	{
		UNREFERENCED_PARAMETER(col);	//the derived classes should override this
		//If the enemy has no hp it should be able to collide with anything anymore
		if (mHealth <= 0) return;
	}

	/*!************************************************************************
	 * \brief Accessor for the renderer of the base enemy 
	 * 
	 * \return CPRender* Pointer to the render component of the base enemy
	**************************************************************************/
	CPRender* CPPSBaseEnemy::GetRender()
	{
		return mRender;
	}

	/*!************************************************************************
	 * \brief Accessor for the rigidbody of the base enemy 
	 * 
	 * \return CPRigidBody* Pointer to the rigidbody component of the base enemy
	**************************************************************************/
	CPRigidBody* CPPSBaseEnemy::GetRigidBody()
	{
		return mRigidBody;
	}

	/*!************************************************************************
	 * \brief Accessor for the collider of the base enemy
	 * 
	 * \return CPCollider* Pointer to the collider component of the base enemy
	**************************************************************************/
	CPCollider* CPPSBaseEnemy::GetCollider()
	{
		return mCollider;
	}

	/*!************************************************************************
	 * \brief Accessor for the player game object
	 * 
	 * \return GameObject* Pointer to the player game object
	**************************************************************************/
	GameObject* CPPSBaseEnemy::GetHero()
	{
		return mPlayer;
	}
	/*!************************************************************************
	 * \brief Accessor for the animator of the base enemy
	 *
	 * \return CPAnimator* Pointer to the animator component of the base enemy
	**************************************************************************/
	CPAnimator* CPPSBaseEnemy::GetAnimator()
	{
		return mAnimator;
	}

	/*!************************************************************************
	 * \brief Accessor for the enemy health
	 * 
	 * \return int& Current health of the enemy
	**************************************************************************/
	int& CPPSBaseEnemy::GetHealth()
	{
		return mHealth;
	}

	/*!************************************************************************
	 * \brief Accessor for the speed magnitude of the base enemy 
	 * 
	 * \return float& Speed of the base enemy
	**************************************************************************/
	float& CPPSBaseEnemy::GetSpeedMag()
	{
		return mSpeedMagnitude;
	}

	/*!************************************************************************
	 * \brief Die function of the base enemy that handles the dying
	 * It will send a call to the Game Manager to reduce the current enemy count
	**************************************************************************/
	void CPPSBaseEnemy::Die()
	{
		//If the enemy dies, regardless we MUST reduce the enemy count
		mGameManager->GetComponent<CPPSGameManager>()->ReduceEnemyCount();
		//Forgive me lord for I have sinned
		GameObj->GetComponent<CPTransform>()->SetPosition(Vec2<float>{10000.0f, 10000.0f});
		mShouldDestroy = true;
	}

	/*!************************************************************************
	 * \brief Hurt function for the base enemy, should be called whenever the 
	 * enemy is hurt
	**************************************************************************/
	void CPPSBaseEnemy::Hurt()
	{
		if (mHealth <= 0) return;
		--mHealth;
		if (mHealth < 1)
		{
			Die();
		}
	}
}
