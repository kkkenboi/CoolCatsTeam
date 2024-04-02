/*!************************************************************************
 * \file				CPPAudioManager.h
 * \author(s)			Amadeus Chia
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2451A
 * \date				27/03/2024
 * \brief 				
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/Serialization/AssetManager.h"
#include <vector>
namespace LB
{
	class CPPSAudioManager : public CPPBehaviour
	{
	public:
		/*!************************************************************************
		* \brief
		* Component functions for the Game Manager
		**************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;
		
		//SOUND GROUPS
		//CHASER SOUND
		std::vector<std::string> ChaserAttackSounds = { "Wolf Attack_1",
														"Wolf Attack_2",
														"Wolf Attack_3",
														"Wolf Attack_4",
														"Wolf Attack_5" };

		std::vector<std::string> ChaserHurtSounds = { "Wolf Hurt_1",
													  "Wolf Hurt_2",
													  "Wolf Hurt_3",
													  "Wolf Hurt_4",
													  "Wolf Hurt_5" };

		std::vector<std::string> ChaserDeathSounds = { "Wolf Death_1",
													   "Wolf Death_2",
													   "Wolf Death_3",
													   "Wolf Death_4" };



		//Plays 2D sound
		void Play2DSound(const std::string& soundName, bool loop = false, float vol = 1.0f, float pitch = 1.0f);
		//Overloaded function to play multiple 2D sounds
		void Play2DSound(const std::vector<std::string>& soundNames, bool loop = false, float vol = 1.0f, float pitch = 1.0f);
		//Plays 3D sound
		void Play3DSound(const std::string& soundName, const Vec2<float>& position, bool loop = false, float vol = 1.0f, float pitch = 1.0f);
		//Overloaded function to play multiple 3D sounds
		void Play3DSound(const std::vector<std::string>& soundNames, const Vec2<float>& position, bool loop = false, float vol = 1.0f, float pitch = 1.0f);
		//Function to cross fade from one BGM to another
		void CrossFadeBGM(const std::string& targetSong, float fadeDuration = 1.0f);
	private:
		//By right the audiomanager should be DDOL, and this BGM Source should persist throughout scenes...
		GameObject* BGM_Source;	    //THERE CAN ONLY BE ONE BGM SOURCE
		std::vector<GameObject*> audioSources;	//Pool of audio sources
		void RemoveUnusedAudioSources();
	};
	REGISTER_SCRIPT(CPPSAudioManager)
}