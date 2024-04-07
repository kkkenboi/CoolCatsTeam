/*!************************************************************************
 * \file				CPPSBaseEnemy.h
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

#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"
namespace LB
{
	class CPPSBaseEnemy : public CPPBehaviour
	{
	public:
		/*!************************************************************************
		* \brief Start function for the base enemy that handles the initialising
		* of values for the base enemy like setting it's right face/left face
		* calculations
		**************************************************************************/
		void Start() override;
		/*!************************************************************************
		* \brief Update function for the base enemy class. We mainly update the 
		* stuff like the transform component of the enemy to make the sprite
		* face the correct direction
		* (Side note, if you press K, enemies should die)
		**************************************************************************/
		void Update() override;
		/*!************************************************************************
		* \brief Handles the destroy function for the base enemy. This should
		* be empty for now for all the derived enemy classes to inherit
		**************************************************************************/
		void Destroy() override;
		/*!************************************************************************
		* \brief Handles the collision with the enemy. 
		* 
		* \param col Collision Data of the collision
		**************************************************************************/
		void OnCollisionEnter(CollisionData col) override;

		//Getter functions
		//Animator once we have it
		/*!************************************************************************
		* \brief Accessor for the renderer of the base enemy 
		* 
		* \return CPRender* Pointer to the render component of the base enemy
		**************************************************************************/
		 CPRender* GetRender();
		/*!************************************************************************
		* \brief Accessor for the rigidbody of the base enemy 
		* 
		* \return CPRigidBody* Pointer to the rigidbody component of the base enemy
		**************************************************************************/
		 CPRigidBody* GetRigidBody();
		/*!************************************************************************
		* \brief Accessor for the collider of the base enemy
		* 
		* \return CPCollider* Pointer to the collider component of the base enemy
		**************************************************************************/
		 CPCollider* GetCollider();
		 /*!************************************************************************
		  * \brief Accessor for the animator of the base enemy
		  *
		  * \return CPAnimator* Pointer to the animator component of the base enemy
		 **************************************************************************/
		 CPAnimator* GetAnimator();
		/*!************************************************************************
		* \brief Accessor for the player game object
		* 
		* \return GameObject* Pointer to the player game object
		**************************************************************************/
		 GameObject* GetHero();

		//Accessor functions
		/*!************************************************************************
		* \brief Accessor for the enemy health
		* 
		* \return int& Current health of the enemy
		**************************************************************************/
		int& GetHealth();
		/*!************************************************************************
		* \brief Accessor for the speed magnitude of the base enemy 
		* 
		* \return float& Speed of the base enemy
		**************************************************************************/
		float& GetSpeedMag();

		/*!************************************************************************
		* \brief Accessor for the Distance of from the enemy to the player
		*
		* \return float dist to the player
		**************************************************************************/
		float GetDistToPlayer();

		/*!************************************************************************
		* \brief Function to face the player when facing left or right
		*
		* \return void
		**************************************************************************/
		void FacePlayer();

		CPRender* mRender{nullptr};
		CPRigidBody* mRigidBody{ nullptr };
		CPCollider* mCollider{ nullptr };
		CPAnimator* mAnimator{ nullptr };
		//I have to make this public because I need to access this
		//and I'm lazy to make a getter/setter for it
		float mDetectionRange{};	//distance before enemy aggros
		bool isAggro{ false };


		/*!************************************************************************
		* \brief Die function of the base enemy that handles the dying
		* It will send a call to the Game Manager to reduce the current enemy count
		**************************************************************************/
		virtual void Die();
		/*!************************************************************************
		* \brief Hurt function for the base enemy, should be called whenever the 
		* enemy is hurt
		**************************************************************************/
		virtual void Hurt();

	protected:
		
		GameObject* mPlayer{ nullptr }; //Caching the player obj
		GameObject* mGameManager{ nullptr }; //Caching the game manager
		//The values below are all for handling the facing of the enemy
		Vec2<float> leftFace; //returns vector x y of where its facing, left
		Vec2<float> rightFace; //returns vector x y of where its facing, right

		bool facingLeft{true}; //bool check if its facing left

		Vec2<float> DirToPlayer;
		Vec2<float> TransformRight{ 1,0 };
		bool mShouldDestroy{ false };
		bool mShouldFace{true};

	private:
		//We technically do not want to randomly expose these variables
		int mHealth{};		//normal health of the enemy
		float mSpeedMagnitude{};	//movespeed of the enemy
		float mDistanceToPlayer{};	//distance between enemy and player
	};
}

