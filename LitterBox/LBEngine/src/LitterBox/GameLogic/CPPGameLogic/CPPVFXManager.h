/*!************************************************************************
 * \file				CPPVFXManager.h
 * \author(s)			Amadeus Chia 
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD
 * \date				15/03/2024
 * \brief 				Header file for the VFX Manager that handles the spawning of vfx in
 * 						the game
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class CPPSVFXManager : public CPPBehaviour
	{
	public :
	//Normal override methods that are not used
		void Start() override;
		void Update() override;
		void Destroy() override;

		/*!************************************************************************
		 * \brief Function to spawn the explosion anim
		 * 
		 * \param pos Position to spawn the vfx in
		 * \param scale scale for the explosion vfx
		**************************************************************************/
		void SpawnExplosion(Vec2<float> pos, float scale = 1.0f);

		/*!************************************************************************
		 * \brief Function to spawn the hit anim
		 * 
		 * \param pos Position to spawn it in
		**************************************************************************/
		void SpawnHitAnim(Vec2<float> pos);

		/*!************************************************************************
		 * \brief Function to spawn the poof anim
		 * 
		 * \param pos Position to spawn it in
		 * \param scaleMult Scale of the poof anim
		**************************************************************************/
		void SpawnPoofAnim(Vec2<float> pos, float scaleMult);
	private:
		
	};
	REGISTER_SCRIPT(CPPSVFXManager)
}