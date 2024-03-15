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
		// Subscribe important events
		GOMANAGER->FindGameObjectWithName("MainChar")->GetComponent<CPPSPlayer>()->onTakingDamage.Subscribe(LB::AudienceCheer);
		GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->onEnemyKill.Subscribe(LB::AudienceCheer);

		GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->onNextLevel.Subscribe(LB::RefreshAudience);
	}

	void CPPSAudienceManager::Update()
	{
		if (!m_init)
		{
			std::vector<GameObject*> audience = GOMANAGER->FindGameObjectsWithName("AudienceMember");
			for (auto* audienceMember : audience)
			{
				m_audience.push_back(audienceMember->GetComponent<CPPSAudience>());
			}
			m_init = true;
		}
	}

	void CPPSAudienceManager::Destroy()
	{
	}

	void CPPSAudienceManager::Cheer()
	{
		for (auto* audience : m_audience)
		{
			audience->Cheer();
		}
	}

	void CPPSAudienceManager::RefreshAudience()
	{
		for (auto* audience : m_audience)
		{
			audience->Refresh();
		}
	}

	void AudienceCheer()
	{
		GOMANAGER->FindGameObjectWithName("AudienceManager")->GetComponent<CPPSAudienceManager>()->Cheer();
	}

	void RefreshAudience()
	{
		GOMANAGER->FindGameObjectWithName("AudienceManager")->GetComponent<CPPSAudienceManager>()->RefreshAudience();
	}
}
