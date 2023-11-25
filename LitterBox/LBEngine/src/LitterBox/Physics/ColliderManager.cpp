/*!************************************************************************
 \file				ColliderManager.cpp
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:		CSD2401A
 \date				03-11-2023
 \brief
 This file contains the ColliderManager class and all its functionalities,
 this class handles all the Colliders and keeps them in a pool, handling
 all the updates

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "ColliderManager.h"
#include "PhysicsMath.h"
#include "Litterbox/Engine/Time.h"
#include "LitterBox/Core/Core.h"

namespace LB 
{
	ColliderManager* COLLIDERS = nullptr;

	/*!***********************************************************************
		\brief
		Constructor of the ColliderManager singleton class, handles
		all the updates of the CPColliders and resolves resolution
		of collisions
	*************************************************************************/
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
		this->m_colliderPool = DBG_NEW CPCollider * [COL_POOL_SIZE];

		// Initialize all the Colliders to nullptrs
		for (size_t i = 0; i < COL_POOL_SIZE; ++i)
		{
			m_colliderPool[i] = nullptr;
		}

		// Initialize the ColliderLayerSystem
		m_layerSystem.Initialize();
	}

	/*!***********************************************************************
		\brief
		Adds a CPCollider* to the collide pool in the ColliderManager
		\param CPCollider* col
		The collider to add to the pool
	*************************************************************************/
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

	/*!***********************************************************************
		\brief
		Removes a CPCollider* from the collider pool in the ColliderManager
		\param CPCollider* col
		The collider to remove from the pool
	*************************************************************************/
	void ColliderManager::RemoveColliderFromPool(CPCollider* col)
	{
		for (size_t i = 0; i < m_poolSize; ++i)
		{
			if (m_colliderPool[i] == nullptr) 
			{
				continue;
			}
			if (m_colliderPool[i] == col)
			{
				m_colliderPool[i] = nullptr;
			}
		}
	}

	/*!***********************************************************************
	\brief
		This function allows you to use a position, and radius and checks
		the ColliderManager and returns a vector of all the CPColliders that were
		found within the circle made
	\param Vec2<float> position
		The position of the circle to make
	\param float radius
		The size of the radius to make
	\return std::vector<CPCollider*>
		The vector of CPColliders* to return that are within the created circle
		overlap
	*************************************************************************/
	std::vector<CPCollider*> ColliderManager::OverlapCircle(Vec2<float> position, float radius)
	{
		Vec2<float> normal{ 0.f,0.f };
		float depth{ 0.f };

		std::vector<CPCollider*> vec_overlapped;

		for (size_t i = 0; i < m_poolSize; ++i)
		{
			if (m_colliderPool[i] == nullptr) 
			{
				continue;
			}
			if (m_colliderPool[i]->m_shape == COL_POLYGON) 
			{
				if (CollisionIntersection_CirclePolygon_SAT(position, radius,
					m_colliderPool[i]->m_transformedVerts, normal, depth))
				{
					vec_overlapped.push_back(m_colliderPool[i]);
				}
				
			}
			
			if (m_colliderPool[i]->m_shape == COL_CIRCLE) 
			{
				if (CollisionIntersection_CircleCircle(position, m_colliderPool[i]->m_pos,
					radius, m_colliderPool[i]->m_radius, normal, depth))
				{
					vec_overlapped.push_back(m_colliderPool[i]);
				}
			}
			
		}

		return vec_overlapped;
	}

	/*!***********************************************************************
	\brief
		This function allows you to use a position, and radius and checks
		the ColliderManager and returns a vector of all the GameObject that were
		found within the circle made
	\param Vec2<float> position
		The position of the circle to make
	\param float radius
		The size of the radius to make
	\return std::vector<GameObject*>
		The vector of GameObject* to return that are within the created circle
		overlap
	*************************************************************************/
	std::vector<GameObject*> ColliderManager::OverlapCircleGameObj(Vec2<float> position)
	{
		float radius = 1.0f;
		Vec2<float> normal{ 0.f,0.f };
		float depth{ 0.f };

		std::vector<GameObject*> vec_overlapped;

		for (size_t i = 0; i < m_poolSize; ++i)
		{
			if (m_colliderPool[i] == nullptr)
			{
				continue;
			}

			if (m_colliderPool[i]->m_shape == COL_POLYGON)
			{
				if (CollisionIntersection_CirclePolygon_SAT(position, radius,
					m_colliderPool[i]->m_transformedVerts, normal, depth))
				{
					vec_overlapped.push_back(m_colliderPool[i]->m_gameobj);
				}
			}
			if (m_colliderPool[i]->m_shape == COL_CIRCLE)
			{
				if (CollisionIntersection_CircleCircle(position, m_colliderPool[i]->m_pos,
					radius, m_colliderPool[i]->m_radius, normal, depth))
				{
					vec_overlapped.push_back(m_colliderPool[i]->m_gameobj);
				}
			}
		}

		return vec_overlapped;
	}

