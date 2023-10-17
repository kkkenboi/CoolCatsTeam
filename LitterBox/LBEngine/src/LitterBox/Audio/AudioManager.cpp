/*!************************************************************************
 \file				AudioManager.cpp
 \author(s)			Amadeus Chia Jinhan, 
 \par DP email(s):	amadeusjinhan.chia@digipen.edu,
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				This file contains the definitions of AudioManager 
					functions (This will be more detailed as more 
					features get added)

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "pch.h"
#include "AudioManager.h"
#include "LitterBox/Debugging/Debug.h"
namespace LB
{
	AudioManager* AUDIOMANAGER = nullptr;
	/*!***********************************************************************
	* \brief Construct a new Audio Manager object
	* (FMOD::System is initialised here)
	**************************************************************************/
	AudioManager::AudioManager()
	{
		//TODO Pull this out to the initialiser next time...
		//Singleton stuff
		if (!AUDIOMANAGER)
			AUDIOMANAGER = this;
		else
			std::cerr << "Audio Manager already exists" << std::endl;

		// Create an instance of the FMOD system object
		result = FMOD::System_Create(&audioSystem);
		if (result != FMOD_OK) {
			// Handle initialization error
			// We can use FMOD_ErrorString(result) to get an error description
			//TODO TAKE THIS OUT AND USE OUR BEAUTIFUL DEBUGGER!
			//DebuggerLogError(std::string{ "FMOD ERROR : " + FMOD_ErrorString(result) + "\n" });
			std::cerr <<"FMOD ERROR : " << FMOD_ErrorString(result) << '\n';
		}

		// Initialize the FMOD system
		result = audioSystem->init(512, FMOD_INIT_NORMAL, 0);
		if (result != FMOD_OK) {
			//TODO USE OUR BEAUTIFUL DEBUGGER!
			// Handle initialization error
		}
	}

	/*!***********************************************************************
	* \brief Functions to play specific sounds
	* 
	**************************************************************************/
	void PlayTestSound()
	{
		AUDIOMANAGER->result = AUDIOMANAGER->audioSystem->playSound(ASSETMANAGER->sampleSound, nullptr, false, nullptr);
		if (AUDIOMANAGER->result != FMOD_OK)
		{
			//TODO TAKE THIS OUT AND USE OUR BEAUTIFUL DEBUGGER!
			std::cout << "PLAY SAMPLE SOUND IS NOT WORKING!! \n";
		}
	}
	void PlayExplosionSound()
	{
		AUDIOMANAGER->result = AUDIOMANAGER->audioSystem->playSound(ASSETMANAGER->explosionSound, nullptr, false, nullptr);
		if (AUDIOMANAGER->result != FMOD_OK)
		{
			//TODO TAKE THIS OUT AND USE OUR BEAUTIFUL DEBUGGER!
			std::cout << "PLAY EXPLOSION SOUND IS NOT WORKING!! \n";
		}
	}
	void PlayAHHSound()
	{
		AUDIOMANAGER->result = AUDIOMANAGER->audioSystem->playSound(ASSETMANAGER->ahhSound, nullptr, false, nullptr);
		if (AUDIOMANAGER->result != FMOD_OK)
		{
			//TODO TAKE THIS OUT AND USE OUR BEAUTIFUL DEBUGGER!
			std::cout << "PLAY AHH SOUND IS NOT WORKING!! \n";
		}
	}

	/*!***********************************************************************
	* \brief Updates the Audio System (Called by ISystem)
	* 
	**************************************************************************/
	void AudioManager::Update()
	{
		//In the future we can check for 
		audioSystem->update();
	}

	void AudioManager::Destroy()
	{
		//This shuts down FMOD studio
		audioSystem->release();
	}
}
