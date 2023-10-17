#include "ColliderManager.h"

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

	void ColliderManager::CheckCollision(CPCollider* colA, CPCollider* colB, Vec2<float> normal_out, float depth_out) 
	{
		for (size_t i = 0; i < m_poolSize; ++i)
		{
			
		}
	}

	void ColliderManager::CollisionResolution() 
	{
	
	}

	void ColliderManager::Initialize()
	{
		SetSystemName("Collider System");
	}

	void ColliderManager::FixedUpdate()
	{
		// Does all the collision Checks here
	}

	void ColliderManager::Update() 
	{
		
	}
}