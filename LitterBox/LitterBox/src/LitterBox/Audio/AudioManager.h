#pragma once
#include "LitterBox/Core/System.h"
#pragma warning(push, 0)
#include "../../dependencies/FMOD/core/inc/fmod.hpp"
#include "../../dependencies/FMOD/core/inc/fmod_errors.h"
#pragma warning(pop)
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

