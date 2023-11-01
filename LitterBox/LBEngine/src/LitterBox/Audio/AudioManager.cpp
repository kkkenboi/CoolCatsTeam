/*!************************************************************************
 \file				AudioManager.cpp
 \author(s)			Amadeus Chia Jinhan, 
 \par DP email(s):	amadeusjinhan.chia@digipen.edu,
 \par Course:       CSD2401A
 \date				02/11/2023
 \brief				This file contains the definitions of AudioManager 
					functions (This will be more detailed as more 
					features get added)

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "pch.h"
#include "AudioManager.h"
#include "LitterBox/Serialization/AssetManager.h"
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
	* Currently they're global, but in the future they will be C#
	**************************************************************************/
	void PlayTestSound()
	{
		AUDIOMANAGER->PlaySound("Oof");
	}
	void PlayExplosionSound()
	{
		AUDIOMANAGER->PlaySound("EXPLOSION");
	}
	void PlayAHHSound()
	{
		AUDIOMANAGER->PlaySound("Enemy hurt");
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

	/*!***********************************************************************
	 * \brief Frees all the audio engine and all the sounds that were loaded
	 * 
	**************************************************************************/
	void AudioManager::Destroy()
	{
		//We loop through all the sounds and release them
		for (const auto& sound : ASSETMANAGER->SoundMap)
		{
			sound.second->release();
		}
		//This shuts down FMOD studio
		audioSystem->release();
	}

	/*!***********************************************************************
	 * \brief Function to play sound using the Sound File name 
	 * 
	 * \param soundName Name of the sound e.g "Explosion" without file extension
	**************************************************************************/
	void AudioManager::PlaySound(std::string soundName)
	{
		if (ASSETMANAGER->SoundMap.find(soundName) != ASSETMANAGER->SoundMap.end())
		{
			result = audioSystem->playSound(ASSETMANAGER->SoundMap[soundName], nullptr, false, nullptr);
			if (result != FMOD_OK) DebuggerLogWarning("SOUND NAME " + soundName + "IS NOT WORKING!");
		}
	}

	//TODO :pensive:
	void AudioManager::StopAllSounds()
	{
		/*audioSystem.*/
	}
}
