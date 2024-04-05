/*!************************************************************************
 * \file				AudioSourceComponent.h
 * \author(s)			Amadeus Chia 
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2401A
 * \date				25/11/2023
 * \brief 				This file contains the declarations for the Audio
 * 						Component
 *  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "TransformComponent.h"
#include "LitterBox/Audio/AudioType.h"

namespace LB
{
	class CPAudioSource : public IComponent
	{
	public:
		/*!***********************************************************************
		* AUDIO COMPONENT FUNCTIONS
		*************************************************************************/

		/*!************************************************************************
		 * \brief Plays the audio component (will play the current audio clip attached)
		 * (by default, it plays the SFX)
		**************************************************************************/
		void Play(SoundType type = SFX);

		/*!************************************************************************
		 * \brief Plays the audioclip attached after X seconds of delay
		 * 
		 * \param delay time in seconds to delay the sound playing by
		**************************************************************************/
		void PlayDelayed(float delay);

		/*!************************************************************************
		 * \brief Updates the currently attached audio with the new one
		 * E.g "Explosion.wav" -> "Explosion" will the the name you use
		 * \param clipName New AudioClip to "attach" to the audio component
		**************************************************************************/
		void UpdateAudio(std::string clipName);

		/*!************************************************************************
		 * \brief Pauses the audio component if it's playing
		 * 
		**************************************************************************/
		void Pause();

		/*!************************************************************************
		 * \brief Unpauses the audio component if it's paused
		 * 
		**************************************************************************/
		void UnPause();

		/*!************************************************************************
		 * \brief Stops the audio component
		 * 
		**************************************************************************/
		void Stop();

		/*!************************************************************************
		 * \brief Checks if the audio component is playing or not
		 * 
		**************************************************************************/
		bool isPlaying();

		/*!************************************************************************
		 * \brief Set the Pitch object
		 * 
		 * \param _pitch Pitch to set (clamped in inspector[-4,4])
		**************************************************************************/
		void SetPitch(float _pitch);

		/*!************************************************************************
		 * \brief Set the Volume object
		 * 
		 * \param _vol Volume to set (clamped in inspector [0,inf])
		**************************************************************************/
		void SetVolume(float _vol);

		void FadeOut(float time = 1.f);
		void FadeIn(float time = 1.f, float volume = 1.f);

		/*!***********************************************************************
		* PUBLIC VARIABLES
		*************************************************************************/
		bool isActive{ false };			
		bool mute{false};				//Sets volume to 0 if true
		bool playOnAwake{false};		//Determines if the audio source should play on awake
		bool loop{false};				//Determines if the audio source should loop
		bool is3D{false};				//Determines if the audio source is 3D
		float volume{ 1.0f };			//Clamped at 0.0f - 1.0f
		float volToSet{};				//Volume to set for fading
		float pitch{ 1.0f };			//Clamped at 0.0f - 1.0f (idk if clamping this is normal)
		float fadeTimer{ 0.f };			//Timer for fading in/out
		float fadeTime{ 0.f };			//Time for fading in/out
		bool fadeIn{ false };				//Determines if the audio source should fade in
		bool fadeOut{ false };			//Determines if the audio source should fade out
		SoundType soundType{ SFX };		//Determines if the audio source is SFX or BGM
		std::string AudioClipName{};		//Name of the audioclip 

		/*!***********************************************************************
		* ICOMPONENT OVERRIDES
		*************************************************************************/
		CPTransform* transform;
		/*!************************************************************************
		 * \brief Destroy the CPAudioSource object
		 * 
		**************************************************************************/
		virtual ~CPAudioSource() = default; //First things first...

		/*!************************************************************************
		 * \brief Initialises the audio component
		 * 
		**************************************************************************/
		void Initialise() override;

		/*!************************************************************************
		 * \brief Updates trhe audio component
		 * 
		**************************************************************************/
		void Update() override;

		/*!************************************************************************
		 * \brief Destroys the audio component
		 * (Currently, this also clears out ALL audio components from the audiomanager)
		**************************************************************************/
		void Destroy() override;

		/*!************************************************************************
		 * \brief Serializes the audio component
		 * 
		**************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc) override;

		/*!************************************************************************
		 * \brief Deserializes the audio component
		 * 
		**************************************************************************/
		bool Deserialize(const Value& data) override;

		void ToggleActive(bool isActive) override;

		/*!************************************************************************
		 * \brief Get the Type object
		 * 
		 * \return ComponentTypeID Label for the AudioComponent
		**************************************************************************/
		static ComponentTypeID GetType()
		{
			return C_CPAudioSource;
		}
	private:
		float timer{ 0 };
		bool playDelayed{ false };
		int channelID{ 0 };
		bool hasPlayed{ false };
	};

}