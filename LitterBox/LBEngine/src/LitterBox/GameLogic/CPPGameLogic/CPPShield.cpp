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

		mTransform = GameObj->GetComponent<CPTransform>();
		mCollider = mTransform->gameObj->GetComponent<CPCollider>();


		//mChargerScript = mCharger->GetComponent<CPPSCharger>(); // <<!!!the script is getting loaded before the object is done constructing
		mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");
		//mCharger = GOMANAGER->FindGameObjectWithName("Charger_Shield");

		Vec2<float> Direction = (GetPlayerPos() - mTransform->GetParent()->GetPosition()).Normalise();
		Direction *= offset;
		mTransform->SetPosition(Direction);
		mTransform->SetRotation(RadToDeg(atan2f(Direction.y, Direction.x)));
		cachedPosition = Direction;
		cachedRot = RadToDeg(atan2f(Direction.y, Direction.x));
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
			mCharger = mTransform->GetParent()->gameObj;
			mChargerScript = mCharger->GetComponent<CPPSCharger>();
			m_ScriptSet = true;
		}

		if (mChargerScript->m_isLocked)
		{
			//std::cout << "LOCKED\n";
			mTransform->SetPosition(cachedPosition);
			mTransform->SetRotation(cachedRot);

		}
		else
		{
			Vec2<float> Direction = (GetPlayerPos() - mTransform->GetParent()->GetPosition()).Normalise();
			Direction *= offset;
			mTransform->SetPosition(Direction);
			mTransform->SetRotation(RadToDeg(atan2f(Direction.y, Direction.x)));
			cachedPosition = Direction;
			cachedRot = RadToDeg(atan2f(Direction.y, Direction.x));
	
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
		//std::string str(colData.colliderOther->m_gameobj->GetName());
		//std::cout << "Collided with: " << colData.colliderOther->m_gameobj->GetName() << "\n";
		//size_t ShieldStr = str.find("Shield");
		if (colData.colliderOther->m_gameobj->GetName() == "Shield")
		{
			std::cout << "Collided with: " << colData.colliderOther->m_gameobj->GetName() << "\n";
			mChargerScript->ChangeToStunned();
		}
		else if (colData.colliderOther->m_gameobj->GetName() == "ball")
		{
			std::cout << "GOT HIT BALL\n";
		}

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