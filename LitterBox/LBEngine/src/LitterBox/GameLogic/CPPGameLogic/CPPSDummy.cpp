/*!************************************************************************
 \file                CPPSDummy.cpp
 \author(s)           Justine Carlo Villa Ilao
 \par DP email(s):    justine.c@digipen.edu
 \par Course:         CSD2401A
 \date                15-03-2024
 \brief

	This file contains the dummy enemy

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