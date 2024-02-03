#pragma once
#include "Collisions.h"
#include "LitterBox/Core/System.h"
#include "ColliderLayers.h"
#include "ImplicitGridSystem.h"

namespace LB 
{
	
	class ColliderManager : public ISystem
	{
	private:
		size_t m_poolSize;
		CPCollider** m_colliderPool;
		ColliderLayerSystem  m_layerSystem;
		ColliderImplicitGridSystem m_implicitgridSystem;
		
	public:
		std::vector<std::pair<std::string, SHAPETYPE>> m_shapeTypes;

		/*!***********************************************************************
			\brief
			Constructor of the ColliderManager singleton class, handles
			all the updates of the CPColliders and resolves resolution
			of collisions
		*************************************************************************/
		ColliderManager();

		/*!***********************************************************************
			\brief
			Adds a CPCollider* to the collide pool in the ColliderManager
			\param CPCollider* col
			The collider to add to the pool
		*************************************************************************/
		void AddColliderToPool(CPCollider* col);

		/*!***********************************************************************
			\brief
			Removes a CPCollider* from the collider pool in the ColliderManager
			\param CPCollider* col
			The collider to remove from the pool
		*************************************************************************/
		void RemoveColliderFromPool(CPCollider* col);

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
		std::vector<CPCollider*> OverlapCircle(Vec2<float> position, float radius);

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
		std::vector<GameObject*> OverlapCircleGameObj(Vec2<float> position);

		/*!***********************************************************************
		\brief
			Gets the ShapeName of the Collider in the ColliderManager
		*************************************************************************/
		std::string GetShapeNames(SHAPETYPE type);

		ColliderLayerSystem& GetLayerSystem();

		ColliderImplicitGridSystem& GetGridSystem();

		// ================
		// ISystem function overrides
		// ================
		/*!***********************************************************************
		\brief
			Initializes the ColliderManager system
		*************************************************************************/
		void Initialize();

		/*!***********************************************************************
		\brief
			The FixedUpdate for all the Colliders will be called in here
		*************************************************************************/
		void FixedUpdate();

		/*!***********************************************************************
		\brief
			ColliderManager's update step, will update the Debug drawing of all
			the colliders within the pool
		*************************************************************************/
		void Update();

		/*!***********************************************************************
		\brief
			Initializes the ColliderManager system
		*************************************************************************/
		void Destroy();
	};

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
	bool CheckColliders(CPCollider* colA, CPCollider* colB, Vec2<float>& normal_out, float& depth_out);

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
	void ResolveColliders(CPCollider* colA, CPCollider* colB, Vec2<float> normal_out, float depth_out);
	
	extern ColliderManager* COLLIDERS;
}