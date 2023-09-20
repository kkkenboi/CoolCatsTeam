#pragma once

#include "Math.h"

struct AABB 
{
    Vec2<float> m_c; // Center of the AABB

    Vec2<float> m_min;
    Vec2<float> m_max;
};

struct LineSegment
{
	Vec2<float>	m_pt0;
	Vec2<float>	m_pt1;
	Vec2<float>	m_normal;
};

struct PhysicsTransform 
{
	float m_posX;
	float m_posY;
	float m_sin;
	float m_cos;

	PhysicsTransform(Vec2<float> position, float angle);
};


bool CollisionIntersection_BoxBox(const AABB & aabb1, const Vec2<float> & vel1, 
									const AABB & aabb2, const Vec2<float> & vel2);

bool CollisionIntersection_CircleCircle(Vec2<float> centerA, Vec2<float> centerB, float radiusA, float radiusB, Vec2<float>& normal_out, float& depth_out);

bool CollisionIntersection_BoxBox_SAT(Vec2<float>* verticesA, Vec2<float>* verticesB, Vec2<float> normal_out, float depth_out);

bool CollisionIntersection_CircleBox_SAT(Vec2<float> circleCenter, float circleRadius, Vec2<float>* verticesBox, Vec2<float> normal_out, float depth_out);

void ProjectPointsOntoAxis(Vec2<float> axisToProj, Vec2<float>* verticesBody, float minPtOnAxis, float maxPtOnAxis);

void ProjectCircleOntoAxis(Vec2<float> axisToProj, Vec2<float> center, float radius, float min, float max);

Vec2<float> FindCenterOfBoxVertices(Vec2<float>* vertices);

int FindIndexClosestPointOnBox(Vec2<float>* vertices, Vec2<float> center);