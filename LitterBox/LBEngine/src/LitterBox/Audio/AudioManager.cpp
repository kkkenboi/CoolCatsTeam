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
		//Store a vector of channels you wanna remove from the map
		std::vector<std::map<int, FMOD::Channel*>::iterator> stoppedChannels;
		//We loop through all our channels and see which is stopped
		for (auto iter = Channels.begin(); iter != Channels.end(); ++iter)
		{	
			//Fmod always does this weird address thing...
			bool isPlaying = false;
			iter->second->isPlaying(&isPlaying);
			if (!isPlaying)
			{
				stoppedChannels.push_back(iter);
			}
		}
		//Now we remove all the channels we don't need
		for (auto& channel : stoppedChannels)
		{
			Channels.erase(channel);
		}
		if (!AudioSources.empty())
		{
			for (const auto& audioSrc : AudioSources)
			{
				audioSrc->Update();
			}
		}
		audioSystem->update();

	}

	/*!***********************************************************************
	 * \brief Frees all the audio engine and all the sounds that were loaded
	 * 
	**************************************************************************/
	void AudioManager::Destroy()
	{
		StopAllChannels();
		////We loop through all the sounds and release them
		for (const auto& sound : ASSETMANAGER->SoundMap)
		{
			sound.second->release();
		}
		//This shuts down FMOD studio
		audioSystem->release();
	}

	/*!***********************************************************************
	 * \brief Function to play sound using the Sound File name 
	 * Returns the channel ID that the sound is playing in
	 * \param soundName Name of the sound e.g "Explosion" without file extension
	**************************************************************************/
	int AudioManager::PlaySound(std::string soundName)
	{
		int _channelID = channelID++;
		//Check to see if the sound is loaded
		if (ASSETMANAGER->SoundMap.find(soundName) != ASSETMANAGER->SoundMap.end())
		{
			FMOD::Channel* _channel = nullptr;
			result = audioSystem->playSound(ASSETMANAGER->SoundMap[soundName], nullptr, false, &_channel);
			if (_channel)
			{
				Channels[_channelID] = _channel;
			}
			if (result != FMOD_OK) DebuggerLogWarning("SOUND NAME " + soundName + "IS NOT WORKING!");
		}
		return _channelID;
	}

	/*!***********************************************************************
	 * \brief Function to play sound. Stops currently playing sound if there's one
	 * 
	 * \param soundName Name of the sound e.g "Explosion" without file extension
	**************************************************************************/
	void AudioManager::ToggleSoundPlaying(std::string soundName)
	{
		if (Channels.empty()) PlaySound(soundName);
		else
		{
			DebuggerLogFormat("%s is still playing! Stopping..", soundName.c_str());
			StopAllChannels();
		}
	}

	/*!***********************************************************************
	 * \brief Function to check if the specified channel is playing
	 *
	 * (Technically... 1 sound 1 channel so...)
	**************************************************************************/
	bool AudioManager::IsPlaying(int ChannelID)
	{
		return (Channels.find(ChannelID) != Channels.end());
	}

	bool AudioManager::IsPaused(int channelID)
	{
		bool isPaused{ false };
		if (Channels.find(channelID) != Channels.end())
		{
			Channels[channelID]->getPaused(&isPaused);
		}
		else
		{
			DebuggerLogWarningFormat("Unable to find channel %d!",channelID);
		}
		return isPaused;
	}

	void AudioManager::StopChannel(int channelID)
	{
		if (Channels.find(channelID) != Channels.end())
		{
			Channels[channelID]->stop();
		} else DebuggerLogWarningFormat("Unable to find channel %d!", channelID);
	}

	void AudioManager::PauseChannel(int channelID)
	{
		if (!IsPaused(channelID))
		{
			Channels[channelID]->setPaused(true);
		}
	}
	void AudioManager::UnPauseChannel(int channelID)
	{
		if (IsPaused(channelID))
		{
			Channels[channelID]->setPaused(false);
		}
	}
	void AudioManager::SetChannelPitch(int channelID,float _pitch)
	{
		if (Channels.find(channelID) != Channels.end())
		{
			Channels[channelID]->setPitch(_pitch);
		}// else DebuggerLogWarningFormat("Unable to find channel %d!", channelID);

	}
	void AudioManager::SetChannelVolume(int channelID, float _vol)
	{
		if (Channels.find(channelID) != Channels.end())
		{
			Channels[channelID]->setVolume(_vol);
		}// else DebuggerLogWarningFormat("Unable to find channel %d!", channelID);
	}
	/*!***********************************************************************
	 * \brief Function to stop all channels from playing
	 *
	 * 
	**************************************************************************/
	void AudioManager::StopAllChannels()
	{
		if (Channels.empty()) return;
		for (auto& channel : Channels)
		{
			channel.second->stop();
		}
	}
}
