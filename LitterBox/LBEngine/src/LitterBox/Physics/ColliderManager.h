#pragma once
#include "Collisions.h"
#include "LitterBox/Core/System.h"

namespace LB 
{
	
	class ColliderManager : public ISystem
	{
	private:
		size_t m_poolSize;
		CPCollider** m_colliderPool;
	public:
		ColliderManager();

		~ColliderManager();

		void AddColliderToPool(CPCollider* col);

		void CheckCollision(CPCollider* colA, CPCollider* colB, Vec2<float> normal_out, float depth_out);;

		void CollisionResolution();

		// ================
		// ISystem function overrides
		// ================
		void Initialize();

		// Updates the pool in a fixed timestep
		void FixedUpdate();

		// Updates the Collider debug render in normal framerate
		void Update();
	};

	//void CheckCollisions(CPCollider* colA, CPCollider* colB, Vec2<float> normal_out, float depth_out);

	//void ResolveCollisions(CPCollider* colA, CPCollider* colB, Vec2<float> normal_out, float depth_out);
	
	extern ColliderManager* COLLIDERS;
}