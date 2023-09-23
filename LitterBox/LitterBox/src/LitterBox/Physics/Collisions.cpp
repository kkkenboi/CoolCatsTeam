#pragma once

#include "Collisions.h"
#include "PhysicsMath.h"
#include <cmath>

PhysicsTransform::PhysicsTransform(LB::Vec2<float> position, float angle) 
{
	this->m_posX = position.x;
	this->m_posY = position.y;

	this->m_sin = sin(angle);
	this->m_cos = cos(angle);
}

bool CollisionIntersection_BoxBox(const AABB & aabb1, const LB::Vec2<float> & vel1, 
									const AABB & aabb2, const LB::Vec2<float> & vel2, float dt)
{	
	/*Implement the collision intersection over here.

	The steps are:	
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
			float tTemp_x = (aabb1.m_max.x - aabb2.m_min.x) / relVel.x ;
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

		if(aabb1.m_max.x > aabb2.m_min.x) {
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
	
	return 1;
}

// Check if there is collision between 2 circles,
// normal stores the direction of where the objects should be pushed towards
// depth stores the magnitude of how much the objects should be pushed
bool CollisionIntersection_CircleCircle(LB::Vec2<float> centerA, LB::Vec2<float> centerB, float radiusA, float radiusB, LB::Vec2<float> normal_out, float depth_out) {
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

// normal_out is pushing boxB from boxA
bool CollisionIntersection_BoxBox_SAT(LB::Vec2<float>* verticesA, LB::Vec2<float>* verticesB, LB::Vec2<float> normal_out, float depth_out) 
{
	normal_out = LB::Vec2<float>{ 0.f,0.f };
	depth_out = 100000.f;

	// Loop through each edge of obj A
	for (int i = 0; i < 4; ++i) 
	{
		// Get two vertices from obj A
		LB::Vec2<float> vert1 = verticesA[i];
		// We get the remainder of 4 as we do not want to loop out of the array
		// This also ensure that we get pt3 and pt0 as the pair of vertices which is right
		LB::Vec2<float> vert2 = verticesA[(i + 1) % 4];

		// We now get the vector from 0 to 1 for example
		LB::Vec2<float> vecA{ vert2.x - vert1.x , vert2.y - vert1.y};

		// Now we have the normal/axis from A
		LB::Vec2<float> axis{-vecA.y , vecA.x};


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

	// Loop through each edge of obj B
	for (int i = 0; i < 4; ++i) 
	{
		// Get two vertices from obj B
		LB::Vec2<float> vert1 = verticesB[i];
		LB::Vec2<float> vert2 = verticesB[(i + 1) % 4];

		// Now we get the vector from vertice 0 to 1 for example
		LB::Vec2<float> vecB{ vert2.x - vert1.x, vert2.y - vert1.y };

		// Now we have the normal/axis from B
		LB::Vec2<float> axis{ -vecB.y, vecB.x };


		float minProjValueA{ 0.f };
		float maxProjValueA{ 0.f };

		float minProjValueB{ 0.f };
		float maxProjValueB{ 0.f };

		// Project all points onto the axis
		ProjectPointsOntoAxis(axis, verticesA, minProjValueA, maxProjValueA);
		ProjectPointsOntoAxis(axis, verticesB, minProjValueB, maxProjValueB);

		if (maxProjValueA <= minProjValueB || maxProjValueB <= minProjValueA)
		{
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
	} // End of looping through of obj B's edges

	// Finally the depth_out and normal_out are not normalized as
	// the axis we used is essentially the same as the edges found on
	// each of the objects, therefore we need to normalize the final
	// depth_out and normal_out
	depth_out /= PHY_MATH::Length(normal_out);
	normal_out = PHY_MATH::Normalize(normal_out);

	// However, sometimes the normal is not facing the direction that we want to
	// push the bodies towrds, normally we want the normal to push obj B
	// then push obj A by the -normal direction

	// We can mitigate this by find the center of obj A and obj B and make a vector
	// from A's center to B's center and compare that vector to the normal

	// We can use the dot product of these two vectors
	// 0> is same direction, 0 is perpendicular, 0< is opposite direction

	// Get the center of each obj's vertices
	LB::Vec2<float> vecCenterA = FindCenterOfBoxVertices(verticesA);
	LB::Vec2<float> vecCenterB = FindCenterOfBoxVertices(verticesB);

	LB::Vec2<float> vecAB = vecCenterB - vecCenterA;

	// This normal is pushing B from A
	if (PHY_MATH::DotProduct(vecAB, normal_out) < 0) {
		normal_out.x = -normal_out.x;
		normal_out.y = -normal_out.y;
	}

	return true;
}

// For CircleBox SAT Collision, we need to check instead of the edges of the
// circle, we need to check only 1 more axis after checking all the axis of
// the given box, we need to check the axis of the circle's center 

// normal_out is pushing the Box away from the Circle
bool CollisionIntersection_CircleBox_SAT(LB::Vec2<float> circleCenter, float circleRadius, LB::Vec2<float>* verticesBox, LB::Vec2<float> normal_out, float depth_out)
{
	normal_out = LB::Vec2<float>{ 0.f, 0.f };
	depth_out = 100000.f;

	LB::Vec2<float> axis{ 0.f, 0.f };

	float minProjValueA{ 0.f };
	float maxProjValueA{ 0.f };

	float minProjValueB{ 0.f };
	float maxProjValueB{ 0.f };

	float axisDepth{ 0.f };

	// Loop through each edge of box
	for (int i = 0; i < 4; ++i)
	{
		// Get two vertices from box
		LB::Vec2<float> vert1 = verticesBox[i];
		// We get the remainder of 4 as we do not want to loop out of the array
		// This also ensure that we get pt3 and pt0 as the pair of vertices which is right
		LB::Vec2<float> vert2 = verticesBox[(i + 1) % 4];

		// We now get the vector from 0 to 1 for example
		LB::Vec2<float> vecA{ vert2.x - vert1.x , vert2.y - vert1.y };

		// Now we have the normal/axis from box
		axis.x = -vecA.y;
		axis.y = vecA.x;


		// Project all points onto the axis
		ProjectPointsOntoAxis(axis, verticesBox, minProjValueA, maxProjValueA);
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
	int nearestVerticeIndex = FindIndexClosestPointOnBox(verticesBox, circleCenter);
	LB::Vec2<float> nearestVertice = verticesBox[nearestVerticeIndex];

	// Next we make a vector from the circle center to nearestVertice
	axis = { nearestVertice.x - circleCenter.x, nearestVertice.y - circleCenter.y };

	// Project all points onto the axis
	ProjectPointsOntoAxis(axis, verticesBox, minProjValueA, maxProjValueA);
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

	// Finally the depth_out and normal_out are not normalized as
	// the axis we used is essentially the same as the edges found on
	// each of the objects, therefore we need to normalize the final
	// depth_out and normal_out
	depth_out /= PHY_MATH::Length(normal_out);
	normal_out = PHY_MATH::Normalize(normal_out);

	// We can use the dot product of these two vectors
	// 0> is same direction, 0 is perpendicular, 0< is opposite direction

	// Get the center of each obj's vertices
	// Use center of the circle
	LB::Vec2<float> vecCenterBox = FindCenterOfBoxVertices(verticesBox);

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

void ProjectPointsOntoAxis(LB::Vec2<float> axisToProj, LB::Vec2<float>*verticesBody, float minPtOnAxis, float maxPtOnAxis) {
	// get some arbitrary min and max things to update
	minPtOnAxis = 100000.f;
	maxPtOnAxis = -100000.f;

	// Loop through the vertices on the body and project them on the axis
	for (int i = 0; i < 4; ++i) 
	{
		float projection_val = PHY_MATH::DotProduct(axisToProj, verticesBody[i]);

		// If the projection value is less than min
		// or more than max change the min or max
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

void ProjectCircleOntoAxis(LB::Vec2<float> axisToProj, LB::Vec2<float> center, float radius, float minPtOnAxis, float maxPtOnAxis)
{
	// Arbitrary min and max things to update
	minPtOnAxis = 100000.f;
	maxPtOnAxis = -100000.f;

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

LB::Vec2<float> FindCenterOfBoxVertices(LB::Vec2<float>* vertices) 
{
	// Loop through all the vertices and add them together and
	// then divide by the number of vertices in the array
	float xSum{ 0.f };
	float ySum{ 0.f };

	for (int i = 0; i < 4; ++i) 
	{
		xSum += vertices[i].x;
		ySum += vertices[i].y;
	}

	return LB::Vec2<float>{xSum / 4, ySum / 4};
}

int FindIndexClosestPointOnBox(LB::Vec2<float>* vertices, LB::Vec2<float> center)
{

	// Loop through the the vertices
	// Get the vec from vertex to center
	// Always keep the vec with the smallest length

	int arr_index = -1;
	float minDistance = 100000.f;

	for (int i = 0; i < 4; ++i)
	{
		LB::Vec2<float> vec = vertices[i];
		float distance = PHY_MATH::Length(vec);

		if (distance < minDistance)
		{
			minDistance = distance;
			arr_index = i;
		}
	}
	return arr_index;
}