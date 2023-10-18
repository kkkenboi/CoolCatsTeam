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
        this->mRestitution = 0.75f;

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
        this->mMass = 1.f;

        // Check if static and update the InvMass
        if (!this->isStatic)
        {
            this->mInvMass = 1.f / this->mMass;
        }
        else
        {
            this->mInvMass = 0.f;
        }
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
        this->mForce += force;
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

    void CPRigidBody::ToggleIsStatic()
    {
        if (isStatic == true)
        {
            isStatic = false;
            mInvMass = 1.f/ mMass;
        }
        if (isStatic == false)
        {
            isStatic = true;
            mInvMass = 0.f;
        }
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
        Updates the position of the CPRigidBody
    *************************************************************************/
    void CPRigidBody::UpdateRigidBodyPos(float time)
    {
        // If static do not move or update the position of the RigidBody
        if (this->isStatic)
        {
            return;
        }

        this->mPosition += this->mVelocity * time;
        transform->SetPosition(mPosition);

        this->mRotation += this->mRotationalVelocity * time;
        transform->SetRotation(mRotation);
        
    }

    /*!***********************************************************************
        \brief
        Updates the CPRigidBody's velocity
    *************************************************************************/
    void CPRigidBody::UpdateRigidBodyVel(float time)
    {
        this->mAcceleration = this->mForce / this->mMass;
        this->mVelocity += this->mAcceleration * time;
        this->mVelocity = this->mVelocity * this->mFriction;

        float threshold = 0.01f;
        if (PHY_MATH::Absolute(mVelocity.x) < threshold) {
            this->mVelocity.x = 0.f;
        }
        if (PHY_MATH::Absolute(mVelocity.y) < threshold) {
            this->mVelocity.y = 0.f;
        }

        this->mForce.x = 0.f;
        this->mForce.y = 0.f;
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
            //this->mInvMass = 0.f;
            return;
        }

        mPosition = transform->GetPosition();

        // Semi-implicit euler system
        this->UpdateRigidBodyVel(time);
        this->UpdateRigidBodyPos(time);

        transform->SetPosition(mPosition);
    }


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