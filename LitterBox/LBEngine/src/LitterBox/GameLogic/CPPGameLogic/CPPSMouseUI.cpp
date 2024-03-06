/*!************************************************************************
 \file				CPPSMouseUI.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:		CSD2451A
 \date				06-03-2024
 \brief
 This file contains the CPPSMouseUI class and all its functionalities.

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/


#include "CPPSMouseUI.h"
#include "CPPSPlayerHUD.h"
#include "LitterBox/Core/Core.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	Overriden start function because of inheritance
	*************************************************************************/
	void CPPSMouseUI::Start()
	{
		m_PlayerHUD = GOMANAGER->FindGameObjectWithName("PlayerHUD");
	}

	/*!***********************************************************************
	\brief
	Overriden update function because of inheritance
	*************************************************************************/
	void CPPSMouseUI::Update()
	{

	}

	/*!***********************************************************************
	\brief
	Overriden destroy function because of inheritance
	*************************************************************************/
	void CPPSMouseUI::Destroy()
	{

	}

	/*!***********************************************************************
	\brief
	This mainly checks for the collision between the mouse and the UI to display
	pop up information upon entering.
	*************************************************************************/
	void CPPSMouseUI::OnCollisionEnter(CollisionData colData)
	{
		if (!CORE->IsPlaying())
		{
			// Do pause menu stuff here
		}
		else
		{
			// If hovering over player's current upgrades
			if (colData.colliderOther->m_gameobj->GetName() == "UpgradeHUD")
			{
				m_PlayerHUD->GetComponent<CPPSPlayerHUD>()->m_mouseHoverUI = true;
				m_PlayerHUD->GetComponent<CPPSPlayerHUD>()->m_currentPopUpIndex = static_cast<UpgradeType>(colData.colliderOther->m_gameobj->GetComponent<CPRender>()->spriteIndex); // Check again when actual sprites are added
			}
		}
	}

	/*!***********************************************************************
	\brief
	This mainly checks for the collision between the mouse and the UI to display
	pop up information upon exiting.
	*************************************************************************/
	void CPPSMouseUI::OnCollisionExit(CollisionData colData)
	{
		m_PlayerHUD->GetComponent<CPPSPlayerHUD>()->m_mouseHoverUI = false;
	}
}