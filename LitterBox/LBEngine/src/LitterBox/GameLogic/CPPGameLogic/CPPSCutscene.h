/*!************************************************************************
 * \file				CPPSCutscene.h
 * \author(s)			Ryan Tan Jian Hao
 * \par DP email(s):  	ryanjianhao.tan@digipen.edu
 * \par Course:       	CSD2450A
 * \date				10/03/2024
 * \brief 				This header file contains the class definition for
 *						the cutscene class that performs the video logic.
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/Components/RenderVideoComponent.h"
#include "LitterBox/Serialization/AssetManager.h"

namespace LB
{
	//NOTE: IT IS ASSUMED THAT THIS SCRIPT WILL BE PLACED ON A SCREEN DATA OBJECT
	//WITH A CPRENDER COMPONENT THAT CAN RENDER OUR TEXTURE
	class CPPSCutscene : public CPPBehaviour
	{
	public:
		/*!************************************************************************
		* \brief Start function of the Cutscene scene
		* Get a pointer to the screen
		**************************************************************************/
		void Start() override;
		/*!************************************************************************
		* \brief Uudate function for the Aim UI, basically just points the
		* image towards the player's cursor
		**************************************************************************/
		void Update() override;
		void Destroy() override;
	};

	REGISTER_SCRIPT(CPPSCutscene)
}