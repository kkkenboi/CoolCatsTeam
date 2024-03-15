/*!************************************************************************
 \file				CPPSAudience.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				14/03/2024
 \brief

 This file contains the behaviour of a general audience member

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPSAudience.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	Inherited Functions from CPPBehaviour that the user must implement
	*************************************************************************/
	void CPPSAudience::Start()
	{
		m_randomDelayMin = 0.05f;
		m_randomDelayMax = 0.5f;
		m_randomDelayElapsed = m_randomDelayMin + static_cast<float>(std::rand()) / (RAND_MAX / (m_randomDelayMax - m_randomDelayMin));

		m_cheerDelayMin = 0.05f;
		m_cheerDelayMax = 0.2f;

		// Play anim and randomize sprite
		m_animator = GetComponent<CPAnimator>();
		Refresh();
	}
	void CPPSAudience::Update()
	{
		if (m_cheerDelayElapsed > 0.f)
		{
			m_cheerDelayElapsed -= static_cast<float>(TIME->GetDeltaTime());
			if (m_cheerDelayElapsed <= 0.f)
			{
				m_animator->PlayAndReset("Action_Cheer");
				m_cheerElapsed = m_cheerTime;
			}
			return;
		}

		if (m_cheerElapsed > 0.f)
		{
			m_cheerElapsed -= static_cast<float>(TIME->GetDeltaTime());
			if (m_cheerElapsed <= 0.f)
			{
				m_randomDelayElapsed = m_randomDelayMin + static_cast<float>(std::rand()) / (RAND_MAX / (m_randomDelayMax - m_randomDelayMin));
			}

			return;
		}

		if (m_randomDelayElapsed > 0.f)
		{
			m_randomDelayElapsed -= static_cast<float>(TIME->GetDeltaTime());
			if (m_randomDelayElapsed <= 0.f)
			{
				m_animator->PlayRepeat("Action_Hype");
			}
		}
	}
	void CPPSAudience::Destroy()
	{

	}

	/*!***********************************************************************
	\brief
	Causes the audience to cheer like mad.WOOHOO!!
	*************************************************************************/
	void CPPSAudience::Cheer()
	{
		m_cheerDelayElapsed = m_cheerDelayMin + static_cast<float>(std::rand()) / (RAND_MAX / (m_cheerDelayMax - m_cheerDelayMin));
	}

	/*!***********************************************************************
	\brief
	Changes the audience sprite
	*************************************************************************/
	void CPPSAudience::Refresh()
	{
		int sprite = 25 + rand() % 5;
		GetComponent<CPRender>()->spriteIndex = sprite;
		GetComponent<CPRender>()->SetSpriteTexture(GetComponent<CPRender>()->spriteSheetName, sprite);
	}
}
