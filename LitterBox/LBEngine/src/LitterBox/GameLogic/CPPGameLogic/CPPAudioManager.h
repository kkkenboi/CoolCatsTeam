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

		
	private:
		//By right the audiomanager should be DDOL, and this BGM Source should persist throughout scenes...
		GameObject* BGM_Source;
	};
	REGISTER_SCRIPT(CPPSAudioManager)
}