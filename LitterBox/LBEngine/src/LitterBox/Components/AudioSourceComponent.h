#pragma once
#include "TransformComponent.h"
#include "LitterBox/Audio/AudioManager.h"
namespace LB
{
	class CPAudioSource : public IComponent
	{
	public:

		void Play();
		void PlayDelayed(float delay);
		void Pause();
		void UnPause();
		void Stop();
		bool isPlaying();



		/*!***********************************************************************
		* PUBLIC VARIABLES
		*************************************************************************/
		bool mute{false};		//Sets volume to 0 if true
		bool playOnAwake{false};//Determines if the audio source should play on awake
		bool loop{false};		//Determines if the audio source should loop
		float volume{ 1.0f };	//Clamped at 0.0f - 1.0f
		float pitch{ 1.0f };	//Clamped at 0.0f - 1.0f (idk if clamping this is normal)
		FMOD::Sound* AudioClip{nullptr}; //Audio clip for the audio source

		/*!***********************************************************************
		* ICOMPONENT OVERRIDES
		*************************************************************************/
		CPTransform* transform;
		virtual ~CPAudioSource() = default; //First things first...
		void Initialise() override;
		void Update() override;
		void Destroy() override;
		/*static ComponentTypeID GetType()
		{
			return C_CPAudioSource;
		}*/
	};

}