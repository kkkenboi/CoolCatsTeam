/*!************************************************************************
 \file				CPPSMouseWorld.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:		CSD2451A
 \date				06-03-2024
 \brief
 This file contains the CPPSMouseWorld class and all its functionalities.

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/


#include "CPPSMouseWorld.h"
#include "CPPSPlayerHUD.h"
#include "LitterBox/Core/Core.h"


namespace LB
{
	/*!***********************************************************************
	\brief
	Overriden start function because of inheritance
	*************************************************************************/
	void CPPSMouseWorld::Start()
	{
		m_PlayerHUD = GOMANAGER->FindGameObjectWithName("PlayerHUD");
	}

	/*!***********************************************************************
	\brief
	Overriden update function because of inheritance
	*************************************************************************/
	void CPPSMouseWorld::Update()
	{

	}

	/*!***********************************************************************
	\brief
	Overriden destroy function because of inheritance
	*************************************************************************/
	void CPPSMouseWorld::Destroy()
	{

	}

	/*!***********************************************************************
	\brief
	This mainly checks for the collision between the mouse and the World to display
	pop up information upon entering.
	*************************************************************************/
	void CPPSMouseWorld::OnCollisionEnter(CollisionData colData)
	{
		if (CORE->IsPlaying())
		{
			// If hovering over new upgrades
			if (colData.colliderOther->m_gameobj->GetName() == "leftUpgrade" ||
					 colData.colliderOther->m_gameobj->GetName() == "middleUpgrade" ||
					 colData.colliderOther->m_gameobj->GetName() == "rightUpgrade")
			{
				//DebuggerLog("Colliding with upgrade!\n");

				// m_currentPopUpIndex should be from 1 - 7 to match the enum
				m_PlayerHUD->GetComponent<CPPSPlayerHUD>()->m_mouseHoverWorld = true;
				m_PlayerHUD->GetComponent<CPPSPlayerHUD>()->m_currentPopUpIndex = static_cast<UpgradeType>(colData.colliderOther->m_gameobj->GetComponent<CPRender>()->spriteIndex - 31); // Check again when actual sprites are added

				UpgradeType tempIndex{ m_PlayerHUD->GetComponent<CPPSPlayerHUD>()->m_currentPopUpIndex };

				//DebuggerLogFormat("CurrentPopUpIndex: %d", static_cast<int>(tempIndex));

				m_UpgradeObject = colData.colliderOther->m_gameobj;
				Vec2 finalPos = m_UpgradeObject->GetComponent<CPTransform>()->GetPosition();
				finalPos -= Vec2<float>(0.f, 60.f);

				//DebuggerLogFormat("Final Position set at: %f, %f", finalPos.x , finalPos.y);
				//DebuggerLogFormat("Upgrade Name: %s", m_PlayerHUD->GetComponent<CPPSPlayerHUD>()->m_totalUpgradePopUps[static_cast<size_t>(tempIndex - 1)].first->GetName().c_str());

				m_PlayerHUD->GetComponent<CPPSPlayerHUD>()->m_totalUpgradePopUps[static_cast<size_t>(tempIndex - 1)].first->GetComponent<CPTransform>()->SetPosition(finalPos);
			}
			else
			{
				m_PlayerHUD->GetComponent<CPPSPlayerHUD>()->m_mouseHoverWorld = false;
				m_PlayerHUD->GetComponent<CPPSPlayerHUD>()->m_currentPopUpIndex = static_cast<UpgradeType>(0);
			}
		}
	}

	/*!***********************************************************************
	\brief
	This mainly checks for the collision between the mouse and the World to display
	pop up information upon exiting.
	*************************************************************************/
	void CPPSMouseWorld::OnCollisionExit(CollisionData colData)
	{
		if (colData.colliderOther->m_gameobj->GetName() == "leftUpgrade" ||
			colData.colliderOther->m_gameobj->GetName() == "middleUpgrade" ||
			colData.colliderOther->m_gameobj->GetName() == "rightUpgrade")
		{
			if (!GetComponent<CPCollider>()->m_collided)
			{
				m_PlayerHUD->GetComponent<CPPSPlayerHUD>()->m_mouseHoverWorld = false;
				m_PlayerHUD->GetComponent<CPPSPlayerHUD>()->m_currentPopUpIndex = static_cast<UpgradeType>(0);
			}
		}
	}
}