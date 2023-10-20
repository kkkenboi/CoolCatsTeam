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

		void RemoveColliderFromPool(CPCollider* col);

		//void CheckCollision(CPCollider* colA, CPCollider* colB, Vec2<float> normal_out, float depth_out)

		//void ResolveColliders(CPCollider* bodyA, CPCollider* bodyB, LB::Vec2<float> normal, float depth);

		std::vector<CPCollider*> OverlapCircle(Vec2<float> position, float radius);

		// ================
		// ISystem function overrides
		// ================
		void Initialize();

		// Updates the pool in a fixed timestep
		void FixedUpdate();

		// Updates the Collider debug render in normal framerate
		void Update();

		// Makes all the ptrs nullptrs in the pool
		//void Destroy();
	};

	bool CheckColliders(CPCollider* colA, CPCollider* colB, Vec2<float>& normal_out, float& depth_out);

	void ResolveColliders(CPCollider* colA, CPCollider* colB, Vec2<float> normal_out, float depth_out);
	
	extern ColliderManager* COLLIDERS;
}