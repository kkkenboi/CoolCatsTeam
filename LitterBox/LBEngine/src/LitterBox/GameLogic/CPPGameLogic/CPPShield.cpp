/*!************************************************************************
 \file				CPPShield.cpp
 \author(s)			Justine Carlo Villa Ilao, Vanessa Chua Siew Jin
 \par DP email(s):	justine.c@digipen.edu, vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				28/02/2024
 \brief
 This file contains the Shield class and all its functionalities,
it handles the logic for the Shield

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPShield.h"
#include "LitterBox/Utils/Matrix3x3.h"

namespace LB
{
	void CPPShield::Start()
	{
		mTransform = CPPBehaviour::GameObj->GetComponent<CPTransform>();
		mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");
		//mCharger = GOMANAGER->FindGameObjectWithName("Charger_Shield");
		mCharger = mTransform->GetParent()->gameObj;

		mLock = false;
	}

	void CPPShield::Update()
	{
		//set the pos of the shield onto the charger
		//setting the pos of the shield to be on the charger
		//Vec2<float> pos = Vec2<float>(GetChargerPos().x, GetChargerPos().y);

		// Rotation : Face the player
		if (!mLock)
		{
			Vec2<float> Direction = (GetPlayerPos() - mTransform->GetParent()->GetPosition()).Normalise();
			DebuggerLogFormat("Player x %.2f Player y %.2f", GetPlayerPos().x, GetPlayerPos().y);
			DebuggerLogFormat("Charger x %.2f Charger y %.2f", mTransform->GetPosition().x, mTransform->GetPosition().y);
			DebuggerLogFormat("Dir x %.2f Dir y %.2f", Direction.x, Direction.y);

			mTransform->SetRotation(RadToDeg(atan2f(Direction.y, Direction.x)));
		}

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

		// Scale : If facing left, set scale x = -1
		if ((mTransform->GetParent()->GetScale().x < 0 && mTransform->GetLocalScale().x > 0) ||
			 mTransform->GetParent()->GetScale().x > 0 && mTransform->GetLocalScale().x < 0)
		{
			Vec2<float> newScale{ mTransform->GetLocalScale() };
			newScale.x = -newScale.x;

			mTransform->SetScale(newScale);
		}

		//mTransform->SetPosition(shieldPos);

		//if (mLock == false)
		//{
		//	//getting the direction from the charger to the player, so shield can face the player
		//	Vec2<float> Direction = (GetPlayerPos() - GetChargerPos()).Normalise();
		//	Vec2<float> shieldPos = GetChargerPos() + (Direction * distance);
		//	mTransform->SetRotation(RadToDeg(atan2f(Direction.y, Direction.x)));
		//	mTransform->SetPosition(shieldPos);
		//	mLock = true;
		//}
		//else
		//{
		//	mLock = true;
		//}
		//Vec2<float> NormalForce = Direction * 
		//grabbing pos of the player
		//mPlayerPos = mCharger
		//have the shield to face the player, direction
		//after it set it eyes on the player, it will stop the shield from moving

	}

	void CPPShield::Destroy()
	{
		//CPTransform* transform = GetComponent<CPTransform>();
		//if (!transform->GetParent())
		//{
		//	transform->Destroy();
		//}
	}

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


}