/*!************************************************************************
 \file				CPPSFPS.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:		CSD2451A
 \date				09-02-2024
 \brief
 This file contains the CPPSFPS class and all its functionalities.

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Components/CPPScriptComponent.h"
#include <sstream>
#include <iomanip>

namespace LB
{
	class CPPSFPS : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		Inherited Functions from CPPBehaviour that the user must implement
		*************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;

	private:
		
		// Member variables to help with forming the FPS string
		GameObject* m_FPSObject{ nullptr };
		GameObject* m_CameraFollow{ nullptr };
		std::string fpsBuffer{};
		char finalFps[256];
		
		// Member variables to check for game settings
		bool m_GamePaused{ false };
		bool m_ShowFPS{ false };
	};
	REGISTER_SCRIPT(CPPSFPS)
}