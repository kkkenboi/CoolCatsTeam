/*!************************************************************************
 * \file				CPPShield.cpp
 * \author(s)			Vanessa Chua Siew Jin
 * \par DP email(s):  	vanessasiewjin@digipen.edu
 * \par Course:       	CSD2450A
 * \date				28/02/2024
 * \brief 				This file contains the file definition for the
 *						Shield
 *
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPShield.h"
#include "LitterBox/Utils/Matrix3x3.h"

namespace LB
{
	void CPPShield::Start()
	{
		//mRender = GameObj->GetComponent<CPRender>();
		//mRigidBody = GameObj->GetComponent<CPRigidBody>();
		mCollider = GameObj->GetComponent<CPCollider>();

		mTransform = GameObj->GetComponent<CPTransform>();
		mCharger = mTransform->GetParent()->gameObj;
		//mChargerScript = mCharger->GetComponent<CPPSCharger>(); // <<!!!the script is getting loaded before the object is done constructing
		mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");
		mCharger = GOMANAGER->FindGameObjectWithName("Charger_Shield");

		
		//mTransform = 
		//mCollider->m_pos.x = 0.f;
		//mCollider->m_pos.y = 0.f;


		//mLock = false;
	}

	void CPPShield::Update()
	{
		//set the pos of the shield onto the charger
		//setting the pos of the shield to be on the charger

		// Rotation : Face the player
		//mCollider->m_pos.x = 0.f;
		//mCollider->m_pos.y = 0.f;
		
		if (!m_ScriptSet)
		{
			mChargerScript = mCharger->GetComponent<CPPSCharger>();
			m_ScriptSet = true;
		}

		if (!mChargerScript->isCharging && !mChargerScript->m_isStunned)
		{
			Vec2<float> Direction = (GetPlayerPos() - mTransform->GetParent()->GetPosition()).Normalise();
			//DebuggerLogFormat("Player x %.2f Player y %.2f", GetPlayerPos().x, GetPlayerPos().y);
			//DebuggerLogFormat("Charger x %.2f Charger y %.2f", mTransform->GetPosition().x, mTransform->GetPosition().y);
			//DebuggerLogFormat("Dir x %.2f Dir y %.2f", Direction.x, Direction.y);

			mTransform->SetRotation(RadToDeg(atan2f(Direction.y, Direction.x)));
			// Position : Offset from charger
			Vec2<float> shieldPos{ offset, 0.0f };
			float angle_radians = DegToRad(mTransform->GetLocalRotation());
			// Calculate sine and cosine of the angle
			float cos_theta = std::cos(angle_radians);
			float sin_theta = std::sin(angle_radians);
			// Perform rotation transformation
			float newX = shieldPos.x * cos_theta - shieldPos.y * sin_theta;
			float newY = shieldPos.x * sin_theta + shieldPos.y * cos_theta;
			shieldPos.x = newX;
			shieldPos.y = newY;
			mTransform->SetPosition(shieldPos);
			//DebuggerLogErrorFormat("COLLIDERS X: %f", mCollider->m_pos.x);
			//DebuggerLogErrorFormat("COLLIDERS Y: %f", mCollider->m_pos.y);
		}

		// Scale : If facing left, set scale x = -1
		if ((mTransform->GetParent()->GetScale().x < 0 && mTransform->GetLocalScale().x > 0) ||
				mTransform->GetParent()->GetScale().x > 0 && mTransform->GetLocalScale().x < 0)
		{
			Vec2<float> newScale{ mTransform->GetLocalScale() };
			newScale.x = -newScale.x;

			mTransform->SetScale(newScale);
		}
	}

	void CPPShield::Destroy() { }

	void CPPShield::OnCollisionEnter(CollisionData colData)
	{
		//if (colData.colliderOther->m_gameobj->GetName() == "ball")
		//{
		//	mRigidBody->addImpulse(Vec2<float>(1000.0f, 1000.0f));
		//}
;	}

	GameObject* CPPShield::GetCharger()
	{
		return mCharger;
	}
	
	Vec2<float> CPPShield::GetChargerPos()
	{
		return GetCharger()->GetComponent<CPRigidBody>()->getPos();
	}

	GameObject* CPPShield::GetPlayer()
	{
		return mPlayer;
	}

	Vec2<float> CPPShield::GetPlayerPos()
	{
		return GetPlayer()->GetComponent<CPRigidBody>()->getPos();
	}

	CPCollider* CPPShield::GetCollider()
	{
		return mCollider;
	}
}