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
		bool IsPaused(int channelID);


		/*!***********************************************************************
		* \brief Function to stop all channels from playing
		*
		*
		**************************************************************************/
		void StopAllChannels();

		//TODO
		void StopChannel(int channelID);
		void PauseChannel(int channelID);
		void UnPauseChannel(int channelID);
		void SetChannelPitch(int channelID, float _pitch);
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

	/*!***********************************************************************
	 * \brief Global Pointer for the AudioManager
	 * 
	 **************************************************************************/
	extern AudioManager* AUDIOMANAGER;
}

