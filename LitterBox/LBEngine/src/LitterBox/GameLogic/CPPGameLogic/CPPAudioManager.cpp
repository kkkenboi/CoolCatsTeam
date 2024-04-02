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
	}
	void CPPSAudioManager::Play2DSound(const std::string& soundName, bool loop, float vol, float pitch)
	{
		GameObject* audioClone = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("AudioSource", *audioClone);
		CPAudioSource* audioSource = audioClone->GetComponent<CPAudioSource>();
		audioSource->UpdateAudio(soundName);
		audioSource->loop = loop;
		audioSource->SetVolume(vol);
		audioSource->SetPitch(pitch);
		audioSource->Play();
		GOMANAGER->RemoveGameObject(audioClone, 2.0f);
	}
	void CPPSAudioManager::Play2DSound(const std::vector<std::string>& soundNames, bool loop, float vol, float pitch)
	{
		int randomIndex = rand() % soundNames.size();
		Play2DSound(soundNames[randomIndex], loop, vol, pitch);
	}
	void CPPSAudioManager::Play3D(const std::string& soundName, const Vec2<float>& position, bool loop, float vol, float pitch)
	{
		GameObject* audioClone = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("AudioSource", *audioClone);
		CPAudioSource* audioSource = audioClone->GetComponent<CPAudioSource>();
		audioSource->UpdateAudio(soundName);
		audioSource->loop = loop;
		audioSource->is3D = true;
		audioSource->SetVolume(vol);
		audioSource->SetPitch(pitch);
		audioSource->Play();
		GOMANAGER->RemoveGameObject(audioClone, 2.0f);
	}
	void CPPSAudioManager::Play3D(const std::vector<std::string>& soundNames, const Vec2<float>& position, bool loop, float vol, float pitch)
	{
		int randomIndex = rand() % soundNames.size();
		Play3D(soundNames[randomIndex], position, loop, vol, pitch);
	}
	void CPPSAudioManager::CrossFadeBGM(const std::string& targetSong, float fadeDuration)
	{
	}
}