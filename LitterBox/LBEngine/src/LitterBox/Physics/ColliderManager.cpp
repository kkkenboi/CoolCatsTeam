#include "ColliderManager.h"
#include "PhysicsMath.h"

namespace LB 
{
	ColliderManager* COLLIDERS = nullptr;

	ColliderManager::ColliderManager()
	{
		if (!COLLIDERS)
		{
			COLLIDERS = this;
		}
		else
		{
			std::cerr << "Collider System already exists\n";
		}

		// HARDCODING NUMBER RN
		constexpr size_t COL_POOL_SIZE = 2500;
		this->m_poolSize = COL_POOL_SIZE;
		this->m_colliderPool = new CPCollider * [COL_POOL_SIZE];

		// Initialize all the Colliders to nullptrs
		for (size_t i = 0; i < COL_POOL_SIZE; ++i)
		{
			m_colliderPool[i] = nullptr;
		}
	}

	ColliderManager::~ColliderManager()
	{
		for (size_t i = 0; i < m_poolSize; ++i)
		{
			if (m_colliderPool[i] != nullptr)
			{
				delete m_colliderPool[i];
			}
		}
		delete[] m_colliderPool;
	}

	void ColliderManager::AddColliderToPool(CPCollider* col)
	{
		for (size_t i = 0; i < m_poolSize; ++i)
		{
			if (m_colliderPool[i] == nullptr)
			{
				m_colliderPool[i] = col;
				return;
			}
		}
	}

	bool CheckColliders(CPCollider* colA, CPCollider* colB, Vec2<float>& normal_out, float& depth_out)
	{
		normal_out.x = 0.f;
		normal_out.y = 0.f;
		depth_out = 0.f;

		if (colA->m_shape == COL_POLYGON)
		{
			if (colB->m_shape == COL_POLYGON)
			{
				// A-B
				// BOX-BOX
				return CollisionIntersection_BoxBox_SAT(colA->m_transformedVerts, colB->m_transformedVerts, normal_out, depth_out);
			}
			else if (colB->m_shape == COL_CIRCLE)
			{
				// A-B
				// BOX-CIRCLE
				bool result = CollisionIntersection_CircleBox_SAT(colB->m_pos, colB->m_radius, colA->m_transformedVerts, normal_out, depth_out);

				normal_out.x = -normal_out.x;
				normal_out.y = -normal_out.y;

				return result;
			}
		}
		if (colA->m_shape == COL_CIRCLE)
		{
			if (colB->m_shape == COL_POLYGON)
			{
				// A-B
				// CIRCLE-BOX
				bool result = CollisionIntersection_CircleBox_SAT(colA->m_pos, colA->m_radius, colB->m_transformedVerts, normal_out, depth_out);
				return result;
			}
			else if (colB->m_shape == COL_CIRCLE)
			{
				// A-B
				// CIRCLE-CIRCLE
				return CollisionIntersection_CircleCircle(colA->m_pos, colB->m_pos, colA->m_radius, colB->m_radius, normal_out, depth_out);
			}
		}
		return false;
	}

	void ResolveColliders(CPCollider* bodyA, CPCollider* bodyB, LB::Vec2<float> normal, float depth) 
	{

		UNREFERENCED_PARAMETER(depth);
		// Need to get relative velocity from A to B
		LB::Vec2<float> relativeVelocity = bodyB->rigidbody->mVelocity - bodyA->rigidbody->mVelocity;

		// If the dot product of relVel and normal is more than 0.f
		if (PHY_MATH::DotProduct(relativeVelocity, normal) > 0.f) {
			return;
		}

		float e = PHY_MATH::FindMin(bodyA->rigidbody->mRestitution, bodyB->rigidbody->mRestitution);

		float j = -(1.f + e) * PHY_MATH::DotProduct(relativeVelocity, normal);
		j /= bodyA->rigidbody->mInvMass + bodyB->rigidbody->mInvMass;

		// Magnitude * Normal to get the impulse given to the objects
		LB::Vec2<float> impulse = normal * j;


		if (!bodyA->rigidbody->isStatic)
		{
			bodyA->rigidbody->mVelocity -= impulse * bodyA->rigidbody->mInvMass;
		}
		if (!bodyB->rigidbody->isStatic)
		{
			bodyB->rigidbody->mVelocity += impulse * bodyB->rigidbody->mInvMass;
		}

	}

	void ColliderManager::Initialize()
	{
		SetSystemName("Collider System");
	}

	void ColliderManager::FixedUpdate()
	{
		// Does all the collision Checks here
		// Does all the collision Checks here
		
		// ==================
		// Update Collider Positions
		// ==================
		for (size_t i = 0; i < this->m_poolSize; ++i)
		{
			if (this->m_colliderPool[i] != nullptr)
			{
				this->m_colliderPool[i]->FixedUpdate();
				this->m_colliderPool[i]->m_collided = false;
			}
		}

		// ==================
		// Collision Step
		// ==================
		LB::Vec2<float> normal_out{ 0.f , 0.f };
		float depth_out{ 0.f };

		for (size_t i = 0; i < m_poolSize; ++i)
		{
			CPCollider* colA = m_colliderPool[i];

			for (size_t j = i + 1; j < m_poolSize; ++j)
			{
				CPCollider* colB = m_colliderPool[j];


				if (colA == nullptr || colB == nullptr)
				{
					continue;
				}


				if (colA == colB)
				{
					continue;
				}

				// Normal here is moving B away from A
				if (CheckColliders(colA, colB, normal_out, depth_out))
				{
					colA->m_collided = true;
					colB->m_collided = true;
					if (colA->rigidbody != nullptr && colB->rigidbody != nullptr)
					{

						LB::Vec2<float>inverse_normal{ -normal_out.x, -normal_out.y };
						if (colA->rigidbody->isStatic)
						{
							colB->rigidbody->Move(normal_out * depth_out);
						}
						else if (colB->rigidbody->isStatic)
						{
							colA->rigidbody->Move(inverse_normal * depth_out);
						}
						else
						{
							colA->rigidbody->Move(inverse_normal * depth_out);
							colB->rigidbody->Move(normal_out * depth_out);
						}

						ResolveColliders(colA, colB, normal_out, depth_out);
					}
				}
			}
		}

	}	// End of FixedUpdate

	void ColliderManager::Update() 
	{
		// Draw Debug here

		if (DEBUG->IsDebugOn())
		{
			for (size_t i = 0; i < m_poolSize; ++i)
			{
				if (m_colliderPool[i] != nullptr)
				{
					m_colliderPool[i] ->DebugDraw();
				}
			}
		}
	}
}