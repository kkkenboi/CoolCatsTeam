/*!************************************************************************
 \file				RigidBodyManager.cpp
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				This file contains the RigidBodyManager class and its
                    functions definitions

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "RigidBody.h"
#include "RigidBodyManager.h"
#include "PhysicsMath.h"
#include "LitterBox/Engine/Time.h"

namespace LB
{
    RigidBodyManager* PHYSICS = nullptr;

    /*!***********************************************************************
        \brief
        RigidBodyManager's constructor, assigns memory to the rigidbody pool
        and rb states
    *************************************************************************/
    RigidBodyManager::RigidBodyManager()
    {

        if (!PHYSICS)
        {
            PHYSICS = this;
        }
        else
        {
            std::cerr << "Physics already exists\n";
        }

        // NEED TO DEFINE A POOL SIZE ANOTHER TIME INSTEAD
        // OF HARDCODING IT HERE
        constexpr int POOL_SIZE = 3000;
        m_poolSize = POOL_SIZE;
        m_currentIndex = 0;
        m_rigidBodies = new CPRigidBody * [3000];
        m_rbStates = new bool[3000];

        // Initialize all the RigidBody States to false
        for (int i = 0; i < POOL_SIZE; ++i)
        {
            m_rigidBodies[i] = nullptr; // All rigidbodies in arr empty first
            m_rbStates[i] = false; // Everything is not in use
        }

        std::cout << "Physics initialized\n";
    }

    /*!***********************************************************************
      \brief
      RigidBodyManager's constructor, assigns memory to the rigidbody pool
      with size
    *************************************************************************/
    RigidBodyManager::RigidBodyManager(int size) : m_poolSize(size), m_currentIndex(0)
    {
        m_rigidBodies = new CPRigidBody * [size];
        m_rbStates = new bool[size];

        // Initialize all the RigidBody States to false;
        for (int i = 0; i < m_poolSize; ++i)
        {
            m_rigidBodies[i] = nullptr; // Instantiate the array to be all empty
            m_rbStates[i] = false; // Everything is not in use
        }
    }

    /*!***********************************************************************
      \brief
      RigidBodyManager destructor, frees the memory allocated for the pool
      and states
    *************************************************************************/
    RigidBodyManager::~RigidBodyManager()
    {
        for (int i = 0; i < m_poolSize; ++i)
        {
            if (m_rigidBodies[i] != nullptr)
            {
                delete m_rigidBodies[i];
            }
        }
        delete[] m_rigidBodies;
        delete[] m_rbStates;
    }

    /*!***********************************************************************
      \brief
      This function allows you to add a RigidBody to the pool
    *************************************************************************/
    void RigidBodyManager::AddRigidBodyToPool(CPRigidBody* rb)
    {
        for (int i = 0; i < m_poolSize; ++i)
        {
            if (m_rigidBodies[i] == nullptr)
            {
                m_rigidBodies[i] = rb;
                m_rbStates[i] = false;
                return;
            }
        }
    }

    /*!***********************************************************************
      \brief
      This function allows you to get a pointer to a RigidBody within the
      pool with the index given
      \return
      Returns a pointer to the RigidBody in the pool
    *************************************************************************/
    CPRigidBody* RigidBodyManager::GetPooledRigidBody(size_t index)
    {
        if (index < 0 || index >= m_poolSize)
        {
            return nullptr;
        }

        return m_rigidBodies[index];
    }

    /*!***********************************************************************
      \brief
      This function gets the nearest unused CPRigidBody according to its
      RBStates, within the pool
      \return
      Returns a pointer to a CPRigidBody that is unused according to RBStates
    *************************************************************************/
    CPRigidBody* RigidBodyManager::GetPooledRigidBody()
    {
        for (int i = 0; i < m_poolSize; ++i)
        {
            size_t index = (m_currentIndex + static_cast<size_t>(i)) % m_poolSize; // Circular index so that we start from the index and loops back until index-1
            if (!m_rbStates[index] && m_rigidBodies[index] != nullptr)
            {
                m_rbStates[index] = true; // This RigidBody is now pulled and allowed to be done work with or calculations with
                m_currentIndex = index;
                return m_rigidBodies[index];
            }
        }
        return nullptr;
    }

    /*!***********************************************************************
      \brief
      This function returns a CPRigidBody that was pulled from GetPooledRigidBody
      back into the pool with RBStates false now
    *************************************************************************/
    void RigidBodyManager::ReturnPooledRigidBody(CPRigidBody* rb)
    {
        for (int i = 0; i < m_poolSize; ++i)
        {
            if (m_rigidBodies[i] == rb)
            {
                m_rbStates[i] = false;
                return;
            }
        }
    }

    /*!***********************************************************************
      \brief
      This function returns a pointer to a CPRigidBody that has the ID of 1
    *************************************************************************/
    CPRigidBody* RigidBodyManager::LookForMainCharacter()
    {
        for (int i = 0; i < m_poolSize; ++i)
        {
            if (m_rigidBodies[i]->mNumberID == 1)
            {
                return m_rigidBodies[i];
            }
        }
        return nullptr;
    }
    
    /*!***********************************************************************
      \brief
      Does all the update steps for all the CPRigidBodies within the pool
      - Movement Step
      - Update Velocities
      - Update Positions
      - Collision Step
      - Check BOXBOX, BOXCIRCLE, CIRCLEBOX, CIRCLECIRCLE Collisions
      - Get normal and depth for separation movement
      - Resolve Collisions
    *************************************************************************/
    void RigidBodyManager::RBSystemSteps()
    {
        // ==================
        // Movement Step
        // ==================
        for (size_t i = 0; i < m_poolSize; ++i)
        {
            if (m_rigidBodies[i] != nullptr)
            {
                m_rigidBodies[i]->FixedUpdate();
            }
        }

        // ==================
        // Collision Step
        // ==================
        LB::Vec2<float> normal_out{ 0.f , 0.f };
        float depth_out{ 0.f };

        for (size_t i = 0; i < m_poolSize; ++i)
        {
            CPRigidBody* bodyA = m_rigidBodies[i];

            for (size_t j = i + 1; j < m_poolSize; ++j)
            {
                CPRigidBody* bodyB = m_rigidBodies[j];

                if (bodyA == bodyB)
                {
                    continue;
                }

                // If either rigidBody is not instantiated
                if (bodyA == nullptr || bodyB == nullptr)
                {
                    continue;
                }

                // If both bodies are static
                if (bodyA->isStatic == true && bodyB->isStatic == true)
                {
                    continue;
                }

                // Normal here is moving B away from A
                if (CheckCollisions(bodyA, bodyB, normal_out, depth_out))
                {

                    LB::Vec2<float>inverse_normal{ -normal_out.x, -normal_out.y };
                    if (bodyA->isStatic)
                    {
                        bodyB->Move(normal_out * depth_out);
                    }
                    else if (bodyB->isStatic)
                    {
                        bodyA->Move(inverse_normal * depth_out);
                    }
                    else
                    {
                        bodyA->Move(inverse_normal * depth_out);
                        bodyB->Move(normal_out * depth_out);
                    }
                    
                    ResolveCollisions(bodyA, bodyB, normal_out, depth_out);
                    
                }


            }

        }
    }
    // ======================================
    // ISystem function overrides
    // ======================================

    /*!***********************************************************************
        \brief
        Initialize function override of ISystem, allocates the CPRigidBody pool
        memory
    *************************************************************************/
    void RigidBodyManager::Initialize()
    {
        SetSystemName("Physics System");
    }

    /*!***********************************************************************
      \brief
      Updates the CPRigidBody pool in a fixed timestep
    *************************************************************************/
    void RigidBodyManager::FixedUpdate()
    {
        RBSystemSteps();
    }

    /*!***********************************************************************
      \brief
      Updates the Physics debugger in normal framerate
    *************************************************************************/
    void RigidBodyManager::Update()
    {
        // Draw collision data if debug is on
        if (DEBUG->IsDebugOn())
        {
            for (size_t i = 0; i < m_poolSize; ++i)
            {
                if (m_rigidBodies[i] != nullptr)
                {
                    m_rigidBodies[i]->DebugDraw();
                }
            }
        }
    }


    // END OF RIGIDBODYMANAGER MEMBER FUNCTIONS
    // =======================================================

    /*!***********************************************************************
      \brief
      Takes in two CPRigidBodies and checks the collision between the two
      while outputting the normal_out and depth_out of the collision for
      collision resolution
      - normal_out is from A to B
      \return
      Returns a bool or whether or not the two CPRigidBodies collided or not
    *************************************************************************/
    bool CheckCollisions(CPRigidBody* bodyA, CPRigidBody* bodyB, LB::Vec2<float>& normal_out, float& depth_out) {
        normal_out.x = 0.f; // Make it zeroed first, in case of any values beforehand
        normal_out.y = 0.f;
        depth_out = 0.f; // Zeroed in case of previous values

        if (bodyA->mShapeType == BOX)
        {
            if (bodyB->mShapeType == BOX)
            {
                // A - B
                // BOX-BOX
                return CollisionIntersection_BoxBox_SAT(bodyA->mTransformedVertices, bodyB->mTransformedVertices, normal_out, depth_out);
                //return CollisionIntersection_BoxBox(bodyA->obj_aabb, bodyA->mVelocity, bodyB->obj_aabb, bodyB->mVelocity, TIME->GetFixedDeltaTime(), normal_out, depth_out);
            }
            else if (bodyB->mShapeType == CIRCLE) {
                // A - B
                // BOX-CIRCLE
                // normal here is pushing B away from A
                bool result = CollisionIntersection_CircleBox_SAT(bodyB->mPosition, bodyB->mRadius, bodyA->mTransformedVertices, normal_out, depth_out);
                // The normal given is inverse bodyB is circle and bodyA is box
                // So normal given is circle away from box, therefore need to inverse it
                normal_out.x = -normal_out.x;
                normal_out.y = -normal_out.y;
                return result;
            }
        }
        if (bodyA->mShapeType == CIRCLE)
        {
            if (bodyB->mShapeType == BOX)
            {
                // A - B
                // CIRCLE-BOX
                // normal here is pushing box away from circle
                bool result = CollisionIntersection_CircleBox_SAT(bodyA->mPosition, bodyA->mRadius, bodyB->mTransformedVertices, normal_out, depth_out);
                return result;
            }
            if (bodyB->mShapeType == CIRCLE)
            {
                // A - B 
                // CIRCLE-CIRCLE
                return CollisionIntersection_CircleCircle(bodyA->mPosition, bodyB->mPosition, bodyA->mRadius, bodyB->mRadius, normal_out, depth_out);
            }
        }
        return false;
    }

    /*!***********************************************************************
      \brief
      Takes in two CPRigidBodies and checks the collision between the two
      while outputting the normal_out and depth_out of the collision for
      collision resolution
    *************************************************************************/
    void ResolveCollisions(CPRigidBody* bodyA, CPRigidBody* bodyB, LB::Vec2<float> normal, float depth) {

        UNREFERENCED_PARAMETER(depth);
        // Need to get relative velocity from A to B
        LB::Vec2<float> relativeVelocity = bodyB->mVelocity - bodyA->mVelocity;

        // If the dot product of relVel and normal is more than 0.f
        if (PHY_MATH::DotProduct(relativeVelocity, normal) > 0.f) {
            return;
        }

        float e = PHY_MATH::FindMin(bodyA->mRestitution, bodyB->mRestitution);

        float j = -(1.f + e) * PHY_MATH::DotProduct(relativeVelocity, normal);
        j /= bodyA->mInvMass + bodyB->mInvMass;

        // Magnitude * Normal to get the impulse given to the objects
        LB::Vec2<float> impulse = normal * j;

        
        if (!bodyA->isStatic)
        {
            bodyA->mVelocity -= impulse * bodyA->mInvMass;
        }
        if (!bodyB->isStatic)
        {
            bodyB->mVelocity += impulse * bodyB->mInvMass;
        }
        
    }

} // Namespace LB