/*!***********************************************************************
\brief
	Gets the ShapeName of the Collider in the ColliderManager
*************************************************************************/
	std::string ColliderManager::GetShapeNames(SHAPETYPE type)
	{
		switch (type)
		{
		case COL_CIRCLE:
			return "Circle";
			break;
		case COL_POLYGON:
			return "Polygon";
			break;
		case COL_NONE:
			return "None";
			break;
		default:
			return "Error!";
			break;
		}
	}

	
	ColliderLayerSystem& ColliderManager::GetLayerSystem()
	{
		return m_layerSystem;
	}
	

	// ===
	// END OF ColliderManager member functions
	// ===

	/*!***********************************************************************
	\brief
		This function allows you to check collisions between two CPColliders
		and gets the result of normal_out and depth_out from the collision if
		made
	\param CPCollider* colA
		ColliderA to check collisions with
	\param CPCollider* colB
		ColliderB to check collisions with
	\param Vec2<float>& normal_out
		The normal for collision resolution
	\param float& depth_out
		The depth for collision resolution
	*************************************************************************/
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
				return CollisionIntersection_PolygonPolygon_SAT(colA->m_transformedVerts, colB->m_transformedVerts, normal_out, depth_out);
			}
			else if (colB->m_shape == COL_CIRCLE)
			{
				// A-B
				// BOX-CIRCLE
				bool result = CollisionIntersection_CirclePolygon_SAT(colB->m_pos, colB->m_radius, colA->m_transformedVerts, normal_out, depth_out);

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
				bool result = CollisionIntersection_CirclePolygon_SAT(colA->m_pos, colA->m_radius, colB->m_transformedVerts, normal_out, depth_out);
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

/*!***********************************************************************
\brief
	This function resolves collisions between two CPCollider* objects using
	normal_out and depth_out
\param CPCollider* bodyA
	The first CPCollider Obj
\param CPCollider* bodyB
	The second CPCollider Obj
\param Vec2<float> normal_out
	The normal used for collision resolution
\param Vec2<float> depth_out
	The depth used for collision resolution
*************************************************************************/
	void ResolveColliders(CPCollider* bodyA, CPCollider* bodyB, LB::Vec2<float> normal, float depth) 
	{

		UNREFERENCED_PARAMETER(depth);
		// Need to get relative velocity from A to B
		LB::Vec2<float> relativeVelocity = bodyB->rigidbody->mVelocity - bodyA->rigidbody->mVelocity;

		// If the dot product of relVel and normal is more than 0.f
		if (PHY_MATH::DotProduct(relativeVelocity, normal) > 0.f) {
			return;
		}

		float minRestitution = PHY_MATH::FindMin(bodyA->rigidbody->mRestitution, bodyB->rigidbody->mRestitution);

		float magnitude = -(1.f + minRestitution) * PHY_MATH::DotProduct(relativeVelocity, normal);
		magnitude /= bodyA->rigidbody->mInvMass + bodyB->rigidbody->mInvMass;

		// Magnitude * Normal to get the impulse given to the objects
		LB::Vec2<float> impulse = normal * magnitude;


		if (!bodyA->rigidbody->isStatic)
		{
			bodyA->rigidbody->mVelocity -= impulse * bodyA->rigidbody->mInvMass;
		}
		if (!bodyB->rigidbody->isStatic)
		{
			bodyB->rigidbody->mVelocity += impulse * bodyB->rigidbody->mInvMass;
		}

	}

/*!***********************************************************************
\brief
	Initializes the ColliderManager system
*************************************************************************/

	void ColliderManager::Initialize()
	{
		SetSystemName("Collider System");
		const SHAPETYPE ShapeTypeVector[] = { COL_CIRCLE, COL_POLYGON, COL_NONE };
		for (SHAPETYPE shape : ShapeTypeVector)
		{
			m_shapeTypes.emplace_back(std::make_pair(COLLIDERS->GetShapeNames(shape), shape));
		}
	}

/*!***********************************************************************
\brief
	The FixedUpdate for all the Colliders will be called in here
*************************************************************************/
	void ColliderManager::FixedUpdate()
	{
		
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

				if (colA->rigidbody != nullptr && colB->rigidbody != nullptr)
				{
					if (colA->rigidbody->isStatic == true && colB->rigidbody->isStatic)
					{
						continue;
					}
				}

				// Check if layers can be collided with, if cannot collide, continue
				if (!this->GetLayerSystem().ShouldLayerCollide(colA->m_collisionlayer, colB->m_collisionlayer))
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
							colA->rigidbody->Move(inverse_normal * depth_out / 2.f);
							colB->rigidbody->Move(normal_out * depth_out / 2.f);
						}

						ResolveColliders(colA, colB, normal_out, depth_out);
						
						if (colA->m_gameobj->HasComponent<CPScriptCPP>()) {
							CollisionData colData;
							colData.colliderThis = colA;
							colData.colliderOther = colB;
							if (colA->m_gameobj->GetComponent<CPScriptCPP>()->GetInstance() != nullptr)
							{
								colA->m_gameobj->GetComponent<CPScriptCPP>()->GetInstance()->OnCollisionEnter(colData);
							}
						}
						if (colB->m_gameobj->HasComponent<CPScriptCPP>()) {
							CollisionData colData;
							colData.colliderThis = colB;
							colData.colliderOther = colA;
							if (colB->m_gameobj->GetComponent<CPScriptCPP>()->GetInstance() != nullptr)
							{
								colB->m_gameobj->GetComponent<CPScriptCPP>()->GetInstance()->OnCollisionEnter(colData);
							}
						}
						
					}
				}
			}
		}

	}	// End of FixedUpdate

