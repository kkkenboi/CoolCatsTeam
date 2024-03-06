/*!************************************************************************
 \file				CPPSMouse.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:		CSD2451A
 \date				06-03-2024
 \brief
 This file contains the CPPSMouse class and all its functionalities.

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "pch.h"
#include "CPPBehaviour.h"
#include "LitterBox/Components/CPPScriptComponent.h"


namespace LB
{
	/*!***********************************************************************
	\brief
	
	*************************************************************************/
	class CPPSMouse : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		Start function where variables will be initialised
		*************************************************************************/
		void Start() override;
		/*!***********************************************************************
		\brief
		Update function where the mouse's position is being updated based on
		whether it is of screen or world space
		*************************************************************************/
		void Update() override;
		/*!***********************************************************************
		\brief
		Overriden destroy function because of inheritance
		*************************************************************************/
		void Destroy() override;

	private:
		GameObject* m_MouseUI { nullptr };
		GameObject* m_MouseWorld { nullptr };
		GameObject* m_CameraFollow{ nullptr };

		float m_mHeight{};
		float m_mWidth{};

		float m_BorderHeight{};
	};
	REGISTER_SCRIPT(CPPSMouse)
}