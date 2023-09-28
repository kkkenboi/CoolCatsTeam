/*!************************************************************************
 \file                Math.h
 \author(s)           Vanessa Chua Siew Jin, Carlo Villa Ilao Justine
 \par DP email(s):    vanessasiewjin.chua@digipen.edu, justine.c@digipen.edu
 \par Course:         CSD2401A
 \date                24-09-2023
 \brief
 The functions in the Math class include:

**************************************************************************/

#include "RigidBody.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/Physics/RigidBodyManager.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Physics/PhysicsMath.h"

//TEST
#include <string>
#include <iostream>

namespace LB
{
    //int CPRigidBody::mNumberID = -1;

    void CPRigidBody::Start(LB::Vec2<float> position, LB::Vec2<float> prevposition, LB::Vec2<float> velocity, LB::Vec2<float> acceleration,
        float rotation, float rotationvelocity, float density, float mass, float invmass, float restitution,
        float area, float friction, bool isstatic, bool isactive, float radius, float width, float height,
        SHAPETYPE shape)
    {
        transform = gameObj->GetComponent<CPTransform>("CPTransform");
        
        this->mPosition = position;
        this->mPrevPosition = prevposition;

        this->mPosition = transform->GetPosition();
        this->mPrevPosition = mPosition;

        this->mVelocity = velocity;
        this->mAcceleration = acceleration;

        this->mRotation = rotation;
        this->mRotationalVelocity = rotationvelocity;

        this->mDensity = density;
        this->mMass = mass;
        this->mInvMass = invmass;
        this->mRestitution = restitution;
        this->mArea = area;

        this->mFriction = friction;
        this->isStatic = isstatic;
        this->isActive = isactive;

        this->mRadius = radius;
        this->mWidth = width;
        this->mHeight = height;
      
        this->mShapeType = shape;

           // TEST
        this->isStatic = false;
        this->mWidth = 100.f;
        this->mHeight = 100.f;
        this->mShapeType = BOX;
        this->mVelocity = LB::Vec2<float>{ 0.f, 0.f };
        this->mAcceleration = LB::Vec2<float>{ 0.f, 0.f };
        this->mFriction = 0.79f;
        this->mMass = 10.f;
        this->mDensity = 10.f;
        this->mNumberID = 0;

        this->mArea = this->mWidth * this->mHeight;
        this->mMass = this->mArea * this->mDensity;

        // Check if static and update the InvMass
        if (!this->isStatic)
        {
            this->mInvMass = 1.f / this->mMass;
        }
        else
        {
            this->mInvMass = 0.f;
        }

        // Check if Box
        if (this->mShapeType == BOX)
        {
            // If it is a box then create the vertices
            // as well as the AABB data
            obj_aabb.m_c = this->mPosition;
            // Max is top right
            obj_aabb.m_max.x = this->mWidth / 2 + this->mPosition.x;
            obj_aabb.m_max.y = this->mHeight / 2 + this->mPosition.y;
            // Min is bottom right
            obj_aabb.m_min.x = this->mPosition.x - this->mWidth / 2;
            obj_aabb.m_min.y = this->mPosition.y - this->mHeight / 2;

            // Create the vertices
            // Untransformed
            CreateBoxVertices(this->mVertices, this->mWidth, this->mHeight);
            // Transformed
            this->UpdateRigidBodyBoxVertices();

        }
        else
        {
            LB::Vec2<float> zeroed;
            zeroed.x = 0.f;
            zeroed.y = 0.f;

            obj_aabb.m_c = zeroed;

            obj_aabb.m_min = zeroed;

            obj_aabb.m_max = zeroed;


            mVertices[0] = zeroed;
            mVertices[1] = zeroed;
            mVertices[2] = zeroed;
            mVertices[3] = zeroed;

            mTransformedVertices[0] = zeroed;
            mTransformedVertices[1] = zeroed;
            mTransformedVertices[2] = zeroed;
            mTransformedVertices[3] = zeroed;

        }

        PHYSICS->AddRigidBodyToPool(this);
    }

    LB::Vec2<float> CPRigidBody::getPos() 
    {
        return mPosition;
    }

    void CPRigidBody::addForce(LB::Vec2<float> force)
    {
        this->mVelocity += force;
    }

    void CPRigidBody::addImpulse(LB::Vec2<float> force)
    {
        this->mAcceleration += force;
    }

    void CPRigidBody::Move(LB::Vec2<float> vec)
    {
        this->mPosition += vec;
        transform->SetPosition(mPosition);
    }

    void CPRigidBody::MoveTo(LB::Vec2<float> position)
    {
        this->mPosition = position;
        transform->SetPosition(mPosition);
    }

    void CPRigidBody::UpdateRigidBodyBoxVertices()
    {
        PhysicsTransform transform{ this->mPosition, this->mRotation };

        for (int i = 0; i < 4; ++i) {
            // Uses the untransformed vertices as the basis for tranasformation
            LB::Vec2<float> og_vec = this->mVertices[i];
            // Transforming the vertices using trigo formulas
            this->mTransformedVertices[i] = LB::Vec2<float>{
                transform.m_cos * og_vec.x - transform.m_sin * og_vec.y + transform.m_posX,
                transform.m_sin * og_vec.x + transform.m_cos * og_vec.y + transform.m_posY };
            //std::cout << this->mTransformedVertices[i].x << " Verticle X" << std::endl;
            //std::cout << this->mTransformedVertices[i].y << " Verticle Y" << std::endl;
        }
        //std::cout << "untranspt0: " << this->mVertices[0].x << " , " << this->mVertices[0].y << std::endl;
        //std::cout << "untranspt1: " << this->mVertices[1].x << " , " << this->mVertices[1].y << std::endl;
        //std::cout << "untranspt2: " << this->mVertices[2].x << " , " << this->mVertices[2].y << std::endl;
        //std::cout << "untranspt3: " << this->mVertices[3].x << " , " << this->mVertices[3].y << std::endl;
        //std::cout << "transformed" << std::endl;
        //std::cout << "pt0: " << this->mTransformedVertices[0].x << " , " << this->mTransformedVertices[0].y << std::endl;
        //std::cout << "pt1: " << this->mTransformedVertices[1].x << " , " << this->mTransformedVertices[1].y << std::endl;
        //std::cout << "pt2: " << this->mTransformedVertices[2].x << " , " << this->mTransformedVertices[2].y << std::endl;
        //std::cout << "pt3: " << this->mTransformedVertices[3].x << " , " << this->mTransformedVertices[3].y << std::endl;



    }

