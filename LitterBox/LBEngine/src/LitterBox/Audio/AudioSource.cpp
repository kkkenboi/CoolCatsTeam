/*!************************************************************************
 * \file				AudioSource.cpp
 * \author(s)			Amadeus Chia 
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2401A
 * \date				25/11/2023
 * \brief 				This file contains the definitions for the 
 * 						Audio component
 *  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "LitterBox/Components/AudioSourceComponent.h"
#include "LitterBox/Audio/AudioManager.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Core/Core.h"
#include <algorithm>

namespace LB
{

	/*!************************************************************************
	* \brief Initialises the audio component
	* 
	**************************************************************************/
	void CPAudioSource::Initialise()
	{
		//We check if the component even has a valid clip name
		if (AudioClipName == "")
		{
			//We don't really want to log warn this anymore because sometimes we can have NO AUDIO set yet
			//DebuggerLogWarning("NO AUDIO CLIP ATTACHED!");
			return;
		}
		//Commented out this because it's kinda annoying to see in console
		//DebuggerLogWarning("Audio Component Initialised!");
		AUDIOMANAGER->AudioSources.push_back(this); 
		
	}

	/*!************************************************************************
	* \brief Updates trhe audio component
	* 
	**************************************************************************/
	void CPAudioSource::Update()
	{
		//We only want to play sounds when the game is running
		if (CORE->IsPlaying())
		{
			if (!gameObj->IsActive()) return;
			//DebuggerLog("Audio Source update!");
			//If the audio source is playonawake and hasn't played yet
			if (playOnAwake && !hasPlayed) {
				//We play the sound
				Play(soundType);	//hasPlayed = true;
				//DebuggerLog("Play On Awake!");
			}
			//Timer stuff for delayed playing
			if (timer > 0) timer -= static_cast<float>(TIME->GetDeltaTime());
			if (timer <= 0 && playDelayed) //If play delayed is called
			{
				Play(soundType);//hasPlayed = true;
				playDelayed = false;
			}
			if (fadeIn)
			{
				fadeTimer+= static_cast<float>(TIME->GetDeltaTime());
				SetVolume(volToSet * fadeTimer / fadeTime);
				if (fadeTimer >= fadeTime)
				{
					fadeIn = false;
					fadeTimer = 0;
					SetVolume(volToSet);
				}
			}
			if (fadeOut)
			{
				fadeTimer += static_cast<float>(TIME->GetDeltaTime());
				SetVolume(volume * (1.f - fadeTimer/fadeTime));
				if (fadeTimer >= fadeTime)
				{
					fadeOut = false;
					fadeTimer = 0;
					SetVolume(0);
				}
			}
		/*	std::cout << "In Update Fade time :" << fadeTime << '\n';
			std::cout << "In Update Bool check :" << fadeOut << '\n';*/
		}
		else
		{
			if(isPlaying())Stop(); //hasPlayed = false;
			playDelayed = false;
			timer = 0;
		}
	}

	/*!************************************************************************
	* \brief Updates the currently attached audio with the new one
	* E.g "Explosion.wav" -> "Explosion" will the the name you use
	* \param clipName New AudioClip to "attach" to the audio component
	**************************************************************************/
	void CPAudioSource::UpdateAudio(std::string clipName)
	{
		//If we update the audio clip while it's playing, it should stop 
		if(isPlaying()) Stop();
		hasPlayed = false;
		AudioClipName = clipName;
	}


	/*!************************************************************************
	* \brief Destroys the audio component
	* (Currently, this also clears out ALL audio components from the audiomanager)
	**************************************************************************/
	void CPAudioSource::Destroy()
	{
		//DebuggerLogWarning("Destroyed!");
		if(isPlaying())Stop();
		//AUDIOMANAGER->AudioSources.clear();
		for (auto iter = std::begin(AUDIOMANAGER->AudioSources); iter != std::end(AUDIOMANAGER->AudioSources); ++iter)
		{
			if (*iter == this)
			{
				AUDIOMANAGER->AudioSources.erase(iter);
				break;
			}
		}
	/*	std::vector<CPAudioSource*>::iterator chosenOne;
		for (auto iter = std::begin(AUDIOMANAGER->AudioSources); iter != std::end(AUDIOMANAGER->AudioSources); ++iter)
		{
			if (*iter == this) chosenOne = iter;
		}
		AUDIOMANAGER->AudioSources.erase(chosenOne);*/
		//std::remove_if(AUDIOMANAGER->AudioSources.begin(), AUDIOMANAGER->AudioSources.end(), [&](auto& src) {std::cout << "found!\n"; return src == this; });
		
	}

	/*!************************************************************************
	* \brief Serializes the audio component
	* 
	**************************************************************************/
	bool CPAudioSource::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		//DebuggerLog("Serializing Audio Source");
		data.SetObject();
		data.AddMember("Active", m_active, alloc);
		data.AddMember("AudioClipName", Value(AudioClipName.c_str(), alloc), alloc);
		data.AddMember("Play On Awake", playOnAwake,alloc);
		data.AddMember("Loop", loop, alloc);
		data.AddMember("is3D", is3D, alloc);
		data.AddMember("Volume", volume, alloc);
		data.AddMember("Pitch", pitch, alloc);
		data.AddMember("SoundType", (int)(soundType), alloc);
		return true;
	}

	/*!************************************************************************
	* \brief Deserializes the audio component
	* 
	**************************************************************************/
	bool CPAudioSource::Deserialize(const Value& data)
	{
		//DebuggerLog("Deserializing Audio Source");
		bool HasActive = data.HasMember("Active");
		bool HasClipName = data.HasMember("AudioClipName");
		bool HasPlayOnAwake = data.HasMember("Play On Awake");
		bool HasLoop = data.HasMember("Loop");
		bool Has3D= data.HasMember("is3D");
		bool HasVolume = data.HasMember("Volume");
		bool HasPitch = data.HasMember("Pitch");
		bool HasSoundType = data.HasMember("SoundType");
		if (HasActive)
		{
			m_active = data["Active"].GetBool();
		}
		if (Has3D) 
		{
			const Value& _is3D = data["is3D"];
			is3D = _is3D.GetBool();
		}
		if (HasSoundType)
		{
			const Value& _soundType = data["SoundType"];
			soundType = (SoundType)_soundType.GetInt();
		}
		if (HasClipName && HasPlayOnAwake && HasLoop && HasVolume && HasPitch)
		{
			const Value& _clipName = data["AudioClipName"];
			const Value& _playOnAwake = data["Play On Awake"];
			const Value& _loop = data["Loop"];
			const Value& _vol = data["Volume"];
			const Value& _pitch = data["Pitch"];
			AudioClipName = _clipName.GetString();
			playOnAwake = _playOnAwake.GetBool();
			loop = _loop.GetBool();
			volume = _vol.GetFloat();
			pitch = _pitch.GetFloat();
			return true;
		}
		return false;
	}

	/*!************************************************************************
	* \brief Function that calls the appropriate functions when the audio component setActive is toggled
	* 
	* \param func_isActive if the audio component is active or not
	**************************************************************************/
	void CPAudioSource::ToggleActive(bool func_isActive)
	{
		if (!func_isActive)
		{ 
			Stop(); 
			hasPlayed = false;
			playDelayed = false;
			timer = 0;
		}
	}


	/*!************************************************************************
	* \brief Plays the audio component (will play the current audio clip attached)
	* 
	**************************************************************************/
	void CPAudioSource::Play(SoundType type)
	{
		if (AudioClipName != "") 
		{
			if (is3D)
			{
				channelID = AUDIOMANAGER->Play3DSound(AudioClipName, GetComponent<CPTransform>()->GetPosition(),type);
			}
			else
			{
				channelID = AUDIOMANAGER->PlaySound(AudioClipName,type);
			}
			SetPitch(pitch);
			SetVolume(volume);
		}

		AUDIOMANAGER->SetLoopChannel(channelID, loop);
		//else DebuggerLogWarningFormat("Unable to find %s !", AudioClipName);
		hasPlayed = true;
	}

	
	/*!************************************************************************
	* \brief Plays the audioclip attached after X seconds of delay
	* 
	* \param delay time in seconds to delay the sound playing by
	**************************************************************************/
	void CPAudioSource::PlayDelayed(float delay)
	{
		if (!playDelayed)
		{
			playDelayed = true;
			timer = delay;
		}
		else DebuggerLogWarning("WARNING! Already playing delayed!");
	}

	/*!************************************************************************
	* \brief Pauses the audio component if it's playing
	* 
	**************************************************************************/
	void CPAudioSource::Pause()
	{
		AUDIOMANAGER->PauseChannel(channelID);
	}

	/*!************************************************************************
	* \brief Unpauses the audio component if it's paused
	* 
	**************************************************************************/
	void CPAudioSource::UnPause()
	{
		AUDIOMANAGER->UnPauseChannel(channelID);
	}

	/*!************************************************************************
	* \brief Stops the audio component
	* 
	**************************************************************************/
	void CPAudioSource::Stop()
	{
		AUDIOMANAGER->StopChannel(channelID);
	}

	/*!************************************************************************
	* \brief Checks if the audio component is playing or not
	* 
	**************************************************************************/
	bool CPAudioSource::isPlaying()
	{
		return AUDIOMANAGER->IsPlaying(channelID);
	}

	/*!************************************************************************
	* \brief Set the Pitch object
	* 
	* \param _pitch Pitch to set (clamped in inspector[-4,4])
	**************************************************************************/
	void CPAudioSource::SetPitch(float _pitch)
	{
		pitch = _pitch;
		if (!CORE->IsPlaying()) return;
		AUDIOMANAGER->SetChannelPitch(channelID, pitch);
	}

	/*!************************************************************************
	* \brief Set the Volume object
	* 
	* \param _vol Volume to set (clamped in inspector [0,inf])
	**************************************************************************/
	void CPAudioSource::SetVolume(float _vol)
	{
		volume = _vol;
		if (!CORE->IsPlaying()) return;
		AUDIOMANAGER->SetChannelVolume(channelID, volume);
	}

	/*!************************************************************************
	 * \brief Fades out the audio source to the given time
	 * 
	 * \param time time to fade out to
	**************************************************************************/
	void CPAudioSource::FadeOut(float time)
	{
		fadeTime = time;
		fadeTimer = 0;
		fadeOut = true;
		fadeIn = false;
	}

	/*!************************************************************************
	 * \brief Fade in the audio source to the given time and volume
	 * (Volume range is [0f-1f])
	 * \param time time to fade in to
	 * \param volumeToSet volume to fade in to
	**************************************************************************/
	void CPAudioSource::FadeIn(float time, float volumeToSet)
	{
		fadeTime = time;	
		fadeTimer = 0;

		fadeIn = true;
		volToSet = volumeToSet;
		fadeOut = false;
	}


	

}