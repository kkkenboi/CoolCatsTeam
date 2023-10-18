/*!************************************************************************
 \file                Collisions.cpp
 \author(s)           Justine Carlo Villa Ilao
 \par DP email(s):    justine.c@digipen.edu
 \par Course:         CSD2401A
 \date                04-09-2023
 \brief
 This file contains the function definitions of all the Collision Checks
 as well as the PhysicsTransform constructor and the Collider constructor

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Collisions.h"
#include "LitterBox/Components/ColliderComponent.h"
#include "ColliderManager.h"
#include "PhysicsMath.h"
#include <cmath>


namespace LB
{
	// ======== !!!!!!!!!
	// CPCollider Class
	// ======== !!!!!!!!!
	/*!***********************************************************************
	  \brief
	  Creating a CPCollider for the CPCollider class
	*************************************************************************/
	void CPCollider::CreateCPCollider()
	{
		this->transform = gameObj->GetComponent<CPTransform>();
		if (gameObj->HasComponent<CPRigidBody>())
		{
			this->rigidbody = gameObj->GetComponent<CPRigidBody>();
		}
		else
		{
			this->rigidbody = nullptr;
		}

		this->m_shape = COL_POLYGON;
		this->m_simpleCol = true;
		this->m_collided = false;

		this->m_pos = transform->GetPosition();
		this->m_width = 100.f;
		this->m_height = 100.f;
		this->m_radius = 50.f;

		this->m_rotation = 0.f;
		this->m_vertAmount = 4;

		this->CreateAABB();
		this->CreatePolygonBox();

		COLLIDERS->AddColliderToPool(this);
	}


	/*!***********************************************************************
	  \brief
	  This function instantiates the AABB with the known values in the Collider class
	*************************************************************************/
	void CPCollider::CreateAABB()
	{
		if (this->m_shape == COL_CIRCLE)
		{
			this->m_aabb.m_c = m_pos;
			this->m_aabb.m_min = LB::Vec2<float>{ m_pos.x - m_radius, m_pos.y - m_radius };
			this->m_aabb.m_max = LB::Vec2<float>{ m_pos.x + m_radius, m_pos.y + m_radius };
		}
		else if (this->m_shape == COL_POLYGON)
		{
			this->m_aabb.m_c = m_pos;
			this->m_aabb.m_min = LB::Vec2<float>{ m_pos.x - m_width / 2, m_pos.y - m_height / 2 };
			this->m_aabb.m_max = LB::Vec2<float>{ m_pos.x + m_width / 2, m_pos.y + m_height / 2 };
		}
		else
		{
			this->m_aabb.m_c = m_pos;
			this->m_aabb.m_min = LB::Vec2<float>{ 0.0f, 0.0f };
			this->m_aabb.m_max = LB::Vec2<float>{ 0.0f, 0.0f };
		}
	}

	/*!***********************************************************************
	  \brief
	  This function instantiates the Polygon Array the known values in the
	  Collider class
	*************************************************************************/
	void CPCollider::CreatePolygonBox()
	{
		// Polygon vertices creations goes from
		// Top-Left -> Top-Right -> Bottom-Right -> Bottom-Left
		// Get vertices for a polygon
		m_untransformedVerts.clear();
		m_transformedVerts.clear();

		float left = -m_width / 2;
		float right = m_width / 2;
		float top = m_height / 2;
		float bottom = -m_height / 2;

		m_untransformedVerts.push_back(Vec2<float>{left, top});

		m_untransformedVerts.push_back(Vec2<float>{right, top});

		m_untransformedVerts.push_back(Vec2<float>{right, bottom});
		
		m_untransformedVerts.push_back(Vec2<float>{left, bottom});

		m_vertAmount = m_untransformedVerts.size();
	}

	void CPCollider::CreatePolygonHexagon()
	{
		m_untransformedVerts.clear();
		m_transformedVerts.clear();

		float left = -m_width / 2;
		float mid_left = -m_width / 3;
		float right = m_width / 2;
		float mid_right = m_width / 3;
		float top = m_height / 2;
		float middle = 0.f;
		float bottom = -m_height / 2;

		m_untransformedVerts.push_back(Vec2<float>{mid_left, top});

		m_untransformedVerts.push_back(Vec2<float>{mid_right, top});

		m_untransformedVerts.push_back(Vec2<float>{right, middle});

		m_untransformedVerts.push_back(Vec2<float>{mid_right, bottom});

		m_untransformedVerts.push_back(Vec2<float>{mid_left, bottom});

		m_untransformedVerts.push_back(Vec2<float>{left, middle});

		m_vertAmount = m_untransformedVerts.size();
	}

	void CPCollider::AddVertice(Vec2<float> vertice)
	{
		m_untransformedVerts.push_back(vertice);
	}


	void CPCollider::AddVertice(float x, float y)
	{
		m_untransformedVerts.push_back(Vec2<float>{x, y});
	}


	/*!***********************************************************************
		\brief
		Updates the CPRigidBody Box Vertices within its' data members
	*************************************************************************/
	void CPCollider::UpdateColliderBoxVertices()
	{
		// Initialize the transformed verts to be the same size as untransformed verts
		// if it is not the same size
		if (this->m_untransformedVerts.size() != this->m_transformedVerts.size()) {
			for (size_t i = m_transformedVerts.size(); i < m_untransformedVerts.size(); ++i) 
			{
				m_transformedVerts.push_back(Vec2<float> {0.f, 0.f});
			}
		}
		PhysicsTransform xtransform{ this->m_pos, this->m_rotation };

		for (int i = 0; i < this->m_untransformedVerts.size(); ++i) {
			// Uses the untransformed vertices as the basis for tranasformation
			LB::Vec2<float> og_vec = this->m_untransformedVerts[i];
			// Transforming the vertices using trigo formulas
			this->m_transformedVerts[i] = LB::Vec2<float>{
				xtransform.m_cos * og_vec.x - xtransform.m_sin * og_vec.y + xtransform.m_posX,
				xtransform.m_sin * og_vec.x + xtransform.m_cos * og_vec.y + xtransform.m_posY };
		}


	}

	/*!***********************************************************************
		\brief
		Updates the AABB collider in the CPRigidBody's data members
	*************************************************************************/
	void CPCollider::UpdateColliderAABB()
	{
		float minX = 10000000.f;
		float maxX = -10000000.f;
		float minY = 10000000.f;
		float maxY = -10000000.f;

		if (this->m_shape == COL_POLYGON)
		{
			for (int i = 0; i < 4; ++i)
			{
				// Take the Transformed Vertices and use it as the new AABB
				LB::Vec2<float> vec = this->m_transformedVerts[i];

				if (vec.x < minX) minX = vec.x;
				if (vec.x > maxX) maxX = vec.x;
				if (vec.y < minY) minY = vec.y;
				if (vec.y > maxY) maxY = vec.y;
			}
		}
		else if (this->m_shape == COL_CIRCLE)
		{
			// Basically grab the position and make a box using radius as the width and height
			// of the box
			minX = this->m_pos.x - this->m_radius;
			maxX = this->m_pos.x + this->m_radius;
			minY = this->m_pos.y - this->m_radius;
			maxY = this->m_pos.y + this->m_radius;
		}

		this->m_aabb.m_c = LB::Vec2<float>{ (minX + maxX) / 2.f, (minY + maxY) / 2.f };
		this->m_aabb.m_max = LB::Vec2<float>{ maxX, maxY };
		this->m_aabb.m_min = LB::Vec2<float>{ minX, minY };
	}

	void CPCollider::DebugDraw()
	{
		if (this->m_shape == COL_POLYGON)
		{
			for (size_t i = 0; i < this->m_vertAmount; ++i)
			{
				if (!this->m_collided)
				{
					DEBUG->DrawLine(this->m_transformedVerts[i], this->m_transformedVerts[(i + 1) % m_vertAmount]
						, Vec4<float> { 0.f, 0.f, 1.0f, 1.0f });
				}
				else
				{
					DEBUG->DrawLine(this->m_transformedVerts[i], this->m_transformedVerts[(i + 1) % m_vertAmount]
						, Vec4<float> { 0.5f, 0.f, 0.f, 1.0f });
				}
			}
		}
		if (this->m_shape == COL_CIRCLE)
		{
			if (!this->m_collided)
			{
				DEBUG->DrawCircle(m_pos, m_radius,
					Vec4<float> { 0.f, 0.f, 1.0f, 1.0f });
			}
			else
			{
				DEBUG->DrawCircle(m_pos, m_radius,
					Vec4<float> { 0.5f, 0.f, 0.f, 1.0f });
			}
		}
		if (this->rigidbody != nullptr)
		{
			DEBUG->DrawLine(m_pos, PHY_MATH::Normalize(this->rigidbody->mVelocity), PHY_MATH::Length(this->rigidbody->mVelocity) / 5.f,
				Vec4<float> {1.0f, 0.f, 0.f, 0.f});
		}
	}


	// Overrides !!!!!!!

	void CPCollider::Initialise() 
	{
		this->CreateCPCollider();
	}

	void CPCollider::FixedUpdate()
	{
		this->m_pos = transform->GetPosition();
		this->m_rotation = transform->GetRotation();
		this->m_vertAmount = this->m_untransformedVerts.size();


		this->UpdateColliderBoxVertices();
		this->UpdateColliderAABB();

	}

	void CPCollider::Destroy()
	{
		COLLIDERS->RemoveColliderFromPool(this);
	}



	// ============================================================================= !!!!!!!!!
	// End of CPCollider Class
	// ============================================================================= !!!!!!!!!

	/*!***********************************************************************
	  \brief
	  Constructor for the PhysicsTransform struct, to be used for rotational
	*************************************************************************/
	PhysicsTransform::PhysicsTransform(LB::Vec2<float> position, float angle)
	{
		this->m_posX = position.x;
		this->m_posY = position.y;

		this->m_sin = sin(angle);
		this->m_cos = cos(angle);
	}

	// ===============================
	// Collision Intersection Checks
	// ===============================

	/*!***********************************************************************
	  \brief
	  This function does Box-Box collision using Axis-Aligned Bounded Boxes
	  \return
	  Returns true if boxes collide and false is boxes do not collide,
	  normal_out and depth_out are used for collision resolution
	*************************************************************************/
	bool CollisionIntersection_BoxBox(const AABB& aabb1, const LB::Vec2<float>& vel1,
		const AABB& aabb2, const LB::Vec2<float>& vel2, float dt,
		LB::Vec2<float>& normal_out, float& depth_out)
	{
		/*
		Step 1: Check for static collision detection between rectangles (before moving).
					If the check returns no overlap you continue with the following next steps (dynamics).
					Otherwise you return collision true*/
		if (vel1.x == 0 && vel1.y == 0 && vel2.x == 0 && vel2.y == 0) {
			if (aabb1.m_max.x < aabb2.m_min.x || aabb1.m_min.x > aabb2.m_max.x) {
				return 0; // No collision
			}

			if (aabb1.m_max.y < aabb2.m_min.y || aabb1.m_min.y > aabb2.m_max.y) {
				return 0;
			}

			LB::Vec2<float> vecAB = aabb2.m_c - aabb1.m_c;

			depth_out = PHY_MATH::Length(vecAB);
			normal_out = PHY_MATH::Normalize(vecAB);

			return 1;

		}

		/*
		Step 2: Initialize and calculate the new velocity of Vb
				tFirst = 0
				tLast = dt
		*/

		LB::Vec2<float> relVel;
		LB::Vec2<float> Vel1, Vel2;
		//Vec2Set(&Vel1, vel1.x, vel1.y);
		Vel1.x = vel1.x;
		Vel1.y = vel1.y;

		//Vec2Set(&Vel2, vel2.x, vel2.y);
		Vel2.x = vel2.x;
		Vel2.y = vel2.y;

		//Vec2Sub(&relVel, &Vel2, &Vel1); // relativeVel is the relativeVel of object2
		relVel.x = Vel2.x - Vel1.x;
		relVel.y = Vel2.y - Vel1.y;

		// Set object2 to 0 velocity for x and y
		//Vec2Set(&Vel1, 0.f, 0.f);
		Vel1.x = 0.f;
		Vel1.y = 0.f;

		float tFirst_x = 0.f;
		float tLast_x = dt;

		float tFirst_y = 0.f;
		float tLast_y = dt;

		/*Step 3: Working with one dimension (x-axis).
				if(Vb < 0)
					case 1
					case 4
				if(Vb > 0)
					case 2
					case 3

				case 5*/

				//object2 is static
				//object1 is moving with relVel

				// X-axis
				// object1 on left object2 on right
		if (relVel.x < 0.f) {

			// Case 1
			if (aabb1.m_min.x > aabb2.m_max.x) return 0;

			// Case 4
			if (aabb1.m_max.x < aabb2.m_min.x) {
				float tTemp_x = (aabb1.m_max.x - aabb2.m_min.x) / relVel.x;
				if (tTemp_x > tFirst_x) tFirst_x = tTemp_x;
			}

			if (aabb1.m_min.x < aabb2.m_max.x) {
				float tTemp_x = (aabb1.m_min.x - aabb2.m_max.x) / relVel.x;
				if (tTemp_x < tLast_x) tLast_x = tTemp_x;
			}
		}

		if (relVel.x > 0.f) {

			// Case 2
			if (aabb1.m_min.x > aabb2.m_max.x) {
				float tTemp_x = (aabb1.m_min.x - aabb2.m_max.x) / relVel.x;
				if (tTemp_x > tFirst_x) tFirst_x = tTemp_x;
			}

			if (aabb1.m_max.x > aabb2.m_min.x) {
				float tTemp_x = (aabb1.m_max.x - aabb2.m_min.x) / relVel.x;
				if (tTemp_x < tLast_x) tLast_x = tTemp_x;
			}

			// Case 3
			if (aabb1.m_max.x < aabb2.m_min.x) {
				return 0;
			}
		}

		if (tFirst_x > tLast_x) return 0;

		// ------------------------------------------
		/*Step 4: Repeat step 3 on the y-axis*/
		// ------------------------------------------

		if (relVel.y < 0.f) {

			// Case 1
			if (aabb1.m_min.y > aabb2.m_max.y) return 0;

			// Case 4
			if (aabb1.m_max.y < aabb2.m_min.y) {
				float tTemp_y = (aabb1.m_max.y - aabb2.m_min.y) / relVel.y;
				if (tTemp_y > tFirst_y) tFirst_y = tTemp_y;
			}

			if (aabb1.m_min.y < aabb2.m_max.y) {
				float tTemp_y = (aabb1.m_min.y - aabb2.m_max.y) / relVel.y;
				if (tTemp_y < tLast_y) tLast_y = tTemp_y;
			}
		}

		if (relVel.y > 0.f) {

			// Case 2
			if (aabb1.m_min.y > aabb2.m_max.y) {
				float tTemp_y = (aabb1.m_min.y - aabb2.m_max.y) / relVel.y;
				if (tTemp_y > tFirst_y) tFirst_y = tTemp_y;
			}

			if (aabb1.m_max.y > aabb2.m_min.y) {
				float tTemp_y = (aabb1.m_max.y - aabb2.m_min.y) / relVel.y;
				if (tTemp_y < tLast_y) tLast_y = tTemp_y;
			}

			// Case 3
			if (aabb1.m_max.y < aabb2.m_min.y) {
				return 0;
			}
		}

		if (tFirst_y > tLast_y) return 0;

		/*Step 5: Otherwise the rectangles intersect*/
		LB::Vec2<float> vecAB = aabb2.m_c - aabb1.m_c;

		depth_out = PHY_MATH::Length(vecAB);
		normal_out = PHY_MATH::Normalize(vecAB);


		return 1;
	}

	/*!***********************************************************************
	  \brief
		Checks if there is collision between 2 circles,
		- normal_out stores the direction of where the objects should be pushed
		towards, with normal_out is pushing B from A
		- depth_out stores the magnitude of how much the objects should be pushed
	  \return
	  Returns true if the circles collided and false if the circles did not collide
	*************************************************************************/
	bool CollisionIntersection_CircleCircle(LB::Vec2<float> centerA, LB::Vec2<float> centerB, float radiusA, float radiusB, LB::Vec2<float>& normal_out, float& depth_out) {
		normal_out.x = 0.f;
		normal_out.y = 0.f;
		depth_out = 0.f;

		// Has the distance between the two centers now			
		float distance = PHY_MATH::Distance(centerA, centerB);
		float radii = radiusA + radiusB;

		if (distance >= radii) {
			return false;
		}

		// Find a vector normal, pointing from A to B
		normal_out = PHY_MATH::Normalize(centerB - centerA);
		depth_out = radii - distance;

		return true;
	}

	/*!***********************************************************************
	  \brief
		Check collision for Box-Box objects using Separating Axis Theorem
		- normal_out stores the direction of where the objects should be pushed
		towards, with normal_out is pushing B from A
		- depth_out stores the magnitude of how much the objects should be pushed
	  \return
	  Returns true if the boxes collided and false if the boxes did not collide
	*************************************************************************/
	bool CollisionIntersection_BoxBox_SAT(std::vector<Vec2<float>> const& verticesA, std::vector<Vec2<float>> const& verticesB, LB::Vec2<float>& normal_out, float& depth_out)
	{
		normal_out = LB::Vec2<float>{ 0.f,0.f };
		depth_out = 100000000.f;

		// Loop through each edge of obj A
		for (int i = 0; i < verticesA.size(); ++i)
		{
			// Get two vertices from obj A
			LB::Vec2<float> vert1 = verticesA[i];
			LB::Vec2<float> vert2 = verticesA[(i + 1) % verticesA.size()];

			// Edge vector
			LB::Vec2<float> vecA{ vert2.x - vert1.x , vert2.y - vert1.y };

			// Normal of edge vector, using it as the axis of separation to check
			LB::Vec2<float> axis{ -vecA.y , vecA.x };
			axis = PHY_MATH::Normalize(axis);

			float minProjValueA{ 0.f };
			float maxProjValueA{ 0.f };

			float minProjValueB{ 0.f };
			float maxProjValueB{ 0.f };


			// Project all points onto the axis
			ProjectPointsOntoAxis(axis, verticesA, minProjValueA, maxProjValueA);
			ProjectPointsOntoAxis(axis, verticesB, minProjValueB, maxProjValueB);


			// Checks if we can find separation by checking if the
			// min projection values of either object is larger than the max
			// projection values of the other object
			if (maxProjValueA <= minProjValueB || maxProjValueB <= minProjValueA) {
				return false;
			}

			// We check for the depth here now based on knowing that
			// the two max values being larger than the two min values
			float axisDepth = PHY_MATH::FindMin(maxProjValueA - minProjValueB, maxProjValueB - minProjValueA);

			if (axisDepth < depth_out)
			{
				depth_out = axisDepth;
				normal_out = axis;
			}
		} // End of looping through obj A's edges

		// Start of obj B's edges
		for (int i = 0; i < verticesB.size(); ++i)
		{
			// Get two vertices from obj B
			LB::Vec2<float> vert1 = verticesB[i];
			LB::Vec2<float> vert2 = verticesB[(i + 1) % verticesB.size()];

			// Edge vector
			LB::Vec2<float> vecB{ vert2.x - vert1.x , vert2.y - vert1.y };

			// Now we have the normal/axis from A
			LB::Vec2<float> axis{ -vecB.y , vecB.x };

			axis = PHY_MATH::Normalize(axis);

			float minProjValueA{ 0.f };
			float maxProjValueA{ 0.f };

			float minProjValueB{ 0.f };
			float maxProjValueB{ 0.f };


			// Project all points onto the axis
			ProjectPointsOntoAxis(axis, verticesA, minProjValueA, maxProjValueA);
			ProjectPointsOntoAxis(axis, verticesB, minProjValueB, maxProjValueB);

			// Checks if we can find separation by checking if the
			// min projection values of either object is larger than the max
			// projection values of the other object
			if (maxProjValueA <= minProjValueB || maxProjValueB <= minProjValueA) {
				return false;
			}

			// We check for the depth here now based on knowing that
			// the two max values being larger than the two min values
			float axisDepth = PHY_MATH::FindMin(maxProjValueA - minProjValueB, maxProjValueB - minProjValueA);

			if (axisDepth < depth_out)
			{
				depth_out = axisDepth;
				normal_out = axis;
			}
		} // End of looping through obj B's edges

		// Finally the depth_out and normal_out are not normalized as
		// the axis we used is essentially the same as the edges found on
		// each of the objects
		depth_out /= PHY_MATH::Length(normal_out);
		normal_out = PHY_MATH::Normalize(normal_out);

		// Get the center of each obj's vertices
		LB::Vec2<float> vecCenterA = FindCenterOfBoxVertices(verticesA);
		LB::Vec2<float> vecCenterB = FindCenterOfBoxVertices(verticesB);

		LB::Vec2<float> vecAB = vecCenterB - vecCenterA;

		// Make sure the normal is pushing B from A
		// and not A from B
		if (PHY_MATH::DotProduct(vecAB, normal_out) < 0) {
			normal_out.x = -normal_out.x;
			normal_out.y = -normal_out.y;
		}

		return true;
	}


	/*!***********************************************************************
	  \brief
		Check collision for Circle-Box objects using Separating Axis Theorem
		- normal_out stores the direction of where the objects should be pushed
		towards, with normal_out is pushing B from A
		- depth_out stores the magnitude of how much the objects should be pushed
	  \return
	  Returns true if the objects collided and false if the objects did not collide
	*************************************************************************/
	bool CollisionIntersection_CircleBox_SAT(LB::Vec2<float> circleCenter, float circleRadius, std::vector<Vec2<float>> const& vertices, LB::Vec2<float>& normal_out, float& depth_out)
	{
		normal_out = LB::Vec2<float>{ 0.f, 0.f };
		depth_out = 100000000.f;

		LB::Vec2<float> axis{ 0.f, 0.f };

		float minProjValueA{ 0.f };
		float maxProjValueA{ 0.f };

		float minProjValueB{ 0.f };
		float maxProjValueB{ 0.f };

		float axisDepth{ 0.f };

		// Loop through each edge of box
		for (int i = 0; i < vertices.size(); ++i)
		{
			// Get two vertices from box
			LB::Vec2<float> vert1 = vertices[i];
			// We get the remainder of 4 as we do not want to loop out of the array
			// This also ensure that we get pt3 and pt0 as the pair of vertices which is right
			LB::Vec2<float> vert2 = vertices[(i + 1) % vertices.size()];

			// We now get the vector from 0 to 1 for example
			LB::Vec2<float> vecA{ vert2.x - vert1.x , vert2.y - vert1.y };

			// Now we have the normal/axis from box
			axis.x = -vecA.y;
			axis.y = vecA.x;

			axis = PHY_MATH::Normalize(axis);

			// Project all points onto the axis
			ProjectPointsOntoAxis(axis, vertices, minProjValueA, maxProjValueA);
			ProjectCircleOntoAxis(axis, circleCenter, circleRadius, minProjValueB, maxProjValueB);

			// Checks if we can find separation by checking if the
			// min projection values of either object is larger than the max
			// projection values of the other object
			if (maxProjValueA <= minProjValueB || maxProjValueB <= minProjValueA) {
				return false;
			}

			// We check for the depth here now based on knowing that
			// the two max values being larger than the two min values
			axisDepth = PHY_MATH::FindMin(maxProjValueA - minProjValueB, maxProjValueB - minProjValueA);

			if (axisDepth < depth_out)
			{
				depth_out = axisDepth;
				normal_out = axis;
			}

		} // End of looping through box's edges

		// Now we need to check the axis from the circle center to closest vertice to circle center

		// First we get the nearest VerticeIndex from the box to the circle's center
		int nearestVerticeIndex = FindIndexClosestPointOnBox(vertices, circleCenter);
		LB::Vec2<float> nearestVertice = vertices[nearestVerticeIndex];

		// Next we make a vector from the circle center to nearestVertice
		axis = { nearestVertice.x - circleCenter.x, nearestVertice.y - circleCenter.y };
		axis = PHY_MATH::Normalize(axis);

		// Project all points onto the axis
		ProjectPointsOntoAxis(axis, vertices, minProjValueA, maxProjValueA);
		ProjectCircleOntoAxis(axis, circleCenter, circleRadius, minProjValueB, maxProjValueB);

		// Checks if we can find separation by checking if the
		// min projection values of either object is larger than the max
		// projection values of the other object
		if (maxProjValueA <= minProjValueB || maxProjValueB <= minProjValueA) {
			return false;
		}

		// We check for the depth here now based on knowing that
		// the two max values being larger than the two min values
		axisDepth = PHY_MATH::FindMin(maxProjValueA - minProjValueB, maxProjValueB - minProjValueA);

		if (axisDepth < depth_out)
		{
			depth_out = axisDepth;
			normal_out = axis;
		}

		// Normalize the normal, and make depth_out the correct magnitude
		depth_out /= PHY_MATH::Length(normal_out);
		normal_out = PHY_MATH::Normalize(normal_out);

		// We can use the dot product of these two vectors
		// 0> is same direction, 0 is perpendicular, 0< is opposite direction

		// Get the center of each obj's vertices
		// Use center of the circle
		LB::Vec2<float> vecCenterBox = FindCenterOfBoxVertices(vertices);

		// Circle center to polygon center
		LB::Vec2<float> vecAB = vecCenterBox - circleCenter;

		// pushing B away from A
		// This normal is pushing Box away from Circle
		if (PHY_MATH::DotProduct(vecAB, normal_out) < 0) {
			normal_out.x = -normal_out.x;
			normal_out.y = -normal_out.y;
		}

		return true;
	}


	/*!***********************************************************************
	  \brief
		Used for SAT collision checks as a helper function to use an axis
		to project to and put all points of a Circle onto that point,
		calculating what is the min and max projection of the Vertices Array
		- minPtOnAxis is an in/out param to use in SAT checks
		- maxPtOnAxis is an in/out param to use in SAT checks
	  \return
	  Returns true if the boxes collided and false if the boxes did not collide
	*************************************************************************/
	void ProjectPointsOntoAxis(LB::Vec2<float> axisToProj, std::vector<Vec2<float>> const& verticesBody, float& minPtOnAxis, float& maxPtOnAxis)
	{
	// get some arbitrary min and max things to update
		minPtOnAxis = 100000000.f;
		maxPtOnAxis = -100000000.f;

		// Loop through the vertices on the body and project them on the axis
		for (int i = 0; i < verticesBody.size(); ++i)
		{
			float projection_val = PHY_MATH::DotProduct(verticesBody[i], axisToProj);

			if (projection_val < minPtOnAxis)
			{
				minPtOnAxis = projection_val;
			}
			if (projection_val > maxPtOnAxis)
			{
				maxPtOnAxis = projection_val;
			}
		}
	}


	/*!***********************************************************************
	  \brief
		Used for SAT collision checks as a helper function to use a
		- normal_out stores the direction of where the objects should be pushed
		towards, with normal_out is pushing B from A
		- depth_out stores the magnitude of how much the objects should be pushed
	  \return
	  Returns true if the boxes collided and false if the boxes did not collide
	*************************************************************************/
	void ProjectCircleOntoAxis(LB::Vec2<float> axisToProj, LB::Vec2<float> center, float radius, float& minPtOnAxis, float& maxPtOnAxis)
	{
		// Arbitrary min and max things to update
		minPtOnAxis = 100000000.f;
		maxPtOnAxis = -100000000.f;

		// Get the direction of the axis
		LB::Vec2<float> direction = PHY_MATH::Normalize(axisToProj);
		//Vec2<float> directionNegative{ -direction.x, -direction.y };
		// Use this axis direction to have the vector of magnitude radius
		LB::Vec2<float> directionMagnitudeRadius = direction * radius;

		// Grab the two points from the edge of the circle
		LB::Vec2<float> point1{ center.x + directionMagnitudeRadius.x, center.y + directionMagnitudeRadius.y };
		LB::Vec2<float> point2{ center.x - directionMagnitudeRadius.x, center.y - directionMagnitudeRadius.y };


		minPtOnAxis = PHY_MATH::DotProduct(point1, axisToProj);
		maxPtOnAxis = PHY_MATH::DotProduct(point2, axisToProj);

		// Double check if the min is actually the min and max is actually the max
		if (minPtOnAxis > maxPtOnAxis)
		{
			float temp = maxPtOnAxis;
			maxPtOnAxis = minPtOnAxis;
			minPtOnAxis = temp;
		}
	}

	/*!***********************************************************************
	  \brief
	  This function takes in an array of Vec2<float> that is the representation
	  of vertices of a polygon, this function then calculates where the center
	  of the given polygon is
	  \return
	  Returns the position of the center of the vertices as a Vec2<float>
	*************************************************************************/
	LB::Vec2<float> FindCenterOfBoxVertices(std::vector<Vec2<float>> const& vertices)
	{
		// Loop through all the vertices and add them together and
		// then divide by the number of vertices in the array
		float xSum{ 0.f };
		float ySum{ 0.f };

		for (int i = 0; i < vertices.size(); ++i)
		{
			xSum += vertices[i].x;
			ySum += vertices[i].y;
		}

		return LB::Vec2<float>{xSum / static_cast<float>(vertices.size()), ySum / static_cast<float>(vertices.size())};
	}

	/*!***********************************************************************
	  \brief
	  This function is a helper function to Circle-Box SAT collision checks
	  It takes in the box vertices and the center of the circle to find the
	  nearest vertice to the circle's center
	  \return
	  Returns the position of the center of the vertices as a Vec2<float>
	*************************************************************************/
	int FindIndexClosestPointOnBox(std::vector<Vec2<float>> const& vertices, LB::Vec2<float> center)
	{
		// Loop through the the vertices
		// Get the vec from vertex to center
		// Always keep the vec with the smallest length

		int arr_index = -1;
		float minDistance = 100000000.f;

		for (int i = 0; i < vertices.size(); ++i)
		{
			LB::Vec2<float> vec = vertices[i];
			float dx = vertices[i].x - center.x;
			float dy = vertices[i].y - center.y;
			float distance = sqrt(dx * dx + dy * dy);

			if (distance < minDistance)
			{
				minDistance = distance;
				arr_index = i;
			}
		}
		return arr_index;
	}
}