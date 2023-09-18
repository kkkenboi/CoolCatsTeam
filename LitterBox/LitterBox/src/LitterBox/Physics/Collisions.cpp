#pragma once

#include "Collisions.h"
#include "Litterbox/Utils/Math.h"
#include "PhysicsMath.h"

void BuildLineSegment(LineSegment& lineSegment, Vec2<float> p0, Vec2<float> p1) {
	lineSegment.m_pt0 = p0;
	lineSegment.m_pt1 = p1;

	// Unnormalized vector
	lineSegment.m_normal = Vec2<float>(p1.y - p0.y, -(p1.x - p0.x));
	// Normalized vector
	lineSegment.m_normal = PHY_MATH::Normalize(lineSegment.m_normal);
}

bool CollisionIntersection_BoxBox(const AABB & aabb1, const Vec2<float> & vel1, 
									const AABB & aabb2, const Vec2<float> & vel2)
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

	Vec2<float> relVel;
	Vec2<float> Vel1, Vel2;
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
	float tLast_x = GET FRAME TIME HERE!!!!!;

	float tFirst_y = 0.f;
	float tLast_y = GET FRAME TIME HERE!!!!!;

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
bool CollisionIntersection_CircleCircle(Vec2<float> centerA, Vec2<float> centerB, float radiusA, float radiusB, Vec2<float>& normal_out, float& depth_out) {
	normal_out = Vec2<float>(0.f , 0.f);
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