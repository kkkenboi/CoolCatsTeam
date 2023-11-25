/*!************************************************************************
 \file				AudioManager.h
 \author(s)			Amadeus Chia Jinhan, 
 \par DP email(s):	amadeusjinhan.chia@digipen.edu,
 \par Course:       CSD2401A
 \date				02/11/2023
 \brief				This file contains the implementation of AudioManager 
                    functions. (This will be more detailed as more 
					features get added)

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "LitterBox/Core/System.h"
#pragma warning(push, 0)
#include "../../dependencies/FMOD/core/inc/fmod.hpp"
#include "../../dependencies/FMOD/core/inc/fmod_errors.h"
#pragma warning(pop)
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Components/AudioSourceComponent.h"

namespace LB
{
	/*!***********************************************************************
     * \brief 
     * AudioManager handles anything sound related, playing sounds in different
	 * channels etc.
     **************************************************************************/
	class AudioManager : public ISystem
	{
	public:
		/*!***********************************************************************
		 * \brief Construct a new Audio Manager object
		 * (FMOD::System is initialised here)
		 **************************************************************************/
		AudioManager();
		/*!***********************************************************************
		 \brief
		 Initalises Audio Manager System
		*************************************************************************/
		void Initialize() override { SetSystemName("Audio System"); }

		/*!***********************************************************************
		 * \brief Updates the Audio System (Called by ISystem)
		 * 
		 **************************************************************************/
		void Update() override;

		/*!***********************************************************************
		 * \brief Frees the audio engine and the sounds
		 * 
		 **************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		 * \brief AudioSystem that provides FMOD functionality to play sounds
		 * MUST be initialised before it can be used!
		 **************************************************************************/
		FMOD::System* audioSystem;

		/*!***********************************************************************
	 	* \brief Function to play sound using the Sound File name 
	 	* Returns the channel ID that the sound is playing in
	 	* \param soundName Name of the sound e.g "Explosion" without file extension
		**************************************************************************/
		int PlaySound(std::string soundName);

		/*!***********************************************************************
		 * \brief Function to play sound. Stops currently playing sound if there's one
		 *
		 * \param soundName Name of the sound e.g "Explosion" without file extension
		**************************************************************************/
		void ToggleSoundPlaying(std::string soundName);

		/*!***********************************************************************
		 * \brief Function to check if the specified channel is playing
		 *
		 * (Technically... 1 sound 1 channel so...)
		**************************************************************************/
		bool IsPlaying(int ChannelID);

		/*!************************************************************************
		 * \brief Function to check if the specified channel is paused
		 * 
		 * \param channelID Channel ID to check for
		**************************************************************************/
		bool IsPaused(int channelID);


		/*!***********************************************************************
		* \brief Function to stop all channels from playing
		*
		**************************************************************************/
		void StopAllChannels();
		
		/*!************************************************************************
		 * \brief Function to pause ALL channels
		 * 
		**************************************************************************/
		void PauseAllChannels();

		/*!************************************************************************
		 * \brief Function to unpuase ALL channels
		 * 
		**************************************************************************/
		void UnPauseAllChannels();

		/*!************************************************************************
		 * \brief Function to stop a specified channel from playing
		 * 
		 * \param channelID Channel ID to stop playing from (grab the ID from playing sound)
		**************************************************************************/
		void StopChannel(int channelID);

		/*!************************************************************************
		 * \brief Function to PAUSE a specific channel from playing
		 * (Grab the channel id from the PlaySound function)
		 * \param channelID Channel ID to PAUSE the sound from
		**************************************************************************/
		void PauseChannel(int channelID);

		/*!************************************************************************
		 * \brief Function to UNPAUSE a specific channel from playing
		 * (Grab the channel id from the PlaySound function)
		 * \param channelID Channel ID to UNPAUSE the sound from
		**************************************************************************/
		void UnPauseChannel(int channelID);

		/*!************************************************************************
		 * \brief Set the Channel Pitch object
		 * (Grab channel id from PlaySound function)
		 * \param channelID Channel ID to set the pitch for
		 * \param _pitch Pitch to set
		**************************************************************************/
		void SetChannelPitch(int channelID, float _pitch);

		/*!************************************************************************
		 * \brief Set the Channel Volume object
		 * (Grab channel id from PlaySound function)
		 * \param ChannelID Channel ID to set the volume for
		 * \param _vol Volume to set
		**************************************************************************/
		void SetChannelVolume(int ChannelID, float _vol);

		//bool IsPlaying(std::string soundName);
		//void StopAllSounds();

		/*!***********************************************************************
		 * \brief Stores the result of any FMOD related function
		 * Allows for error checking and error logging
		 **************************************************************************/
		FMOD_RESULT result;
		std::vector<CPAudioSource*> AudioSources;
	private:
		int channelID{ 0 };
		std::map<int, FMOD::Channel*> Channels;
	};

	/*!***********************************************************************
	 * \brief Functions to play specific sounds
	 * 
	 **************************************************************************/
	void PlayTestSound();
	void PlayExplosionSound();
	void PlayAHHSound();
	void RemoveAllAudioSources(bool isPlaying);


	/*!************************************************************************
	 * \brief GLOBAL PAUSE function wrappers
	 * 
	**************************************************************************/
	void Pause();	//Wrapper for PauseAllChannels
	void UnPause();	//Wrapper for UnPauseAllChannels

	/*!***********************************************************************
	 * \brief Global Pointer for the AudioManager
	 * 
	 **************************************************************************/
	extern AudioManager* AUDIOMANAGER;
}

