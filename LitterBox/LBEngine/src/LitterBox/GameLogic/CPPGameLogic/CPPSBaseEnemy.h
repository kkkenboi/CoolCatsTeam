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
		 CPRender* GetRender();
		 CPRigidBody* GetRigidBody();
		 CPCollider* GetCollider();
		 GameObject* GetHero();

		//Accessor functions
		int& GetHealth();
		float& GetSpeedMag();
		
		int mHealth{};		//normal health of the enemy
		float mSpeedMagnitude{};	//movespeed of the enemy

		CPRender* mRender{nullptr};
		CPRigidBody* mRigidBody{ nullptr };
		CPCollider* mCollider{ nullptr };
		virtual void Die();
		virtual void Hurt();
	protected:
		GameObject* mPlayer{ nullptr }; //Caching the player obj
		GameObject* mGameManager{ nullptr }; //Caching the game manager
		Vec2<float> leftFace;
		Vec2<float> rightFace;

		Vec2<float> DirToPlayer;
		Vec2<float> TransformRight{ 1,0 };
		bool mShouldDestroy{ false };
	};
}

