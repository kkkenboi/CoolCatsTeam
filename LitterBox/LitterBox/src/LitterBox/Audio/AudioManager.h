/*!************************************************************************
 \file				AudioManager.h
 \author(s)			Amadeus Chia Jinhan, 
 \par DP email(s):	amadeusjinhan.chia@digipen.edu,
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				This file contains the implementation of AudioManager 
                    functions. (This will be more detailed as more 
					features get added)

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "LitterBox/Core/System.h"
#pragma warning(push, 0)
#include "../../dependencies/FMOD/core/inc/fmod.hpp"
#include "../../dependencies/FMOD/core/inc/fmod_errors.h"
#pragma warning(pop)
#include "LitterBox/Serialization/AssetManager.h"

namespace LB
{
	/*!***********************************************************************
     * \brief 
     * AudioManager handles anything sound related, playing sounds in different
	 * channels etc.
     **************************************************************************/
	class AudioManager : public ISystem
	{
	public:
		/*!***********************************************************************
		 * \brief Construct a new Audio Manager object
		 * (FMOD::System is initialised here)
		 **************************************************************************/
		AudioManager();
		/*!***********************************************************************
		 \brief
		 Initalises Audio Manager System
		*************************************************************************/
		void Initialize() override { SetSystemName("Audio System"); }

		/*!***********************************************************************
		 * \brief Updates the Audio System (Called by ISystem)
		 * 
		 **************************************************************************/
		void Update() override;

		/*!***********************************************************************
		 * \brief AudioSystem that provides FMOD functionality to play sounds
		 * MUST be initialised before it can be used!
		 **************************************************************************/
		FMOD::System* audioSystem;

		/*!***********************************************************************
		 * \brief Stores the result of any FMOD related function
		 * Allows for error checking and error logging
		 **************************************************************************/
		FMOD_RESULT result;
	private:

	};

	/*!***********************************************************************
	 * \brief Functions to play specific sounds
	 * 
	 **************************************************************************/
	void PlayTestSound();
	void PlayExplosionSound();
	void PlayAHHSound();

	/*!***********************************************************************
	 * \brief Global Pointer for the AudioManager
	 * 
	 **************************************************************************/
	extern AudioManager* AUDIOMANAGER;
}

