/*!************************************************************************
 \file				CPPSChaser.cpp
 \author(s)			Justine Carlo Villa Ilao, Vanessa Chua Siew Jin
 \par DP email(s):	justine.c@digipen.edu, vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				03-11-2023
 \brief
 This file contains the CPPSChaser class and all its functionalities,
it handles the logic for the chaser enemy

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPSDummy.h"

#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Debugging/Debug.h"

#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Physics/PhysicsMath.h"
#include "CPPGameManager.h"
#include "CPPVFXManager.h"

namespace LB 
{
	void CPPSDummy::Start()
	{
		mGameManager = GOMANAGER->FindGameObjectWithName("GameManager");
		GetHealth() = 1;
	}

	void CPPSDummy::Update()
	{
		//CPPSBaseEnemy::Update();
		if (mShouldDestroy)
		{
			GOMANAGER->RemoveGameObject(this->GameObj);
			return;
		}
	}

	void CPPSDummy::Destroy()
	{
	}

	void CPPSDummy::Hurt()
	{
		CPPSBaseEnemy::Hurt();
	}

	void CPPSDummy::OnCollisionEnter(CollisionData colData)
	{
		CPPSBaseEnemy::OnCollisionEnter(colData);

		if (colData.colliderOther->m_gameobj->GetName() == "ball")
		{
			Hurt();
		}
	}

	void CPPSDummy::Die()
	{
		CPPSBaseEnemy::Die();
	}
}