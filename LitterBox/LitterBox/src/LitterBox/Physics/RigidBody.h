
#pragma once

#include "LitterBox/Utils/Math.h"
#include "Collisions.h"
#include "LitterBox/Factory/Components.h"

struct AABB;

namespace LB
{

    class RigidBody{


    public:

        // Vectors here allow the user to know
        // the current and previous location of the object
        LB::Vec2<float> mPosition;
        LB::Vec2<float> mPrevPosition;

        // Vectors here allow the user to know
        // which direection the object is moving towards
        LB::Vec2<float> mVelocity;
        LB::Vec2<float> mAcceleration;

        float mRotation;
        float mRotationalVelocity;

    public:
        // These data will not change will be set upon initialization
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
        LB::Vec2<float> mVertices[4];
        LB::Vec2<float> mTransformedVertices[4];

        AABB obj_aabb;
        // =====
        // PLS WRAP
        // =====

        bool mUpdateVerticesRequired;
        bool mUpdateAABBRequired;

        // =================
        // Constructor
        // =================
        RigidBody(LB::Vec2<float> position = LB::Vec2<float>{ 0.f,0.f },
            LB::Vec2<float> prevposition = LB::Vec2<float>{ 0.f, 0.f },
            LB::Vec2<float> velocity = LB::Vec2<float>{ 0.f, 0.f },
            LB::Vec2<float> acceleration = LB::Vec2<float>{ 0.f, 0.f },
            float rotation = 0.0f, float rotationvelocity = 0.0f, float density = 0.0f, float mass = 0.0f, float invmass = 0.0f, float restitution = 0.0f,
            float area = 0.0f, float friction = 0.0f, bool isstatic = false, bool isactive = false, float radius = 0.0f, float width = 0.0f, float height = 0.0f,
            SHAPETYPE shape = SHAPETYPE::NONE);


    public:

        LB::Vec2<float> getPos();

        // Force is added upon object's velocity
        void addForce(LB::Vec2<float> force);

        // Just to move the RigidBody with a vector
        void Move(LB::Vec2<float> vec);

        // Move the RigidBody to a specific position
        void MoveTo(LB::Vec2<float> position);

        // Updates the TransformedVertices of the RigidBody
        // Accounts for positional changes and rotational changes
        void UpdateRigidBodyBoxVertices();

        void UpdateRigidBodyAABB();

        // Updates the position of the RigidBody
        void UpdateRigidBodyPos(float time);

        // Updates the velocities of the RigidBody
        void UpdateRigidBodyVel(float time);

        // Updates the RigidBody with all the steps
        // Using a semi-implicit euler system, updating velocity first
        // then updating positions
        // Side note: Might be impulsed based system instead
        // therefore accel = force / mass should be introduced later
        void BodyStep(float time);

    };   // End of RigidBody class

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
