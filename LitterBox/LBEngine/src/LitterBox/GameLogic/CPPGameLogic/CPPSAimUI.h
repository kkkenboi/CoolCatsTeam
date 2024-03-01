/*!************************************************************************
 * \file				CPPSAimUI.h
 * \author(s)			Amadeus Chia 
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2450A
 * \date				09/02/2024
 * \brief 				This file contains the file declarations for the 
 * 						Aim script. (Basically points UI to mouse cursor)
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "Platform/Windows/Windows.h"
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/Engine/Input.h"
namespace LB
{
	class CPPSAimUI : public CPPBehaviour
	{
	public: 
		/*!************************************************************************
		* \brief Start function of the AIM UI 
		* It gets the player GO and its own transform and caches it
		**************************************************************************/
		void Start() override;
		/*!************************************************************************
		* \brief Uudate function for the Aim UI, basically just points the 
		* image towards the player's cursor
		**************************************************************************/
		void Update() override;
		/*!************************************************************************
		* \brief Handles the destruction of the Aim UI. Should be empty for now
		* 
		**************************************************************************/
		void Destroy() override;
		//Caching the Aim UI's own transform for easy code access
		CPTransform* mTransform;
		//Caching the player GO for easy access
		GameObject* mPlayer;
		//Caching the CameraFollow GO for easy access
		GameObject* mCameraFollow;
		//Mouse position for caching
		Vec2<float> MousePos;
		//Vector for calculating the direction to the mouse
		Vec2<float> DirToMouse;
	};
	REGISTER_SCRIPT(CPPSAimUI)

}

