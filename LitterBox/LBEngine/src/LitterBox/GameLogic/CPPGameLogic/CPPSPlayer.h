/*!************************************************************************
 \file				CPPSPlayer.h
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				03-11-2023
 \brief
 This file contains the CPPSPlayer class and all its functionalities.

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class CPPSPlayer : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		Inherited Functions from CPPBehaviour that the user must implement
		*************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;
		void OnCollisionEnter(CollisionData colData) override;

		CPRigidBody* rb{ nullptr };
		CPCollider* col{ nullptr };
		CPRender* rend{ nullptr };
		CPTransform* trans{ nullptr };

		float m_walkSpeed, m_maxSpeed, m_stepSoundInterval, m_stepSoundCurrent;

		int m_maxBalls, m_currentBalls;

		float m_shootForce, m_shootRadius;

		bool m_isFacingLeft;

	private:
		LB::Vec2<float> right_face;
		LB::Vec2<float> left_face;
	};
}