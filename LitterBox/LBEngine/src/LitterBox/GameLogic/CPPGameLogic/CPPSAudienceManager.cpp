/*!************************************************************************
 \file				CPPSAudience.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				14/03/2024
 \brief

 This file manages the behaviours of all the audience members in the
 current scene.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPSAudienceManager.h"

#include "CPPSPlayer.h"
#include "CPPGameManager.h"
#include "LitterBox/Factory/GameObjectManager.h"

namespace LB
{
	void CPPSAudienceManager::Start()
	{
		m_audience = GOMANAGER->FindGameObjectsWithName("AudienceMember");

		// Subscribe important events
		GOMANAGER->FindGameObjectWithName("MainChar")->GetComponent<CPPSPlayer>()->onTakingDamage.Subscribe(AudienceCheer);
		GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->onEnemyKill.Subscribe(AudienceCheer);
	}

	void CPPSAudienceManager::Update()
	{
	}

	void CPPSAudienceManager::Destroy()
	{
	}

	void CPPSAudienceManager::Cheer()
	{
		for (auto* audience : m_audience)
		{
			audience->GetComponent<CPAnimator>()->PlayAndReset("Action_Cheer");
		}
	}

	void AudienceCheer()
	{
		GOMANAGER->FindGameObjectWithName("AudienceManager")->GetComponent<CPPSAudienceManager>()->Cheer();
	}
}
