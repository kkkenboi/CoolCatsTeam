/*!************************************************************************
 \file				CPPSAudience.h
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

#pragma once

#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class CPPSAudienceManager : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		Start function to find all audience
		*************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;

		void Cheer();

	private:
		std::vector<GameObject*> m_audience;
	};
	REGISTER_SCRIPT(CPPSAudienceManager);

	void AudienceCheer();
}