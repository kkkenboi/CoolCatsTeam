#pragma once
#include "TransformComponent.h"
namespace LB
{
	class CPAudioSource : public IComponent
	{
	public:

		void Play();
		void PlayDelayed(float delay);
		void UpdateAudio(std::string clipName);
		void Pause();
		void UnPause();
		void Stop();
		bool isPlaying();
		void SetPitch(float _pitch);
		void SetVolume(float _vol);



		/*!***********************************************************************
		* PUBLIC VARIABLES
		*************************************************************************/
		bool isActive{ false };
		bool mute{false};				//Sets volume to 0 if true
		bool playOnAwake{false};		//Determines if the audio source should play on awake
		bool loop{false};				//Determines if the audio source should loop
		float volume{ 1.0f };			//Clamped at 0.0f - 1.0f
		float pitch{ 1.0f };			//Clamped at 0.0f - 1.0f (idk if clamping this is normal)
		std::string AudioClipName{};		//Name of the audioclip 

		/*!***********************************************************************
		* ICOMPONENT OVERRIDES
		*************************************************************************/
		CPTransform* transform;
		virtual ~CPAudioSource() = default; //First things first...
		void Initialise() override;
		void Update() override;
		void Destroy() override;
		bool Serialize(Value& data, Document::AllocatorType& alloc) override;
		bool Deserialize(const Value& data) override;
		static ComponentTypeID GetType()
		{
			return C_CPAudioSource;
		}
	};

}