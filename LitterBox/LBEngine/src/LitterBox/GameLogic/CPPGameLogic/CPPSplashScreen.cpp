/*!************************************************************************
 * \file				CPPSplashScreen.cpp
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

#include "CPPSplashScreen.h"
//#include "LitterBox/Serialization/AssetManager.h"
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
		////DigiPenTexture->GetAllComponents();
		//DigiPenTexture = GOMANAGER->FindGameObjectWithName("DigipenTexture");
		//DigiPenTexture->SetActive(true);

		//PurrfectPuttTexture = GOMANAGER->FindGameObjectWithName("PurrfectPuttTexture");
		//PurrfectPuttTexture->SetActive(false);
		//mDuration = 3.0f;
		//mSecondDuration = 3.0f;

		VideoPlayerSystem::Instance()->PlayCutscene("SplashScreen", "SceneMainMenu");
		//int chanl = AUDIOMANAGER->PlaySound("SplashScreen");
		//AUDIOMANAGER->SetChannelVolume(VideoPlayerSystem::, 0.3f);
		//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->PurrfectPuttSounds, 0.4f, 1.0f);

	}

	/*!************************************************************************
	 * \brief Update function of the splash screen
	 * 
	**************************************************************************/
	void CPPSplashScreen::Update()
	{
		//mDuration -= static_cast<float>(TIME->GetDeltaTime());
		//if (mDuration <= 0.0f)
		//{
		//	DigiPenTexture->SetActive(false);
		//	PurrfectPuttTexture->SetActive(true);
		//	mSecondDuration -= static_cast<float>(TIME->GetDeltaTime());
		//	AUDIOMANAGER->PlaySound("Purrfect Putt!!!_Group");
		//	if (mSecondDuration <= 0.0f)
		//	{
		//		PurrfectPuttTexture->SetActive(false);
		//		SCENEMANAGER->LoadScene("SceneMainMenu");
		//	}
		//	//go to next scene
		//}
	}

	/*!************************************************************************
	 * \brief Destroy function of the splash screen
	 *
	**************************************************************************/
	void CPPSplashScreen::Destroy()
	{

	}

}