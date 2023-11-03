/*!************************************************************************
 \file				RigidBodyComponent.h
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief
 This file contains functions of the CPRigidBody class.

 CPRigidBody is a component that allows the GameObject to interact with other
 physics game objects. If the game object wants to move, be pushed, etc. They
 will need to add this RigidBody component.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "TransformComponent.h"
//#include "LitterBox/Physics/Collisions.h"

namespace LB
{
	class CPRigidBody : public IComponent
	{
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

		LB::Vec2<float> mForce;

	public:
		CPTransform* transform;

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

		// =====
		// TEST STUFF
		// =====
		int mNumberID;
		// ==============

		bool mUpdateVerticesRequired;
		bool mUpdateAABBRequired;

		// =================
		// Constructor
		// =================
		/*!***********************************************************************
		  \brief
		  This function initializes the CPRigidBody
		*************************************************************************/
		void Initialise() override
		{
			CreateRigidBody();
		}

		/*!***********************************************************************
		  \brief
		  This function allows the user to get the Type of the CPRigidBody
		*************************************************************************/
		static ComponentTypeID GetType()
		{
			return C_CPRigidBody;
		}

		/*!***********************************************************************
			\brief
			Creates a CPRigidBody with default parameters stated in the function
			definition
		*************************************************************************/
		void CreateRigidBody();

		/*!***********************************************************************
		  \brief
		  This function serializes some of the data members of CPRigidBody
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc) override
		{
			DebuggerLog("Serialising RB");
			data.SetObject();
			data.AddMember("Width", mWidth, alloc);
			data.AddMember("Height", mHeight, alloc);
			data.AddMember("Density", mDensity, alloc);
			data.AddMember("IsStatic", isStatic, alloc);
			return true;
		}

		/*!***********************************************************************
		  \brief
		  This function deserializes some of the data members of CPRigidBody
		*************************************************************************/
		bool Deserialize(const Value& data) override
		{
			DebuggerLog("Deserialising RB");
			bool HasWidth = data.HasMember("Width");
			bool HasHeight = data.HasMember("Height");
			bool HasDensity = data.HasMember("Density");
			bool HasStatic = data.HasMember("IsStatic");
			if (data.IsObject())
			{
				if (HasWidth && HasHeight && HasDensity && HasStatic)
				{
					mWidth = data["Width"].GetFloat();
					mHeight = data["Height"].GetFloat();
					mDensity = data["Density"].GetFloat();
					isStatic = data["IsStatic"].GetBool();
					return true;
				}
			}
			return false;
		}

	public:
		/*!***********************************************************************
			\brief
			Gets the position of the CPRigidBody
		*************************************************************************/
		LB::Vec2<float> getPos();

		/*!***********************************************************************
			\brief
			Adds a Force to the CPRigidBody
		*************************************************************************/
		void addForce(LB::Vec2<float> force);

		/*!***********************************************************************
			\brief
			Adds an Impulse to the CPRigidBody
		*************************************************************************/
		void addImpulse(LB::Vec2<float> force);

		/*!***********************************************************************
			\brief
			Adds a rotation to the CPRigidBody
		*************************************************************************/
		void addRotation(float angle);

		/*!***********************************************************************
			\brief
			Toggles the IsStatic data member of the CPRigidBody
		*************************************************************************/
		void ToggleIsStatic();


		/*!***********************************************************************
			\brief
			Moves the CPRigidBody directly with a vector
		*************************************************************************/
		void Move(LB::Vec2<float> vec);

		/*!***********************************************************************
			\brief
			Moves the CPRigidBody directly to a certain position
		*************************************************************************/
		void MoveTo(LB::Vec2<float> position);

		/*!***********************************************************************
			\brief
			Updates the position of the CPRigidBody
		*************************************************************************/
		void UpdateRigidBodyPos(float time);

		/*!***********************************************************************
			\brief
			Updates the CPRigidBody's velocity
		*************************************************************************/
		void UpdateRigidBodyVel(float time);

		/*!***********************************************************************
			\brief
			Override of the FixedUpdate() of IComponent
			Updates the RigidBody within a fixed timestep
			- Semi-implicit Euler system, updating velocity first then positions
			- Also updates the Transform IComponent that is stored in CPRigidBody
		*************************************************************************/
		void FixedUpdate();

		/*!***********************************************************************
			\brief
			Destroys the current CPRigidBody
		*************************************************************************/
		void Destroy();
	};
}