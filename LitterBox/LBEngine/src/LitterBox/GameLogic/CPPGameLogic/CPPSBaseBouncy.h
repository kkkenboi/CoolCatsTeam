/*!************************************************************************
 \file
 \author(s)
 \par DP email(s):
 \par Course:		CSD2451A
 \date
 \brief
  This file contains

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class CPPSBaseBouncy : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		Start function where variables will be initialised
		*************************************************************************/
		virtual void Start() {};
		/*!***********************************************************************
		\brief
		Update function where the mushroom's scale is being changed
		*************************************************************************/
		virtual void Update() {};
		/*!***********************************************************************
		\brief
		Overriden destroy function because of inheritance
		*************************************************************************/
		virtual void Destroy() {};
		/*!***********************************************************************
		\brief
		Gets the object's render component
		*************************************************************************/
		CPRender* GetRender();
		/*!***********************************************************************
		\brief
		Gets the object's rigidbody component
		*************************************************************************/
		CPRigidBody* GetRigidBody();
		/*!***********************************************************************
		\brief
		Gets the objcet's collider component
		*************************************************************************/
		CPCollider* GetCollider();

		/*!***********************************************************************
		\brief
		Every time the mushroom collides with anything it's scale changes
		*************************************************************************/
		void OnCollisionEnter(CollisionData colData);

		/*!***********************************************************************
		\brief
		To the sprite scale when interacting with other game objects
		*************************************************************************/
		void OnInteraction(int spriteIndexStart, int spriteIndexEnd);

	protected:
		CPTransform* mTransform{ nullptr };
		CPRender* mRender{ nullptr };
		CPRigidBody* mRigidBody{ nullptr };
		CPCollider* mCollider{ nullptr };
		GameObject* mPlayer{ nullptr };

		float mToMaxTimer{};
		float mToMinTimer{};
		float mScaleTimer{};
		float mScaleTimerRemaining{};

		Vec2<float> mScaleOG{};
		Vec2<float> mScaleMax{};


		bool mScaledUp{ false };
		bool mScaledDown{ true };

	private:

	};

	Vec2<float> VecLerp(const Vec2<float>& a, const Vec2<float>& b, float t);
	REGISTER_SCRIPT(CPPSBaseBouncy)
}