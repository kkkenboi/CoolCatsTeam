/*!************************************************************************
 \file				CPPSAudience.h
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

#pragma once

#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class CPPSAudience : public CPPBehaviour
	{
	public:
		void Start() override;
		void Update() override;
		void Destroy() override;

		void Cheer();
		void Refresh();

	private:
		float m_randomDelayMin, m_randomDelayMax, m_randomDelayElapsed{ 0.0f };
		float m_cheerTime{ 0.65f }, m_cheerElapsed{ 0.0f };
		float m_cheerDelayMin, m_cheerDelayMax, m_cheerDelayElapsed{ 0.0f };
		CPAnimator* m_animator;
	};
	REGISTER_SCRIPT(CPPSAudience);
}
