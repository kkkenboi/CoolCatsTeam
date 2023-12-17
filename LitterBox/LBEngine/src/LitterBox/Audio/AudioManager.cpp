/*!************************************************************************
 \file				AudioManager.cpp
 \author(s)			Amadeus Chia Jinhan, 
 \par DP email(s):	amadeusjinhan.chia@digipen.edu,
 \par Course:       CSD2401A
 \date				25/11/2023
 \brief				This file contains the definitions of AudioManager 
					functions. The AudioManager also updates all the 
					audio components in the game. The audio manager 
					can currently handle pause and unpause, volume and pitch
					controls (This will be more detailed as more 
					features get added)

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "pch.h"
#include "AudioManager.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Debugging/Debug.h"
#include "LitterBox/Core/Core.h"
#include "Platform/Windows/Windows.h"
#include "LitterBox/Engine/Time.h"
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
		CORE->onPlayingModeToggle.Subscribe(RemoveAllAudioSources);
		WINDOWSSYSTEM->OnApplicationFocus.Subscribe(UnPause);
		WINDOWSSYSTEM->OnApplicationUnFocus.Subscribe(Pause);
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
	void RemoveAllAudioSources(bool isPlaying)
	{
		if(!isPlaying)
		AUDIOMANAGER->AudioSources.clear();
	}


	/*!************************************************************************
	 * \brief GLOBAL PAUSE function wrapper
	 * (Calls Audiomanager->PauseAllChannels)
	**************************************************************************/
	void Pause()
	{
		AUDIOMANAGER->PauseAllChannels();
	}

	/*!************************************************************************
	 * \brief GLOBAL UNPAUSE function wrapper
	 * (Calls Audiomanager->UnPauseAllChannels)
	**************************************************************************/
	void UnPause()
	{
		AUDIOMANAGER->UnPauseAllChannels();
	}

	/*!***********************************************************************
	* \brief Updates the Audio System (Called by ISystem)
	* 
	**************************************************************************/
	void AudioManager::Update()
	{
		if (TIME->IsPaused())
		{
			Pause();
		}
		else UnPause();
		//Store a vector of channels you wanna remove from the map
		std::vector<std::map<int, FMOD::Channel*>::iterator> stoppedChannels;
		//We loop through all our channels and see which is stopped
		for (auto iter = Channels.begin(); iter != Channels.end(); ++iter)
		{	
			//Fmod always does this weird address thing...
			bool isPlaying = false;
			bool isPaused = false;
			iter->second->getPaused(&isPaused);
			iter->second->isPlaying(&isPlaying);
			if (!isPlaying &&!isPaused)
			{
				stoppedChannels.push_back(iter);
			}
		}
		//Now we remove all the channels we don't need
		for (auto& channel : stoppedChannels)
		{
			Channels.erase(channel);
		}
		
		//then we update any audio sources that are in the game
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
		if (CORE->IsPlaying())
		{
			DebuggerLogWarning("Unable to preview sounds while editor is in playmode!");
			return;
		}
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

	/*!************************************************************************
	* \brief Function to check if the specified channel is paused
	* 
	* \param channelID Channel ID to check for
	**************************************************************************/	
	bool AudioManager::IsPaused(int ChannelID)
	{
		bool isPaused{ false };
		if (Channels.find(ChannelID) != Channels.end())
		{
			Channels[ChannelID]->getPaused(&isPaused);
		}
		else
		{
			//DebuggerLogWarningFormat("Unable to find channel %d!",channelID);
		}
		return isPaused;
	}

	/*!************************************************************************
	* \brief Function to stop a specified channel from playing
	* 
	* \param channelID Channel ID to stop playing from (grab the ID from playing sound)
	**************************************************************************/
	void AudioManager::StopChannel(int ChannelID)
	{
		if (Channels.find(ChannelID) != Channels.end())
		{
			Channels[ChannelID]->stop();
		} //else DebuggerLogWarningFormat("Unable to find channel %d!", channelID);
	}

	/*!************************************************************************
	* \brief Function to PAUSE a specific channel from playing
	* (Grab the channel id from the PlaySound function)
	* \param channelID Channel ID to PAUSE the sound from
	**************************************************************************/	
	void AudioManager::PauseChannel(int ChannelID)
	{
		if (!IsPaused(ChannelID))
		{
			Channels[ChannelID]->setPaused(true);
		}
	}

	/*!************************************************************************
	* \brief Function to UNPAUSE a specific channel from playing
	* (Grab the channel id from the PlaySound function)
	* \param channelID Channel ID to UNPAUSE the sound from
	**************************************************************************/	
	void AudioManager::UnPauseChannel(int ChannelID)
	{
		if (IsPaused(ChannelID))
		{
			Channels[ChannelID]->setPaused(false);
		}
	}


	/*!************************************************************************
	* \brief Set the Channel Pitch object
	* (Grab channel id from PlaySound function)
	* \param channelID Channel ID to set the pitch for
	* \param _pitch Pitch to set
	**************************************************************************/
	void AudioManager::SetChannelPitch(int ChannelID,float _pitch)
	{
		if (Channels.find(ChannelID) != Channels.end())
		{
			Channels[ChannelID]->setPitch(_pitch);
		}// else DebuggerLogWarningFormat("Unable to find channel %d!", channelID);

	}

	/*!************************************************************************
	* \brief Set the Channel Volume object
	* (Grab channel id from PlaySound function)
	* \param ChannelID Channel ID to set the volume for
	* \param _vol Volume to set
	**************************************************************************/
	void AudioManager::SetChannelVolume(int ChannelID, float _vol)
	{
		if (Channels.find(ChannelID) != Channels.end())
		{
			Channels[ChannelID]->setVolume(_vol);
		}// else DebuggerLogWarningFormat("Unable to find channel %d!", channelID);
	}

	/*!************************************************************************
	* \brief Function to pause ALL channels
	* 
	**************************************************************************/
	void AudioManager::PauseAllChannels()
	{
		for (const auto& src : AudioSources)
		{
			src->Pause();
		}
		//for (auto iter = Channels.begin(); iter != Channels.end(); ++iter)
		//{
		//	//Fmod always does this weird address thing...
		//	bool isPlaying = false;
		//	iter->second->isPlaying(&isPlaying);
		//	if (isPlaying)
		//	{
		//		iter->second->setPaused(true);
		//	}
		//}
	}

	/*!************************************************************************
	* \brief Function to unpuase ALL channels
	* 
	**************************************************************************/
	void AudioManager::UnPauseAllChannels()
	{
		for (const auto& src : AudioSources)
		{
			src->UnPause();
		}
		//for (auto iter = Channels.begin(); iter != Channels.end(); ++iter)
		//{
		//	//Fmod always does this weird address thing...
		//	bool isPaused{ false };
		//	iter->second->getPaused(&isPaused);
		//	if (isPaused)
		//	{
		//		iter->second->setPaused(false);
		//	}
		//}
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
