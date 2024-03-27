/*!************************************************************************
 * \file				CPPSplashScreen.h
 * \author(s)			Vanessa Chua Siew Jin
 * \par DP email(s):  	vanessasiewjin@digipen.edu
 * \par Course:       	CSD2450A
 * \date				10/03/2024
 * \brief 				This file contains the file declarations for the
 *						SplashScreen (WIP)
 * 						
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "CPPBehaviour.h"
//#include "LitterBox/Audio/AudioManager.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/Components/RenderVideoComponent.h"
#include "LitterBox/Serialization/AssetManager.h"

namespace LB
{
	class CPPSplashScreen : public CPPBehaviour //inherit from CPPBehaviour
	{
	public:
		//from cppbehaviour
		void Start() override;

		void Update() override;

		void Destroy() override;

		//Game Object to get the texture
		//GameObject* DigiPenTexture{ nullptr };
		//GameObject* PurrfectPuttTexture{ nullptr };


	private:
		float mDuration{}, mSecondDuration{};
	};

	REGISTER_SCRIPT(CPPSplashScreen)
}