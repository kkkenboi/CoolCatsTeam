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

		/*!***********************************************************************
		\brief
		On collision function if it collides with any other colliders
		*************************************************************************/
		void OnCollisionEnter(CollisionData colData) override;

		//------------------Rendering,RB,Collider,Transform------------------
		CPRigidBody* rb{ nullptr };
		CPCollider* col{ nullptr };
		CPRender* rend{ nullptr };
		CPTransform* trans{ nullptr };

		//------------------variables------------------
		float m_walkSpeed, m_maxSpeed, m_stepSoundInterval, m_stepSoundCurrent; //walking Speed, max speed cap for walking, playing sound duration 

		int m_maxBalls, m_currentBalls; //max golf balls that the player can hold, and the current count of how many balls it has spawn

		int m_maxHealth, m_currentHealth; //health of the player
		float mGotAttacked, mGotAttackedCooldown; //attacking and cooling down of the attack

		float m_shootForce, m_shootRadius; //shooting of the golf ball force and the radius to shoot

		bool m_isFacingLeft; //facing which direction

		Event<> onTakingDamage;
		Event<> onPlacingBall;

	private:
		//direction of where the player is facing
		LB::Vec2<float> right_face;
		LB::Vec2<float> left_face;
	};
	REGISTER_SCRIPT(CPPSPlayer)
}