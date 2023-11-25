#pragma once
#include "CPPBehaviour.h"
#include "Litterbox/Factory/Components.h"

namespace LB
{
	class CPPSBaseGolfBall : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		Inherited Functions from CPPBehaviour that the user must implement
		*************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;

		CPRender* GetRender();
		CPRigidBody* GetRigidBody(); //for RigidBody for the golf ball
		CPCollider* GetCollider(); //for collider for the golf ball
		GameObject* GetHero();

		void OnCollisionEnter(CollisionData colData);		

		//timer to despawn the golfball
		//destroy the golfball
	private:
		CPRender* mRender{ nullptr }; //animation purpose
		CPRigidBody* mRigidBody{ nullptr }; //Getting the RB of the enemy
		CPCollider* mCollider{ nullptr }; //Getting the collider of the enemy
		GameObject* mPlayer{ nullptr };

		//Stats of the ball should have
		float mSpeedMagnitude{};
		float mVelocity{};
		float mSize{};

		float mLifetime, mCurrentLifetime;
	};
}