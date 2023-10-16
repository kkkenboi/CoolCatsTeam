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

#include "LitterBox/Utils/Math.h"
#include "LitterBox/Components/ColliderComponent.h"

namespace LB
{

	struct PhysicsTransform
	{
		float m_posX;
		float m_posY;
		float m_sin;
		float m_cos;

		/*!***********************************************************************
		  \brief
		  Constructor for the PhysicsTransform struct, to be used for rotational
		*************************************************************************/
		PhysicsTransform(LB::Vec2<float> position, float angle);
	};

	// ========================================================
	// Collision Intersection Checks
	// ========================================================

	/*!***********************************************************************
	  \brief
	  This function does Box-Box collision using Axis-Aligned Bounded Boxes
	  \return
	  Returns true if boxes collide and false is boxes do not collide,
	  normal_out and depth_out are used for collision resolution
	*************************************************************************/
	bool CollisionIntersection_BoxBox(const AABB& aabb1, const LB::Vec2<float>& vel1,
		const AABB& aabb2, const LB::Vec2<float>& vel2, float dt,
		LB::Vec2<float>& normal_out, float& depth_out);

	/*!***********************************************************************
	  \brief
		Checks if there is collision between 2 circles,
		- normal_out stores the direction of where the objects should be pushed
		towards, with normal_out is pushing B from A
		- depth_out stores the magnitude of how much the objects should be pushed
	  \return
	  Returns true if the circles collided and false if the circles did not collide
	*************************************************************************/
	bool CollisionIntersection_CircleCircle(LB::Vec2<float> centerA, LB::Vec2<float> centerB, float radiusA, float radiusB, LB::Vec2<float>& normal_out, float& depth_out);

	/*!***********************************************************************
	  \brief
		Check collision for Box-Box objects using Separating Axis Theorem
		- normal_out stores the direction of where the objects should be pushed
		towards, with normal_out is pushing B from A
		- depth_out stores the magnitude of how much the objects should be pushed
	  \return
	  Returns true if the boxes collided and false if the boxes did not collide
	*************************************************************************/
	bool CollisionIntersection_BoxBox_SAT(LB::Vec2<float>* verticesA, LB::Vec2<float>* verticesB, LB::Vec2<float>& normal_out, float& depth_out);


	/*!***********************************************************************
	  \brief
		Check collision for Circle-Box objects using Separating Axis Theorem
		- normal_out stores the direction of where the objects should be pushed
		towards, with normal_out is pushing B from A
		- depth_out stores the magnitude of how much the objects should be pushed
	  \return
	  Returns true if the objects collided and false if the objects did not collide
	*************************************************************************/
	bool CollisionIntersection_CircleBox_SAT(LB::Vec2<float> circleCenter, float circleRadius, LB::Vec2<float>* verticesBox, LB::Vec2<float>& normal_out, float& depth_out);

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
	void ProjectPointsOntoAxis(LB::Vec2<float> axisToProj, LB::Vec2<float>* verticesBody, float& minPtOnAxis, float& maxPtOnAxis);

	/*!***********************************************************************
	  \brief
		Used for SAT collision checks as a helper function to use a
		- normal_out stores the direction of where the objects should be pushed
		towards, with normal_out is pushing B from A
		- depth_out stores the magnitude of how much the objects should be pushed
	  \return
	  Returns true if the boxes collided and false if the boxes did not collide
	*************************************************************************/
	void ProjectCircleOntoAxis(LB::Vec2<float> axisToProj, LB::Vec2<float> center, float radius, float& min, float& max);


	/*!***********************************************************************
	  \brief
	  This function takes in an array of Vec2<float> that is the representation
	  of vertices of a polygon, this function then calculates where the center
	  of the given polygon is
	  \return
	  Returns the position of the center of the vertices as a Vec2<float>
	*************************************************************************/
	LB::Vec2<float> FindCenterOfBoxVertices(LB::Vec2<float>* vertices);

	/*!***********************************************************************
	  \brief
	  This function is a helper function to Circle-Box SAT collision checks
	  It takes in the box vertices and the center of the circle to find the
	  nearest vertice to the circle's center
	  \return
	  Returns the position of the center of the vertices as a Vec2<float>
	*************************************************************************/
	int FindIndexClosestPointOnBox(LB::Vec2<float>* vertices, LB::Vec2<float> center);
}