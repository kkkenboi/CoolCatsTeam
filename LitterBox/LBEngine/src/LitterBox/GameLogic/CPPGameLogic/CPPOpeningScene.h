#pragma once
/*!************************************************************************
 * \file				CPPOpeningScene.h
 * \author(s)			Vanessa Chua Siew Jin
 * \par DP email(s):  	vanessasiewjin@digipen.edu
 * \par Course:       	CSD2450A
 * \date				10/03/2024
 * \brief 				This file contains the file declarations for the
 *						Opening Scene
 *
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
	class CPPOpeningScene : public CPPBehaviour //inherit from CPPBehaviour
	{
	public:
		//overriding function from cppbehaviour
		void Start() override;
		void Update() override;
		void Destroy() override;


	private:
	};

	REGISTER_SCRIPT(CPPOpeningScene)
}