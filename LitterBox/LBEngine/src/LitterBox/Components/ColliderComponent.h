/*!************************************************************************
 \file				ColliderComponent.cpp
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

#pragma once
#include "LitterBox/Utils/Math.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "LitterBox/Physics/ColliderLayers.h"

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

		float m_widthUnscaledOG;
		float m_heightUnscaledOG;
		float m_radiusUnscaledOG;

		float m_width;
		float m_height;
		float m_radius;

		float m_ratio_width;
		float m_ratio_height;

		AABB m_aabb;

		// For SAT
		float m_rotation;
		int m_vertAmount;
		std::vector<Vec2<float>> m_untransformedVerts;
		std::vector<Vec2<float>> m_ratioUntransformedVerts;
		std::vector<Vec2<float>> m_transformedVerts;

		// For physics
		Vec2<float> normal_out;
		float depth_out;

		// Layer
		ColliderLayer m_collisionlayer;

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

		/*!***********************************************************************
		  \brief
		  This function instantiates the Polygon Array the known values in the
		  Collider class
		*************************************************************************/
		void CreatePolygonHexagon();

		/*!***********************************************************************
		  \brief
		  This function allows you to add vertices to the Collider's unTransformedVerts
		*************************************************************************/
		void AddVertice(Vec2<float> vertice);

		/*!***********************************************************************
		  \brief
		  This function allows you to add vertices to the Collider's unTransformedVerts
		*************************************************************************/
		void AddVertice(float x, float y);

		/*!***********************************************************************
		  \brief
		  Updates the Scaled data members of CPCollider
		*************************************************************************/
		void UpdateScaledData();

		/*!***********************************************************************
		  \brief
		  This function updates the Vertices vector of the CPCollider
		*************************************************************************/
		void UpdateColliderBoxVertices();

		/*!***********************************************************************
		  \brief
		  This function updates the AABB collider of the CPCollider
		*************************************************************************/
		void UpdateColliderAABB();

		/*!***********************************************************************
		  \brief
		  This function allows the Debugger draws for the Collider vertices and lines
		*************************************************************************/
		void DebugDraw();

		/*!***********************************************************************
		  \brief
		  This function allows the user to change the data members of,
		  mWidth, mHeight and mRadius
		*************************************************************************/
		void SetWidthHeightRadius(float width, float height, float radius);

		/*!***********************************************************************
		  \brief
		  This function allows you to check whether or not
		*************************************************************************/
		bool HasRB();

		/*!***********************************************************************
		  \brief
		  This function gets the ShapeName of the CPCollider
		*************************************************************************/
		std::string GetShapeName();

		std::string GetLayerName();

		CPTransform* GetTransform();

		CPRigidBody* GetRigidBody();

		GameObject* GetGameObject();

		/*!***********************************************************************
		  \brief
		  This serializes some of the data members of CPCollider
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc) override;

		/*!***********************************************************************
		  \brief
		  This deserializes some of the data members of CPColliders
		*************************************************************************/
		bool Deserialize(const Value& data) override;

	public:
		// ========
		// IComponent Overrides
		// ========

		/*!***********************************************************************
		  \brief
		  This function allows you to check whether or not
		*************************************************************************/
		void Initialise();

		/*!***********************************************************************
		  \brief
		  This is the FixedUpdate of the Collider class
		*************************************************************************/
		void FixedUpdate();

		/*!***********************************************************************
		\brief
		This is the destructor of the Collider class
		*************************************************************************/
		void Destroy();

		/*!***********************************************************************
		\brief
		This gets the Tupe of the CPCollider
		*************************************************************************/
		static ComponentTypeID GetType()
		{
			return C_CPCollider;
		}

	};

}