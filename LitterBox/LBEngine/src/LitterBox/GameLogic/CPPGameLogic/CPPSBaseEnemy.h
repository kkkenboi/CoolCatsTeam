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
		
		int mHealth{};		//normal health of the enemy
		float mSpeedMagnitude{};	//movespeed of the enemy

		CPRender* mRender{nullptr};
		CPRigidBody* mRigidBody{ nullptr };
		CPCollider* mCollider{ nullptr };

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
		Vec2<float> leftFace;
		Vec2<float> rightFace;

		Vec2<float> DirToPlayer;
		Vec2<float> TransformRight{ 1,0 };
		bool mShouldDestroy{ false };
	};
}

