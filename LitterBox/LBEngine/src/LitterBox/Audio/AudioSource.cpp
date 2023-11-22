#include "LitterBox/Components/AudioSourceComponent.h"
#include "LitterBox/Audio/AudioManager.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Core/Core.h"

namespace LB
{
	float timer{ 0 };
	bool playDelayed{ false };
	int channelID{ 0 };
	bool hasPlayed{ false };
	void CPAudioSource::Initialise()
	{
		if (AudioClipName == "")
		{
			DebuggerLogWarning("NO AUDIO CLIP ATTACHED!");
			return;
		}
		if(CORE->IsPlaying()) AUDIOMANAGER->AudioSources.push_back(this);
	}

	void CPAudioSource::Update()
	{
		//We only want to play sounds when the game is running
		if (CORE->IsPlaying())
		{
			//If the audio source is playonawake and hasn't played yet
			if (playOnAwake && !hasPlayed) {
				//We play the sound
				Play();	//hasPlayed = true;
				DebuggerLog("Play On Awake!");
			}
			//Timer stuff for delayed playing
			if (timer > 0) timer -= TIME->GetDeltaTime();
			if (timer <= 0 && playDelayed) //If play delayed is called
			{
				Play();//hasPlayed = true;
				playDelayed = false;
			}
			//If it's looping and it has been played 
			if (loop && !isPlaying() && hasPlayed)
			{
				Play();
			}
		}
		else
		{
			hasPlayed = false;
			playDelayed = false;
			timer = 0;
		}
	}
	void CPAudioSource::Destroy()
	{
		DebuggerLog("Destroyed!");
	}

	bool CPAudioSource::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		DebuggerLog("Serializing Audio Source");
		data.SetObject();
		data.AddMember("AudioClipName", Value(AudioClipName.c_str(), alloc), alloc);
		data.AddMember("Play On Awake", playOnAwake,alloc);
		data.AddMember("Loop", loop, alloc);
		data.AddMember("Volume", volume, alloc);
		data.AddMember("Pitch", pitch, alloc);
		return true;
	}

	bool CPAudioSource::Deserialize(const Value& data)
	{
		DebuggerLog("Deserializing Audio Source");
		bool HasClipName = data.HasMember("AudioClipName");
		bool HasPlayOnAwake = data.HasMember("Play On Awake");
		bool HasLoop = data.HasMember("Loop");
		bool HasVolume = data.HasMember("Volume");
		bool HasPitch = data.HasMember("Pitch");
		if (HasClipName && HasPlayOnAwake && HasLoop && HasPitch)
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


	void CPAudioSource::Play()
	{
		if (AudioClipName != "") 
		{
			channelID = AUDIOMANAGER->PlaySound(AudioClipName);
		}
		else DebuggerLogWarningFormat("Unable to find %s !", AudioClipName);
		hasPlayed = true;
	}
	void CPAudioSource::PlayDelayed(float delay)
	{
		if (!playDelayed)
		{
			playDelayed = true;
			timer = delay;
		}
		else DebuggerLogWarning("WARNING! Already playing delayed!");
	}
	void CPAudioSource::Pause()
	{
		AUDIOMANAGER->PauseChannel(channelID);
	}
	void CPAudioSource::UnPause()
	{
		AUDIOMANAGER->UnPauseChannel(channelID);
	}
	void CPAudioSource::Stop()
	{
		AUDIOMANAGER->StopChannel(channelID);
	}
	bool CPAudioSource::isPlaying()
	{
		return AUDIOMANAGER->IsPlaying(channelID);
	}

	void CPAudioSource::SetPitch(float _pitch)
	{
		pitch = _pitch;
		if (!CORE->IsPlaying()) return;
		AUDIOMANAGER->SetChannelPitch(channelID, pitch);
	}

	void CPAudioSource::SetVolume(float _vol)
	{
		volume = _vol;
		if (!CORE->IsPlaying()) return;
		AUDIOMANAGER->SetChannelVolume(channelID, volume);
		
	}

	

}