
#pragma once

#include "Litterbox/Utils/Math.h"
#include "PhysicsMath.h"
#include "Collisions.h"
#include <cmath>

enum SHAPETYPE
{
    CIRCLE = 0,
    BOX = 1,
    NONE = 2
};

class RigidBody {


public:

    // Vectors here allow the user to know
    // the current and previous location of the object
    Vec2<float> mPosition;
    Vec2<float> mPrevPosition;

    // Vectors here allow the user to know
    // which direection the object is moving towards
    Vec2<float> mVelocity;
    Vec2<float> mAcceleration;

public:
    // These data will not change will be set upon initialization
    float mRotation;
    float mRotationalVelocity;

    float mDensity;
    float mMass; // Used for F = MA
    float mInvMass;
    float mRestitution; // How bouncy the object is
    float mArea;

    float mFriction;

    bool isStatic;
    bool isActive;

    // For storing data for circles or rectangles
    float mRadius;
    float mWidth;
    float mHeight;

    SHAPETYPE mShapeType;

    // Vertices and AABB

    // Untransformed vertices (Vertices from origin)
    Vec2<float> vertices[4];

    AABB aabb;

    // =================
    // Constructor
    // =================
    RigidBody()
    {
        this->mPosition = Vec2<float>(0, 0);
        this->mPrevPosition = Vec2<float>(0, 0);

        this->mVelocity = Vec2<float>(0, 0);
        this->mAcceleration = Vec2<float>(0, 0);

        this->mRotation = 0.f;
        this->mRotationalVelocity = 0.f;

        this->mDensity = 0.f;
        this->mMass = 0.f;
        this->mInvMass = 0.f;
        this->mRestitution = 0.f;
        this->mArea = 0.f;

        this->mFriction = 0.f;
        this->isActive = 0.f;

        this->mRadius = 0.f;
        this->mWidth = 0.f;
        this->mHeight = 0.f;

        this->mShapeType = NONE;
    }

public:
    
    // Force is added upon object's velocity
    void addForce(Vec2<float> force)
    {
        this->mVelocity += force;
    }
    

    // Just to move the RigidBody with a vector
    void Move(Vec2<float> vec)
    {
        this->mPosition += vec;
    }

    // Move the RigidBody to a specific position
    void MoveTo(Vec2<float> position)
    {
        this->mPosition = position;
    }


};   // End of RigidBody class


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
