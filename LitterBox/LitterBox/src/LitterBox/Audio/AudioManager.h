#pragma once
#include "LitterBox/Core/System.h"
#include "../../dependencies/FMOD/core/inc/fmod.hpp"
#include "../../dependencies/FMOD/core/inc/fmod_errors.h"
#include "LitterBox/Serialization/AssetManager.h"

namespace LB
{
	class AudioManager : public ISystem
	{
	public:
		AudioManager();
		//void PlayTestSound();
		void Update() override;

		FMOD::System* audioSystem;
		FMOD_RESULT result;
	private:

	};
	void PlayTestSound();
	void PlayExplosionSound();
	void PlayAHHSound();
	extern AudioManager* AUDIOMANAGER;
}

