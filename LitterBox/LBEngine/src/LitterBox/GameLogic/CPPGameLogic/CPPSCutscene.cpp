/*!************************************************************************
 * \file				CPPSCutscene.cpp
 * \author(s)			Ryan Tan Jian Hao
 * \par DP email(s):  	ryanjianhao.tan@digipen.edu
 * \par Course:       	CSD2450A
 * \date				10/03/2024
 * \brief 				This source file contains the definitions for the
 *						member functions in the CPPSCutscene class script.
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "CPPSCutscene.h"

/*!************************************************************************
* \brief Start function of the Cutscene scene
* Get a pointer to the screen
**************************************************************************/
void LB::CPPSCutscene::Start()
{
	//we make the the screen render the frame from the video file
	GameObj->GetComponent<CPRender>()->UpdateTexture(
		31,
		1920,
		1080,
		Vec2<float>{0.f, 1.f},
		Vec2<float>{1.f, 0.f}
	);
}

void LB::CPPSCutscene::Update()
{}

void LB::CPPSCutscene::Destroy()
{}
