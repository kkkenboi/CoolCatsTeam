/*!************************************************************************
 \file                RigidBody.cpp
 \author(s)           Justine Carlo Villa Ilao
 \par DP email(s):    justine.c@digipen.edu
 \par Course:         CSD2401A
 \date                04-09-2023
 \brief
 This file contains the function definitions for the CPRigidBody class'
 member functions and non-member functions

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
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
    /*!***********************************************************************
        \brief
        Creates a CPRigidBody with default parameters stated in the function
        definition
    *************************************************************************/
    void CPRigidBody::CreateRigidBody()
    {
        transform = gameObj->GetComponent<CPTransform>();

        this->mPosition = transform->GetPosition();
        this->mPrevPosition = mPosition;
        
        this->mVelocity = LB::Vec2<float>{ 0.f, 0.f };
        this->mAcceleration = LB::Vec2<float>{ 0.f, 0.f };

        this->mRotation = 0.f;
        this->mRotationalVelocity = 0.f;

        this->mMass = 10.f;
        this->mDensity = 10.f;
        this->mRestitution = 0.5f;

        this->mFriction = 0.95f;
        this->isStatic = false;
        this->isActive = true;

        this->mRadius = 50.f;
        this->mWidth = 100.f;
        this->mHeight = 100.f;

        //this->mShapeType = COL_BOX;
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
        /*
        // Check if Box
        if (this->mShapeType == COL_BOX)
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

        this->UpdateRigidBodyAABB();
        */
        PHYSICS->AddRigidBodyToPool(this);
    }

    /*!***********************************************************************
        \brief
        Gets the position of the CPRigidBody
    *************************************************************************/
    LB::Vec2<float> CPRigidBody::getPos() 
    {
        return mPosition;
    }

    /*!***********************************************************************
        \brief
        Adds a Force to the CPRigidBody
    *************************************************************************/
    void CPRigidBody::addForce(LB::Vec2<float> force)
    {
        this->mVelocity += force;
    }

    /*!***********************************************************************
        \brief
        Adds an Impulse to the CPRigidBody
    *************************************************************************/
    void CPRigidBody::addImpulse(LB::Vec2<float> force)
    {
        this->mAcceleration += force;
    }

    void CPRigidBody::addRotation(float angle)
    {
        this->mRotation += angle;
    }

    /*!***********************************************************************
        \brief
        Moves the CPRigidBody directly with a vector
    *************************************************************************/
    void CPRigidBody::Move(LB::Vec2<float> vec)
    {
        this->mPosition += vec;
        transform->SetPosition(mPosition);
    }

    /*!***********************************************************************
        \brief
        Moves the CPRigidBody directly to a certain position
    *************************************************************************/
    void CPRigidBody::MoveTo(LB::Vec2<float> position)
    {
        this->mPosition = position;
        transform->SetPosition(mPosition);
    }

    /*!***********************************************************************
        \brief
        Updates the CPRigidBody Box Vertices within its' data members
    *************************************************************************/
    /*
    void CPRigidBody::UpdateRigidBodyBoxVertices()
    {
        PhysicsTransform xtransform{ this->mPosition, this->mRotation };

        for (int i = 0; i < 4; ++i) {
            // Uses the untransformed vertices as the basis for tranasformation
            LB::Vec2<float> og_vec = this->mVertices[i];
            // Transforming the vertices using trigo formulas
            this->mTransformedVertices[i] = LB::Vec2<float>{
                xtransform.m_cos * og_vec.x - xtransform.m_sin * og_vec.y + xtransform.m_posX,
                xtransform.m_sin * og_vec.x + xtransform.m_cos * og_vec.y + xtransform.m_posY };
        }


    }
    */

    /*!***********************************************************************
        \brief
        Updates the AABB collider in the CPRigidBody's data members
    *************************************************************************/
    /*
    void CPRigidBody::UpdateRigidBodyAABB()
    {
        float minX = 10000000.f;
        float maxX = -10000000.f;
        float minY = 10000000.f;
        float maxY = -10000000.f;

        if (this->mShapeType == COL_BOX)
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
        else if (this->mShapeType == COL_CIRCLE)
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
    */

    /*!***********************************************************************
        \brief
        Updates the position of the CPRigidBody
    *************************************************************************/
    void CPRigidBody::UpdateRigidBodyPos(float time)
    {
        // If static do not move or update the position of the RigidBody
        if (this->isStatic)
        {
            return;
        }

        //std::cout << "POS: " << mPosition.x << " , " << mPosition.y << std::endl;
        //std::cout << "VEL: " << mVelocity.x << " , " << mVelocity.y << std::endl;
        //Vec2<float> temp_pos = this->mPosition;
        //float temp_rot = this->mRotation;

        this->mPosition += this->mVelocity * time;
        transform->SetPosition(mPosition);

        this->mRotation += this->mRotationalVelocity * time;
        transform->SetRotation(mRotation);

        //if (temp_pos != this->mPosition || temp_rot != this->mRotation)
        //{

        // Update the TransformedVertices
        // HERE
        //this->UpdateRigidBodyBoxVertices();
        //}
        //if (temp_pos != this->mPosition)
        //{
        // Update the AABB
        // HERE
        //this->UpdateRigidBodyAABB();
        //}
        
    }

    /*!***********************************************************************
        \brief
        Updates the CPRigidBody's velocity
    *************************************************************************/
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

    /*!***********************************************************************
        \brief
        Override of the FixedUpdate() of IComponent
        Updates the RigidBody within a fixed timestep
        - Semi-implicit Euler system, updating velocity first then positions
        - Also updates the Transform IComponent that is stored in CPRigidBody
    *************************************************************************/
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

    /*!***********************************************************************
        \brief
        This is the function that calls the debug drawer to draw all the
        RigidBody collision boxes as well as their velocities
    *************************************************************************/
    /*
    void CPRigidBody::DebugDraw()
    {
        if (this->mShapeType == COL_BOX)
        {
            DEBUG->DrawBox(mPosition, mWidth, mHeight,
                Vec4<float> { 0.f, 0.f, 1.0f, 1.0f }, mRotation);
        }
        if (this->mShapeType == COL_CIRCLE)
        {
            DEBUG->DrawCircle(mPosition,mRadius,
                Vec4<float> { 0.f, 0.f, 1.0f, 1.0f });
        }
        DEBUG->DrawLine(mPosition, PHY_MATH::Normalize(mVelocity), PHY_MATH::Length(mVelocity) / 5.f,
            Vec4<float> {1.0f, 0.f, 0.f, 0.f});
    }
    */

    // END OF RIGIDBODY MEMBER FUNCTIONS
    // ===========================================
    /*!***********************************************************************
        \brief
        Creates the Vertice Box placing it in the give Vec2<float> pointer
    *************************************************************************/
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

    /*!***********************************************************************
        \brief
        Creates the BoxVertices Transformed
    *************************************************************************/
    void CreateBoxVerticesTransformed(LB::Vec2<float>* vertices_arr, LB::Vec2<float> position)
    {
        vertices_arr[0] += position;
        vertices_arr[1] += position;
        vertices_arr[2] += position;
        vertices_arr[3] += position;
    }

}