/*!************************************************************************
 \file				RigidBodyManager.h
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				This file contains the RigidBodyManager class and its
                    functions declarations

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "RigidBody.h"
#include "LitterBox/Core/System.h"


namespace LB
{

    class RigidBodyManager : public ISystem
    {
    private:
        size_t m_poolSize;
        size_t m_currentIndex; // Holds current index in the pool when checking activeness
        CPRigidBody** m_rigidBodies; // Array of pointers to RigidBodies
        bool* m_rbStates; // To track if a RigidBody is in use or not (pulled out of the pool)

    public:

        /*!***********************************************************************
            \brief
            RigidBodyManager's constructor, assigns memory to the rigidbody pool
            and rb states
        *************************************************************************/
        RigidBodyManager();

        /*!***********************************************************************
          \brief
          RigidBodyManager's constructor, assigns memory to the rigidbody pool
          with size
        *************************************************************************/
        RigidBodyManager(int size);

        /*!***********************************************************************
          \brief
          This function allows you to add a RigidBody to the pool
        *************************************************************************/
        void AddRigidBodyToPool(CPRigidBody* rb);

        /*!***********************************************************************
          \brief
          This function allows you to get a pointer to a RigidBody within the
          pool with the index given
          \return
          Returns a pointer to the RigidBody in the pool
        *************************************************************************/
        CPRigidBody* GetPooledRigidBody(size_t index);

        /*!***********************************************************************
          \brief
          This function gets the nearest unused CPRigidBody according to its
          RBStates, within the pool
          \return
          Returns a pointer to a CPRigidBody that is unused according to RBStates
        *************************************************************************/
        CPRigidBody* GetPooledRigidBody();

        /*!***********************************************************************
          \brief
          This function returns a CPRigidBody that was pulled from GetPooledRigidBody
          back into the pool with RBStates false now
        *************************************************************************/
        void ReturnPooledRigidBody(CPRigidBody* rb);

        void RemoveRigidBodyFromPool(CPRigidBody* rb);

        /*!***********************************************************************
          \brief
          This function returns a pointer to a CPRigidBody that has the ID of 1
        *************************************************************************/
        CPRigidBody* LookForMainCharacter();

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
        void RBSystemSteps();


        // ======================================
        // ISystem function overrides
        // ======================================
        /*!***********************************************************************
          \brief
          Initialize function override of ISystem, allocates the CPRigidBody pool
          memory
        *************************************************************************/
        void Initialize() override;

        /*!***********************************************************************
          \brief
          Updates the CPRigidBody pool in a fixed timestep  
        *************************************************************************/
        void FixedUpdate() override;

        /*!***********************************************************************
          \brief
          Updates the Physics debugger in normal framerate
        *************************************************************************/
        void Update() override;

        /*!***********************************************************************
          \brief
          RigidBodyManager Destroy, frees the memory allocated for the pool
          and states
        *************************************************************************/
        void Destroy() override;

    };

    extern RigidBodyManager* PHYSICS;
}