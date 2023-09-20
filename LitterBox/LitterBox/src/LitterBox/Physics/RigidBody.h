
#pragma once

#include "Math.h"
#include "Collisions.h"

struct AABB;

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

    // =====
    //  WRAP THIS SECTION IN A COLLIDER CLASS, separating from RigidBody
    // =====
    // Vertices and AABB

    // Untransformed vertices (Vertices from origin)
    Vec2<float> mVertices[4];
    Vec2<float> mTransformedVertices[4];

    AABB obj_aabb;
    // =====
    // PLS WRAP
    // =====

    bool mUpdateVerticesRequired;
    bool mUpdateAABBRequired;

    // =================
    // Constructor
    // =================
    RigidBody(Vec2<float> position, Vec2<float> prevposition, Vec2<float> velocity, Vec2<float> acceleration,
        float rotation, float rotationvelocity, float density, float mass, float invmass, float restitution,
        float area, float friction, bool isstatic, bool isactive, float radius, float width, float height,
        SHAPETYPE shape);
    

public:
    
    // Force is added upon object's velocity
    void addForce(Vec2<float> force);
    
    // Just to move the RigidBody with a vector
    void Move(Vec2<float> vec);

    // Move the RigidBody to a specific position
    void MoveTo(Vec2<float> position);

    // Updates the TransformedVertices of the RigidBody
    // Accounts for positional changes and rotational changes
    void UpdateRigidBodyBoxVertices();

    void UpdateRigidBodyAABB();

    // Updates the position of the RigidBody
    void UpdateRigidBodyPos(float time);

    // Updates the velocities of the RigidBody
    void UpdateRigidBodyVel(float time);

};   // End of RigidBody class

void CreateBoxVertices(Vec2<float>* vertices_arr, float width, float height);

void CreateBoxVerticesTransformed(Vec2<float>* vertices_arr, Vec2<float> position);



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
