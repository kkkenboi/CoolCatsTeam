/*!************************************************************************
 \file				CPPSPlayer.h
 \author(s)			Vanessa Chua Siew Jin, Amadeus Chia 
 \par DP email(s):	vanessasiewjin@digipen.edu, amadeusjinhan.chia@digipen.edu
 \par Course:		CSD2401A
 \date				09-02-2024
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
		GameObject* m_GameManager{ nullptr };
		GameObject* m_CameraFollow{ nullptr };

		float m_stepSoundInterval, m_stepSoundCurrent; //walking Speed, max speed cap for walking, playing sound duration 

		float mGotAttacked, mGotAttackedCooldown; //attacking and cooling down of the attack

		bool mIsStunned;
		float mStunRemaining, mStunTimer; // when getting attack get stunned

		float m_shootForce, m_shootRadius; //shooting of the golf ball force and the radius to shoot

		bool m_isFacingLeft; //facing which direction

		Event<> onTakingDamage;
		Event<> onPlacingBall;
	private:
		//direction of where the player is facing
		LB::Vec2<float> right_face;
		LB::Vec2<float> left_face;
		bool hasPlayedHitSound{ false };
	};
	REGISTER_SCRIPT(CPPSPlayer)
}