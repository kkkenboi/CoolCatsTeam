/*!************************************************************************
 * \file				CPPOpeningScene.cpp
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

#include "CPPOpeningScene.h"
#include "LitterBox/Scene/SceneManager.h"
#include "LitterBox/Audio/AudioManager.h"


namespace LB
{
	/*!************************************************************************
	 * \brief Start function of the splash screen
	 *
	**************************************************************************/
	void CPPOpeningScene::Start()
	{
		VideoPlayerSystem::Instance()->PlayCutscene("OpeningCutsceneESC", "SceneMainMenu");
	}

	/*!************************************************************************
	 * \brief Update function of the splash screen
	 *
	**************************************************************************/
	void CPPOpeningScene::Update() {}

	/*!************************************************************************
	 * \brief Destroy function of the splash screen
	 *
	**************************************************************************/
	void CPPOpeningScene::Destroy() {}

}