/*!***********************************************************************
\brief
	ColliderManager's update step, will update the Debug drawing of all
	the colliders within the pool
*************************************************************************/
	void ColliderManager::Update() 
	{

		if (TIME->IsPaused() || !CORE->IsPlaying())
		{
			for (size_t i = 0; i < m_poolSize; ++i)
			{
				if (m_colliderPool[i] != nullptr)
				{
					m_colliderPool[i]->m_rotation = m_colliderPool[i]->GetTransform()->GetRotation();
					m_colliderPool[i]->m_pos = m_colliderPool[i]->GetTransform()->GetPosition();
					m_colliderPool[i]->UpdateScaledData();
					m_colliderPool[i]->UpdateColliderBoxVertices();
					m_colliderPool[i]->UpdateColliderAABB();
				}
			}
		}

		if (DEBUG->IsDebugOn())
		{
			for (size_t i = 0; i < m_poolSize; ++i)
			{
				if (m_colliderPool[i] != nullptr)
				{
					m_colliderPool[i]->DebugDraw();
				}
			}
		}
	}

	/*!***********************************************************************
	\brief
		Initializes the ColliderManager system
	*************************************************************************/
	void ColliderManager::Destroy()
	{
		for (size_t i = 0; i < m_poolSize; ++i)
		{
			m_colliderPool[i] = nullptr;
		}

		delete[] m_colliderPool;

		COLLIDERS = nullptr;
	}
	
}