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
#include "AudioListenerComponent.h"
#include "LitterBox/Audio/AudioType.h"

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
		int PlaySound(std::string soundName, SoundType type = SFX);

		/*!************************************************************************
		 * \brief Function to play sound 3D using the Sound File name
		 * Returns the channel ID that the sound is playing in
		 * \param soundName name of the sound e.g "Explosion" without file extension
		 * \param pos position to play the sound at
		 * \param type type of sound (for the channel group)
		**************************************************************************/
		int Play3DSound(std::string soundName, Vec2<float> pos, SoundType type = SFX);


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
		bool IsPaused(int ChannelID);


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
		void StopChannel(int ChannelID);

		/*!************************************************************************
		 * \brief Function to PAUSE a specific channel from playing
		 * (Grab the channel id from the PlaySound function)
		 * \param channelID Channel ID to PAUSE the sound from
		**************************************************************************/
		void PauseChannel(int ChannelID);

		/*!************************************************************************
		 * \brief Function to UNPAUSE a specific channel from playing
		 * (Grab the channel id from the PlaySound function)
		 * \param channelID Channel ID to UNPAUSE the sound from
		**************************************************************************/
		void UnPauseChannel(int ChannelID);

		/*!************************************************************************
		 * \brief Set the Channel Pitch object
		 * (Grab channel id from PlaySound function)
		 * \param channelID Channel ID to set the pitch for
		 * \param _pitch Pitch to set
		**************************************************************************/
		void SetChannelPitch(int ChannelID, float _pitch);

		/*!************************************************************************
		 * \brief Set the Channel Volume object
		 * (Grab channel id from PlaySound function)
		 * \param ChannelID Channel ID to set the volume for
		 * \param _vol Volume to set
		**************************************************************************/
		void SetChannelVolume(int ChannelID, float _vol);

		/*!************************************************************************
		 * \brief Set the Channel Group Volume object
		 * volume range is [0f,1f]
		 * \param _vol volume to set
		 * \param type type of sound (for the channel group)
		**************************************************************************/
		void SetChannelGroupVolume(float _vol,SoundType type);

		/*!************************************************************************
		 * \brief Set the Master Volume object
		 * volume range is [0f,1f]
		 * \param _vol volume to set
		**************************************************************************/
		void SetMasterVolume(float _vol);

		/*!************************************************************************
		 * \brief Function to fade out a specific channel
		 * 
		 * \param duration duration of the fade out
		**************************************************************************/
		void FadeOutChannels(float duration);

		/*!************************************************************************
		 * \brief Function to fade in a specific channel
		 * 
		 * \param duration duration of the fade in
		**************************************************************************/
		void FadeinChannels(float duration);

		/*!************************************************************************
		 * \brief Set the Loop Channel object
		 * 
		 * \param ChannelID channel ID to set the loop for
		 * \param loop boolean to set the loop (true means yes loop, false means no loop)
		**************************************************************************/
		void SetLoopChannel(int ChannelID, bool loop);

		/*!***********************************************************************
		 * \brief Stores the result of any FMOD related function
		 * Allows for error checking and error logging
		 **************************************************************************/
		FMOD_RESULT result;
		std::vector<CPAudioSource*> AudioSources;
		CPAudioListener* AudioListener{nullptr};

	
		FMOD::ChannelGroup* BGMChannelGroup{nullptr};
		FMOD::ChannelGroup* SFXChannelGroup{nullptr};
		FMOD::ChannelGroup* MasterChannelGroup{nullptr};
		float BGMVolume = 1.0f;
		float SFXVolume = 1.0f;
		float MasterVolume = 1.0f;


	private:
		int channelID{ 0 };
		std::map<int, FMOD::Channel*> Channels;
		float fadeOutTimer{};
		float fadeInTimer{};
		bool fadeOut{}, fadeIn{};
	};




	/*!************************************************************************
	 * \brief GLOBAL PAUSE function wrappers
	 * 
	**************************************************************************/
	void Pause();	//Wrapper for PauseAllChannels
	void UnPause();	//Wrapper for UnPauseAllChannels
	void FadeOut();	//Wrapper to fadeout all channels
	void FadeIn();	//Wrapper to fadein all channels
	void RemoveAllAudioSources(bool isPlaying);
	/*!***********************************************************************
	 * \brief Global Pointer for the AudioManager
	 * 
	 **************************************************************************/
	extern AudioManager* AUDIOMANAGER;
}

