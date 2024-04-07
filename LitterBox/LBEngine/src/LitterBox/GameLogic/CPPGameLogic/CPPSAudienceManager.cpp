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
	/*!***********************************************************************
	\brief
	Inherited Functions from CPPBehaviour that the user must implement
	*************************************************************************/
	void CPPSAudienceManager::Start()
	{
		// Subscribe important events
		GOMANAGER->FindGameObjectWithName("MainChar")->GetComponent<CPPSPlayer>()->onTakingDamage.Subscribe(LB::AudienceCheer);
		GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->onEnemyKill.Subscribe(LB::AudienceCheer);

		GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->onNextLevel.Subscribe(LB::RefreshAudience);
		GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->onNewMapStart.Subscribe(LB::CreateAudience);
	}
	void CPPSAudienceManager::Update()
	{
		if (!m_init)
		{
			CreateAudience();
			m_init = true;
		}
	}
	void CPPSAudienceManager::Destroy()
	{
	}

	/*!***********************************************************************
	\brief
	Calls every audience member to cheer
	*************************************************************************/
	void CPPSAudienceManager::Cheer()
	{
		for (auto* audience : m_audience)
		{
			audience->Cheer();
		}
	}

	/*!***********************************************************************
	\brief
	Resets all audience members' sprite
	*************************************************************************/
	void CPPSAudienceManager::RefreshAudience()
	{
		for (auto* audience : m_audience)
		{
			audience->Refresh();
		}
	}

	/*!***********************************************************************
	\brief
	Finds all audience members in the scene
	*************************************************************************/
	void CPPSAudienceManager::CreateAudience()
	{
		m_audience.clear();

		std::vector<GameObject*> audience = GOMANAGER->FindGameObjectsWithName("AudienceMember");
		for (auto* audienceMember : audience)
		{
			m_audience.push_back(audienceMember->GetComponent<CPPSAudience>());
		}
	}

	/*!***********************************************************************
	\brief
	For event subscribing, cheers or refreshes the audience
	*************************************************************************/
	void AudienceCheer()
	{
		GOMANAGER->FindGameObjectWithName("AudienceManager")->GetComponent<CPPSAudienceManager>()->Cheer();
	}
	void RefreshAudience()
	{
		GOMANAGER->FindGameObjectWithName("AudienceManager")->GetComponent<CPPSAudienceManager>()->RefreshAudience();
	}
	/*!***********************************************************************
	\brief
	For event subscribing, finds all audience members in the scene
	*************************************************************************/
	void CreateAudience()
	{
		GOMANAGER->FindGameObjectWithName("AudienceManager")->GetComponent<CPPSAudienceManager>()->CreateAudience();
	}
}
