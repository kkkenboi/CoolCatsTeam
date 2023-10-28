#pragma once
#include "LitterBox/Utils/Math.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"

namespace LB {

	enum SHAPETYPE
	{
		COL_CIRCLE = 0,
		COL_POLYGON = 1,
		COL_NONE = 2
	};

	struct AABB
	{
		LB::Vec2<float> m_c; // Center of the AABB

		LB::Vec2<float> m_min;
		LB::Vec2<float> m_max;
	};

	struct LineSegment
	{
		LB::Vec2<float>	m_pt0;
		LB::Vec2<float>	m_pt1;
		LB::Vec2<float>	m_normal;
	};


	class CPCollider : public IComponent
	{
	public:
		CPTransform* transform;
		CPRigidBody* rigidbody;
		GameObject* m_gameobj;
	public:
		SHAPETYPE m_shape;
		bool m_simpleCol;

		bool m_collided; // true

		LB::Vec2<float> m_pos;

		float m_widthUnscaled;
		float m_heightUnscaled;
		float m_radiusUnscaled;

		float m_width;
		float m_height;
		float m_radius;

		AABB m_aabb;

		// For SAT
		float m_rotation;
		int m_vertAmount;
		std::vector<Vec2<float>> m_untransformedVerts;
		std::vector<Vec2<float>> m_transformedVerts;

		// For physics
		Vec2<float> normal_out;
		float depth_out;

		/*!***********************************************************************
		  \brief
		  Constructor for the Collider class
		*************************************************************************/
		void CreateCPCollider();

		/*!***********************************************************************
		  \brief
		  This function instantiates the AABB with the known values in the Collider class
		*************************************************************************/
		void CreateAABB();
		/*!***********************************************************************
		  \brief
		  This function instantiates the Polygon Array the known values in the
		  Collider class
		*************************************************************************/
		void CreatePolygonBox();

		void CreatePolygonHexagon();

		void AddVertice(Vec2<float> vertice);

		void AddVertice(float x, float y);

		void UpdateColliderBoxVertices();

		void UpdateColliderAABB();

		void DebugDraw();

		void SetWidthHeightRadius(float width, float height, float radius);

		std::string GetShapeName();

	public:
		// ========
		// IComponent Overrides
		// ========
		void Initialise();

		void FixedUpdate();

		void Destroy();

		ComponentTypeID GetType() override
		{
			return C_CPCollider;
		}

		
		

		// CPCollider* ChecksWhoisColliding(mPosition, mRadius) 
		// Third param layer? Only enemy layers
		// Makes an array of Colliders that are within this temp Collider Circle
		// and returns the array
	};

}