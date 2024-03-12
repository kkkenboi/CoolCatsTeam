/*!************************************************************************
 * \file				CPPSAimUI.cpp
 * \author(s)			Amadeus Chia 
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2450A
 * \date				09/02/2024
 * \brief 				This file contains the source code for the Aim UI
 * 						which basically just points the Aim UI towards the mouse
 * 						cursor
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "CPPSAimUI.h"
#include "CPPSCameraFollow.h"
#include "CPPSMouseWorld.h"

namespace LB
{
	/*!************************************************************************
	 * \brief Start function of the AIM UI 
	 * It gets the player GO and its own transform and caches it
	**************************************************************************/
	void CPPSAimUI::Start()
	{
		//Grabbing the current obj's transform
		mTransform = GameObj->GetComponent<CPTransform>();
		//Grabbing the player reference
		std::vector<GameObject*> const& GOs = GOMANAGER->GetGameObjects();
		for (GameObject* GO : GOs) {
			if (GO->GetName() == "MainChar") {
				mPlayer = GO;
				break;
			}
		}
		mCameraFollow = GOMANAGER->FindGameObjectWithName("CameraFollow");
		mMouseWorld = GOMANAGER->FindGameObjectWithName("MouseWorld");
	}

	/*!************************************************************************
	 * \brief Uudate function for the Aim UI, basically just points the 
	 * image towards the player's cursor
	**************************************************************************/
	void CPPSAimUI::Update()
	{
		//Setting the position of the aim script to the player
		mTransform->SetPosition(mPlayer->GetComponent<CPTransform>()->GetPosition());
		//All this funky code is just to account for editor/release mode difference
		MousePos = mMouseWorld->GetComponent<CPPSMouseWorld>()->GetComponent<CPTransform>()->GetPosition();

		//Getting the direction from player to mouse
		DirToMouse = MousePos - mPlayer->GetComponent<CPTransform>()->GetPosition();
		//DirToMouse.Normalise();	//normalise for nicer calculations
		//Set the rotation of the AimUI to the direction
		mTransform->SetRotation(RadToDeg(atan2f(DirToMouse.y, DirToMouse.x)));

	}

	/*!************************************************************************
	 * \brief Handles the destruction of the Aim UI. Should be empty for now
	 * 
	**************************************************************************/
	void CPPSAimUI::Destroy()
	{
		//Should be empty
	}

}
