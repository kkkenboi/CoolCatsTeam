#pragma once

#include "LitterBox/Utils/Math.h"

enum SHAPETYPE
{
	CIRCLE = 0,
	BOX = 1,
	NONE = 2
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

struct PhysicsTransform 
{
	float m_posX;
	float m_posY;
	float m_sin;
	float m_cos;

	PhysicsTransform(LB::Vec2<float> position, float angle);
};


class Collider {
public:

private:
	SHAPETYPE m_shape;

	LB::Vec2<float> m_pos;

	float m_length;
	float m_height;
	float m_radius;

	AABB m_aabb;

	// For SAT
	float m_rotation;
	int m_vertAmount;
	LB::Vec2<float> m_untransformedVerts[4];
	LB::Vec2<float> m_transformedVerts[4];

	// Collider class functions
	Collider(SHAPETYPE shape = NONE, LB::Vec2<float> pos = LB::Vec2<float>{ 0.f,0.f },
		float length = 0.f, float height = 0.f, float radius = 0.f);

	void CreateAABB();

	void CreatePolygon();
};

// ===============================
// Collision Intersection Checks
// ===============================

bool CollisionIntersection_BoxBox(const AABB & aabb1, const LB::Vec2<float> & vel1, 
									const AABB & aabb2, const LB::Vec2<float> & vel2, float dt);

bool CollisionIntersection_CircleCircle(LB::Vec2<float> centerA, LB::Vec2<float> centerB, float radiusA, float radiusB, LB::Vec2<float> normal_out, float depth_out);

bool CollisionIntersection_BoxBox_SAT(LB::Vec2<float>* verticesA, LB::Vec2<float>* verticesB, LB::Vec2<float> normal_out, float depth_out);

bool CollisionIntersection_CircleBox_SAT(LB::Vec2<float> circleCenter, float circleRadius, LB::Vec2<float>* verticesBox, LB::Vec2<float> normal_out, float depth_out);

void ProjectPointsOntoAxis(LB::Vec2<float> axisToProj, LB::Vec2<float>* verticesBody, float minPtOnAxis, float maxPtOnAxis);

void ProjectCircleOntoAxis(LB::Vec2<float> axisToProj, LB::Vec2<float> center, float radius, float min, float max);

LB::Vec2<float> FindCenterOfBoxVertices(LB::Vec2<float>* vertices);

int FindIndexClosestPointOnBox(LB::Vec2<float>* vertices, LB::Vec2<float> center);