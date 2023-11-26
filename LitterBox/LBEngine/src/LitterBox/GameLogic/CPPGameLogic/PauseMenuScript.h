/*!************************************************************************
 \file				PauseMenuScript.h
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:		CSD2401A
 \date				26-11-2023
 \brief
  This file contains the functionality of the PauseMenuScript

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class PauseMenuScript : public CPPBehaviour
	{
	public:
		void Start() override;
		void Update() override;
		void Destroy() override;
	private:
		GameObject* MenuTextureObject;
		GameObject* ResumeButtonObject;
		GameObject* HowToPlayButtonObject;
		GameObject* QuitButtonObject;
	};

}