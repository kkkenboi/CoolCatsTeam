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
            DebuggerLogError("Physics already exists");
        }

        // NEED TO DEFINE A POOL SIZE ANOTHER TIME INSTEAD
        // OF HARDCODING IT HERE
        constexpr int POOL_SIZE = 3000;
        m_poolSize = POOL_SIZE;
        m_currentIndex = 0;
        m_rigidBodies = DBG_NEW CPRigidBody * [3000];
        m_rbStates = DBG_NEW bool[3000];

        // Initialize all the RigidBody States to false
        for (int i = 0; i < POOL_SIZE; ++i)
        {
            m_rigidBodies[i] = nullptr; // All rigidbodies in arr empty first
            m_rbStates[i] = false; // Everything is not in use
        }

        DebuggerLog("Physics initialized");
    }

    /*!***********************************************************************
      \brief
      RigidBodyManager's constructor, assigns memory to the rigidbody pool
      with size
    *************************************************************************/
    RigidBodyManager::RigidBodyManager(int size) : m_poolSize(size), m_currentIndex(0)
    {
        m_rigidBodies = DBG_NEW CPRigidBody * [size];
        m_rbStates = DBG_NEW bool[size];

        // Initialize all the RigidBody States to false;
        for (int i = 0; i < m_poolSize; ++i)
        {
            m_rigidBodies[i] = nullptr; // Instantiate the array to be all empty
            m_rbStates[i] = false; // Everything is not in use
        }
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
      This function removes a CPRigidBody* from the RigidBodyPool
    *************************************************************************/
    void RigidBodyManager::RemoveRigidBodyFromPool(CPRigidBody* rb)
    {
        for (size_t i = 0; i < m_poolSize; ++i)
        {
            if (m_rigidBodies[i] == rb)
            {
                m_rigidBodies[i] = nullptr;
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
    }


    /*!***********************************************************************
      \brief
      RigidBodyManager destroy, frees the memory allocated for the pool
      and states
    *************************************************************************/
    void RigidBodyManager::Destroy()
    {
        for (size_t i = 0; i < m_poolSize; ++i)
        {
            m_rigidBodies[i] = nullptr;
        }

        delete[] m_rigidBodies;
        delete[] m_rbStates;

        PHYSICS = nullptr;
    }

    // END OF RIGIDBODYMANAGER MEMBER FUNCTIONS
    // =======================================================


} // Namespace LB