    void CPRigidBody::UpdateRigidBodyAABB()
    {
        float minX = 10000000.f;
        float maxX = -10000000.f;
        float minY = 10000000.f;
        float maxY = -10000000.f;

        if (this->mShapeType == BOX)
        {
            for (int i = 0; i < 4; ++i)
            {
                // Take the Transformed Vertices and use it as the new AABB
                LB::Vec2<float> vec = this->mTransformedVertices[i];

                if (vec.x < minX) minX = vec.x;
                if (vec.x > maxX) maxX = vec.x;
                if (vec.y < minY) minY = vec.y;
                if (vec.y > maxY) maxY = vec.y;
            }
        }
        else if (this->mShapeType == CIRCLE)
        {
            // Basically grab the position and make a box using radius as the width and height
            // of the box
            minX = this->mPosition.x - this->mRadius;
            maxX = this->mPosition.x + this->mRadius;
            minY = this->mPosition.y - this->mRadius;
            maxY = this->mPosition.y + this->mRadius;
        }

        this->obj_aabb.m_c = LB::Vec2<float>{ (minX + maxX) / 2.f, (minY + maxY) / 2.f };
        this->obj_aabb.m_max = LB::Vec2<float>{ maxX, maxY };
        this->obj_aabb.m_min = LB::Vec2<float>{ minX, minY };
    }

    void CPRigidBody::UpdateRigidBodyPos(float time)
    {
        // If static do not move or update the position of the RigidBody
        if (this->isStatic)
        {
            return;
        }

        this->mPosition += this->mVelocity * time;
        transform->SetPosition(mPosition);

        //std::cout << mPosition.ToString() << "\n";

        this->mRotation += this->mRotationalVelocity * time;
        transform->SetRotation(mRotation);


        // Update the TransformedVertices
        // HERE
        this->UpdateRigidBodyBoxVertices();

        // Update the AABB
        // HERE
        this->UpdateRigidBodyAABB();


        // Debug draw box
        if (this->mShapeType == BOX)
        {
            //DEBUG->DrawBox(this->mPosition, this->mWidth, this->mHeight, LB::Vec4<float>{0.f, 0.f, 1.f, 0.f});
            // Debug draw velocity from center
            //DEBUG->DrawLine(this->mPosition, this->mVelocity, LB::Vec4<float>{1.f, 0.f, 0.f, 0.f});
        }
    }

    void CPRigidBody::UpdateRigidBodyVel(float time)
    {
        //std::cout << "BEFORE: " << this->mVelocity.ToString() << std::endl;
        this->mVelocity += this->mAcceleration * time;
        this->mVelocity = this->mVelocity * this->mFriction;
        //std::cout << "AFTER: " << this->mVelocity.ToString() << std::endl;
        //std::cout << this->mFriction << std::endl;

        LB::Vec2<float> zeroed;
        zeroed.x = 0.f;
        zeroed.y = 0.f;
        this->mAcceleration = zeroed;
    }

    void CPRigidBody::FixedUpdate()
    {
        float time = static_cast<float>(TIME->GetFixedDeltaTime());

        // If body is static do not update velocities or pos
        if (this->isStatic) {
            return;
        }

        mPosition = transform->GetPosition();

        // Semi-implicit euler system
        this->UpdateRigidBodyVel(time);
        this->UpdateRigidBodyPos(time);

        transform->SetPosition(mPosition);
    }

    void CPRigidBody::DebugDraw()
    {
        // Add if check to see if DEBUG MODE on
        DEBUG->DrawBox(mPosition, mWidth, mHeight,
            Vec4<float> { 0.f, 0.f, 1.0f, 1.0f }, mRotation);
        DEBUG->DrawLine(mPosition, PHY_MATH::Normalize(mVelocity), PHY_MATH::Length(mVelocity) / 2.f,
            Vec4<float> {1.0f, 0.f, 0.f, 0.f});
    }

    // END OF RIGIDBODY MEMBER FUNCTIONS
    // ===========================================

    void CreateBoxVertices(LB::Vec2<float>* vertices_arr, float width, float height)
    {
        float left = -width / 2;
        float right = width / 2;
        float top = height / 2;
        float bottom = -height / 2;


        vertices_arr[0].x = left;
        vertices_arr[0].y = top;

        vertices_arr[1].x = right;
        vertices_arr[1].y = top;

        vertices_arr[2].x = right;
        vertices_arr[2].y = bottom;

        vertices_arr[3].x = left;
        vertices_arr[3].y = bottom;
    }

    void CreateBoxVerticesTransformed(LB::Vec2<float>* vertices_arr, LB::Vec2<float> position)
    {
        vertices_arr[0] += position;
        vertices_arr[1] += position;
        vertices_arr[2] += position;
        vertices_arr[3] += position;
    }

}