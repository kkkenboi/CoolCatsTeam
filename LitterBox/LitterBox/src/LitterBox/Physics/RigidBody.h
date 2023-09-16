
#pragma once

#include "Litterbox/Utils/Math.h"
#include "PhysicsMath.h"
#include "Collisions.h"
#include <cmath>

class RigidBody {
    
    enum SHAPETYPE
    {
        CIRCLE = 0,
        BOX = 1
    };
    

    private:

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
    Vec2<float> vertices[4];
    AABB aabb;
    
    // =================
    // Constructor
    // =================
    RigidBody(Vec2<float> position, float density, float mass,
        float restitution, float area, float friction, bool is_active,
        float radius, float width, float height, SHAPETYPE shapeType) 
    {
        this->mPosition = position;
        this->mPrevPosition= Vec2<float> (0,0);

        this->mVelocity = Vec2<float> (0,0);
        this->mAcceleration = Vec2<float> (0,0);

        this->mRotation = 0.f;
        this->mRotationalVelocity = 0.f;

        this->mDensity = density;
        this->mMass = mass;
        this->mRestitution = restitution;
        this->mArea = area;

        this->mFriction = friction;
        this->isActive = is_active;

        this->mRadius = radius;
        this->mWidth = width;
        this->mHeight = height;

        this->mShapeType = shapeType;
    }
    
    public:
    /*
    // Force is added upon object's velocity
    void addForce(Vec2<float> force) 
    {
        this->mVelocity += force;
    }
    */

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


}   // End of RigidBody class


// Create a circle rigid body
RigidBody CreateCircleBody()
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
