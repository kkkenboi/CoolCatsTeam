#include "CPPAudioManager.h"
namespace LB
{
	void LB::CPPSAudioManager::Start()
	{
	}
	void CPPSAudioManager::Update()
	{
	}
	void CPPSAudioManager::Destroy()
	{
		audioSources.clear();
	}
	void CPPSAudioManager::Play2DSound(const std::string& soundName, bool loop, float vol, float pitch)
	{	//Probably could make it return an audiosource or something then use a lot of overloading to reuse playsound 3d but oh well
		//I think for this short period of time we do it this way
		GameObject* audioClone = FACTORY->SpawnGameObject();
		audioSources.push_back(audioClone);			//we store it inside our list of audiosources and remove if it's not playing
		JSONSerializer::DeserializeFromFile("AudioSource", *audioClone);
		CPAudioSource* audioSource = audioClone->GetComponent<CPAudioSource>();
		audioSource->UpdateAudio(soundName);
		audioSource->loop = loop;
		audioSource->is3D = false;
		std::cout << "Play sound 2D setting vol to " << vol << '\n';
		audioSource->SetVolume(vol);
		audioSource->SetPitch(pitch);
		audioSource->Play();
		RemoveUnusedAudioSources();
		//GOMANAGER->RemoveGameObject(audioClone, 2.0f);
	}
	void CPPSAudioManager::Play2DSound(const std::vector<std::string>& soundNames, bool loop, float vol, float pitch)
	{
		int randomIndex = rand() % soundNames.size();
		std::cout << "Play soundSSS 2D setting vol to " << vol << '\n';
		Play2DSound(soundNames[randomIndex], loop, vol, pitch);
	}
	void CPPSAudioManager::Play3DSound(const std::string& soundName, const Vec2<float>& position, bool loop, float vol, float pitch)
	{	
		GameObject* audioClone = FACTORY->SpawnGameObject();
		audioClone->GetComponent<CPTransform>()->SetPosition(position);	//Set the position of the audio source to the position of the object that called it
		audioSources.push_back(audioClone);			//we store it inside our list of audiosources and remove if it's not playing
		JSONSerializer::DeserializeFromFile("AudioSource", *audioClone);
		CPAudioSource* audioSource = audioClone->GetComponent<CPAudioSource>();
		audioSource->UpdateAudio(soundName);
		audioSource->loop = loop;
		audioSource->is3D = true;
		audioSource->SetVolume(vol);
		audioSource->SetPitch(pitch);
		audioSource->Play();
		RemoveUnusedAudioSources();
		//GOMANAGER->RemoveGameObject(audioClone, 2.0f);
	}
	void CPPSAudioManager::Play3DSound(const std::vector<std::string>& soundNames, const Vec2<float>& position, bool loop, float vol, float pitch)
	{
		int randomIndex = rand() % soundNames.size();
		Play3DSound(soundNames[randomIndex], position, loop, vol, pitch);
	}
	void CPPSAudioManager::CrossFadeBGM(const std::string& targetSong, float fadeDuration)
	{
	}
	void CPPSAudioManager::RemoveUnusedAudioSources()
	{
		for(auto it = audioSources.begin(); it != audioSources.end();)
		{
			if(!(*it)->GetComponent<CPAudioSource>()->isPlaying())
			{
				GOMANAGER->RemoveGameObject(*it);
				it = audioSources.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}