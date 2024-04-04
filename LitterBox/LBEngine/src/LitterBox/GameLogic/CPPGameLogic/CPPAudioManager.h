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
		



		//SOUND GROUPS (If we had RTTR and serialization, this would be so much cleaner..)
		//BALL SOUNDS
		std::vector<std::string> BallCollisionSounds = { "BallImpact_1","BallImpact_2" };
		std::vector<std::string> PlayerHitBallSounds = { "GolfBallHit","GolfSwingHit" };
		//PLAYER SOUND
		std::vector<std::string> PlayerHurtSounds = { "The Cat Hurt_1",
													  "The Cat Hurt_2",
													  "The Cat Hurt_3",
													  "The Cat Hurt_4",
													  "The Cat Hurt_5",
													  "The Cat Hurt_6" };

		std::vector<std::string> PlayerDeathSounds = { "The Cat Death_1",
													  "The Cat Death_2",
													  "The Cat Death_3",
													  "The Cat Death_4",
													  "The Cat Death_5",
													  "The Cat Death_6",
													  "The Cat Death_7",
													  "The Cat Death_8" };

		std::vector<std::string> PlayerPositiveSounds =
													{ "The Cat Positive_1",
													  "The Cat Positive_2",
													  "The Cat Positive_3",
													  "The Cat Positive_4" };

		std::vector<std::string> PlayerFootStepsSounds = { "Footsteps-Grass-Far-Small_1",
													 "Footsteps-Grass-Far-Small_2",
													 "Footsteps-Grass-Far-Small_3",
													 "Footsteps-Grass-Far-Small_4" };

		std::vector<std::string> PlayerSandStepSounds = { "Footsteps sand 1",
															"Footsteps sand 2",
															"Footsteps sand 3",
															"Footsteps sand 4",
															"Footsteps sand 5",
															"Footsteps sand 6" };

		std::vector<std::string> PlayerSlashSounds = { "Sward-Whoosh_1",
													   "Sward-Whoosh_2",
													   "Sward-Whoosh_3",
													   "Sward-Whoosh_4",
													   "Sward-Whoosh_5" };
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
		//MAGE SOUND
		std::vector<std::string> MageHurtSounds = { "Mage Hurt_1",
													"Mage Hurt_2",
													"Mage Hurt_3",
													"Mage Hurt_4",
													"Mage Hurt_5" };

		std::vector<std::string> MageAttackSounds = { "Mage Attack_1",
													  "Mage Attack_2",
													  "Mage Attack_3",
													  "Mage Attack_4",
													  "Mage Attack_5"
													  "Mage Attack_6",
													  "Mage Attack_7",
													  "Mage Attack_8",
													  "Mage Attack_9",
													  "Mage Attack_10",
													  "Mage Attack_11" };

		std::vector<std::string> MageDeathSounds = { "Mage Death_1",
													 "Mage Death_2",
													 "Mage Death_3",
													 "Mage Death_4",
													 "Mage Death_5" };

		//CHARGER SOUND
		std::vector<std::string> ChargerAttackSounds = { "Rhyno_Attack_1",
														 "Rhyno_Attack_2",
														 "Rhyno_Attack_3",
														 "Rhyno_Attack_4",
														 "Rhyno_Attack_5",
														 "Rhyno_Attack_6" };

		std::vector<std::string> ChargerDeathSounds = { "Rhyno_Death_1",
														 "Rhyno_Death_2",
														 "Rhyno_Death_3",
														 "Rhyno_Death_4",
														 "Rhyno_Death_5",
														 "Rhyno_Death_6" };

		std::vector<std::string> ChargerHurtSounds = { "Rhyno Hurt_1",
														"Rhyno Hurt_2",
														"Rhyno Hurt_3",
														"Rhyno Hurt_4",
														"Rhyno Hurt_5" };

		std::string ChargerHitSound { "Bullets_Impact_Hit_Metal_01_SND50289_1"};
		std::string ChargerChargeSound{ "Trains_Horn_Train_Whistle_Old_SND59554" };

		//BUSHES SOUNDS
		std::vector<std::string> BushSounds = { "Passing Through Shrug_SFX_1",
												"Passing Through Shrug_SFX_2",
												"Passing Through Shrug_SFX_3",
												"Passing Through Shrug_SFX_4" };


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