
#pragma once

#include "LitterBox/Utils/Math.h"
#include "Collisions.h"
#include "LitterBox/Factory/Components.h"

struct AABB;

namespace LB
{
  // End of RigidBody class

    void CreateBoxVertices(LB::Vec2<float>* vertices_arr, float width, float height);

    void CreateBoxVerticesTransformed(LB::Vec2<float>* vertices_arr, LB::Vec2<float> position);

}

/*
// Create a circle rigid body
void CreateCircleBody()
{   
    // Should add to the RigidBodyPool

    this->mArea = mRadius * mRadius * M_PI;
    // Mass = Area * Depth * Density, Depth is 1 as its 2D
    this->mMass = mArea * mDensity;

    this->mRestitution = PHY_MATH::Clamp(mRestitution, 0.f, 1.f);
    this->mShapeType = SHAPETYPE::CIRCLE;
}


void CreateBoxBody()
{
    this->mArea = mWidth * mHeight;
    // Mass = Area * Depth * Density, Depth is 1 as its 2D
    this->mMass = mArea * mDensity;

    this->mRestitution = PHY_MATH::Clamp(mRestitution, 0.f, 1.f);
    this->mShapeType = SHAPETYPE::BOX;
}
*/
