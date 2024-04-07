/*!************************************************************************
 * \file				CPPSplashScreen.cpp
 * \author(s)			Vanessa Chua Siew Jin
 * \par DP email(s):  	vanessasiewjin@digipen.edu
 * \par Course:       	CSD2450A
 * \date				10/03/2024
 * \brief 				This file contains the file declarations for the
 *						SplashScreen 
 *
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPSplashScreen.h"
#include "LitterBox/Scene/SceneManager.h"
#include "LitterBox/Audio/AudioManager.h"


namespace LB
{
	/*!************************************************************************
	 * \brief Start function of the splash screen
	 * 
	**************************************************************************/
	void CPPSplashScreen::Start()
	{
		VideoPlayerSystem::Instance()->PlayCutscene("SplashScreen", "SceneOpening");
	}

	/*!************************************************************************
	 * \brief Update function of the splash screen
	 * 
	**************************************************************************/
	void CPPSplashScreen::Update(){}

	/*!************************************************************************
	 * \brief Destroy function of the splash screen
	 *
	**************************************************************************/
	void CPPSplashScreen::Destroy(){}

}