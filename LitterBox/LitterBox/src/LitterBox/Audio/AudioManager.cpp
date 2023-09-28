#include "pch.h"
#include "AudioManager.h"
namespace LB
{
	AudioManager* AUDIOMANAGER = nullptr;
	AudioManager::AudioManager()
	{
		if (!AUDIOMANAGER)
			AUDIOMANAGER = this;
		else
			std::cerr << "Audio Manager already exists" << std::endl;

		// Create an instance of the FMOD system object
		result = FMOD::System_Create(&audioSystem);
		if (result != FMOD_OK) {
			// Handle initialization error
			// You can use FMOD_ErrorString(result) to get an error description
			std::cerr <<"FMOD ERROR : " << FMOD_ErrorString(result) << '\n';
		}

		// Initialize the FMOD system
		result = audioSystem->init(512, FMOD_INIT_NORMAL, 0);
		if (result != FMOD_OK) {
			// Handle initialization error
		}
	}
	void PlayTestSound()
	{
		AUDIOMANAGER->result = AUDIOMANAGER->audioSystem->playSound(ASSETMANAGER->sampleSound, nullptr, false, nullptr);
		if (AUDIOMANAGER->result != FMOD_OK)
		{
			std::cout << "PLAY SAMPLE SOUND IS NOT WORKING!! \n";
		}
	}
	void PlayExplosionSound()
	{
		AUDIOMANAGER->result = AUDIOMANAGER->audioSystem->playSound(ASSETMANAGER->explosionSound, nullptr, false, nullptr);
		if (AUDIOMANAGER->result != FMOD_OK)
		{
			std::cout << "PLAY EXPLOSION SOUND IS NOT WORKING!! \n";
		}
	}
	void PlayAHHSound()
	{
		AUDIOMANAGER->result = AUDIOMANAGER->audioSystem->playSound(ASSETMANAGER->ahhSound, nullptr, false, nullptr);
		if (AUDIOMANAGER->result != FMOD_OK)
		{
			std::cout << "PLAY AHH SOUND IS NOT WORKING!! \n";
		}
	}
	void AudioManager::Update()
	{
		audioSystem->update();
	}
}
