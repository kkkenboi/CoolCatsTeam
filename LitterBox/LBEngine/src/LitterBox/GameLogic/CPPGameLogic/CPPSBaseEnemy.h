#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"
namespace LB
{
	class CPPSBaseEnemy : public CPPBehaviour
	{
	public:

		void Start() override;
		void Update() override;
		void Destroy() override;

		void OnCollisionEnter(CollisionData col) override;

		//Getter functions
		//Animator once we have it
		virtual CPRender* GetRender();
		virtual CPRigidBody* GetRigidBody();
		virtual CPCollider* GetCollider();
		GameObject* GetHero();

		//Accessor functions
		int& GetHealth();
		float& GetSpeedMag();
		

	private:
		int mHealth{};		//normal health of the enemy
		float mSpeedMagnitude{};	//movespeed of the enemy
		GameObject* mPlayer{ nullptr }; //Caching the player obj
		Vec2<float> leftFace;
		Vec2<float> rightFace;
		
		CPRender* mRender;
		CPRigidBody* mRigidBody;
		CPCollider* mCollider;
	};
}

