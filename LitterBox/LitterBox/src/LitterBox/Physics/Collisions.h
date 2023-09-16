#pragma once

#include "Litterbox/Physics/PhysicsMath.h"

struct AABB {
    Vec2<float> c; // Center of the AABB

    Vec2<float> m_min;
    Vec2<float> m_max;
};

struct LineSegment
{
	Vec2<float>	m_pt0;
	Vec2<float>	m_pt1;
	Vec2<float>	m_normal;
};

void BuildLineSegment(LineSegment& lineSegment, Vec2<float> vel1, Vec2<float> vel2);

bool CollisionIntersection_RectRect(const AABB & aabb1, const Vec2<float> & vel1, 
									const AABB & aabb2, const Vec2<float> & vel2);

bool CollisionIntersection_CircleCircle(Vec2<float> centerA, Vec2<float> centerB, float radiusA, float radiusB);